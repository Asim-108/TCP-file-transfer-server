CC = gcc
CFLAGS1 = -Wall -g -c
CFLAGS2 = -g

all: client server temp

# Target for the "deliver" executable.
client: client.o
	${CC} ${CFLAGS2} -o client client.o

# Target for the "server" executable.
server: server.o
	${CC} ${CFLAGS2} -o server server.o

# Target for the "server" executable.
temp: temp.o
	${CC} ${CFLAGS2} -o temp temp.o

# Compile the "deliver" source file to an object.
client.o: client.c
	${CC} ${CFLAGS1} -o client.o client.c

# Compile the "server" source file to an object.
server.o: server.c
	${CC} ${CFLAGS1} -o server.o server.c

# Compile the "server" source file to an object.
temp.o: temp.c
	${CC} ${CFLAGS1} -o temp.o temp.c

clean:
	rm -f deliver server temp
