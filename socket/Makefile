CPPFLAGS =-W -Wall -pedantic -ansi
CC = g++
BIN = ./bin/
EXEFILE1 = server
EXEFILE2 = client
OBJ_FILES1 = server.o 
OBJ_FILES2 = client.o

$(EXEFILE1) : $(OBJ_FILES1)
	$(CC) -o $(BIN)$@ $(OBJ_FILES1) 

$(EXEFILE2) : $(OBJ_FILES2)
	$(CC) -o $(BIN)$@ $(OBJ_FILES2) 	

%.o : %.cpp
	$(CC) -o $@ -c $(CPPFLAGS) $^

.DEFAULT_GOAL = all

all : $(EXEFILE1) $(EXEFILE2) 

.PHONY : clean

clean :
	rm -rf $(OBJ_FILES1) $(EXEFILE1) $(OBJ_FILES2) $(EXEFILE2) *.s 	