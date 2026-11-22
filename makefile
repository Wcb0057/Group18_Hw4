CC=g++
DEPS = bowling_frame.h

# Use g++ when make compiles .cpp files into .o files.
CXX = $(CC)

# Tell g++ to compile the source files using C++11.
CXXFLAGS += -std=c++11

all: bowling_frame.o main.o
	$(CC) -std=c++11 bowling_frame.o main.o

# Rebuild both .o files when bowling_frame.h changes.
bowling_frame.o main.o: $(DEPS)

bowling: bowling_frame.o $(DEPS)
	$(CC) -c -std=c++11 bowling_frame.cpp

main: main.o $(DEPS)
	$(CC) -c -std=c++11 main.cpp

clean:
	rm *.o *.out
