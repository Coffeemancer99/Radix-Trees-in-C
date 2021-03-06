#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct node{
    char* word;
    struct node * children;
    struct node * parent;
};

struct node* initNode(char* word){

    struct node *currNode=(struct node*)malloc(sizeof(struct node));
    currNode->word = (char *)calloc(35, sizeof(char));
    currNode->children=(struct node*)calloc(26, sizeof(struct node));
    strcpy(currNode->word, word);
    //if (currNode->children[1].children==0){ //This checks if it is an empty node
    return(currNode);
}

char* compareWords(char* newWord, char* currentWord, int index){
  char* match=(char *)calloc(35, sizeof(char));
  for (int i=index;i<35;i++){
    if (newWord[i]==currentWord[i]){
      match[i]=newWord[i];
    }
    else{
      i=35;
    }
  }
  if (match[0]==0){
    //No match
    return 0;
  }
  return match;
}

//This helper method will free variables of the current node.
void freeNode(struct node* currNode){

  free(currNode->word); //Free the word
  free(currNode->children); //Free the 26 pointers
  free(currNode); //Free the node itself

}



void insert(struct node* root, char* newWord){
  struct node* tempRoot=root;
  while(tempRoot!=0){
    int nextIndex=(newWord[0]-97); //Turn the first char into ASCII


    break;
  }

  //while(tempRoot!=0){

  //}

}

int main() {
    //struct node *tree=(struct node*)malloc(sizeof(struct node));//This is a root sentinel node.
    //char* tester = (char *)malloc(sizeof(char)*35);
    struct node *tree=initNode("hey");


    //if (compareWords(tree->word, "he", 0)==0){
    //  printf("Yo this aint right");
    //}
    insert(tree, "aey");
    freeNode(tree);

    //char* h=initNode(*tree, "This is the root");
    //printf("%s",h);
    //printf("%s", h);

    int hamburg=1;
    char input = ' ';
    while(hamburg) {

        scanf("%c", &input);
        printf("%c", input);

        switch (input) {

            case 'i': //Insert
                break;

            case 's': //Search
                break;

            case 'e': //Empty
                break;

            case 'p': //Print
                break;

            case 'n': //Trie n number of nodes
                break;

            case 'w': //Trie w number of words
                break;

            case 'q': //Quit
                hamburg=0;
                break;
        }
    }


    return 0;
}
