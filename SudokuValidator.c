#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/mman.h>
#include <sys/syscall.h>
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
    		board[i][j] = now;
    	}
    }

    void *dummy;
    int status[3];
    pthread_create(&ids[0], NULL, validateSubGrid, dummy);
    int pid = getpid();
    pid_t p1;
    p1 = fork();
	if (p1 > 0){
    	printf("Reeee");
    	pthread_create(&ids[1], NULL, validateCols, dummy);
    	pthread_join(ids[0], (void *)&status[0]);
    	pthread_join(ids[1], (void *)&status[1]);
    	pid_t tid = syscall(SYS_gettid);
    	printf("%d", tid);
    	wait(NULL);
    	pthread_create(&ids[2], NULL, validateRows, dummy);
    	pthread_join(ids[2], (void *)&status[2]);
    	int h, flag;
    	for (h = 0; h < 3; h++)
    	{
    		if (!status[h]) {
    			flag = 1;
    		}
    	}
    	if (flag)
    	{
    		printf("Sudoku invalido");
    	}else {
    		printf("Sudoku valido");
    	}
    	return 0;
    } else {
    	printf("%d", 3443);
    	char buffer[55];
    	sprintf(buffer, "ps -p %ld -lLf", pid);
    	printf("%s", buffer);
    	execlp((char *)&buffer, (char *)&buffer, (char *)NULL);
    }
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
				return (void *) 0;
			// Si el numero no habia sido encontrado, se agrega al array de encontrados
			} else {
				valid[current - 1] = 1;
			}
		}	
	}
	return (void *) 1;
}

void *validateCols(void *data) {
	int i, j;
	// Para que recorra todas las columnas
	for (i = 0; i < 9; j++) {
		// Reiniciando el array de validacion
		int valid[9] = {0};
		// Recorriendo la fila
		for (j = 0; j < 9; j++) {
			// Ver que numero hay en esta posicion del array
			int current = board[j][i];
			// Si el numero ya habia sido encontrado, que regrese null
			if (valid[current - 1] == 1){
				return (void *) 0;
			// Si el numero no habia sido encontrado, se agrega al array de encontrados
			} else {
				valid[current - 1] = 1;
			}
		}
	}
	return (void *) 1;
}

void *validateSubGrid(void *data) {
	int si, sj, i, j;
	for (si = 0; si < 9; si+=3)
	{
		for (sj = 0; sj < 9; sj+=3)
		{
			int valid[9] = {0};
			for (i = 0; i < 3; i++)
			{
				for (j = 0; j < 3; j++)
				{
					// Ver que numero hay en esta posicion del array
					int current = board[si + i][sj + j];
					// Si el numero ya habia sido encontrado, que regrese null
					if (valid[current - 1] == 1){
						return (void *) 0;
					// Si el numero no habia sido encontrado, se agrega al array de encontrados
					} else {
						valid[current - 1] = 1;
					}
				}
			}
		}
	}
	return (void *) 1;
}
