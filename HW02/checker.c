#include<stdio.h>

void reset(int *A);
void closefile(FILE *fpin, FILE *fpout);

int main(int argc, char *argv[]){
    
    //There should be three arguments, i.e. executable file, input file and output file.
    if (argc!=3){
        printf("There should be three arguments. Check once again.\n");
        return -1;
    }

    //Open the input file. If there are any errors, then print the error message and terminate the program.
    FILE *fpin=fopen(argv[1], "rt");
    if (!fpin){
        printf("Failure to open the input file. Program is terminated.\n");
        return -1;
    }
    
    //Open the output file. If there are any errors, then print the error message and terminate the program.
    FILE *fpout=fopen(argv[2], "rt");
    if (!fpout){
        printf("Failure to open the output file. Program is terminated.\n");
        fclose(fpin);
        return -1;
    }

    char mode, sp;
    int op, num, err, cnt=0;
    int A[10000];

    reset(A);
    while(1){
        while (1){
            sp=fgetc(fpin);
            if (!(sp == '\n' || sp == ' ')){
                mode = sp;
                break;
            }
            else if (sp == EOF){
                closefile(fpin, fpout);
                printf("1\n");
                return 0;
            }
        }

        fscanf(fpin, "%d", &op);
        fscanf(fpout, "%d", &num);
        
        switch(mode){
            case 'I':
                if (!A[op]){
                    A[op] = 1;
                    cnt++;
                    if (num != op)
                        err = 1;
                }
                else{
                    if (num != 0)
                        err = 1;
                }
                break;
            case 'D':
                if (A[op]){
                    A[op] = 0;
                    cnt--;
                    if (num != op)
                        err = 1;
                }
                else{
                    if (num != 0)
                        err = 1;
                }
                break;
            case 'S':
                if (num > cnt)
                    err = 1;
                else{
                    int tot = 0, idx = 1;
                    while (1){
                        tot += A[idx];
                        if (tot == op)
                            break;
                        idx++;
                    }
                    if (idx != num)
                        err = 1;
                }
                break;
            case 'R':
                if (A[op]){
                    int tot = 0;
                    for (int i = 1; i <= op; i++)
                        tot += A[i];
                    if (tot != num)
                        err = 1;
                }
                else{
                    if (num != 0)
                        err = 1;
                }
                break;
        }
        if (err){
            closefile(fpin, fpout);
            printf("0\n");
            return 0;
        }
    }

    closefile(fpin, fpout);
    return 0;
}

void reset(int *A){
    for (int i=0;i<10000;i++)
        A[i]=0;
}

void closefile(FILE *fpin, FILE *fpout){
    fclose(fpin);
    fclose(fpout);
}