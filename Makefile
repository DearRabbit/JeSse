CC = clang
DBGFLAGS = -Wall -g -O0
LDFLAGS = 

.PHONY = clean all test

ALL_OBJECT =
TEST_TARGET = test
TARGET = 

all: 

object.o: object.c
	$(CC) $(DBGFLAGS) -c $^

numobject.o: numobject.c
	$(CC) $(DBGFLAGS) -c $^

stringobject.o: stringobject.c
	$(CC) $(DBGFLAGS) -c $^

boolobject.o: boolobject.c
	$(CC) $(DBGFLAGS) -c $^

dictobject.o: dictobject.c
	$(CC) $(DBGFLAGS) -c $^

funcobject.o: funcobject.c
	$(CC) $(DBGFLAGS) -c $^

typeobject.o: typeobject.c
	$(CC) $(DBGFLAGS) -c $^

operator.o: operator.c
	$(CC) $(DBGFLAGS) -c $^

runtime.o: runtime.c
	$(CC) $(DBGFLAGS) -c $^

jsvm.o: jsvm.c
	$(CC) $(DBGFLAGS) -c $^

test: object.o typeobject.o numobject.o stringobject.o boolobject.o\
	dictobject.o funcobject.o operator.o runtime.o jsvm.o testunit.o
	$(CC) $(DBGFLAGS) $(LDFLAGS) $^ -o $(TEST_TARGET)

testunit.o: testunit.c
	$(CC) $(DBGFLAGS) -c $^

clean:
	rm -rf *.dSYM/
	rm -f *.o
	rm -f $(TEST_TARGET)
