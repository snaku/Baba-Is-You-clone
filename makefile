CXX = g++

TARGET = bin/BabaIsYou.exe

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

SRCS = $(shell find $(SRC_DIR) -name "*.cpp")
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CXXFLAGS = -std=c++17 -MMD -MP -O2 -I$(INC_DIR) $(shell pkg-config --cflags sdl2)

LDFLAGS = $(shell pkg-config --libs \
			sdl2 SDL2_image)

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p bin
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

-include $(OBJS:.o=.d)

.PHONY: all clean