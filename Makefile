# ======================================================= #
#                                                         #
#                      Definitions                        #
#                                                         #
# ======================================================= #

CC 	   	= 	g++
CFLAGS 	= 	-Wall -O2
TARGET 	= 	ROUTER.exe

SRC_DIR = 	./src
OBJ_DIR = 	./obj

SRC 	= 	${wildcard $(SRC_DIR)/*.cpp}
OBJ 	= 	${patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC)}
LIB 	= 	-lpthread -lm

# ======================================================= #
#                                                         #
#                      Commands                           #
#                                                         #
# ======================================================= #

.PHONY: all clean

all : clean init $(TARGET)

init :
	if [ ! -d "obj" ]; then mkdir obj; fi

clean :
	rm -rf obj
	if [ -f $(TARGET) ]; then rm $(TARGET); fi

project : clean init $(TARGET)
	time ./ROUTER.exe -d=1 -n=1 benchmarks/adaptec1.gr benchmarks/adaptec1.out
	./556_eval.exe benchmarks/adaptec1.gr benchmarks/adaptec1.out 0
	./556_eval.exe benchmarks/adaptec1.gr benchmarks/adaptec1.out 1
	time ./ROUTER.exe -d=1 -n=1 benchmarks/adaptec2.gr benchmarks/adaptec2.out
	./556_eval.exe benchmarks/adaptec2.gr benchmarks/adaptec2.out 0
	./556_eval.exe benchmarks/adaptec2.gr benchmarks/adaptec2.out 1
	time ./ROUTER.exe -d=1 -n=1 benchmarks/adaptec3.gr benchmarks/adaptec3.out
	./556_eval.exe benchmarks/adaptec3.gr benchmarks/adaptec3.out 0
	./556_eval.exe benchmarks/adaptec3.gr benchmarks/adaptec3.out 1
	
leak:  clean init $(TARGET)
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./ROUTER.exe -d=1 -n=1 benchmarks/test.gr benchmarks/test.out

test1: clean init $(TARGET)
	time ./ROUTER.exe -d=1 -n=1 benchmarks/adaptec1.gr benchmarks/adaptec1.out
	./556_eval.exe benchmarks/adaptec1.gr benchmarks/adaptec1.out 0
	./556_eval.exe benchmarks/adaptec1.gr benchmarks/adaptec1.out 1

test2: clean init $(TARGET)
	time ./ROUTER.exe -d=1 -n=1 benchmarks/adaptec2.gr benchmarks/adaptec2.out
	./556_eval.exe benchmarks/adaptec2.gr benchmarks/adaptec2.out 0
	./556_eval.exe benchmarks/adaptec2.gr benchmarks/adaptec2.out 1

test3: clean init $(TARGET)
	time ./ROUTER.exe -d=1 -n=1 benchmarks/adaptec3.gr benchmarks/adaptec3.out
	./556_eval.exe benchmarks/adaptec3.gr benchmarks/adaptec3.out 0
	./556_eval.exe benchmarks/adaptec3.gr benchmarks/adaptec3.out 1


test: clean init $(TARGET)
	./ROUTER.exe -d=1 -n=1 benchmarks/test.gr benchmarks/test.out
	./556_eval.exe benchmarks/test.gr benchmarks/test.out 0
	./556_eval.exe benchmarks/test.gr benchmarks/test.out 1

evaluate : clean init $(TARGET)
	sh ./testing/evaluate.sh

toy : clean init $(TARGET)
	./ROUTER.exe -d=1 -n=1 benchmarks/input benchmarks/output


$(TARGET) 		: 	$(OBJ)
	$(CC) $(CFLAGS) -o $@ $? $(LIB)

${OBJ_DIR}/%.o	:	${SRC_DIR}/%.cpp
	$(CC) -c $< -o $@

