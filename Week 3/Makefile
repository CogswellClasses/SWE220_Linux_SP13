objects = hello.o msg.o

hello: $(objects)
	gcc -o hello $(objects)

*.o: *.c

anew:
	make
	./hello
