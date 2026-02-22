NAME        = endgame

CC          = clang
CFLAGS      = -Wall -Wextra -Werror -I$(INC_DIR)
LDFLAGS     = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

SRC_DIR     = src
INC_DIR     = inc
OBJ_DIR     = obj

SRC         = $(wildcard $(SRC_DIR)/*.c)
OBJ         = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(OBJ) -o $@ $(LDFLAGS)
	@printf "Project %s successfully built!\n" $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@printf "Object files deleted.\n"

uninstall: clean
	@rm -f $(NAME)
	@printf "Executable %s deleted.\n" $(NAME)

reinstall: uninstall all

.PHONY: all clean uninstall reinstall
