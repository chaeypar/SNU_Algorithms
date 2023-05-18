#include <stdio.h>
#include <stdlib.h>

char **map;
char *l, *r, *h, *v;
int cnt;

int possible(int i, int j, int n){
    if (v[j] == 1|| l[i-j+n] == 1|| r[i+j] ==1)
        return 0;
    else if (v[j])
        return 1;
    else if (l[i - j + n])
        return 2;
    else if (r[i + j])
        return 3;
    return 4;
}
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

int checkV(int i, int j, int n){
    for (int k = i-1; k >= 0;k --){
        if (map[k][j]=='H')
            return 1;
        else if (map[k][j]=='Q')
            return 0;
    }
    return 1;
}

int checkL(int i, int j, int n){
    while (i>=0 && j>=0){
        if (map[i][j]=='H')
            return 1;
        else if (map[i][j]=='Q')
            return 0;
        i--;
        j--;
    }
    return 1;
}

int checkR(int i, int j, int n){
    while(i>=0 && j<n){
        if (map[i][j]=='H')
            return 1;
        else if (map[i][j]=='Q')
            return 0;
        i--;
        j++;
    }
    return 1;
}

void recursive_N(int i, int k, int cur, int n){
    if (cur == n){
        cnt++;
        printMap(n);
        return;
    }
    else if (i == n){
        //printMap(n);
        return;
    }
    int flag = 0;
    for (int j = k;j < n;j++){
        if (map[i][j]=='H')
            continue;
        switch(possible(i, j, n)){
            //not possible
            case 0:
                continue;
            //v is marked as 'H'
            case 1:
                if (checkV(i, j, n)){
                    if (l[i-j+n] && !checkL(i, j, n))
                        continue;
                    if (r[i+j] && !checkR(i, j, n))
                        continue;
                    map[i][j] = 'Q';
                    if (!l[i-j+n])
                        l[i-j+n] = 1;
                    if (!r[i+j])
                        r[i+j] = 1;
                if (h[i]){
                    for (int idx = j+1; idx < n-1; idx++){
                        if (map[i][idx]=='H'){        
                            recursive_N(i, idx+1, cur+1, n);
                            break;
                        }
                    }
                }
                recursive_N(i+1, 0, cur+1, n);
                    map[i][j]='*';
                    if (l[i-j+n] == 1)
                        l[i-j+n] = 0;
                    if (r[i+j] == 1)
                        r[i+j] = 0;
                }else
                    continue;
                break;
            //l is marked as 'H'
            case 2:
                if (checkL(i, j, n)){
                    if (r[i+j] && !checkR(i, j, n))
                        continue;
                    map[i][j]='Q';
                    v[j] = 1;
                    if (!r[i+j])
                        r[i+j] = 1;
                    if (h[i]){
                        for (int idx = j + 1; idx < n - 1 ;idx++){
                            if (map[i][idx]=='H'){ 
                                recursive_N(i, idx+1, cur+1, n);
                                break;
                            }
                        }
                    }
                    recursive_N(i+1, 0, cur+1, n);
                    map[i][j]='*';
                    v[j] = 0;
                    if (r[i+j] == 1)
                        r[i+j] = 0;
                }else   
                    continue;
                break;
            //r is marked as 'H'
            case 3:
                if (checkR(i, j, n)){
                    map[i][j] = 'Q';
                    v[j] = l[i-j+n] = 1;
                    if (h[i]){
                        for (int idx = j + 1; idx < n - 1; idx++){
                            if (map[i][idx]=='H'){
                                recursive_N(i, idx+1, cur+1, n);
                                break;
                            }
                        }
                    }
                    recursive_N(i+1, 0, cur+1, n);
                    map[i][j] = '*';
                    v[j] = l[i-j+n] = 0;
                }else
                    continue;
                break;
            //clean board as 'H'
            case 4:
                map[i][j] = 'Q';
                v[j] = l[i-j+n] =r[i+j] = 1;
                int flag = 0;
                if (h[i]){
                    for (int idx = j + 1; idx < n - 1; idx++){
                        if (map[i][idx]=='H'){
                            recursive_N(i, idx+1, cur+1, n);
                            break;
                        }
                    }
                }
                recursive_N(i+1, 0, cur+1, n);
                map[i][j] = '*';
                v[j] = l[i-j+n] = r[i+j] = 0;
                break;
        }
    }
    if (k==0)
        recursive_N(i+1, 0, cur, n);
    
}

void makeMap(int n, int holenum){
    map = (char **)malloc(n * sizeof(char *));
    for (int i=0;i<n;i++){
        map[i] = (char *)malloc(n*sizeof(char));
        for (int j = 0;j < n; j++)
            map[i][j] = '*';
    }
    int t1, t2;
    l = (char *)calloc(2*n, sizeof(char));
    r = (char *)calloc(2*n, sizeof(char));
    h = (char *)calloc(n, sizeof(char));
    v = (char *)calloc(n, sizeof(char));
    for (int i = 0; i < holenum; i++){
        scanf("%d %d", &t1, &t2);
        map[t1-1][t2-1] = 'H';
        l[t1 - t2 + n] = r[t1 + t2 - 2] = h[t1-1] = v[t2-1] = 'H';
    }
    printMap(n);
}

int main(void){
    int n, holenum, t1, t2;

    scanf("%d %d", &n, &holenum);
    makeMap(n, holenum);
    cnt = 0;
    recursive_N(0, 0, 0, n);
    printf("%d\n", cnt);
    return 0;
}