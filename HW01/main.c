#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define MAXNUM 5000000

int arr[MAXNUM];

int partition(int p, int r);
int randomized_select(int p, int r, int i);
void insertion_sort(int p, int r);
int partition_with_pivot(int p, int r, int pividx);
int deterministic_select(int p, int r, int i);

int main(int argc, char *argv[]){

    int N, M, mode, ans, c;
    time_t start, end;

    //There should be four arguments, i.e. executable file, mode, input file and output file.
    if (argc!=4){
        printf("There should be four arguments. Check once again.\n");
        return -1;
    }

    //If argv[1] is not 1 or 2, then it is not valid.
    mode=atoi(argv[1]);
    if (mode!=1 && mode!=2){
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
    FILE *fpout=fopen(argv[3], "wb");
    if (!fpout){
        printf("Failure to open the output file. Program is terminated.\n");
        fclose(fpin);
        return -1;
    }

    //Store the values of N, M and input array to the variables from input file.
    c = fscanf(fpin, "%d", &N);
    c = fscanf(fpin, "%d", &M);
    for (int i=0;i<N;i++){
       c = fscanf(fpin, "%d", &arr[i]);
    }

    //If mode=1, we should execute randomized selection algorithm.
    //If mode=2, we shoudl execute deterministic selection algorithm.
    switch (mode){
        case 1:
            //start = clock();
            ans = randomized_select(0, N-1, M);
            //end = clock();
            break;
        case 2:
            //start = clock();
            ans = deterministic_select(0, N-1, M);
            //end = clock();
            break;
    }
    fprintf(fpout, "%d\n", ans);
    //printf("Total running time : %lfs\n", (double)(end-start)/CLOCKS_PER_SEC);

    fclose(fpin);
    fclose(fpout);
    return 0;
}

//partition algorithm in the lecture note. It sets arr[r] as a pivot.
int partition(int p, int r){
    int x, i, temp;

    //Set arr[r] as a pivot.
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

//Randomized_selection algorithm
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

//Insertion sort on the array = (arr[p], arr[p+1], ..., arr[r])
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

//Modified version of partition() function. 
int partition_with_pivot(int p, int r, int pividx){
    //Swap arr[pividx] and arr[r] at first.
    //Note that pivot is now equal to arr[r].
    int temp=arr[pividx];
    arr[pividx]=arr[r];
    arr[r]=temp;
    
    //Then call partition(p, r) since arr[r] is a pivot. 
    return partition(p, r);

}

//Deterministic selection algorithm
int deterministic_select(int p, int r, int i){
    //If n<=5, find the answer by insertion sort and return
    if (r-p+1<=5){
        insertion_sort(p, r);
        return arr[p+i-1];
    }

    //Divide n elements into groups of 5 elements.
    //gcnt means the number of the groups.
    int gcnt=(r-p+1)/5;
    if ((r-p+1)%5)
        gcnt++;

    int pividx, temp, q, k;
    for (int j=0; j<gcnt; j++){
        int gnum=r-(p+j*5)+1;
        //find median in eaach group by insertion sort.
        //let m_1, m_2, ..., m_gcnt be these medians.
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

    //find median M of m_1, m_2, ..., m_gcnt recursively.
    int median = deterministic_select(p, p + gcnt - 1, (gcnt+1)/2);
 
    //find the index of the medain in the 'arr' array.
    for (int j = p; j < p + gcnt ; j++){
        if (arr[j]==median){
            pividx=j;
            break;
        }
    }

    //partition n elements using median as pivot.   
    q = partition_with_pivot(p, r, pividx);
    k = q - p + 1;
    
    //same as randomized_select()
    if (i==k)
        return arr[q];
    else if (i<k)
        return deterministic_select(p, q-1, i);
    else
        return deterministic_select(q+1, r, i-k);
}