#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

char **adjMatrix;
char **reverseMatrix;
int **result;
char *visited;

int *qu;
int cur = 0;
int curidx;
int rescnt;

int compare(const void *first, const void *second){
    if (*(int *)first > *(int *)second)
        return 1;
    else if (*(int *)first < *(int *)second)
        return -1;
    return 0;
}


void makeMatrix(FILE *fd, int n){
    int num, in, out;

    result = (int **)malloc((n+1)*sizeof(int *));
    adjMatrix = (char **)malloc((n+1)*sizeof(char *));
    reverseMatrix = (char **)malloc((n+1)*sizeof(char *));

    qu = (int *)malloc((n+1)*sizeof(int));
    visited = (char *)malloc((n+1)*sizeof(char));

    for (int i = 0;i <= n; i++){
        adjMatrix[i] = (char *)calloc(n+1, sizeof(char));
        reverseMatrix[i] = (char *)calloc(n+1, sizeof(char));
        result[i] = (int *)calloc(n+1, sizeof(int));
    }

    fscanf(fd, "%d", &num);
    for (int i = 0; i < num;i++){
        fscanf(fd, "%d %d", &in, &out);
        adjMatrix[in][out] = 1;
        reverseMatrix[out][in] = 1;
    }
}

void dfs_regular(int i, int n){
    for (int j = 0; j <= n; j++){
        if (adjMatrix[i][j] && !visited[j]){
            visited[j] = 1;
            dfs_regular(j, n);
        }
    }
    qu[cur++] = i;
}

void dfs_reverse(int i, int n){
    result[rescnt][curidx++] = i;
    for (int j = 0; j <= n; j++){
        if (reverseMatrix[i][j] && !visited[j]){
            visited[j] = 1;
            dfs_reverse(j, n);
        }
    }
}

void calMatrix(FILE *fd, int n){
    memset(visited, 0, n+1);
    for (int i=1;i<=n;i++){
        if (!visited[i]){
            visited[i] = 1;
            dfs_regular(i, n);
        }
    }

    memset(visited, 0, n+1);
    for (int i = n-1; i >= 0; i--){
        curidx = 0;
        if (!visited[qu[i]]){
            visited[qu[i]] = 1;
            dfs_reverse(qu[i], n);
            result[rescnt][curidx] = -1;
            rescnt++;
        }
    }

    int arr[5001];
    int arrcnt = 0;

    for (int i = 0; i < rescnt; i++){
        int tot = 0;
        for (int j = 0; result[i][j] != -1; j++){
            tot = tot ^ result[i][j];
        }
        arr[arrcnt++] = tot;
    }
    fprintf(fd, "%d\n", rescnt);
    qsort(arr, arrcnt, sizeof(int), compare);
    for (int i = 0; i < arrcnt - 1; i++){
        fprintf(fd, "%d ", arr[i]);
    }
    fprintf(fd, "%d\n", arr[arrcnt-1]);
}

int main(int argc, char *argv[]){

    int n, mode;

    //There should be four arguments, i.e. executable file, mode, input file and output file.
    if (argc!=4){
        printf("There should be four arguments. Check once again.\n");
        return -1;
    }

    //If argv[1] is not 1 or 2, then it is not valid.
    mode = atoi(argv[1]);
    if (mode!=1 && mode!=2 && mode!=3){
        printf("The mode is invalid. It should be either 1 or 2.\n");
            return -1;
    }

    //Open the input file. If there exist any errors, then terminate the program.
    FILE *fpin=fopen(argv[2], "r");
    if (!fpin){
        printf("Failure to open the input file. Program is terminated.\n");
        return -1;
    }

    //Open the output file. If there exist any errors, then terminate the program.
    FILE *fpout=fopen(argv[3], "w");
    if (!fpout){
        printf("Failure to open the output file. Program is terminated.\n");
        fclose(fpin);
        return -1;
    }

    fscanf(fpin, "%d", &n);
    switch(mode){
        case 1:
            makeMatrix(fpin, n);
            calMatrix(fpout, n);
            break;
        case 2:
            break;
        case 3:
            
            break;
    }
    


    return 0;
}