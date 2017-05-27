CC = gcc
CFLAGS = -g -c
OFLAGS = -g -o
SVR_OBJS = server.o network.o
CLNT_OBJS = client.o network.o

make: server.c client.c
	$(CC) $(CFLAGS) server.c
	$(CC) $(CFLAGS) client.c
	$(CC) $(CFLAGS) network.c
	$(CC) $(CFLAGS) network.h
	$(CC) $(OFLAGS) svr $(SVR_OBJS)
	$(CC) $(OFLAGS) clnt $(CLNT_OBJS)

clean:
	$(RM) *.o , svr , clnt , *.gch , *~ 
