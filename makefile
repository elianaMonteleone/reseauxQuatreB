
# cphex\makefile
EXECS = admin ser cli 
LIBS=
CC = gcc

.SILENT:
all: $(EXECS)


LibSerME_RC.o:	LibSerME_RC.c	LibSerME_RC.h	data.h	
	echo "compilation de LibSerME_RC"
	$(CC) -c LibSerME_RC.c

admin:	data.h	admin.c	LibSerME_RC.o			
	echo "Compilation de admin"
	$(CC) -o admin	admin.c	LibSerME_RC.o 

udplib.o: ../udplib/udplib.h  ../udplib/udplib.c
	echo "Compilation de udplib.o"
	$(CC) -c ../udplib/udplib.c

cli: cli.c  requeteme_rc.h  data.h  udplib.o
	echo "Compilation de client"
	$(CC) -o cli cli.c	udplib.o LibSerME_RC.o $(LIBS)

ser: ser.c  requeteme_rc.h  data.h  udplib.o LibSerME_RC.o
	echo "Compilation de serveur"
	$(CC) -o ser ser.c  udplib.o LibSerME_RC.o  $(LIBS)

clean:
	rm -rf *.o $(EXECS)
