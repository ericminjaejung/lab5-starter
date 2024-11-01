number-server:	number-server.c
	gcc number-server.c http-server.c -o number-server -std=c11
