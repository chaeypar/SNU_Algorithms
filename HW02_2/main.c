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

// To sort the results in an ascending order
int compare(const void *first, const void *second);

// To print the result at the output file
void find_answer(FILE *fd);

// For mode 1: an adjacency matrix
void makeMatrix(FILE *fd, int n);
void dfs_regular_matrix(int i, int n);
void dfs_reverse_matrix(int i, int n);
void calMatrix(FILE *fd, int n);

// For mode 2: an adjacency list
void makeList(FILE *fd, int n);
int is_connected_list(int in, int out);
int is_reverse_connected_list(int in, int out);
void dfs_regular_list(int i, int n);
void dfs_reverse_list(int i, int n);
void calList(FILE *fd, int n);

// For mode 3: an adjacency array
void makeArray(FILE *fd, int n);
int is_connected_arr(int in, int out);
int is_reverse_connected_arr(int in, int out);
void dfs_regular_arr(int i, int n);
void dfs_reverse_arr(int i, int n);
void calArray(FILE *fd, int n);

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
        // Mode 1: an adjacency matrix
        case 1:
            makeMatrix(fpin, n);
            calMatrix(fpout, n);
            break;
        // Mode 2: an adjacency list
        case 2:
            makeList(fpin, n);
            calList(fpout, n);
            break;
        // Mode 3: an adjacency array
        case 3:
            makeArray(fpin, n);
            calArray(fpout, n);
            break;
    }

    // Close the input and output file
    fclose(fpin);
    fclose(fpout);
    return 0;
}

// compare : to sort the results in an ascending order
int compare(const void *first, const void *second){
    if (*(int *)first > *(int *)second)
        return 1;
    else if (*(int *)first < *(int *)second)
        return -1;
    return 0;
}

// find_answer : to print the output at the output file
void find_answer(FILE *fd){
    int arr[5001];
    int arrcnt = 0;

    // to do 'xor' operation for each cycle
    for (int i = 0; i < rescnt; i++){
        int tot = 0;
        for (int j = 0; result[i][j] != -1; j++){
            tot = tot ^ result[i][j];
        }
        arr[arrcnt++] = tot;
    }

    //to print the number of cycle
    fprintf(fd, "%d\n", rescnt);

    //to sort the 'xor'ed results in an ascending order
    qsort(arr, arrcnt, sizeof(int), compare);
    
    //to print the 'xor'ed results in an ascending order
    for (int i = 0; i < arrcnt - 1; i++){
        fprintf(fd, "%d ", arr[i]);
    }
    fprintf(fd, "%d\n", arr[arrcnt-1]);
}

// makeMarix : to make an adjacency matrix
void makeMatrix(FILE *fd, int n){
    int num, in, out;

    // to allocate a matrix for recording each cycle, an adjacency matrix and its reverse one.
    result = (int **)malloc((n+1)*sizeof(int *));
    adjMatrix = (char **)malloc((n+1)*sizeof(char *));
    reverseMatrix = (char **)malloc((n+1)*sizeof(char *));

    // to allocate an array for queue and an array for marking whether the nodes were visited
    qu = (int *)malloc((n+1)*sizeof(int));
    visited = (char *)malloc((n+1)*sizeof(char));

    for (int i = 0;i <= n; i++){
        adjMatrix[i] = (char *)calloc(n+1, sizeof(char));
        reverseMatrix[i] = (char *)calloc(n+1, sizeof(char));
        result[i] = (int *)calloc(n+1, sizeof(int));
    }

    // to initialize an adjacent matrix and its reverse one
    fscanf(fd, "%d", &num);
    for (int i = 0; i < num;i++){
        fscanf(fd, "%d %d", &in, &out);
        adjMatrix[in][out] = 1;
        reverseMatrix[out][in] = 1;
    }
}

