CC = clang
DBGFLAGS = -g -O0
LDFLAGS = 

.PHONY = clean all test

all: 

object.o: object.c
	$(CC) $(DBGFLAGS) -c $^

intobject.o: intobject.c
	$(CC) $(DBGFLAGS) -c $^

typeobject.o: typeobject.c
	$(CC) $(DBGFLAGS) -c $^
	

test: object.o typeobject.o intobject.o testunit.o 
	$(CC) $(DBGFLAGS) $(LDFLAGS) $^ -o test

testunit.o: testunit.c
	$(CC) $(DBGFLAGS) -c $^

clean:
	rm -f *.o
	rm test
