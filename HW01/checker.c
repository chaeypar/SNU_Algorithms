#include<stdio.h>

int main(int argc, char *argv[]){

    int N, M, smallcnt=0, samecnt=0, num, target;

    //There should be four arguments, i.e. executable object file, input file and output file.
    if (argc!=3){
        printf("There should be three arguments. Check once again.\n");
        return -1;
    }

    FILE *fpin=fopen(argv[1], "r");
    if (!fpin){
        printf("Failure to open the input file. Program is terminated.\n");
        return -1;
    }
    
    FILE *fpout=fopen(argv[2], "r");
    if (!fpout){
        printf("Failure to open the output file. Program is terminated.\n");
        fclose(fpin);
        return -1;
    }

    
    fscanf(fpout, "%d", &target);
    fscanf(fpin, "%d", &N);
    fscanf(fpin, "%d", &M);

    for (int i=0;i<N;i++){
        fscanf(fpin, "%d", &num);
        if (num<target)
            smallcnt++;
        else if (num==target)
            samecnt++;
    }
    if (M>smallcnt&&M<=smallcnt+samecnt)
        printf("1\n");
    else
        printf("0\n");

    fclose(fpin);
    fclose(fpout);

    return 0;
}