epoch: epoch.c
	gcc `pkg-config --cflags gtk+-3.0` -o epoch epoch.c `pkg-config --libs gtk+-3.0`

install: epoch
	cp epoch /usr/bin

