CFLAGS = -Wall -O3 -g
main: main.o
	gcc -o main main.o -lfcgi -L/usr/local/lib -laqua
main.o: main.c
	gcc -c main.c $(CFLAGS)
clean:
	rm -f main *.o
deploy:
	cp -R public/* /var/www/html
