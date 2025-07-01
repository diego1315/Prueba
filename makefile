CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I/usr/include -I/usr/include/ncurses
LDFLAGS = -lncurses  

TARGET = space_race
SRCS = main.cpp Menu.h ScoreSystem.h UserRegistration.h Game.h List.h DLinkedList.h DNode.h KVPair.h 

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
