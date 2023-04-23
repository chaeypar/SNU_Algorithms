#include<stdio.h>
#include<stdlib.h>

typedef struct Node{
    struct Node *left;
    struct Node *right;
    struct Node *p;
    int key;
    int size;
    int red;
}Node;

typedef struct Tree{
    Node *root;
    Node *nil;
}Tree;

void resetTree(Tree **tree){
    *tree = (Tree*)malloc(sizeof(Tree));
    (*tree)->nil = (Node *)malloc(sizeof(Node));
    
    Node *nil = (*tree)->nil;
    nil->key = nil->size = nil->red = 0;
    nil->p = nil->left = nil->right = NULL;

    (*tree)->root = nil;
    (*tree)->root->p = nil;
}

void nodefree(Tree* tree, Node *node){
    if (node->left != tree->nil){
        nodefree(tree, node->left);
        node->left = NULL;
    }
    if (node->right != tree->nil){
        nodefree(tree, node->right);
        node->right = NULL;
    }
}

void closefile(FILE *fpin, FILE *fpout){
    fclose(fpin);
    fclose(fpout);
}

void leftRotate(Tree *tree, Node *x){
    Node *y = x->right;
    x->right = y->left;
    if (y->left != tree->nil)
        y->left->p = x;
    
    y->p = x->p;
    if (x->p == tree->nil)
        tree->root = y;
    else if (x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;
    
    y->left = x;
    x->p = y;

    y->size = x->size;
    x->size = x->left->size + x->right->size + 1;
}

void rightRotate(Tree *tree, Node *x){
    Node *y = x->left;
    x->left = y->right;
    if (y->right != tree->nil)
        y->right->p = x;
    y->p = x->p;
    if (x->p == tree->nil)
        tree->root = y;
    else if (x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;
    
    y->right = x;
    x->p = y;

    y->size = x->size;
    x->size = x->left->size + x->right->size + 1;
}

void RB_Insert_Fixup(Tree *tree, Node *node){
    
    Node *y;

    while (node->p->red){
        if (node->p == node->p->p->left){
            y = node->p->p->right;
            if (y->red){
                node->p->red = y->red = 0;
                node->p->p->red = 1;
                node = node->p->p;
            }
            else {
                if (node == node->p->right){
                    node = node->p;
                    leftRotate(tree, node);
                }
                else{
                    node->p->red = 0;
                    node->p->p->red = 1;
                    rightRotate(tree, node->p->p);
                }
            }
        }
        else if (node->p == node->p->p->right){
            y = node->p->p->left;
            if (y->red){
                node->p->red = y->red = 0;
                node->p->p->red = 1;
                node = node->p->p;
            }
            else{
                if (node == node->p->left){
                    node = node->p;
                    rightRotate(tree, node);
                }
                else{
                    node->p->red = 0;
                    node->p->p->red = 1;
                    leftRotate(tree, node->p->p);
                }
            }
        }
    }
    tree->root->red=0;
}

int OS_Insert(Tree *tree, int op){
    Node *prev = tree->nil;
    Node *cur = tree->root;

    while (cur!=tree->nil){
        prev = cur;
        if (op == cur->key)
            return 0;
        else if (op < cur->key)
            cur = cur->left;
        else
            cur = cur->right;
    }

    Node *node = (Node *)malloc(sizeof(Node)); 
    node->left = node->right = tree->nil;
    node->key = op;
    node->red = 1;
    node->size = 1;   
    node->p = prev;

    if (prev == tree->nil)
        tree->root = node;
    else if (op < prev->key)
        prev->left = node;
    else
        prev->right = node;

    if (node != tree->root){
        cur = node->p;
        while (1){
            (cur->size)++;
            if (cur == tree->root)
                break;
            cur = cur -> p; 
        }
    }

    RB_Insert_Fixup(tree, node);
    return op;
}

void RB_Transplant(Tree *tree, Node *u, Node *v){
    if (u->p == tree->nil)
        tree->root = v;
    else if (u == u->p->left)
        u->p->left = v;
    else
        u->p->right = v;
    v->p = u->p;
}

void RB_Delete_Fixup(Tree *tree, Node *node){
    while ((node!=tree->root) && !(node->red)){
        if (node == node->p->left){
            Node *w = node->p->right;
            if (w->red){
                w->red = 0;
                node->p->red = 1;
                leftRotate(tree, node->p);
                w = node->p->right;
            }
            if (!(w->left->red) && !(w->right->red)){
                w->red=1;
                node = node->p;
            }
            else {
                if (!(w->right->red)){
                    w->left->red = 0;
                    w->red = 1;
                    rightRotate(tree, w);
                    w = node->p->right;
                }
                else{
                    w->red = node->p->red;
                    node->p->red = 0;
                    w->right->red = 0;
                    leftRotate(tree, node->p);
                    node = tree->root;    
                }
            }
        }
        else{
            Node *w = node->p->left;
            if (w->red){
                w->red = 0;
                node->p->red = 1;
                rightRotate(tree, node->p);
                w = node->p->left;
            }
            if (!(w->left->red) && !(w->right->red)){
                w->red = 1;
                node = node->p;
             }
            else{
                if (!(w->left->red)){
                    w->right->red = 0;
                    w->red = 1;
                    leftRotate(tree, w);
                    w = node->p->left;
                }
                else{
                    w->red = node->p->red;
                    node->p->red = 0;
                    w->left->red = 0;
                    rightRotate(tree, node->p);
                    node = tree->root;
                }
            }
        }
    }
    node->red = 0;
}

Node *Tree_Minimum(Tree *tree, Node *node){
    while(node->left != tree->nil)
        node = node->left;
    return node;
}

int OS_Delete(Tree *tree, int op){
    
    Node *node = tree->root;
    while (node != tree->nil){
        if (op == node->key)
            break;
        else if (op < node->key)
            node = node->left;
        else
            node = node->right;
    }
    if (node == tree->nil)
        return 0;
    
    Node *x, *y = node;
    int original_red = y->red;
    if (node->left == tree->nil){
        Node *tp = node->p;
        while (tp!= tree->nil){
            (tp->size)--;
            tp = tp->p;
        }

        x = node->right;
        RB_Transplant(tree, node, node->right);
        free(node);
        node = NULL;
    }
    else if (node->right == tree->nil){
        Node *tp = node->p;
        while (tp!= tree->nil){
            (tp->size)--;
            tp = tp->p;
        }

        x = node->left;
        RB_Transplant(tree, node, node->left);
        free(node);
        node = NULL;
    }
    else{
        y = Tree_Minimum(tree, node->right);

        Node *tp = y->p;
        while (tp!= tree->nil){
            (tp->size)--;
            tp = tp->p;
        }
        y->size = node->size;

        original_red = y->red;
        x = y->right;
        if (y->p == node)
            x->p = y;
        else{
            RB_Transplant(tree, y, y->right);
            y->right = node->right;
            y->right->p = y;
        }
        RB_Transplant(tree, node, y);
        y->left = node->left;
        y->left->p = y;
        y->red = node->red;
        free(node);
        node = NULL;
    }
    if (!original_red)
        RB_Delete_Fixup(tree, x);
    
    return op;
}

int RB_Select(Node *node, int i){
    int r = node->left->size + 1;
    if (i == r)
        return node->key;
    else if (i < r)
        return RB_Select(node->left, i);
    else
        return RB_Select(node->right, i-r);
}

int OS_Select(Tree *tree, int op){
    if (op > tree->root->size)
        return 0;

    return RB_Select(tree->root, op);
}

int OS_Rank(Tree *tree, int op){
    Node *node = tree->root;

    while (node != tree->nil){
        if (node->key == op)
            break;
        if (node->key > op)
            node = node->left;
        else 
            node = node->right;
    }
    if (node == tree->nil)
        return 0;
    
    int rk = node->left->size+1;
    while (node!=tree->root){
        if (node == node->p->right)
            rk += node->p->left->size+1;
        node = node->p;
    }
    return rk;
}

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
    FILE *fpout=fopen(argv[2], "wt");
    if (!fpout){
        printf("Failure to open the output file. Program is terminated.\n");
        return -1;
    }

    Tree *tree;
    resetTree(&tree);

    char mode, sp;
    int op, num;

    while(1){
        while(1){
            sp = fgetc(fpin);
            if (sp == '\n' || sp == ' ')
                continue;
            else if (sp == EOF){
                closefile(fpin, fpout);
                nodefree(tree, tree->root);
                tree->root = NULL;
                free(tree->nil);
                tree->nil = NULL;
                free(tree);
                tree = NULL;
                return 0;
            }
            else{
                mode = sp;
                break;
            }
        }
        fscanf(fpin, "%d", &op);
        switch(mode){
            case 'I':
                num = OS_Insert(tree, op);
                break;
            case 'D':
                num = OS_Delete(tree, op);
                break;
            case 'S':
                num = OS_Select(tree, op);
                break;
            case 'R':
                num = OS_Rank(tree, op);
                break;
        }
        fprintf(fpout, "%d\n", num);
    }
    
    return 0;
}