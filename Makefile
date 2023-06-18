all: MyShell.o MyCd.o MyFunc.o Myls.o Mycp.o Mymv.o Myps.o Mygrep.o Help.o
	gcc MyShell.o MyFunc.o MyCd.o Myls.o Mycp.o Mymv.o  Myps.o  Mygrep.o Help.o -lreadline -o myShell && rm *.o

MyShell.o: MyShell.c
	gcc -c -g MyShell.c

MyCd.o: MyCd.c
	gcc -c -g MyCd.c

MyFunc.o: MyFunc.c 
	gcc -c -g MyFunc.c

Myls.o: Myls.c 
	gcc -c -g Myls.c

Mycp.o: Mycp.c
	gcc -c -g Mycp.c

Mymv.o: Mymv.c
	gcc -c -g Mymv.c

Myps.o: Myps.c
	gcc -c -g Myps.c

Mygrep.o: Mygrep.c
	gcc -c -g Mygrep.c

Help.o: Help.c
	gcc -c -g Help.c
clean:
	rm myShell && rm *.o
