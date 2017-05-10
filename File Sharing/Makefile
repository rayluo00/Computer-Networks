CC = cc
CFLAGS = -g -c
OFLAGS = -lm -o
SEED_OBJS = seed.o network.o
LEECH_OBJS = leech.o network.o
STREAM_OBJS = stream.o network.o

make: stream.c
	$(CC) $(CFLAGS) stream.c
	$(CC) $(CFLAGS) network.c
	$(CC) $(CFLAGS) network.h
	$(CC) $(OFLAGS) stream $(STREAM_OBJS)

leech: leech.c
	$(CC) $(CFLAGS) leech.c
	$(CC) $(OFLAGS) leech $(LEECH_OBJS)

seed: seed.c
	$(CC) $(CFLAGS) seed.c
	$(CC) $(OFLAGS) seed $(SEED_OBJS)

clean:
	$(RM) *.o , *~ , *.gch , seed , leech , stream
	@echo Cleaned up.
