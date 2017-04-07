CC = gcc
CFLAGS = -g -Wall
CLNT_OBJS = db_ccmd.o
SVR_OBJS = db_scmd.o

all: $(CLNT_OBJS) $(SVR_OBJS)
	$(CC) $(CFLAGS) -lc -o clnt $(CLNT_OBJS)
	$(CC) $(CFLAGS) -lc -o svr $(SVR_OBJS)

client: $(CLNT_OBJS)
	$(CC) $(CFLAGS) -lc -o clnt $(CLNT_OBJS)

server: $(SVR_OBJS)
	$(CC) $(CFLAGS) -lc -o svr $(SVR_OBJS)

clean:
	$(RM) *.o , clnt , svr , *# , *~
	@echo Cleaning done.

$(CLNT_OBJS): database.h
$(SVR_OBJS): database.h
