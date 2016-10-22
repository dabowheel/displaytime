CFLAGS = -Wall -O3 -g
LDFLAGS = -lfcgi -laqua -lsqlite3
LDPATH = -L/usr/local/lib
all: public/build.js displaytime
public/build.js: src/*.js src/*.vue
	webpack
displaytime: main.o
	gcc -o displaytime main.o $(LDPATH) $(LDFLAGS)
main.o: main.c
	gcc -c main.c $(CFLAGS)
clean:
	rm -f displayname *.o
deploy:
	rm -rf /var/www/html/*
	cp -R public/* /var/www/html
	bin/restart.sh