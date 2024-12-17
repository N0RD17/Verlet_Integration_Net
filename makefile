CXX	= g++
CFLAGS	= -Wall -Wextra
CLIBS	= -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
CFILES	= main.cpp

# Direcotires
OBJ_DIR = obj

# Shell command
${shell mkdir -p ${OBJ_DIR}}

# Source files
SRCS = $(wildcard ./*.cpp)

# Object Files
OBJS = $(patsubst ./%.cpp, ${OBJ_DIR}/%.o, ${SRCS})

COUT = Particle

all: compile
.PHONY: all

compile: ${OBJS}
	${CXX} ${CFLAGS} -o ${COUT} $^ ${CLIBS}
.PHONY: compile

${OBJ_DIR}/%.o: %.cpp
	${CXX} ${CFLAGS} -c $< -o $@

# Rule to create the object direction if it doesn't exist
$(OBJ_DIR):
	@mkdir -p ${OBJ_DIR}

run: ${COUT}
	make clean && make && ./${COUT}
.PHONY: run

clean:
	@echo "Cleaning up..."
	@rm -rf ${OBJ_DIR} ${COUT}
.PHONY: clean

