# 

OBJ = obj
SRC = src
BIN = bin
INC = include
OUT = output
GEN = generated
MAINSRC = ./Robert.txt

.PHONY: all

all: crear clean $(BIN)/semantico

$(BIN)/traduccion: $(SRC)/traduccion.c
		gcc $(SRC)/traduccion.c -o $(BIN)/traduccion
$(BIN)/semantico: $(OBJ)/main.o $(OBJ)/y.tab.o $(OBJ)/semantico.o
		gcc -o $(BIN)/semantico $(OBJ)/main.o $(OBJ)/y.tab.o $(OBJ)/semantico.o
$(OBJ)/semantico.o: $(SRC)/semantico.c
		gcc -c $(SRC)/semantico.c -I$(INC) -o $(OBJ)/semantico.o
$(OBJ)/y.tab.o: $(GEN)/y.tab.c
		gcc -c $(GEN)/y.tab.c -I$(INC) -o $(OBJ)/y.tab.o
$(OBJ)/main.o: $(SRC)/main.c
		gcc -c $(SRC)/main.c -I$(INC) -o $(OBJ)/main.o
$(GEN)/y.tab.c: $(SRC)/gramatica.y $(GEN)/lex.yy.c
		bison -v -o $(GEN)/y.tab.c $(SRC)/gramatica.y
		@mv $(GEN)/y.output $(OUT)
$(GEN)/lex.yy.c: $(SRC)/tokens.l
		flex $(SRC)/tokens.l
		@mv lex.yy.c $(GEN)

cc: $(BIN)/traduccion

test:
	./$(BIN)/traduccion

run:
	./$(BIN)/semantico $(MAINSRC)

clean:
	@rm -rf $(GEN)/* $(OBJ)/* $(BIN)/* $(OUT)/* $(SRC)/traduccion.c

crear:
	@mkdir -p $(BIN) $(GEN) $(OBJ) $(OUT)