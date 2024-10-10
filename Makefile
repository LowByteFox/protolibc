# Config
TARGET = amd64
PREFIX = /usr/local
CC = tcc
AR = tcc -ar
AS = tcc
CFLAGS = -g -O2 -Iinclude

# Build spec
OBJS = src/__libc_start_main.o src/assert.o src/atoi.o src/ctype.o			 \
	src/dirent.o src/errno.o src/isatty.o src/localtime.o src/mkstemp.o		 \
	src/pledge.o src/qsort.o src/rand.o src/regex.o src/scanf.o src/signal.o \
	src/stdarg.o src/stdio.o src/stdlib.o src/strftime.o src/stringc.o		 \
	src/termios.o src/string.o src/unistd.o src/arena.o src/malloc.o		 \
	arch/$(TARGET)/bits.o arch/$(TARGET)/lmem.o arch/$(TARGET)/setjmp.o		 \
	arch/$(TARGET)/syscall.o

all: arch/$(TARGET)/syscall.s libc.a start.o

start.o: arch/$(TARGET)/start.o
	ln $? $@

libc.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

syscall2gen: syscall2gen.c
	$(CC) syscall2gen.c -o syscall2gen

arch/$(TARGET)/syscall.s: base/syscall.s syscall2gen
	cp base/syscall.s arch/$(TARGET)/syscall.s
	./syscall2gen $(TARGET)

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
	for file in $$(cd include; find . -type f && find . -type d | tail -n +2); do \
		rm -rvf "$(ININC)/$$file"; \
	done

clean:
	rm -f $(OBJS) libc.a start.o syscall2gen

.s.o:
	$(AS) -c $< -o $@

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: all clean install uninstall
.SUFFIXES: .c .s
