BIN = skeleton

CC = g++
RM = rm -f
LDFLAGS = -std=c++11 -Wall -g
LDLIBS = -pthread

all:
	$(CC) $(LDFLAGS) skeleton.cpp $(LDLIBS) -o $(BIN)

clean:
	$(RM) *.o $(BIN)
