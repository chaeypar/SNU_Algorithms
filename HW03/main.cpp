#include<iostream>
#include<stack>

using namespace std;
stack<pair<int,int>> qu;

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
    if (i == n){
        if (cur == n)
            cnt++;       
        //printMap(n);
        return;
    }
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

void iterative_N(int n){
    int i = 0, j = 0;

    while (i != n){
        while (j < n){
            if (map[i][j] == 'H'){
                j++;
                continue;
            }
            switch(possible(i, j, n)){
                case 0:
                    j++;
                    break;
                case 1:        
                    if (checkV(i, j, n)){
                        if (l[i-j+n] && !checkL(i, j, n)){
                            j++;
                            continue;
                        }
                        if (r[i+j] && !checkR(i, j, n)){
                            j++;
                            continue;
                        }
                        map[i][j] = 'Q';
                        qu.emplace(i, j);
                        if (!l[i-j+n])
                            l[i-j+n] = 1;
                        if (!r[i+j])
                            r[i+j] = 1;
                        if (h[i]){
                            int flag = 0;
                            for (int idx = j+1; idx < n-1; idx++){
                                if (map[i][idx]=='H'){        
                                    j = idx + 1;
                                    flag = 1;
                                    break;
                                }
                            }
                                if (!flag){
                                    i++;
                                    j = 0;
                                }
                        }
                        else{
                            i++;
                            j = 0;
                        }
                    }
                    else{
                        j++;
                    }
                    break;
                case 2:
                    if (checkL(i, j, n)){
                        if (r[i+j] && !checkR(i, j, n)){
                            j++;
                            continue;
                        }
                        map[i][j]='Q';
                        qu.emplace(i, j);
                        v[j] = 1;
                        if (!r[i+j])
                            r[i+j] = 1;
                        if (h[i]){
                            int flag = 0;
                            for (int idx = j + 1; idx < n - 1 ;idx++){
                                if (map[i][idx]=='H'){ 
                                    j = idx + 1;
                                    flag = 1;
                                    break;
                                }
                            }
                            if (!flag){
                                i++;
                                j = 0;
                            }
                        }
                        else{
                            i++;
                            j = 0;
                        }
                    }
                    else{
                        j++;
                    }
                    break;
                case 3:
                    if (checkR(i, j, n)){
                        map[i][j] = 'Q';
                        qu.emplace(i, j);
                        v[j] = l[i-j+n] = 1;
                        if (h[i]){
                            int flag = 0;
                            for (int idx = j + 1; idx < n - 1; idx++){
                                if (map[i][idx]=='H'){
                                    j = idx + 1;
                                    flag = 1;
                                    break;
                                }
                            }
                            if (!flag){
                                i++;
                                j = 0;
                            }
                        }
                        else{
                            i++;
                            j = 0;
                        }
                    }
                    else{
                        j++;
                    }
                    break;
                case 4:
                    map[i][j] = 'Q';
                    qu.emplace(i, j);
                    v[j] = l[i-j+n] = r[i+j] = 1;
                    if (h[i]){
                        int flag = 0;
                        for (int idx = j + 1; idx < n - 1; idx++){
                            if (map[i][idx]=='H'){
                                j = idx + 1;
                                flag = 1;
                                break;
                            }
                        }
                        if (!flag){
                            i++;
                            j = 0;
                        }
                    }
                    else{
                        i++;
                        j = 0;
                    }
                    break;
            }
            break;
        }
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
        if (j == n){
            i++;
            j = 0;
        }
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

void makeMap(FILE *fd, int n, int holenum){
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
        fscanf(fd, "%d %d", &t1, &t2);
        map[t1-1][t2-1] = 'H';
        l[t1 - t2 + n] = r[t1 + t2 - 2] = h[t1-1] = v[t2-1] = 'H';
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

    fscanf(fpin, "%d %d", &n, &holenum);
    makeMap(fpin, n, holenum);
    cnt = 0;
    switch(mode){
        case 1:
            iterative_N(n);
            break;
        case 2:
            recursive_N(0, 0, 0, n);
            break;
    }

    fprintf(fpout, "%d\n", cnt);
    fclose(fpin);
    fclose(fpout);
    return 0;
}