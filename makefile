# Nome do executável final
TARGET = TaPyoCompiler

# Arquivos de entrada
LEX_FILE = lexer.l
BISON_FILE = parserTaPyoCa.y
SEM_C = semantico.c
SEM_H = semantico.h

# Arquivos gerados pelas ferramentas
LEX_C = lex.yy.c
BISON_C = y.tab.c
BISON_H = y.tab.h
BISON_OUT = y.output
BISON_GRAPH = y.gv y.dot

# Regra principal
all: $(TARGET)

# Compila o executável
$(TARGET): $(LEX_C) $(BISON_C) $(SEM_C) $(SEM_H)
	gcc $(LEX_C) $(BISON_C) $(SEM_C) -o $(TARGET) -Wall

# Gera o código do lexer (depende de BISON_H para conhecer os tokens e structs)
$(LEX_C): $(LEX_FILE) $(BISON_H)
	flex $(LEX_FILE)

# Gera o código do parser
$(BISON_C) $(BISON_H): $(BISON_FILE) $(SEM_H)
	bison $(BISON_FILE) -o $(BISON_C) -d -v -g

# Limpa todos os arquivos gerados
clean:
	rm -f $(TARGET) $(LEX_C) $(BISON_C) $(BISON_H) $(BISON_OUT) $(BISON_GRAPH)

.PHONY: all clean
