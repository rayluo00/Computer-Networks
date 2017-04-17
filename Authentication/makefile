CC = gcc
CFLAGS = -g
CLNT_OBJS = database_xdr.o database_clnt.o db_ccmd.o db_client.o proto.h
SVR_OBJS = database_xdr.o database_svc.o db_scmd.o db_server.o proto.h

all: $(CLNT_OBJS) $(SVR_OBJS)
	$(CC) $(CFLAGS) -lc -lgdbm -o clnt $(CLNT_OBJS)
	$(CC) $(CFLAGS) -lc -lgdbm -o svr $(SVR_OBJS)

client: $(CLNT_OBJS)
	$(CC) $(CFLAGS) -lc -lgdbm -o clnt $(CLNT_OBJS)

server: $(SVR_OBJS)
	$(CC) $(CFLAGS) -lc -lgdbm -o svr $(SVR_OBJS)

clean:
	$(RM) *.o , clnt , svr , *# , *~
	@echo Cleaning done.

$(CLNT_OBJS): database.h
$(SVR_OBJS): database.h
