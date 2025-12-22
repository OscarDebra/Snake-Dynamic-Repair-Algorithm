CC = g++
CFLAGS = -std=c++23 -Wall -I/opt/homebrew/Cellar/raylib/5.5/include
LDFLAGS = -L/opt/homebrew/Cellar/raylib/5.5/lib -lraylib
TARGET = main

SRCS = main.cpp SnakeClass/SnakeClass.cpp Variables/Variables.cpp Foodclass/FoodClass.cpp GameClass/GameClass.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
	@$(MAKE) clean

repairrun: 
	g++ ./repair.cpp -o repair && ./repair && rm ./repair

repair2run:
	g++ ./repairWithAdjacencyList.cpp -o repairWithAdjacencyList && ./repairWithAdjacencyList && rm ./repairWithAdjacencyList
