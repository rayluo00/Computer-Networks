CC = gcc
CFLAGS = -g -Wall
CLNT_OBJS = db_ccmd.o database_clnt.o rpc_calls.o
SVR_OBJS = db_scmd.o database_svc.o db_server.o

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