// dfs_regular_matrix : to run dfs on a graph to compute finish time for each vertex
void dfs_regular_matrix(int i, int n){
    for (int j = 1; j <= n; j++){
        if (adjMatrix[i][j] && !visited[j]){
            visited[j] = 1;
            // recursive call for running dfs
            dfs_regular_matrix(j, n);
        }
    }
    // to compute finish time for each vertex
    qu[cur++] = i;
}

// dfs_reverse_matrix : to run dfs on a reversed graph
void dfs_reverse_matrix(int i, int n){
    result[rescnt][curidx++] = i;
    for (int j = 1; j <= n; j++){
        if (reverseMatrix[i][j] && !visited[j]){
            visited[j] = 1;
            // recursive call for running dfs
            dfs_reverse_matrix(j, n);
        }
    }
}

// calMatrix: to find strongly connected components
void calMatrix(FILE *fd, int n){
    // run dfs on a graph
    memset(visited, 0, n+1);
    for (int i=1;i<=n;i++){
        if (!visited[i]){
            visited[i] = 1;
            dfs_regular_matrix(i, n);
        }
    }

    // run dfs on G_r and output each tree as a strongly connected component
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
    //to print the output at the output file
    find_answer(fd);
}

// makeList : to make an adjacency list
void makeList(FILE *fd, int n){
    // to make an adjacency matrix for making an adjacency list easily
    makeMatrix(fd, n);

    // to allocate the head and reversehead for making linked lists
    head = (node_list **)calloc(n+1,sizeof(node_list *));
    reversehead = (node_list **)calloc(n+1, sizeof(node_list *));

    // to make an adjacency list 
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

    // to make a reversed adjacency list
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

// is_connected_list : to check whether node 'in' and 'out' is connected in an adjacency list
int is_connected_list(int in, int out){
    node_list *temp = head[in];
    while (temp){
        // the edge from the node 'in' to 'out' exists
        if (temp->num == out)
            return 1;
        // the node 'out' does not exist since it is sorted in an ascending order
        else if (temp->num > out)
            break;
        temp = temp->next;
    }
    return 0;
}

// is_reverse_connected_list : to check whether node 'in' and 'out' is connected in a reversed adjacency list
int is_reverse_connected_list(int in, int out){
    node_list *temp = reversehead[in];
    while (temp){
        // the edge from the node 'out' to 'in' exists
        if (temp->num == out)
            return 1;
        // the node 'out' does not exist since it is sorted in an ascending order
        else if (temp->num > out)
            break;
        temp = temp->next;
    }
    return 0;
}

// dfs_regular_list : to run dfs on a graph to compute the finish time for each vertex
void dfs_regular_list(int i, int n){
    for (int j = 1; j <= n; j++){
        if (!visited[j] && is_connected_list(i, j)){
            visited[j] = 1;
            // recursive call for running dfs
            dfs_regular_list(j, n);
        }
    }
    // to compute the finish time for each vertex
    qu[cur++] = i;
}

// dfs_reverse_list : to run dfs on a reversed graph
void dfs_reverse_list(int i, int n){
    result[rescnt][curidx++] = i;
    for (int j = 1; j <= n; j++){
        if (!visited[j] && is_reverse_connected_list(i, j)){
            visited[j] = 1;
            // recursive call for running dfs
            dfs_reverse_list(j, n);
        }
    }
}

// calList : to find strongly connected components
void calList(FILE *fd, int n){
    // run dfs on a gragh
    memset(visited, 0, n+1);
    for (int i=1;i<=n;i++){
        if (!visited[i]){
            visited[i] = 1;
            dfs_regular_list(i, n);
        }
    }

    // run dfs on G_r and output each tree as a strongly connected component
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
    //to print the output at the output file
    find_answer(fd);
}

// makeArray : to make an adjacency array
void makeArray(FILE *fd, int n){
    int num, in, out;

    // to allocate two arrays for recording the number of edges
    vertices_cnt = (int *)calloc(n + 1 ,sizeof(int));
    reverse_vertices_cnt = (int *)calloc(n + 1, sizeof(int));

    // to allocate a matrix for recording each cycle, an adjacency matrix and it reversed one
    result = (int **)malloc((n + 1)*sizeof(int *));
    adjMatrix = (char **)malloc((n + 1)*sizeof(char *));
    reverseMatrix = (char **)malloc((n + 1)*sizeof(char *));

    // to allocate an array for queue and an array for marking whether the nodes were visited
    qu = (int *)malloc((n + 1)*sizeof(int));
    visited = (char *)malloc((n + 1)*sizeof(char));

    for (int i = 0;i <= n; i++){
        adjMatrix[i] = (char *)calloc(n + 1, sizeof(char));
        reverseMatrix[i] = (char *)calloc(n + 1, sizeof(char));
        result[i] = (int *)calloc(n + 1, sizeof(int));
    }

    // to initialize an adjacent matrix and its reversed one
    fscanf(fd, "%d", &num);
    for (int i = 0; i < num;i++){
        fscanf(fd, "%d %d", &in, &out);
        // calculate the number of edges for G and G_r
        vertices_cnt[in]++;
        reverse_vertices_cnt[out]++;
        adjMatrix[in][out] = 1;
        reverseMatrix[out][in] = 1;
    }

    // to allocate two arrays for recording edges
    adjArr = (int **)malloc((n+1)*sizeof(int *));
    reverseArr = (int **)malloc((n+1)*sizeof(int *));

    // to initialize an array for recording edges of G
    for (int i = 0; i <= n; i++){
        adjArr[i] = (int *)malloc(vertices_cnt[i]*sizeof(int));
        int tp = 0;
        for (int j = 1; j <= n; j++){
            if (adjMatrix[i][j]){
                adjArr[i][tp++] = j;
            }
        }
    }

    // to initialize an array for recording edges of G_r
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

// is_connected_arr : to check whether the edge from node 'in' to 'out' exists in G 
int is_connected_arr(int in, int out){
    int temp = vertices_cnt[in];
    for (int i = 0; i < temp; i++){
        // the edge from node 'in' to 'out' exists in G
        if (adjArr[in][i] == out)
            return 1;
        // there does not exist the edge since an array is sorted in an ascending order
        else if (adjArr[in][i] > out)
            break;
    }
    return 0;
}

// is_reverse_connected_arr : to check whether the edge from node 'in' to 'out' exists in G_r
int is_reverse_connected_arr(int in, int out){
    int temp = reverse_vertices_cnt[in];
    for (int i = 0; i < temp; i++){
        // the edge from node 'in' to 'out' exists in G_r
        if (reverseArr[in][i] == out)
            return 1;
        // there does not exist the edge since an array is sorted in an ascending order
        else if (reverseArr[in][i] > out)
            break;
    }
    return 0;
}

// dfs_regular_arr : to run dfs on a graph to compute the finish time for each vertex
void dfs_regular_arr(int i, int n){
    for (int j = 1; j <= n; j++){
        if (!visited[j] && is_connected_arr(i, j)){
            visited[j] = 1;
            // recursive call for running dfs
            dfs_regular_arr(j, n);
        }
    }
    // to compute the finish time for each vertex
    qu[cur++] = i;
}

// dfs_reverse_arr : to run dfs on a reversed graph
void dfs_reverse_arr(int i, int n){
    result[rescnt][curidx++] = i;
    for (int j = 1; j <= n; j++){
        if (!visited[j] && is_reverse_connected_arr(i, j)){
            visited[j] = 1;
            // recursive call for running dfs
            dfs_reverse_arr(j, n);
        }
    }
}

// calArray : to find strongly connected components
void calArray(FILE *fd, int n){
    // run dfs on a graph
    memset(visited, 0, n+1);
    for (int i=1;i<=n;i++){
        if (!visited[i]){
            visited[i] = 1;
            dfs_regular_arr(i, n);
        }
    }

    // run dfs on G_r and output each tree as a strongly connected component
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
    // to print the output at the output file
    find_answer(fd);
}