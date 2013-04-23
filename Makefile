CC=g++
CFLAGS=-ggdb -fpermissive

GTEST= /usr/local
INCLUDE= -I ${GTEST}/include -I src/ -I ../utils/src/ \
		 -I ../taiju-0.0.3/include/taiju/
LIB= ${addprefix ${GTEST}/lib/, libgtest.a libgtest_main.a} \
	 ../taiju-0.0.3/lib/libtaiju.a

D_TEST=test
D_SRC=src
D_OBJ=obj
D_BIN=bin
D_SAMPLE=tool

DIR_TO_CREATE=${D_OBJ} ${D_BIN}
VPATH=${D_SRC}/:${D_OBJ}/:${D_TEST}/:${D_SAMPLE}/: ../utils/obj

SUFFIX=cpp

SRC_TEST=${wildcard ${D_TEST}/*.cpp}
OBJ_TEST=${patsubst ${D_TEST}/%.cpp, ${D_OBJ}/%.o, ${SRC_TEST}}
BIN_TEST=gtest_main

SRC_SAMPLE=${wildcard ${D_SAMPLE}/*.${SUFFIX}}
OBJ_SAMPLE=${patsubst ${D_SAMPLE}/%.cpp, ${D_OBJ}/%.o, ${SRC_SAMPLE}}

SRC=$(wildcard ${D_SRC}/*.cpp)
OBJ=${patsubst ${D_SRC}/%.cpp, ${D_OBJ}/%.o, ${SRC}}  ${OBJ_TEST} ${OBJ_SAMPLE}
BIN=${addprefix ${D_BIN}/, main ${BIN_TEST} test_AMT }



all: printSeparator ${D_BIN} ${BIN} 
.PHONY: all

# generate dependency files.
${D_OBJ}/%.d : %.cpp 
	@mkdir -p ${D_OBJ}
	${CC} > $@    $< -MP -MM -MT ${@D}/${*F}.o ${INCLUDE}
include $(OBJ:%.o=%.d)

# compile cpp files.
${D_OBJ}/%.o : %.cpp
	$(CC) $< -o $@ -c ${INCLUDE} ${CFLAGS}

# clean all build output files.
clean:
	rm -rf ${D_OBJ} ${D_BIN} *.o
.PHONY: clean

# run test cases.
test: all
	./${D_BIN}/${BIN_TEST}
.PHONY: test

# create dirs.
${DIR_TO_CREATE}:
	mkdir -p $@

printSeparator:
	@echo
	@echo ====== dependency generation finished =======
	@echo
.PHONY: printSeparator


# bin targets
${D_BIN}/main: TrieNode.o main.o
	${CC} -o $@  $^  ${LIB} 

${D_BIN}/test_AMT: test_AMT.o TrieNode.o AMTrieNode.o utils.o
	${CC} -o $@  $^  ${LIB}

${D_BIN}/${BIN_TEST}: ${OBJ_TEST}  TrieNode.o AMTrieNode.o LinkedTrieNode.o utils.o
	${CC} -o $@  $^  ${LIB}

