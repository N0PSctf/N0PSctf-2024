CC=gcc
CFLAGS =-g -O3 -s

lizwol: lizwol.o
	$(CC) $(CFLAGS) encoder.c $< -o $@

solve: solve.o
	$(CC) decoder.c $< -o $@

lzw.a: lizwol.o solve.o
	$(AR) -cq $@ $<

lizwol.o solve.o: %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

lizwol.o solve.o: lzw.h

clean:
	rm -f lizwol.o solve.o lzw.a lizwol solve