/**************************************************************************
*                  Copyright (C) Antonio Riccio, 2019				      *
* ------------------------------------------------------------------------*
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.        *
* ----------------------------------------------------------------------- *
* 																		  *
* Examples:																  *
*	- Asynchronous I/O through the select() system call.				  *
*	- User-space Linux input subsystem						  			  *
* 																		  *
* More info at:															  *
*	- Asynchronous I/O: 'The Linux Programming Interface' chapter 63      *
*	- Linux input subsystem: Documentation/input/input_uapi.rst			  *
***************************************************************************/
#include <linux/input.h>

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/select.h>

static const unsigned int event_type = EV_ABS;
static const unsigned int event_code_x = ABS_MT_POSITION_X;
static const unsigned int event_code_y = ABS_MT_POSITION_Y;

static const unsigned long simulated_work_duration_cycles = 100000000;
static const unsigned short max_iterations = 100;

static bool stop = 0;

/* abstraction platform-independent of input position */
static struct input_data
{
	unsigned int x;
	unsigned int y;
} input_data;

/**
 * interrupt_handler() - handles reception of signals
 * @sig: the signal received.
 *
 * The function stops the infine loops by setting the variable stop to true
 */
static void interrupt_handler(int sig)
{
	stop = 1;
}

/**
 * print_events() - print all REL_[X]/[Y] event values.
 * @fd: uevent file descriptor (e.g. /dev/input/event5)
 *
 * The function runs in a infine loop which is stopped by pressing CTRL+C.
 *
 * Context: the function sleeps,
 *
 * Return: EXIT_SUCCESS on success or EXIT_FAILURE on failure.
 */
static int print_events(char const *file_to_open)
{
	struct input_event ev[64] = { 0 };
	int i = -1, rd = -1, fd = -1;

	if ((fd = open(file_to_open, O_RDONLY)) < 0) {
		fprintf(stderr, "[%s] error open (%i): %s\n", __func__, errno, strerror(errno));
		return EXIT_FAILURE;
	}

	while (!stop) {
		if (stop)
			break;

		rd = read(fd, ev, sizeof(ev));

		if (rd < (int) sizeof(struct input_event)) {
			printf("expected %d bytes, got %d\n", (int) sizeof(struct input_event), rd);
			perror("\nevtest: error reading");
			return EXIT_FAILURE;
		}

		for (i = 0; i < rd / sizeof(struct input_event); i++) {
			unsigned int type, code;

			type = ev[i].type;
			code = ev[i].code;

			if (type == EV_REL && (code == REL_X || code == REL_Y)) {
				printf("Event: time %ld.%06ld, ", ev[i].time.tv_sec, ev[i].time.tv_usec);
				printf("type %d (%s), code %d (%s), ",
					type, "EV_REL",
					code, (code == REL_X) ? "REL_X" : "REL_Y");
				printf("value %d\n", ev[i].value);
			}
		}
	}

	return EXIT_SUCCESS;
}

/**
 * can_extract_data() - retrieves event_code_x and event_code_y values
 * @fd: file descriptor of the uevent file
 * @id: output data structure, containing the x and y coordinates
 *
 * The function analyses a burst of events until a SYN_REPORT is
 * encountered. Then it extracts the x and y coords and put them in
 * the id data structure
 *
 * Context: the function does not sleep
 *
 * Return: true if data in 'id' are meaningful, otherwise false
 */
