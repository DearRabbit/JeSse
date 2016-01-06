CC = clang
DBGFLAGS = -g -O0
LDFLAGS = 

.PHONY = clean all test

TEST_TARGET = test
TARGET = 

all: 

object.o: object.c
	$(CC) $(DBGFLAGS) -c $^

numobject.o: numobject.c
	$(CC) $(DBGFLAGS) -c $^

stringobject.o: stringobject.c
	$(CC) $(DBGFLAGS) -c $^

typeobject.o: typeobject.c
	$(CC) $(DBGFLAGS) -c $^

runtime.o: runtime.c
	$(CC) $(DBGFLAGS) -c $^
	

test: object.o typeobject.o numobject.o stringobject.o runtime.o testunit.o 
	$(CC) $(DBGFLAGS) $(LDFLAGS) $^ -o $(TEST_TARGET)

testunit.o: testunit.c
	$(CC) $(DBGFLAGS) -c $^

clean:
	rm -rf *.dSYM/
	rm -f *.o
	rm -f $(TEST_TARGET)
