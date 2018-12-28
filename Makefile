CC = gcc
BIN = main.exe
OBJ = learning.o main.o trie.o word.o word_processing.o

all: $(BIN) clean

$(BIN): $(OBJ)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) -c $< -o $@

clean:	
	rm -f *.o