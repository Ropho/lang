CC = g++

lang.o: lang.cpp
	$(CC) lang.cpp func.cpp libr\libr_tree_diff.cpp -o a