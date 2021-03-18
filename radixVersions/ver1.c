#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct node{
    char* word;
    struct node * children;
    int size;
    int isWord;
    int totalWords;
    int totalNodes;
};


struct node* initNode(char* word, int isWord){
    struct node *currNode=(struct node*)calloc(1, sizeof(struct node));
    currNode->word = (char *)calloc(35, sizeof(char));
    currNode->children=(struct node*)calloc(26, sizeof(struct node));
    strcpy(currNode->word, word);
    currNode->isWord=isWord;
    //if (currNode->children[1].children==0){ //This checks if it is an empty node
    return(currNode);
}

char* compareWords(char* newWord, char* currentWord, int index){
    char* match=(char *)calloc(35, sizeof(char));
    int latestChar=0;
    for (int i=0;i<35;i++){
        if (newWord[i]==currentWord[i]){
            match[i]=newWord[i];
            latestChar=i;
        }
        else{
            i=35;
        }
    }
    if (match[0]==0){

        //No match
        return 0;
    }
    latestChar++;
    if (index==0)
    {
        for (int j = 0; j < 35 - latestChar; j++) {
            match[j] = currentWord[j + latestChar];
        }
    }

    return match;
}
//This is a helper method that compares two words to see
//if a newly added word will be a suffix.
int checkSuffix(char* newWord, char* currentWord) {
    for (int i = 0; i < strlen(currentWord); i++) {
        if (newWord[i] != currentWord[i]) {
            return 0;
        }
    }
    return 1;
}

//This helper method will free variables of the current node.
void freeNode(struct node* currNode){
    free(currNode->word); //Free the word
    free(currNode->children); //Free the 26 pointers
    //free(currNode); //Free the node itself
}

int search(struct node* root, char* query){

    int nextIndex=(query[0]-97);
        if (root->children[nextIndex].children!=0){
            if (strcmp(query, root->children[nextIndex].word)==0){
                return 1;
            }
            if (checkSuffix(query, root->children[nextIndex].word)==1){
                char *ourSuffix =(compareWords(root->children[nextIndex].word, query, 0));
                return search(root, ourSuffix);
            }
        }
    return 0;

}
void empty(struct node* root, struct node* trueRoot){
    int start=0;

    while(start<26) {
        if (root->children[start].children!=0 ){

            empty(&(root->children[start]), trueRoot);

                      }
        start++;
    }
    freeNode(root);
}

void walk(struct node* root, struct node* trueRoot, char* together, int currCount){
    int start=0;
    int otherCount =0;

    while(start<26) {
        if (root->children[start].children!=0){
            if (trueRoot==root){
                strcpy(together, "\0");
            }
            for (int i=0;i<35;i++){
                otherCount=i+currCount;
                if (root->children[start].word[i]==0){
                    i=35;
                    break;
                }
                together[i+currCount]=root->children[start].word[i];
            }
            for (int i=otherCount;i<35;i++){
                together[i]=0;
            }
            if(root->children[start].isWord==1){
                printf("%s\n", together);
            }
            walk(&root->children[start], trueRoot, together, otherCount);
        }
        start++;
    }
}


void insert(struct node* root, char* newWord){
    struct node* tempRoot=root;
    int nextIndex=(newWord[0]-97); //Turn the first char into ASCII
    while(tempRoot!=0){
        //If the respective slot is empty
        if (tempRoot->children[nextIndex].children==0){
            //Add it as a new node.
            root->totalWords++;
            root->totalNodes++;
            tempRoot->children[nextIndex]=*initNode(newWord, 1); //Initialize a node as a new word

            tempRoot->children[nextIndex].size=strlen(newWord); //Set the size
            return;
        }
        else { //There is a conflict with the word we are trying to add.

            //If the slot that occupies the conflict happens to be the same word
            //Then we should not add it at all.
            if (strcmp(newWord, tempRoot->children[nextIndex].word)==0){

                return;
            }
            //If the word is a suffix(Identical up until a specific char)...
                if (checkSuffix(newWord, tempRoot->children[nextIndex].word)) {

                    //We get the characters that do NOT match(The suffix)
                    newWord = (compareWords(tempRoot->children[nextIndex].word, newWord, 0));
                    //Increment the pointer
                    tempRoot = &(tempRoot->children[nextIndex]);
                }
            //If the word is a prefix... Problem child
                else{

                    //The beginning of the word we are on/current word.
                    root->totalWords++;
                    root->totalNodes++;
                    root->totalNodes++;
                    char *prefix = (compareWords(tempRoot->children[nextIndex].word, newWord, 1));
                    //The end of our current word
                    char *ourSuffix =(compareWords(tempRoot->children[nextIndex].word, newWord, 0));
                    //The end of the word we are on
                    char *newSuffix = (compareWords( newWord, tempRoot->children[nextIndex].word, 0));

                    //Store the word we are on before altering it
                    struct node tempNode = tempRoot->children[nextIndex];

                    if (ourSuffix[0]==0) {

                        tempRoot->children[nextIndex] = *initNode(prefix, 1);
                    }
                    else {
                        tempRoot->children[nextIndex] = *initNode(prefix, 0);
                    }
                        int newIndex=(newSuffix[0]-97);
                        int ourIndex=(ourSuffix[0]-97);
                        strcpy(tempNode.word, newSuffix);
                        tempRoot->children[nextIndex].children[newIndex]=tempNode;
                        tempRoot->children[nextIndex].children[ourIndex]=*initNode(ourSuffix,1);
                        return;
                }
        }
            nextIndex=(newWord[0]-97);
        }
    }




int main() {

    struct node *tree=initNode("", 0);

    tree->totalWords=0;
    tree->totalNodes=1;

    int hamburg=1;
    char input = ' ';
    char *heh = (char *)calloc(35, sizeof(char));
    char *word = (char *)calloc(35, sizeof(char));
    while(hamburg) {

        scanf("%c", &input);


        switch (input) {

            case 'i': //Insert
                scanf("%s", word);
                insert(tree, word);
                break;

            case 's': //Search
                scanf("%s", word);
                if (search(tree, word)){
                    printf("%s is present\n", word);
                }
                else {
                    printf("%s is missing\n", word);
                }
                break;

            case 'e': //Empty
                empty(tree, tree);
                //free(tree);
                tree=initNode("", 0);

                break;

            case 'p': //Print
                walk(tree, tree, heh, 0);
                break;

            case 'n': //Trie n number of nodes
                printf("Trie contains %d nodes\n", tree->totalNodes);
                break;

            case 'w': //Trie w number of words
                printf("Trie contains %d words\n", tree->totalWords);
                break;

            case 'q': //Quit
                hamburg=0;
                empty(tree, tree);
                free(tree);
                free(heh);
                free(word);

                break;
        }
    }


    return 0;
}
