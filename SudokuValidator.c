#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/mman.h>
#include <pthread.h>

void *validateRows(void *);
void *validateCols(void *);
void *validateSubGrid(void *);

int **board;

int main(int argc, char const *argv[])
{
    unsigned char *f;
    int size = 81;
    // Create and allocate array of integer pointers
    board = (int**)malloc(9 * sizeof(int *));
    int c;
    for (c = 0; c < 9; ++c)
    	board[c] = (int *)malloc(9 * sizeof(int *));
    // Array of threads
    pthread_t ids[3];

    const char * file_name = argv[1];
    int fd = open (argv[1], O_RDONLY);
    int i, j, now;
    f = (char *) mmap (0, size, PROT_READ, MAP_PRIVATE, fd, 0);
    for (i = 0; i < 9; i++) {
    	for (j = 0; j < 9; j++)
    	{
    		now = (int)f[i*9 + j] - '0';
    		printf("%d\n", now);
    		board[i][j] = now;
    	}
    }

    return 0;
}

void *validateRows(void *data) {
	int i, j;
	// Para que recorra todas las filas
	for (i = 0; i < 9; j++) {
		// Reiniciando el array de validacion
		int valid[9] = {0};
		// Recorriendo la fila
		for (j = 0; j < 9; j++) {
			// Ver que numero hay en esta posicion del array
			int current = board[i][j];
			// Si el numero ya habia sido encontrado, que regrese null
			if (valid[current - 1] == 1){
				pthread_exit(NULL);
			// Si el numero no habia sido encontrado, se agrega al array de encontrados
			} else {
				valid[current - 1] = 1;
			}
		}	
	}
}
