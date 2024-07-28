# Makefile

my_program: main.o Prefix_Trie.o
	g++ -std=c++17 main.o Prefix_Trie.o -o my_program

main.o: main.cpp main.h
	g++ -std=c++17 -c -Wall -Wextra main.cpp

prefix_trie.o: Prefix_Trie.cpp Prefix_Trie.h
	g++ -std=c++17 -c -Wall -Wextra Prefix_Trie.cpp

clean:
	rm -f *.o my_program
