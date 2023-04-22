#include<stdio.h>
#include<stdlib.h>
#include<time.h>

char operation[4] = {'I', 'D', 'S', 'R'};

int main(int argc, char* argv[]){

    //There should be three arguments, i.e. executable file, input file and the number of operations.
    if (argc!=3){
        printf("There should be three arguments. Check once again.\n");
        return -1;
    }

    //Open the input file. If there are any errors, then print the error message and terminate the program.
    FILE *fpin=fopen(argv[1], "wt");
    if (!fpin){
        printf("Failure to open the input file. Program is terminated.\n");
        return -1;
    }

    int num = atoi(argv[2]);
    if (num<=0){
        printf("The number of operations should be positive.\n");
        return -1;
    }

    srand((unsigned int)time(NULL));
    for (int i = 0; i < num; i++){
        fprintf(fpin, "%c ", operation[rand()%4]);
        fprintf(fpin, "%d\n", rand()%10000);
    }

    fclose(fpin);
    return 0;
}