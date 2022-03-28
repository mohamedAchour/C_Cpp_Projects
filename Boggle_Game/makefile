CC=gcc
LIBS=-lncurses -lm

BIN = ./bin/
OBJ= ./obj/
EXEFILE = boggle
OBJ_FILES = $(addprefix $(OBJ),main.o game.o)

$(EXEFILE) : $(OBJ_FILES)
	$(CC) -o $(BIN)$@ $(OBJ_FILES) ${LIBS}

$(OBJ)%.o : %.c
	$(CC) -o $@ -c  $<

.DEFAULT_GOAL = all

all : $(EXEFILE) 

.PHONY : clean

clean :
	rm -rf $(OBJ_FILES) $(BIN)$(EXEFILE) *.s 