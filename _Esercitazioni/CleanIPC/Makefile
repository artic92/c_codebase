
MNTDIR=../monitor
all:  p_c_monitor

p_c_monitor: p_c_monitor.o procedure.o monitor.o
	g++  -o start p_c_monitor.o procedure.o monitor.o

p_c_monitor.o:p_c_monitor.c header.h 
	g++ -c -DDEBUG_  p_c_monitor.c

monitor.o: $(MNTDIR)/monitor.c $(MNTDIR)/monitor.h
	  g++ -DDEBUG_ -c $(MNTDIR)/monitor.c

procedure.o: procedure.c header.h
	g++ -c -DDEBUG_ procedure.c 
clean:
	 rm -f *.o 
	 rm -f ./start
	 rm -f *~
	 rm -f ./monitor.o

cleanipc:
	 ipcs -m | awk -f ./removeShm.awk
	 ipcs -s | awk -f ./removeSems.awk
	  
	
	  
