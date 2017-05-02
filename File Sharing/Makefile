CC = gcc
CFLAGS = -g
OFLAGS = -g -o
SEED_OBJS = seed.o
LEECH_OBJS = leech.o

make: $(SEED_OBJS) $(LEECH_OBJS)
	$(CC) $(CFLAGS) -o seed $(SEED_OBJS)
	$(CC) $(CFLAGS) -o leech $(LEECH_OBJS)

leech:
	$(CC) $(CFLAGS) -o leech $(LEECH_OBJS)

seed:
	$(CC) $(CFLAGS) -o seed $(SEED_OBJS)

clean:
	$(RM) *.o , *~ , *.gch
	@echo Cleaned up.
