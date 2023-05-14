#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

// For an adjacency matrix
char **adjMatrix;
char **reverseMatrix;
int **result;

// For an adjaceny list
typedef struct node_list{
    int num;
    struct node_list* next;
}node_list;

node_list **head;
node_list **reversehead;

// For an adjacency array
int *vertices_cnt;
int **adjArr;

int *reverse_vertices_cnt;
int **reverseArr;

// Common variables for three implementations
char *visited;
int *qu;
int cur = 0;
int curidx;
int rescnt;

int compare(const void *first, const void *second){
    if (*(int *)first > *(int *)second)
        return 1;
    else if (*(int *)first < *(int *)second)
        return -1;
    return 0;
}

void find_answer(FILE *fd){
    int arr[5001];
    int arrcnt = 0;

    for (int i = 0; i < rescnt; i++){
        int tot = 0;
        for (int j = 0; result[i][j] != -1; j++){
            tot = tot ^ result[i][j];
        }
        arr[arrcnt++] = tot;
    }
    fprintf(fd, "%d\n", rescnt);
    qsort(arr, arrcnt, sizeof(int), compare);
    for (int i = 0; i < arrcnt - 1; i++){
        fprintf(fd, "%d ", arr[i]);
    }
    fprintf(fd, "%d\n", arr[arrcnt-1]);
}

void makeMatrix(FILE *fd, int n){
    int num, in, out;

    result = (int **)malloc((n+1)*sizeof(int *));
    adjMatrix = (char **)malloc((n+1)*sizeof(char *));
    reverseMatrix = (char **)malloc((n+1)*sizeof(char *));

    qu = (int *)malloc((n+1)*sizeof(int));
    visited = (char *)malloc((n+1)*sizeof(char));

    for (int i = 0;i <= n; i++){
        adjMatrix[i] = (char *)calloc(n+1, sizeof(char));
        reverseMatrix[i] = (char *)calloc(n+1, sizeof(char));
        result[i] = (int *)calloc(n+1, sizeof(int));
    }

    fscanf(fd, "%d", &num);
    for (int i = 0; i < num;i++){
        fscanf(fd, "%d %d", &in, &out);
        adjMatrix[in][out] = 1;
        reverseMatrix[out][in] = 1;
    }
}

void dfs_regular_matrix(int i, int n){
    for (int j = 0; j <= n; j++){
        if (adjMatrix[i][j] && !visited[j]){
            visited[j] = 1;
            dfs_regular_matrix(j, n);
        }
    }
    qu[cur++] = i;
}

void dfs_reverse_matrix(int i, int n){
    result[rescnt][curidx++] = i;
    for (int j = 0; j <= n; j++){
        if (reverseMatrix[i][j] && !visited[j]){
            visited[j] = 1;
            dfs_reverse_matrix(j, n);
        }
    }
}

void calMatrix(FILE *fd, int n){
    memset(visited, 0, n+1);
    for (int i=1;i<=n;i++){
        if (!visited[i]){
            visited[i] = 1;
            dfs_regular_matrix(i, n);
        }
    }

    memset(visited, 0, n+1);
    for (int i = n-1; i >= 0; i--){
        curidx = 0;
        if (!visited[qu[i]]){
            visited[qu[i]] = 1;
            dfs_reverse_matrix(qu[i], n);
            result[rescnt][curidx] = -1;
            rescnt++;
        }
    }
    find_answer(fd);
}

int is_connected_list(int in, int out){
    node_list *temp = head[in];
    while (temp){
        if (temp-> num == out)
            return 1;
        else if (temp -> num > out)
            break;
        temp = temp->next;
    }
    return 0;
}
int is_reverse_connected_list(int in, int out){
    node_list *temp = reversehead[in];
    while (temp){
        if (temp-> num == out)
            return 1;
        else if (temp -> num > out)
            break;
        temp = temp->next;
    }
    return 0;
}

void dfs_regular_list(int i, int n){
    for (int j = 1; j <= n; j++){
        if (!visited[j] && is_connected_list(i, j)){
            visited[j] = 1;
            dfs_regular_matrix(j, n);
        }
    }
    qu[cur++] = i;
}

void dfs_reverse_list(int i, int n){
    result[rescnt][curidx++] = i;
    for (int j = 0; j <= n; j++){
        if (!visited[j] && is_reverse_connected_list(i, j)){
            visited[j] = 1;
            dfs_reverse_matrix(j, n);
        }
    }
}

void makeList(FILE *fd, int n){
    makeMatrix(fd, n);

    head = (node_list **)calloc(n+1,sizeof(node_list *));
    reversehead = (node_list **)calloc(n+1, sizeof(node_list *));

    for (int i = 1; i <= n; i++){
        int flag = 0;
        node_list *curpt = NULL;
        for (int j = 1; j <= n; j++){
            if (adjMatrix[i][j]){
                node_list *temp = (node_list *)malloc(sizeof(node_list));
                temp -> num = j;
                temp -> next = NULL;
                if (!flag){
                    flag = 1;
                    head[i] = temp;
                    curpt = temp;
                }
                else{
                    curpt -> next = temp;
                    curpt = curpt -> next;
                }
            }
        }
    }

    for (int i = 1; i <= n; i++){
        int flag = 0;
        node_list *curpt = NULL;
        for (int j = 1; j <= n; j++){
            if (reverseMatrix[i][j]){
                node_list *temp = (node_list *)malloc(sizeof(node_list));
                temp -> num = j;
                temp -> next = NULL;
                if (!flag){
                    flag = 1;
                    reversehead[i] = temp;
                    curpt = temp;
                }
                else {
                    curpt -> next = temp;
                    curpt = curpt -> next;
                }
            }
        }
    }
}

