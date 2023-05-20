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

int map_x[20], map_y[20];
int iter_l[20], iter_r[20];
int iter_cnt;
int iter_tot;
int iter_possible(int i, int j, int n){
    int flag, k;
    for (k = 0; k < iter_cnt; k++){
        if (j == map_y[k]){
            if (v[j] == 'H'){
                for (int p = map_x[k]+1; p < i;p++){
                    if (map[p][j] == 'H')
                        return 1;
                }
            }
            else
                return 0;
        }
        else if (i - j + n == iter_l[k]){
            if (l[i-j+n] == 'H'){
                int left = map_x[k], right = map_y[k];
                while (left != i && right != j){
                    if (map[left][right] == 'H')
                        return 1;
                    left++;
                    right++;
                }
            }
            else
                return 0;
        }
        else if (i + j == iter_r[k]){
            if (r[i+j] == 'H'){
                int left = map_x[k], right = map_y[k];
                while (left != i && right != j){
                    if (map[left][right] == 'H')
                        return 1;
                    left++;
                    right--;
                }
            }
            else
                return 0;
        }
    }
    return 1;
}

void iterative_N(int n){
    int cur_r = 0, cur_c = 0;
    int cnt = 0;
    while (cur_r >= 0){
        while (cur_c < n){
            printf("%d %d %d\n", cur_r, cur_c, iter_cnt);
            if (map[cur_r][cur_c] == 'H'){
                cur_c++;
                continue;
            }
            else if (!iter_possible(cur_r, cur_c, n))
                cur_c++;
            else {
                map_x[iter_cnt] = cur_r;
                map_y[iter_cnt] = cur_c;
                iter_l[iter_cnt] = cur_r-cur_c+n;
                iter_r[iter_cnt] = cur_r+cur_c;
                iter_cnt++;

                if (h[cur_r]){
                    int idx;
                    printf("AA%d %d\n",cur_r, cur_c);
                    for (idx = cur_c + 1; idx < n - 1; idx++){
                        if (map[cur_c][idx]=='H'){
                            cur_c = idx + 1;
                            break;
                        }
                    }
                    if (idx >= n - 1){
                        cur_r++;
                        cur_c = 0;
                    }
                }
                else {
                    cur_r++;
                    cur_c = 0;
                }
            }
        }
        if (cur_c == n){
            cur_r++;
            cur_c = 0;
        }
        if (cur_r == n){
            printf("%d %d %daaa", cur_r, cur_c, iter_tot);
            if (iter_cnt == n){
                iter_tot++;
            }
            iter_tot--;
            cur_r = map_x[iter_tot];
            cur_c = map_y[iter_tot];
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
    //printMap(n);
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
            printf("%d\n", iter_tot);
            break;
        case 2:
            recursive_N(0, 0, 0, n);
            fprintf(fpout, "%d\n", cnt);
            break;
    }

    fclose(fpin);
    fclose(fpout);
    return 0;
}