PROJECT=fast
PROGQWERTY=felix_smart
PROGS=$(PROGQWERTY)
SRCSQWERTY=src/qwertycode.c src/felix_smart.c

CFLAGS=-O3 -funroll-all-loops -finline-functions -Wall
LDFLAGS=-s
RM=rm -f

OBJSPBM=$(SRCSPBM:.c=.o)
OBJSG4=$(SRCSG4:.c=.o)
OBJSQWERTY=$(SRCSQWERTY:.c=.o)

all: $(PROGS)

clean:
	$(RM) $(PROGS) $(OBJSPBM) $(OBJSG4) $(OBJSQWERTY)


$(PROGQWERTY): $(OBJSPBM) $(OBJSQWERTY)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
