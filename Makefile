number-server:	number-server.c
	gcc -D_GNU_SOURCE number-server.c http-server.c -o number-server -std=c11
