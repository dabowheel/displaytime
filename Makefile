CFLAGS = -Wall -O3 -g
LDFLAGS = -lfcgi -laqua -lsqlite3
LDPATH = -L/usr/local/lib
all: public/build.js api
public/build.js: ui/*
	webpack
api: build app
build:
	mkdir -p build
app:
	make -C build -f ../Makefile displaytime
displaytime: main.o
	gcc -o displaytime main.o $(LDPATH) $(LDFLAGS)
%.o: ../api/%.c
	gcc -c $< $(CFLAGS)
clean:
	rm -rf build
	rm -f public/build.js
deploy:
	rm -rf /var/www/html/*
	cp -R public/* /var/www/html
	script/restart.sh