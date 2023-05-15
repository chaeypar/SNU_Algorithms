#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int visited[5001][5001];

int main(int argc, char *argv[]){
    int randnum, N, M;

    srand((unsigned int)time(NULL));

    if (argc != 4){
        printf("There should be four arguments.");
        return -1;
    }

    FILE *fp=fopen(argv[3], "w");
    if (!fp){
        printf("The output file is invalid. Program is terminated.\n");
        return -1;
    }

    N=atoi(argv[1]);
    M=atoi(argv[2]);
    fprintf(fp, "%d\n%d\n", N, M);
    
    for (int i = 0; i < M; i++){
        int temp_1 = 0, temp_2 = 0;
        while (temp_1 == temp_2 || visited[temp_1][temp_2]){
            temp_1 = (rand() % N) + 1;
            temp_2 = (rand() % N) + 1;
        }
        fprintf(fp, "%d %d\n", temp_1, temp_2);
        visited[temp_1][temp_2] = 1;
    }

    fclose(fp);
    return 0;
}