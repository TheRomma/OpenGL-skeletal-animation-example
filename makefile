CXX := g++
CXXFLAGS := -Wall -std=c++2a -MD -MP -g
LDFLAGS := -lstdc++ -lpthread -lSDL2 -lGL -lGLEW -ldl
TARGET := executable

all: $(TARGET)

$(TARGET): $(patsubst %.cpp,%.o,$(wildcard *.cpp))
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

-include $(wildcard *.d)

clean:
	@ rm *.o
	@ rm *.d

run:
	@ ./$(TARGET)
