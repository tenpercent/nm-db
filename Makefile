CXX = g++
CXXFLAGS = -std=c++14 -c -Wall
LD = g++
LDFLAGS = -lsqlite3
SOURCES = *.cpp
OBJECTS = *.o
PROJECT = nm-db

all: compile link

compile:
	$(CXX) $(CXXFLAGS) $(SOURCES)
link:
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(PROJECT)
clean:
	rm -f $(OBJECTS) $(PROJECT)
