TARGET = amd64
CC = tcc
AR = tcc -ar
AS = tcc

CFLAGS = -O2 -Iinclude

all: start.o libc.a

%.o: %.s
	$(AS) -c $^ -o $@
%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

OBJS1 = $(patsubst %.c,%.o,$(wildcard src/*.c))
OBJS2 = $(patsubst %.s,%.o,$(wildcard cpu/$(TARGET)/*.s))

start.o: cpu/$(TARGET)/start.o
	ln cpu/$(TARGET)/start.o start.o
libc.a: $(OBJS1) $(OBJS2)
	$(AR) rcs $@ $(OBJS1) $(filter-out cpu/$(TARGET)/start.o, $(OBJS2))

clean:
	rm -f $(OBJS1) start.o *.a $(OBJS2)
