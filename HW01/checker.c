#include<stdio.h>

int main(int argc, char *argv[]){

    int N, M, smallcnt=0, samecnt=0, num, target, c;

    //There should be three arguments, i.e. executable file, input file and output file.
    if (argc!=3){
        printf("There should be three arguments. Check once again.\n");
        return -1;
    }

    //Open the input file. If there are any errors, then print the error message and terminate the program.
    FILE *fpin=fopen(argv[1], "r");
    if (!fpin){
        printf("Failure to open the input file. Program is terminated.\n");
        return -1;
    }
    
    //Open the output file. If there are any errors, then print the error message and terminate the program.
    FILE *fpout=fopen(argv[2], "r");
    if (!fpout){
        printf("Failure to open the output file. Program is terminated.\n");
        fclose(fpin);
        return -1;
    }

    //We should check whether the integer in the output file is the correct answer for the given input file.
    c = fscanf(fpout, "%d", &target);
    c = fscanf(fpin, "%d", &N);
    c = fscanf(fpin, "%d", &M);

    //If each integer in the given input file is smaller than 'target', then increment the 'smallcnt' by 1.
    //If each integer in the given input file is equal to 'target', then increment the 'samecnt' by 1.
    for (int i=0;i<N;i++){
        c = fscanf(fpin, "%d", &num);
        if (num<target)
            smallcnt++;
        else if (num==target)
            samecnt++;
    }

    //If smallcnt is 7 and samecnt is 3, it means that there are 7 integers smaller than 'target' and 3 integers equal to 'target'.
    //Therefore, 'target' is the 8(=7+1)th smallest, the 9(=7+2)th smallest, and the 10(=7+3)th smallest value.
    //Hence, we should check whether M is greater than M and less than or equal to (smallcnt + samecnt). 
    //If it is, we should print 1 to stdout. Otherwise, we should print 0 to stdout.
    if (M>smallcnt&&M<=smallcnt+samecnt)
        printf("1\n");
    else
        printf("0\n");

    //Close the given input and output files.
    fclose(fpin);
    fclose(fpout);

    return 0;
}