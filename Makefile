PS_form : main.o ps_form.o
	gcc main.o ps_form.o -o PS_form  

main.o : main.c ps_form.h  
	gcc -c main.c -o main.o

ps_form.o : ps_form.c ps_form.h  
	gcc -c ps_form.c -o ps_form.o

clear:
	rm -f *.o PS_form