# Nome do executável final
TARGET = TaPyoCompiler

# Arquivos de entrada
LEX_FILE = lexer.l
BISON_FILE = parserTaPyoCa.y

# Arquivos gerados pelas ferramentas
LEX_C = lex.yy.c
BISON_C = y.tab.c
BISON_H = y.tab.h
BISON_OUT = y.output
BISON_GRAPH = y.gv y.dot

# Regra principal
all: $(TARGET)

# Compila o executável
$(TARGET): $(LEX_C) $(BISON_C)
	gcc $(LEX_C) $(BISON_C) -o $(TARGET)

# Gera o código do lexer
$(LEX_C): $(LEX_FILE)
	flex $(LEX_FILE)

# Gera o código do parser
$(BISON_C): $(BISON_FILE)
	bison $(BISON_FILE) -o $(BISON_C) -d -v -g

# Limpa todos os arquivos gerados
clean:
	rm -f $(TARGET) $(LEX_C) $(BISON_C) $(BISON_H) $(BISON_OUT) $(BISON_GRAPH)