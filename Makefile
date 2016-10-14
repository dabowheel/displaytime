CFLAGS = -Wall -O3 -g
all: build.js
build.js:
	webpack
wmain: main.o
	gcc -o main main.o -lfcgi -L/usr/local/lib -laqua
main.o: main.c
	gcc -c main.c $(CFLAGS)
clean:
	rm -f main *.o
deploy:
	rm -rf /var/www/html/*
	cp -R public/* /var/www/html
