CC = gcc

BIN = ./bin/
OBJ= ./Obj/
EXEFILE = output
OBJ_FILES = $(addprefix $(OBJ),main.o test.o array_manipulation.o)

$(EXEFILE) : $(OBJ_FILES)
	$(CC) -o $(BIN)$@ $(OBJ_FILES) 

$(OBJ)%.o : %.c
	$(CC) -o $@ -c  $<

.DEFAULT_GOAL = all

all : $(EXEFILE) 

.PHONY : clean

clean :
	rm -rf $(OBJ_FILES) $(BIN)$(EXEFILE) *.s 