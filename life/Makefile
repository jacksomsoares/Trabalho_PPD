CC =cc

FLAGS =-O3
OMP_FLAG =-fopenmp

all: life

life: life.c
	$(CC) $(FLAGS) $(OMP_FLAG) life.c -o life

debug: life.c
	$(CC) $(FLAGS) -g $(OMP_FLAG) life.c -o life

original: life.c
	$(CC) $(FLAGS) life.c -o lifeOriginal
clean:
	rm life
