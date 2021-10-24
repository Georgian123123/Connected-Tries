build:
	gcc helper.c helper.h Trie.c main.c Trie.h -Wall -o tema3
run:
	./tema3  ./_test/input/input24.txt out
valgrind:
	valgrind --leak-check=full ./tema3  ./_test/input/input14.txt out
clean:
	rm -rf Trie.o main.o main helper.o