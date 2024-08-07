

MNTLIB=../monitor/monitor.o
all:  l_s_monitor

l_s_monitor: l_s_monitor.o procedure.o  
	gcc  -o l_s_monitor l_s_monitor.o procedure.o $(MNTLIB)

l_s_monitor.o: l_s_monitor.c header.h 
	gcc -c -DDEBUG_  l_s_monitor.c

procedure.o: procedure.c header.h
	gcc -c -DDEBUG_ procedure.c 
clean:
	 rm -f *.o 
	 rm -f ./l_s_monitor
	 rm -f *~
	
