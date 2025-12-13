
OBJS := src/_start.o \
	src/init.o \
	src/lib.o \
	src/alloc/heap.o \
	src/c/string.o \
	src/c/syscalls.o \
	src/io.o \
	src/memory.o

.PHONY: all clean
.SUFFIXES: .c .s .o

CPPFLAGS += -Iinclude
CFLAGS += -std=c23 -nostdinc -fno-stack-protector

all: libc.a echo cat

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

.s.o:
	$(AS) $(ASFLAGS) -o $@ $<

libc.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

echo: libc.a utils/echo.o
	$(LD) $(LDFLAGS) -o $@ utils/echo.o libc.a

cat: libc.a utils/cat.o
	$(LD) $(LDFLAGS) -o $@ utils/cat.o libc.a

clean:
	rm -f libc.a $(OBJS)
	rm -f echo utils/echo.o
	rm -f cat utils/cat.o

