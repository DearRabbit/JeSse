CC = gcc
LEX = flex
YACC = bison -dtv
DBGFLAGS = -Wall -g -O0
LDFLAGS =

.PHONY: clean all test

ALL_OBJECT =
TEST_TARGET = test
TARGET = jesse

all: $(TARGET)

$(TARGET): object.o numobject.o stringobject.o boolobject.o dictobject.o\
	funcobject.o typeobject.o operator.o runtime.o \
	lex.yy.o yacc.tab.o jsast.o jesse.o main.o
	$(CC) $(DBGFLAGS) $(LDFLAGS) $^ -o $@

main.o: main.c
	$(CC) $(DBGFLAGS) -c $<
jesse.o: jesse.c jesse.h
	$(CC) $(DBGFLAGS) -c $<

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

test: object.o typeobject.o numobject.o stringobject.o boolobject.o\
	dictobject.o funcobject.o operator.o runtime.o testunit.o
	$(CC) $(DBGFLAGS) $(LDFLAGS) $^ -o $(TEST_TARGET)

testunit.o: testunit.c
	$(CC) $(DBGFLAGS) -c $^

lex.yy.o: lex.yy.c
	$(CC) $(DBGFLAGS) -c $^

yacc.tab.o: yacc.tab.c
	$(CC) $(DBGFLAGS) -c $^ $(COMFLAGS)

lex.yy.c: lex.l yacc.tab.h
	$(LEX) $<

yacc.tab.h yacc.tab.c: yacc.y
	$(YACC) $<

jsast.o: jsast.c
	$(CC) -o $@ -c $< $(DBGFLAGS)

test_parser.o: test_parser.c
	$(CC) -c $<

# used stl somewhere...
test_parser: CC=g++
test_parser: jsast.o lex.yy.o yacc.tab.o test_parser.o
	$(CC) -o $@ $^ $(DBGFLAGS)

clean:
	rm -rf *.dSYM/
	rm -f *.o
	rm -f $(TEST_TARGET) $(TARGET)
	rm -f yacc.tab.* lex.yy.c yacc.output
	rm -f *.exe