static bool can_extract_data(int fd, struct input_data* id)
{
	struct input_event ev = { 0 };
	bool synReached = false;
	bool canRead = false;
	int rd = -1;
	int i = 0;

	for( i = 0; !synReached && (i < max_iterations); i++ ){
		rd = read(fd, &ev, sizeof(struct input_event));

		if (rd == -1){
			fprintf(stderr, "[%s] error read (%i): %s\n", __func__, errno, strerror(errno));
			break;
		} else if (rd < sizeof(struct input_event)){
			fprintf(stderr, "[%s] error read: not received enough data (%i)%s\n", __func__, errno, strerror(errno));
			break;
		}

		if ( ev.type == EV_SYN )
		{
			#ifdef DEBUG
				printf("\n[%s] -------------- EV_SYN ------------\n", __func__);
			#endif
		}
		else if ( ev.type == event_type && ev.code == event_code_x )
		{
			#ifdef DEBUG
				printf("[%s] value x: %d\n", __func__, ev.value);
			#endif
			id->x = ev.value;
		}
		else if ( ev.type == event_type && ev.code == event_code_y )
		{
			#ifdef DEBUG
				printf("[%s] value y: %d\n", __func__, ev.value);
			#endif
			id->y = ev.value;
		}
		else if ( (ev.type == event_type) && (ev.code == ABS_MT_TRACKING_ID) && (ev.value == -1) )
		{
			#ifdef DEBUG
				printf("#######################################\n");
				printf("Got ABS_MT_TRACKING_ID to -1!\n");
				printf("value x: %d\n", id->x);
				printf("value y: %d\n", id->y);
				printf("#######################################\n");
			#endif
			canRead = true;
		}
	}

	return canRead;
}

/**
 * simulation() - gets mouse movements coords in an async fashion
 * @file_to_open: path of the uvent file to open (e.g. /dev/input/event12).
 *
 * The function runs in a infine loop which is stopped by pressing CTRL+C.
 * To do the async I/O the open() is made with the O_NONBLOCK flag and the
 * select() system call is used to check when is possible non-blocking I/O.
 *
 * Context: The function does not sleep
 */
static void simulation(char const *file_to_open)
{
	int fd = -1, ready = -1, i = -1;
	struct input_data id = { 0 };
	struct timeval tv = { 0 };
	fd_set readfds;

	// select() must not block
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	signal(SIGINT, interrupt_handler);
	signal(SIGTERM, interrupt_handler);

	if ((fd = open(file_to_open, O_RDONLY | O_NONBLOCK)) < 0) {
		fprintf(stderr, "[%s] error open (%i): %s", __func__, errno, strerror(errno));
	}

	while(!stop){
		/* Simulated workload */
		fprintf(stdout, "\nsimulating some work...\n");
		for (i = 0; i < simulated_work_duration_cycles; i++);

		/* Notify me when there are data to read
			Must be done for each call of select() */
		FD_ZERO(&readfds);
		FD_SET(fd, &readfds);

		/* 'ready' is the number of fd(s) available to read */
		ready = select(fd + 1, &readfds, NULL, NULL, &tv);
		#ifdef DEBUG
			fprintf(stdout, "[%s] ready: %i\n", __func__, ready);
		#endif

		if (ready == -1)
			fprintf(stderr, "[%s] error select (%i): %s\n", __func__, errno, strerror(errno));
		else if (ready == 0)
			// fprintf(stdout, "[%s] Select timed out!\n", __func__);
			continue;
		else { /* ready > 0 */
			// fprintf(stdout, "[%s] One of more fds (%i) are ready\n", __func__, ready);
			if(can_extract_data(fd, &id))
				fprintf(stdout, "[%s] got coordinates x: %d, y: %d\n", __func__, id.x, id.y);
		}
	}
}

static void usage(char const *exec_name)
{
	printf("usage: %s [-pe][-h] ", exec_name);
	printf("EVENTFILEPATH\n");
	printf("\te.g. sudo %s -pe /dev/input/event5\n", exec_name);
}

int main(int argc, char const *argv[])
{
	if(argc < 2)
	{
		usage(argv[0]);
		return EXIT_FAILURE;
	}

	if(strcmp(argv[1], "-h") == 0)
	{
		usage(argv[0]);
		return EXIT_SUCCESS;
	}
	else if(strcmp(argv[1], "-pe") == 0)
	{
		return print_events(argv[argc-1]);
	}

	simulation(argv[argc-1]);
    return 0;
}

