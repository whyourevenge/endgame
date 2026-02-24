NAME        = endgame

CC          = clang
CFLAGS      = -Wall -Wextra -Werror -I$(INC_DIR)
LDFLAGS     = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

SRC_DIR     = src
INC_DIR     = inc
OBJ_DIR     = obj

SRC         = $(shell find $(SRC_DIR) -name "*.c")
OBJ         = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(OBJ) -o $@ $(LDFLAGS)
	@printf "Project %s successfully built!\n" $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@printf "Object files deleted.\n"

uninstall: clean
	@rm -f $(NAME)
	@printf "Executable %s deleted.\n" $(NAME)

reinstall: uninstall all

.PHONY: all clean uninstall reinstall