void calList(FILE *fd, int n){
    memset(visited, 0, n+1);
    for (int i=1;i<=n;i++){
        if (!visited[i]){
            visited[i] = 1;
            dfs_regular_list(i, n);
        }
    }

    memset(visited, 0, n+1);
    for (int i = n-1; i >= 0; i--){
        curidx = 0;
        if (!visited[qu[i]]){
            visited[qu[i]] = 1;
            dfs_reverse_list(qu[i], n);
            result[rescnt][curidx] = -1;
            rescnt++;
        }
    }
    find_answer(fd);
}


int is_connected_arr(int in, int out){
    int temp = vertices_cnt[in];
    for (int i = 0; i < temp; i++){
        if (adjArr[in][i] == out)
            return 1;
    }
    return 0;
}

int is_reverse_connected_arr(int in, int out){
    int temp = reverse_vertices_cnt[in];
    for (int i = 0; i < temp; i++){
        if (reverseArr[in][i] == out)
            return 1;
    }
    return 0;
}

void makeArray(FILE *fd, int n){
    int num, in, out;

    vertices_cnt = (int *)calloc(n +1 ,sizeof(int));
    reverse_vertices_cnt = (int *)calloc(n + 1, sizeof(int));

    result = (int **)malloc((n+1)*sizeof(int *));
    adjMatrix = (char **)malloc((n+1)*sizeof(char *));
    reverseMatrix = (char **)malloc((n+1)*sizeof(char *));

    qu = (int *)malloc((n+1)*sizeof(int));
    visited = (char *)malloc((n+1)*sizeof(char));

    for (int i = 0;i <= n; i++){
        adjMatrix[i] = (char *)calloc(n+1, sizeof(char));
        reverseMatrix[i] = (char *)calloc(n+1, sizeof(char));
        result[i] = (int *)calloc(n+1, sizeof(int));
    }

    fscanf(fd, "%d", &num);
    for (int i = 0; i < num;i++){
        fscanf(fd, "%d %d", &in, &out);
        vertices_cnt[in]++;
        reverse_vertices_cnt[out]++;
        adjMatrix[in][out] = 1;
        reverseMatrix[out][in] = 1;
    }

    adjArr = (int **)malloc((n+1)*sizeof(int *));
    reverseArr = (int **)malloc((n+1)*sizeof(int *));

    for (int i = 0; i <= n; i++){
        adjArr[i] = (int *)malloc(vertices_cnt[i]*sizeof(int));
        int tp = 0;
        for (int j = 1; j <= n; j++){
            if (adjMatrix[i][j]){
                adjArr[i][tp++] = j;
            }
        }
    }

    for (int i = 0; i <= n; i++){
        reverseArr[i] = (int *)malloc(reverse_vertices_cnt[i]*sizeof(int));
        int tp = 0;
        for (int j = 1; j <= n; j++){
            if (reverseMatrix[i][j]){
                reverseArr[i][tp++] = j;
            }
        }
    }

}

void dfs_regular_arr(int i, int n){
    for (int j = 1; j <= n; j++){
        if (!visited[j] && is_connected_arr(i, j)){
            visited[j] = 1;
            dfs_regular_arr(j, n);
        }
    }
    qu[cur++] = i;
}

void dfs_reverse_arr(int i, int n){
    result[rescnt][curidx++] = i;
    for (int j = 1; j <= n; j++){
        if (!visited[j] && is_reverse_connected_arr(i, j)){
            visited[j] = 1;
            dfs_reverse_arr(j, n);
        }
    }
}

void calArray(FILE *fd, int n){
   memset(visited, 0, n+1);
    for (int i=1;i<=n;i++){
        if (!visited[i]){
            visited[i] = 1;
            dfs_regular_arr(i, n);
        }
    }

    memset(visited, 0, n+1);
    for (int i = n-1; i >= 0; i--){
        curidx = 0;
        if (!visited[qu[i]]){
            visited[qu[i]] = 1;
            dfs_reverse_arr(qu[i], n);
            result[rescnt][curidx] = -1;
            rescnt++;
        }
    }

    find_answer(fd);
}


int main(int argc, char *argv[]){

    int n, mode;

    //There should be four arguments, i.e. executable file, mode, input file and output file.
    if (argc!=4){
        printf("There should be four arguments. Check once again.\n");
        return -1;
    }

    //If argv[1] is not 1 or 2, then it is not valid.
    mode = atoi(argv[1]);
    if (mode!=1 && mode!=2 && mode!=3){
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

    fscanf(fpin, "%d", &n);
    switch(mode){
        case 1:
            makeMatrix(fpin, n);
            calMatrix(fpout, n);
            break;
        case 2:
            makeList(fpin, n);
            calList(fpout, n);
            break;
        case 3:
            makeArray(fpin, n);
            calArray(fpout, n);
            break;
    }
    
    fclose(fpin);
    fclose(fpout);
    return 0;
}