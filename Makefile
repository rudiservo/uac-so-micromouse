.DEFAULT_GOAL := build

.PHONY: build cursor sockets labyrinth single_mouse

cursor:
	@echo "compiling"
	gcc source/screencursor.c -o builds/screencursor

sockets:
	@echo "compiling Sockets"
	gcc source/socket_server.c -o builds/socket_server
	gcc source/socket_client.c -o builds/socket_client


labyrinth:
	@echo "compiling Labyrinth"
	gcc source/labyrinth.c -o builds/labytinth

single_mouse:
	@echo "compiling Single Mouse"
	gcc source/single_mouse.c -o builds/single_mouse

build:
	@echo "bulding server"
	gcc source/socket_server.c -o builds/socket_server
	@echo "bulding client"
	gcc source/socket_client.c -o builds/socket_client