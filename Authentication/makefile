CC = gcc
CFLAGS = -g -lc -lnsl -lgdbm
OFLAGS = -g -o -lc -lnsl -lgdbm
CLNT_OBJS = database_xdr.o database_clnt.o db_ccmd.o db_client.o
SVR_OBJS = database_xdr.o database_svc.o db_scmd.o db_server.o

make: $(CLNT_OBJS) $(SVR_OBJS)
	$(CC) $(CFLAGS) -o clnt $(CLNT_OBJS)
	$(CC) $(CFLAGS) -o svr $(SVR_OBJS)

client: $(CLNT_OBJS)
	$(CC) $(CFLAGS) -lc -lgdbm -o clnt $(CLNT_OBJS)

server: $(SVR_OBJS)
	$(CC) $(CFLAGS) -lc -lgdbm -o svr $(SVR_OBJS)

every: database_xdr.c database_clnt.c database_svc.c db_ccmd.c db_scmd.c db_client.c db_server.c
	$(CC) $(CFLAGS) database_xdr.c
	$(CC) $(CFLAGS) database_clnt.c
	$(CC) $(CFLAGS) database_svc.c
	$(CC) $(CFLAGS) db_ccmd.c
	$(CC) $(CFLAGS) db_scmd.c
	$(CC) $(CFLAGS) db_client.c
	$(CC) $(CFLAGS) db_server.c
	$(CC) $(CFLAGS) database.h
	$(CC) $(CFLAGS) proto.h

clean:
	$(RM) *.o , clnt , svr , *# , *~ , *.gch
	@echo Cleaning done.

$(CLNT_OBJS): database.h
$(SVR_OBJS): database.h
