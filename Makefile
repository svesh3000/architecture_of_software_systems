CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2

TARGET = course_work

SOURCES = \
	main.cpp \
	console/console_application.cpp \
	console/input.cpp \
	console/output.cpp \
	console/automatic_mode.cpp \
	console/step_mode.cpp \
	simulator/simulator.cpp \
	simulator/buffer.cpp \
	simulator/device.cpp \
	simulator/producer.cpp \
	simulator/source_statistics.cpp \
	simulator/dispatchers.cpp

OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

rebuild: clean all

.PHONY: all run clean rebuild
