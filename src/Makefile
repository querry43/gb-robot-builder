CC = lcc

all: robot-builder.gb

robot-builder.gb: robot-builder.o agent.o ../tile-data/*.o

test.gb: test.o

config=config.h

%.gb:
	$(CC) -o $@ $^

%.o: %.c %.h $(config)
	$(CC) -c -o $@ $<

robot-builder.o: robot-builder.c *.h $(config)

clean:
	rm -f *.o *.gb *.sav

.PHONY: clean
