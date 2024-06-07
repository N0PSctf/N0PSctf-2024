.PHONY: help all acquire attack check clean

.DEFAULT_GOAL := help
CFLAGS        += -O3
N             := 20000
DATA          := ta.dat
KEY           := ta.key

help:
	@sed -n '/^```bash/,/^```/{/^```/d;p;}' README.md

all: aes_check target ta

acquire: $(DATA) $(KEY)

$(DATA) $(KEY): ta.%: target
	./$< $N

attack: $(DATA) ta
	./ta $(DATA) $N

check: aes_check
	./$<

aes.o aes_check.o target.o ta.o: aes.h

aes_check: aes_check.o aes.o

target: target.o aes.o utils.o rdtsc_timer.o

ta: LDLIBS += -lm
ta: ta.o aes.o

clean:
	rm -f *.o aes_check target ta $(DATA) $(KEY)
