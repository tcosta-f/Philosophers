NAME = philo
INCLUDE = -I include
SRC = $(wildcard src/*.c)
OBJ_DIR = obj
OBJ = $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRC))

RM = @rm -rf
CC = @cc -g -pthread -Wextra -Werror -Wall $(INCLUDE)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

# Regra para criar o diretório obj se não existir e compilar os objetos
$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $@ 

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean
	$(MAKE) all

.PHONY: all clean fclean re
