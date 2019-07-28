#ifndef __MONITOR_H__
#define __MONITOR_H__

#define CHIAVE IPC_PRIVATE
#define PERMESSI IPC_CREAT | 0600

typedef struct{
	int mutex;
	int idCond;
	int idCondCounts;
	int numCondVar;
	int* condCounts;
}Monitor;

void initMonitor(Monitor *m, int cond_var);
void removeMonitor(Monitor *m);
void enterMonitor(Monitor *m);
void leaveMonitor(Monitor *m);
void wait_cond(Monitor *m, int idCond);
void signal_cond(Monitor *m, int idCond);

#endif /* __MONITOR_H__ */
