PROYECTO=DFT
CFLAGS=-Wall 
LFLAGS=-lm
CC=gcc

all: $(PROYECTO)

%.o: %.c
	$(CC) -c $< $(CFLAGS) 

$(PROYECTO) : $(patsubst %.c,%.o,$(wildcard *.c))
	$(CC) $^ -o $@ $(LFLAGS) 

.PHONY: clean

clean:
	rm -f *.o $(PROYECTO)

