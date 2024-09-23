TARGET = amd64
PREFIX = /usr/local
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
OBJS2 = $(patsubst %.s,%.o,$(wildcard arch/$(TARGET)/*.s))

start.o: arch/$(TARGET)/start.o
	ln arch/$(TARGET)/start.o start.o
libc.a: $(OBJS1) $(OBJS2)
	$(AR) rcs $@ $(OBJS1) $(filter-out arch/$(TARGET)/start.o, $(OBJS2))

INLIB = $(DESTDIR)$(PREFIX)/lib
ININC = $(DESTDIR)$(PREFIX)/include

install: start.o libc.a
	install -vd $(INLIB)
	install -vd $(ININC)
	install -vm 755 libc.a $(INLIB)/libc.a
	install -vm 644 start.o $(INLIB)/crt1.o
	cp -rv include/* $(ININC)

uninstall:
	rm -vf $(INLIB)/libc.a
	rm -vf $(INLIB)/crt1.o
	for file in $$(cd include; find . -type f); do \
		rm -vf "$(ININC)/$$file"; \
	done

	for dir in $$(cd include; find . -type d | tail -n +2); do \
		rm -rvf "$(ININC)/$$dir"; \
	done

clean:
	rm -f $(OBJS1) start.o *.a $(OBJS2)
