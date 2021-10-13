#include<stdio.h>
#include <string.h>

char to_lower(char c){
    if (c - 'A' >= 0 && 'Z' - c >= 0) {
        return c + 32;
    }
    return c;
}

int is_valid_char(char i) {
    if (i == ' ' || i == '\t' || i == '\n' || i == '(' || i == ')' || i == ',')
        return 0;
    return 1;
}

void normalize(char* buff) {
    for(int i = 0; i<strlen(buff); ++i){
        buff[i] = to_lower(buff[i]);
    }
}

void remove_redundant(char* word) {
    if (word[strlen(word)-1] != '.') return;
    int cnt = 0;
    for(int i = 0; i<strlen(word); ++i){
        if (word[i] == '.')
            cnt ++;
    }
    if (cnt > 1) return;
    word[strlen(word)-1] = '\0';
}

struct node
{
    char word[255];
    int times;
    int x[1000], y[1000];
    struct node *right_child;
    struct node *left_child;
};

struct node* new_node(char*word, int x, int y)
{
    struct node *p;
    p = malloc(sizeof(struct node));
    p->x[0] = x;
    p->y[0] = y;
    p->times = 1;
    strcpy(p->word, word);
    p->left_child = NULL;
    p->right_child = NULL;

    return p;
}

struct node* insert(struct node *root, char*word, int x, int y)
{
    if(root==NULL)
        return new_node(word,x,y);
    int cmp = strcmp(word,root->word);
    if(cmp>0)
        root->right_child = insert(root->right_child, word, x, y);
    else if (cmp < 0)
        root->left_child = insert(root->left_child, word, x, y);
    else {
        (root->x)[root->times] = x;
        (root->y)[root->times] = y;
        (root->times)++;
    }
    return root;
}

void inorder(struct node *root, FILE *out)
{
    if(root==NULL) return;
    inorder(root->left_child, out);

    fprintf(out, "%s %d, ", root->word, root->times);
    for(int i = 0; i<root->times; ++i)
        fprintf(out, "(%d,%d) ",(root->x)[i],(root->y)[i]);
    fprintf(out, "\n");

    inorder(root->right_child, out);
}

int main() {
     FILE *inp, *out, *stopw_inp;
     char buff[255];

     inp = fopen("vanban.txt", "r");
     stopw_inp = fopen("stopw.txt", "r");
     out = fopen("index.txt", "w+");
     char stopw[100000];

     while(fgets(buff, 255, (FILE*)stopw_inp)){
        strcat(stopw, buff);
     }

     fclose(stopw_inp);

     int cnt = 0;
     char* s[255];
     struct node *root = NULL;

     while(fgets(buff, 255, (FILE*)inp)){ // read line
            cnt++;
            normalize(buff); // lower all character
            int i = 0;
            // start split word
            while(i < strlen(buff)) {
                while(i < strlen(buff) && !is_valid_char(buff[i])) i++;
                int j = i;
                while(i < strlen(buff) && is_valid_char(buff[i])) i++;
                char word[255];
                strncpy(word,&buff[j],i-j); // get the word
                word[i-j] = '\0';
                remove_redundant(word); // remove redundant char '.'
                if (strstr(stopw, word)) // ignore if word is stop word
                    continue;
                if(root == NULL)
                    root = new_node(word,cnt,j+1);
                else
                    insert(root,word,cnt,j+1);
            }
     }
     inorder(root, out); // print index

     fclose(inp);
     fclose(out);
    return 0;
}
