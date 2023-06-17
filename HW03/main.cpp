#include<iostream>
#include<stack>
#include<time.h>

using namespace std;
stack<pair<int,int>> qu;

char map[15][15];
char l[30], r[30], h[15], v[15];
int cnt;
int holeh[15][4];

// possible: to check whether a queen can be placed on (i, j)
int possible(int i, int j, int n){
    // impossible
    if (v[j] == 1|| l[i-j+n] == 1|| r[i+j] ==1)
        return 0;
    // may be possible (need to check more)
    return 1;
}

// printMap : for debugging (It prints the map status)
void printMap(int n){
    printf("%d map: \n", cnt);
    for (int i = 0; i < n;i++){
        for (int j = 0;j < n; j++){
            printf("%c ", map[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// checkV: It checks whether a queen can be placed on (i, j) when there is a hole on (k, j) for some  0 <= k < i
int checkV(int i, int j, int n){
    for (int k = i-1; k >= 0;k --){
        // There is a hole.
        if (map[k][j]=='H')
            return 1;
        // There is a queen that attacks (i, j)
        else if (map[k][j]=='Q')
            return 0;
    }
    return 1;
}

// CheckL : It checks whether a queen can be placed on (i, j) when there is a hole on (k, i-j+n-k) for some 0 <= k < i
int checkL(int i, int j, int n){
    while (i>=0 && j>=0){
        // There is a hole
        if (map[i][j]=='H')
            return 1;
        // There is a queen that attacks (i, j)
        else if (map[i][j]=='Q')
            return 0;
        i--;
        j--;
    }
    return 1;
}

// CheckR : It checks whether a queen can be placed on (i, j) when there is a hole on (k, i+j-k) for some 0 <= k < i
int checkR(int i, int j, int n){
    while(i>=0 && j<n){
        // There is a hole
        if (map[i][j]=='H')
            return 1;
        //There is a queen that attacks (i, j)
        else if (map[i][j]=='Q')
            return 0;
        i--;
        j++;
    }
    return 1;
}

// recursive_N : recursive backtracking algorithm  
void recursive_N(int i, int k, int cur, int n){
    // finished to check the rows 
    if (i == n){
        if (cur == n)
            cnt++;       
        // printMap(n);
        return;
    }
    for (int j = k;j < n;j++){
        // If the hole is placed on (i, j), the queen can't be placed on that location.
        if (map[i][j]=='H')
            continue;
        switch(possible(i, j, n)){
            // It is impossible to put a queen on (i, j)
            case 0:
                continue;
            // There is at least one hole on the vertical line or left diagonal or right diagonal
            case 1:
                // If there is a queen that attacks (i, j) on the vertical line
                if (v[j] && !checkV(i, j, n))
                    continue;
                // If there is a queen that attacks (i, j) on the left diagonal
                if (l[i-j+n] && !checkL(i, j, n))
                    continue;
                // If there is a queen that attacks (i, j) on the right diagnal
                if (r[i+j] && !checkR(i, j, n))
                    continue;
                // place a queen on (i, j)
                map[i][j] = 'Q';
                if (!v[j])
                    v[j] = 1;
                if (!l[i-j+n])
                    l[i-j+n] = 1;
                if (!r[i+j])
                    r[i+j] = 1;
                // If there is a hole on the horizontal line passing through (i, j)
                if (h[i]){
                    // find the nearest hole on the right side of (i, j)
                    int num = holeh[i][0];
                    for (int idx = 1; idx <= num; idx++){
                        if (holeh[i][idx] <= j)
                            continue;
                        //recursive call
                        recursive_N(i, holeh[i][idx]+1, cur+1, n);
                        break;
                    }
                }
                // recursive for the next row
                recursive_N(i+1, 0, cur+1, n);

                // remove a queen from (i, j)
                map[i][j]='*';
                if (v[j] == 1)
                    v[j] = 0;
                if (l[i-j+n] == 1)
                    l[i-j+n] = 0;
                if (r[i+j] == 1)
                    r[i+j] = 0;
            }
    }
    // not place any queen on the i-th row
    if (k==0)
        recursive_N(i+1, 0, cur, n);
    
}

// Iterative backtracking algorithm
void iterative_N(int n){
    int i = 0, j = 0;

    while (i != n){
        while (j < n){
            // If the hole is placed on (i, j), the queen can't be placed on that location
            if (map[i][j] == 'H'){
                j++;
                continue;
            }
            switch(possible(i, j, n)){
                // It is impossible to put a queen on (i, j)
                case 0:
                    j++;
                    break;
                // There is at least one hole on the vertical line or left diagonal or right diagonal
                case 1:
                    // If there is a queen that attacks (i, j) on the vertical line
                    if (v[j] && !checkV(i, j, n)){
                        j++;
                        continue;
                    }
                    // If there is a queen that attacks (i, j) on the left diagonal
                    if (l[i-j+n] && !checkL(i, j, n)){
                        j++;
                        continue;
                    }
                    // If there is a queen that attacks (i, j) on the right diagonal
                    if (r[i+j] && !checkR(i, j, n)){
                        j++;
                        continue;
                    }
                    // place a queen on (i, j)
                    map[i][j] = 'Q';
                    qu.emplace(i, j);
                    if (!v[j])
                        v[j] = 1;
                    if (!l[i-j+n])
                        l[i-j+n] = 1;
                    if (!r[i+j])
                        r[i+j] = 1;
                    // If there is a hole on the horizontal line passing through (i, j)
                    if (h[i]){
                        int flag = 0;
                        int num = holeh[i][0];
                        // find the nearest hole on the right side of (i, j)
                        for (int idx = 1; idx <= num; idx++){
                            if (holeh[i][idx] <= j)
                                continue;        
                            j = holeh[i][idx] + 1;
                            flag = 1;
                            break;
                        }
                        //there is no hole on the right side of (i, j)
                        if (!flag){
                            i++;
                            j = 0;
                        }
                    }
                    // there is no hole on the horizontal line passing through (i, j)
                    else{
                        i++;
                        j = 0;
                    }
                    break;
            }
            break;
        }
        // If we already placed n queens on the map
        if (qu.size() == n){
            cnt++;
            i = qu.top().first;
            j = qu.top().second;
            qu.pop();
            map[i][j] = '*';
            if (v[j] != 'H')
                v[j] = 0;
            if (l[i-j+n] != 'H')
                l[i-j+n] = 0;
            if (r[i+j] != 'H')
                r[i+j] = 0;
            j++;
        }
        // index out of range: 0 <= i < n , 0 <= j < n
        if (j == n){
            i++;
            j = 0;
        }
        // finished to check from 0 th row to n-1 th row, but could not place n queens 
        if (i == n){
            i = qu.top().first;
            j = qu.top().second;
            qu.pop();
            map[i][j] = '*';
            if (v[j] != 'H')
                v[j] = 0;
            if (l[i-j+n] != 'H')
                l[i-j+n] = 0;
            if (r[i+j] != 'H')
                r[i+j] = 0;
            j++;
            if (qu.size() == 0 &&i == n-1 && j >= n-2)
                return;
        }
    }
}

// makeMap : mark 'H' if there is a hole, '*' otherwise.
void makeMap(FILE *fd, int n, int holenum){
    for (int i=0;i<n;i++){
        for (int j = 0;j < n; j++)
            map[i][j] = '*';
    }
    int t1, t2;
    for (int i = 0; i < holenum; i++){
        fscanf(fd, "%d %d", &t1, &t2);
        map[t1-1][t2-1] = 'H';
        l[t1 - t2 + n] = r[t1 + t2 - 2] = h[t1-1] = v[t2-1] = 'H';
        holeh[t1-1][0]++; 
        holeh[t1-1][holeh[t1-1][0]] = t2-1;
    }
}

int main(int argc, char *argv[]){
    int n, holenum, t1, t2, mode;
    //There should be four arguments, i.e. executable file, mode, input file and output file.
    if (argc!=4){
        printf("There should be four arguments. Check once again.\n");
        return -1;
    }

    //If argv[1] is not 1 or 2, then it is not valid.
    mode = atoi(argv[1]);
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
    FILE *fpout=fopen(argv[3], "w");
    if (!fpout){
        printf("Failure to open the output file. Program is terminated.\n");
        fclose(fpin);
        return -1;
    }
    //make a map and arrays l, r, v, h
    fscanf(fpin, "%d %d", &n, &holenum);
    makeMap(fpin, n, holenum);
    
    cnt = 0;
    switch(mode){
        // iterative backtracking algorithm
        case 1:
            iterative_N(n);
            break;
        // recursive backtracking algorithm
        case 2:
            recursive_N(0, 0, 0, n);
            break;
    }

    // write the number of possible ways that we can place n queens properly into the output file
    fprintf(fpout, "%d\n", cnt);
    fclose(fpin);
    fclose(fpout);
    return 0;
}