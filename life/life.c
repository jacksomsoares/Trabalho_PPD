/*
 * The Game of Life
 *
 * a cell is born, if it has exactly three neighbours 
 * a cell dies of loneliness, if it has less than two neighbours 
 * a cell dies of overcrowding, if it has more than three neighbours 
 * a cell survives to the next generation, if it does not die of loneliness 
 * or overcrowding 
 *
 * In this version, a 2D array of ints is used.  A 1 cell is on, a 0 cell is off.
 * The game plays a number of steps (given by the input), printing to the screen each time.  'x' printed
 * means on, space means off.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

typedef unsigned char cell_t; 
double s1, s2, s3, s4;
double f1, f2, f3, f4;
int numProcs;

cell_t ** allocate_board (int size) {
	cell_t ** board = (cell_t **) malloc(sizeof(cell_t*)*size);
	int	i;
	for (i=0; i<size; i++)
		board[i] = (cell_t *) malloc(sizeof(cell_t)*size);
	return board;
}

void free_board (cell_t ** board, int size) {
        int     i;
        for (i=0; i<size; i++)
                free(board[i]);
	free(board);
}


/* return the number of on cells adjacent to the i,j cell */
int adjacent_to (cell_t ** board, int size, int i, int j) {
	int	k, l, count=0;
	
	int sk = (i>0) ? i-1 : i;
	int ek = (i+1 < size) ? i+1 : i;
	int sl = (j>0) ? j-1 : j;
	int el = (j+1 < size) ? j+1 : j;	
	
	for (k=sk; k<=ek; k++)
		for (l=sl; l<=el; l++)
			count+=board[k][l];

	count-=board[i][j];

	return count;
}

void play (cell_t ** board, cell_t ** newboard, int size) {
	int	i, j, a;
	/* for each cell, apply the rules of Life */
	
	#pragma omp parallel num_threads(numProcs)
	{
	#pragma omp for collapse(2) private(i, j, a) schedule(dynamic, (size)/numProcs) nowait
	for (i=0; i<size; i++){
		for (j=0; j<size; j++) {			
			a = adjacent_to (board, size, i, j);
			if (a == 2) newboard[i][j] = board[i][j];
			else if (a == 3) newboard[i][j] = 1;
			else if (a < 2) newboard[i][j] = 0;
			else if (a > 3) newboard[i][j] = 0;
		}
	}
	}
}

/* print the life board */
void print (cell_t ** board, int size) {
	int	i, j;
	/* for each row */
	for (j=0; j<size; j++) {
		/* print each column position... */
		for (i=0; i<size; i++) 
			printf ("%c", board[i][j] ? 'x' : ' ');
		/* followed by a carriage return */
		printf ("\n");
	}
}

/* read a file into the life board */
void read_file (FILE * f, cell_t ** board, int size) {
	int	i, j;
	char	*s = (char *) malloc(size+10);
	//char c;
	
	for (j=0; j<size; j++) {
		/* get a string */
		fgets (s, size+10,f);		
		/* copy the string to the life board */
		for (i=0; i<size; i++)
		{
		 	//c=fgetc(f);
			//putchar(c);
			board[i][j] = s[i] == 'x';
		}
		//fscanf(f,"\n");
	}
}

int main () {
	int size, steps;
	FILE    *f;
  	f = stdin;
	fscanf(f,"%d %d", &size, &steps);
	cell_t ** prev = allocate_board (size);	
	s1 = omp_get_wtime();
	read_file (f, prev,size);
	f1 = omp_get_wtime();
	fclose(f);
	cell_t ** next = allocate_board (size);
	cell_t ** tmp;
	int i;
	#ifdef DEBUG
	printf("Initial \n");
	print(prev,size);
	printf("----------\n");
	#endif

	omp_set_nested(1);
	numProcs = omp_get_num_procs();
	s2 = omp_get_wtime();

	for (i=0; i<steps; i++) {
		play (prev,next,size);
		#ifdef DEBUG
		printf("%d ----------\n", i);
		print (next,size);
		#endif		
		tmp = next;
		next = prev;
		prev = tmp;
	}
	f2 = omp_get_wtime();
	s3 = omp_get_wtime();
	print (prev,size);
	f3 = omp_get_wtime();
	free_board(prev,size);
	free_board(next,size);

	double total = (f1-s1) + (f2-s2) + (f3-s3);

	printf("Tempo 1 - Leitura arquivo: %f Percentual: %f%% \n", f1-s1, (f1-s1)/total);
	printf("Tempo 2 - Funçao Play(): %f Percentual: %f%% \n", f2-s2, (f2-s2)/total);
	printf("Tempo 3 - Printar na tela resultado: %f Percentual: %f%% \n", f3-s3, (f3-s3)/total);
	

	//printf("Tempo total acumulado função adjacente: %f\n", timeAdjacente);
}
