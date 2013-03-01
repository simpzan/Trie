CC=g++
CFLAGS=-ggdb 

GTEST= /Volumes/Docs/workspace/gtest-1.6.0
INCLUDE= -I ${GTEST}/include
LIB= ${addprefix ${GTEST}/, libgtest.a gtest_main.a}

SRC_TEST=ArrayTrieTests.cpp ArrayTrieNodeTests.cpp
SRC=ArrayTrie.cpp ArrayTrieNode.cpp ${SRC_TEST}
OBJ=${SRC:.cpp=.o}
BIN_TEST=test_main.out
BIN=main.out ${BIN_TEST}

all: ${BIN} test

main: main.o ArrayTrie.o ArrayTrieNode.o
	${CC} ${CFLAGS} $^ -o $@

${BIN_TEST}: ArrayTrie.o ArrayTrieNode.o  ArrayTrieNodeTests.o ArrayTrieTests.o
	${CC} $^ -o $@  ${LIB}

%.o: %.cpp
	${CC} $< -o $@ -c ${INCLUDE} ${CFLAGS}

test: 
	${addprefix ./, ${BIN_TEST}}

clean:
	rm -f *.o ${BIN} depend


depend: ${SRC}
	${CC} -MM $^ > depend


include depend
