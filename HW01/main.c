#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define MAXNUM 10000000

int arr[MAXNUM];

int rpartition(int p, int r);
int randomized_select(int p, int r, int i);
void insertion_sort(int p, int r);
int deterministic_select(int p, int r, int i);

int main(int argc, char *argv[]){

    int N, M, mode, ans;
    time_t start, end;

    //There should be four arguments, i.e. executable object file, mode, input file and output file.
    if (argc!=4){
        printf("There should be four arguments. Check once again.\n");
        return -1;
    }

    mode=atoi(argv[1]);
    if (mode!=1 && mode!=2){
        printf("The mode is invalid. It should be either 1 or 2.\n");
            return -1;
    }

    FILE *fpin=fopen(argv[2], "r");
    if (!fpin){
        printf("Failure to open the input file. Program is terminated.\n");
        return -1;
    }
    
    FILE *fpout=fopen(argv[3], "wb");
    if (!fpout){
        printf("Failure to open the output file. Program is terminated.\n");
        fclose(fpin);
        return -1;
    }

    fscanf(fpin, "%d", &N);
    fscanf(fpin, "%d", &M);
    for (int i=0;i<N;i++){
        fscanf(fpin, "%d", &arr[i]);
    }

    switch (mode){
        case 1:
            start = clock();
            ans = randomized_select(0, N-1, M);
            end = clock();
            break;
        case 2:
            start = clock();
            ans = deterministic_select(0, N-1, M);
            end = clock();
            break;
    }
    fprintf(fpout, "%d\n", ans);
    printf("Programming running time : %ldms\n", (end-start)*1000/CLOCKS_PER_SEC);

    fclose(fpin);
    fclose(fpout);
    return 0;
}

int partition(int p, int r){
    int x, i, temp;

    x=arr[r];
    i=p-1;
    for (int j=p;j<r;j++){
        if (arr[j]<=x){
            i++;
            temp=arr[j];
            arr[j]=arr[i];
            arr[i]=temp;
        }
    }
    temp=arr[i+1];
    arr[i+1]=arr[r];
    arr[r]=temp;
    return i+1;

}

int randomized_select(int p, int r, int i){
    int q, k;

    if (p==r)
        return arr[p];
    q = partition(p, r);
    k = q-p+1;
    if (i==k)
        return arr[q];
    else if (i<k)
        return randomized_select(p,q-1,i);
    else
        return randomized_select(q+1,r,i-k);
}

void insertion_sort(int p, int r){
    int x, j;
    for (int i=p+1;i<=r;i++){
        x=arr[i];
        j=i-1;
        while (j>=p&&arr[j]>x){
            arr[j+1]=arr[j];
            j--;
        }
        arr[j+1]=x;
    }
}

int partition_with_pivot(int p, int r, int pividx){
    int temp=arr[pividx];
    arr[pividx]=arr[r];
    arr[r]=temp;
    return partition(p, r);

}

int deterministic_select(int p, int r, int i){
    if (r-p+1<=5){
        insertion_sort(p, r);
        return arr[p+i-1];
    }

    int gcnt=(r-p+1)/5;
    if ((r-p+1)%5)
        gcnt++;

    int pividx, temp, q, k;
    for (int j=0; j<gcnt; j++){
        int gnum=r-(p+j*5)+1;
        if (gnum < 5){
            insertion_sort(p+5*j, r);
            temp = arr[p + j];
            arr[p + j]=arr[p + 5*j + (gnum-1)/2];
            arr[p + 5*j + (gnum-1)/2] = temp;
        }
        else{
            insertion_sort(p+5*j, p+5*j+4);
            temp = arr[p + j];
            arr[p + j] = arr[p + 5*j + 2];
            arr[p + 5*j + 2] = temp;
        }
    }

    int median = deterministic_select(p, p + gcnt - 1, (gcnt+1)/2);
    for (int j = p; j < p + gcnt ; j++){
        if (arr[j]==median){
            pividx=j;
            break;
        }
    }
    q = partition_with_pivot(p, r, pividx);
    k = q - p + 1;
    if (i==k)
        return arr[q];
    else if (i<k)
        return deterministic_select(p, q-1, i);
    else
        return deterministic_select(q+1, r, i-k);
}