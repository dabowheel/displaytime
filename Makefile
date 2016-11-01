CFLAGS = -Wall -O3 -g
LDFLAGS = -lfcgi -laqua -lsqlite3 -lrandomid -lgmp -lcrypto
LDPATH = -L/usr/local/lib
object_files = main.o route.o com.o auth.o util.o session.o

all: public/build.js api

public/build.js: ui/*
	webpack

api: build app

build:
	mkdir -p build

app:
	make -C build -f ../Makefile displaytime

displaytime: $(object_files)
	gcc -o displaytime $(object_files) $(LDPATH) $(LDFLAGS)

%.o: ../api/%.c
	gcc -c $< $(CFLAGS)

clean:
	rm -rf build
	rm -f public/build.js

deploy:
	rm -rf /var/www/html/*
	cp -R public/* /var/www/html
	script/restart.sh