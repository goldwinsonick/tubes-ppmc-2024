build: main.c
	gcc -o main main.c algos/tsp_dfs.c

buildrun: main.c
	gcc -o main main.c algos/tsp_dfs.c
	./main