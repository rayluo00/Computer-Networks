CC = cc
CFLAGS = -g -c
OFLAGS = -lm -o
SEED_OBJS = seed.o
LEECH_OBJS = leech.o

make: seed.c leech.c
	$(CC) $(CFLAGS) seed.c $(OFLAGS) $(SEED_OBJS)
	$(CC) $(CFLAGS) leech.c $(OFLAGS) $(LEECH_OBJS)
	$(CC) $(OFLAGS) seed $(SEED_OBJS)
	$(CC) $(OFLAGS) leech $(LEECH_OBJS)

leech:
	$(CC) $(CFLAGS) -o leech $(LEECH_OBJS)

seed:
	$(CC) $(CFLAGS) -o seed $(SEED_OBJS)

clean:
	$(RM) *.o , *~ , *.gch
	@echo Cleaned up.
