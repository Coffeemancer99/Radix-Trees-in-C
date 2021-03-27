#ifndef DEC_H
#define DEC_H
//This file contains the primary structure used within the Radix Trie, the node
//structure. In addition to the main structure, the functions are stored in here
//and later utilized when running the Makefile. //Comments regarding these
//functions and the structure can be found in the radixFunc.c file.
struct node{
    char* word;
    struct node * children;
    int size;
    int isWord;
    int totalWords;
    int totalNodes;
};
struct node* initNode(char* word, int isWord);
char* compareWords(char* newWord, char* currentWord, int index);
int checkSuffix(char* newWord, char* currentWord);
void freeNode(struct node* currNode);
int search(struct node* root, char* query);
void empty(struct node* root, struct node* trueRoot);
void walk(struct node* root, struct node* trueRoot, char* together, int currCount);
void insert(struct node* root, char* newWord);


#endif
