CC := g++
BIN_DIR := bin
OUT := $(BIN_DIR)/chess
OBJ_DIR := obj
DIR_SRC := src src/core src/chess
INC := -Isrc
SRC := $(wildcard $(addsuffix /*.cpp, $(DIR_SRC)))
OBJ := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC))
LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system

all: create_dirs copy_res $(OBJ) $(OUT)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OUT): $(OBJ)
	@mkdir -p $(@D)
	$(CC) $(LDFLAGS) $(OBJ) -o $@

create_dirs:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

copy_res:
	@rm -rf $(BIN_DIR)/res
	@cp -r res $(BIN_DIR)

clean:
	rm $(OBJ) $(OUT)