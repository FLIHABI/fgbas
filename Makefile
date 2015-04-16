OUTBIN=fgbas
CXX=g++
CXXFLAGS=-Wall -Wextra -Wno-unused-parameter -std=c++14 -g -I include -I dependencies/commons/include/
OBJS=src/assembler.o\
     src/mnemonics.o\
     src/main.o

.PHONY: all clean

all: fgbas

fgbas: $(OBJS)
	$(CXX) -o $(OUTBIN) $(OBJS)

clean:
	rm -rf $(OBJS) $(OUTBIN)
