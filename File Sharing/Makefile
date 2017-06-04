#####################################################################
# Makefile
#
# Author: Raymond Weiming Luo
#
#####################################################################

CC = cc
CFLAGS = -g -c
OFLAGS = -g -o
SEED_OBJS = seed.o network.o
LEECH_OBJS = leech.o network.o
STREAM_OBJS = stream.o network.o

make: stream.c leech.c seed.c network.c
	$(CC) $(CFLAGS) stream.c
	$(CC) $(CFLAGS) seed.c
	$(CC) $(CFLAGS) leech.c
	$(CC) $(CFLAGS) network.c
	$(CC) $(CFLAGS) network.h
	$(CC) $(OFLAGS) stream $(STREAM_OBJS) -lm
	$(CC) $(OFLAGS) seed $(SEED_OBJS) -lm
	$(CC) $(OFLAGS) leech $(LEECH_OBJS) -lm

stream: stream.c network.c
	$(CC) $(CFLAGS) stream.c
	$(CC) $(CFLAGS) network.c
	$(CC) $(CFLAGS) network.h
	$(CC) $(OFLAGS) stream $(STREAM_OBJS)

leech: leech.c network.c
	$(CC) $(CFLAGS) leech.c
	$(CC) $(CFLAGS) network.c
	$(CC) $(CFLAGS) network.h
	$(CC) $(OFLAGS) leech $(LEECH_OBJS)

seed: seed.c network.c
	$(CC) $(CFLAGS) seed.c
	$(CC) $(CFLAGS) network.c
	$(CC) $(CFLAGS) network.h
	$(CC) $(OFLAGS) seed $(SEED_OBJS)

clean:
	$(RM) *.o , *~ , *.gch , seed , leech , stream
	@echo Cleaned up.
