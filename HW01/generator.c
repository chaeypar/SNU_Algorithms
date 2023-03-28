#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(int argc, char *argv[]){
    int randnum, N, M;

    srand((unsigned int)time(NULL));

    FILE *fp=fopen(argv[3], "w");
    if (!fp){
        printf("The output file is invalid. Program is terminated.\n");
        return -1;
    }

    N=atoi(argv[1]);
    M=atoi(argv[2]);
    fprintf(fp, "%d %d\n", N, M);
    
    for (int i=0;i<N;i++){
        if (rand()%2)
            fprintf(fp, "%d ", rand()%2147483647);
        else
            fprintf(fp, "%d ", (-1)*(rand()%2147483647));
    }

    fclose(fp);
    return 0;
}