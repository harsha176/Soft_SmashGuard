#
# Makefile -- for user threads library with synchronization
#
 
CC = gcc
INCLUDE_PATH=.
CFLAGS = -O1 -g -I$(INCLUDE_PATH)
CFLAGSG = -g -I$(INCLUDE_PATH)
 
TESTS = hello hello3 trigger genpage
 
all: $(TESTS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

hello: hello.o
	$(CC) $(CFLAGS) -o $@ $^

hello2: hello2.o
	$(CC) $(CFLAGS) -o $@ $^

hello3: align.o readString.o align2.o hello3.o align3.o sandbox.o 
	$(CC) $(CFLAGS) -o $@ $^

trigger: trigger.o
	$(CC) $(CFLAGS1) -o $@ $^

genpage: genpage.o
	$(CC) $(CFLAGS1) -o $@ $^

align.c: genpage
	./genpage 1 4096 > $@

align2.c: genpage
	./genpage 2 4096 > $@

align3.c: genpage
	./genpage 3 4096 > $@

clean:
	$(RM) align* $(TESTS) a.out core *.o *.BAK *.CKP *~ #*
