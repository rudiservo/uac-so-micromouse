To build just do make, it will make two programs in the builds folder
socker_server and socket_client

the socker_server accepts an argument that is the path of the map
the socket_client accepst two argument that are the ip and the port

the port will be showned in the bottom of the map in the server

On the Client use WASD to move and Q to quit (lowercase)

The server you can always "CTRL" + "C" to terminate.


you have also some other programs to compile has a Make argument


cursor: to move along the screen, buffer overflow protection is not implemented

sockets: copied from the the Prof José Cascalho from UAc

labyrinth: loading of a labyrinth only accepts the map path argument

single_mouse: single user with no server of a labyrinth exploration. does not use termios so "CTRL" + "C" wont work.