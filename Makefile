OUTBIN=fgbas
CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++14 -g -I include -I dependencies/commons/include/
OBJS=src/assembler.o\
     src/main.o

.PHONY: all clean

all: tolk

tolk: $(OBJS)
	$(CXX) -o $(OUTBIN) $(OBJS)

clean:
	rm -rf $(OBJS) $(OUTBIN)
