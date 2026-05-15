CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -Iinclude
TARGET = bin/virtualDisk

# Updated for your Virtual Disk files
SRCS = src/display.cpp \
       src/files.cpp \
       src/main.cpp \
       src/utils.cpp \
	   src/validation.cpp

OBJS = $(SRCS:.cpp=.o)

all: dir $(TARGET)

dir:
	mkdir -p bin

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o
	rm -rf bin