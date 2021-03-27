#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "radHeader.h"

//This file holds all of the functions used in the radixMain file. 

//This function will allocate space in memory for a new addition to the trie.
struct node* initNode(char* word, int isWord){
    //Allocate space for a node and empty out (calloc).
    struct node *currNode=(struct node*)calloc(1, sizeof(struct node));
    //Allocate space for a string and empty it out (calloc).
    currNode->word = (char *)calloc(35, sizeof(char));
    //Allocate space for children(up to 26) and empty it out by default (calloc)
    currNode->children=(struct node*)calloc(26, sizeof(struct node));
    //Set the new node's word equal to the desired phrase.
    strcpy(currNode->word, word);
    //This is a flag that will be used later to tell if the current node is an
    //"Ending word". Mainly used in the print tree function.
    currNode->isWord=isWord;
    return(currNode); //Return their newly allocated node!
}
//This function will compare two words. Depending on the index flag, it will
//return either the suffix or prefix(matching) words;
char* compareWords(char* newWord, char* currentWord, int index){
  //Allocate and empty out space in memory for new string
    char* match=(char *)calloc(35, sizeof(char));
    //This variable keeps track of which position we are at(for suffix case)
    int latestChar=0;
    for (int i=0;i<35;i++){ //For every character in both words...
        if (newWord[i]==currentWord[i]){ //If they are equal.
            match[i]=newWord[i];//We assign that char to the matching char array
            latestChar=i; //We adjust the position of the latestChar
        }
        else{ //The moment we see a case that it is not equal...
            i=35; //We exit the loop, we have found the matching characters.
        }
    }
    if (match[0]==0){ //If we did not find any matching characters (Zeroed.)
        //No match
        return 0;
    }
    //Start at the next position in the word.
    latestChar++;
    if (index==0) //This is the suffix case, will give us words after match
    { //Will be useful when there are conflicts within containers.
        for (int j = 0; j < 35 - latestChar; j++) {
            match[j] = currentWord[j + latestChar];
        }
    }
    //Return either the matching words, or the suffix. Depending on the flag.
    return match;
}

//This is a helper method that compares two words to see
//if a newly added word will be a suffix.
int checkSuffix(char* newWord, char* currentWord) {
    for (int i = 0; i < strlen(currentWord); i++) {
        if (newWord[i] != currentWord[i]) {
            return 0; //The moment they are not equal, return 0
        }
    }
    return 1;
}

//This helper method will free variables of the current node.
void freeNode(struct node* currNode){
    free(currNode->word); //Free the word
    free(currNode->children); //Free the 26 pointers
    free(currNode); //Free the node itself
}

//This method is envoked when the user enters the "s" option. It will
//Search for the query and return if it is present within the Radix trie.
int search(struct node* root, char* query){

    int nextIndex=(query[0]-97); //Get the ascii container for the query.
    if (root->children[nextIndex].children!=0){ //If there is a word in the slot
      //Check if their query is equal to the one in that container
        if (strcmp(query, root->children[nextIndex].word)==0){
            return 1; //Return 1, success! We have found their word.
        }
        //Otherwise, more work may need to be done. Suffixes come to mind to
        //identify which position of the trie needs to be explored next.
        if (checkSuffix(query, root->children[nextIndex].word)==1){
          //If we are a suffix, then lets grab the words after the ones that match.
            char *ourSuffix =(compareWords(root->children[nextIndex].word, query, 0));
            strcpy(query, ourSuffix); //Copy it over from the one in memory
            free(ourSuffix); //Free the allocation from memory
            //And now we search with the new query recursively.
            return search(&root->children[nextIndex], query);
        }
    }
    return 0; //If we have reached here, the query was not identified.

}

//This is a method that is envoked when the user enters the "e" option. It will
//not only provide a fresh tree, but will additionally free everything from
//memory for safe allocation practices. No leaks will occur due to this method
//walking and freeing simutaniously.
void empty(struct node* root, struct node* trueRoot){
  //This is the starting position of the children.
    int start=0;
    //For each child...
    while(start<26) {
        //If there is an occupied node here
        if (root->children[start].word!=0 ){
          //Walk down the tree from the node(lexographically)
            empty(&(root->children[start]), trueRoot);
        }
        start++; //Increment to the next child
    }
    free(root->children); //Free the children in memory
    free(root->word); //Free the word in memory
}

//This method will walk the tree and print out the words within the trie.
//Is initiated through the "p" command.
void walk(struct node* root, struct node* trueRoot, char* together, int currCount){
    int start=0; //This will keep track of the children
    int otherCount =0; //This will determine which characters must be zeroed out
    //For every child
    while(start<26) {
        //If there is no child, then we are a leaf node.
        if (root->children[start].children!=0){
            if (trueRoot==root){ //If we are equal to the root, zero out
                strcpy(together, "\0"); //This is a new phrase now
            }
            //For every character
            for (int i=0;i<35;i++){
                otherCount=i+currCount; //Adjust the zeroed out count
                //If the character is a null character, break out.
                if (root->children[start].word[i]==0){
                    i=35;
                    break;
                }
                //Otherwise, add to the current phrase the characters
                //In this position.
                together[i+currCount]=root->children[start].word[i];
            }
            //Now we must zero out the string if the previous string is
            //greater than the current word we are printing. This Will
            //zero out everything AFTER the word for an accurate representation
            //of the phrase.
            for (int i=otherCount;i<35;i++){
                together[i]=0;
            }
          //If we are equal to a word, print the phrase we have been adding to

            if(root->children[start].isWord==1){
                printf("%s\n", together);
            }
            //Walk recursively using the current child.
            walk(&root->children[start], trueRoot, together, otherCount);
        }
        start++; //Move to the next child.
    }
}

//This method will insert a node node into the radix trie. Cases are handled
//differently depending on if they are a suffix, new word, or a collision.
void insert(struct node* root, char* newWord){
    struct node* tempRoot=root;
    int nextIndex=(newWord[0]-97); //Turn the first char into ASCII
    while(tempRoot!=0){
        //If the respective slot is empty
        if (tempRoot->children[nextIndex].children==0){
            //Add it as a new node.
            //We have a new word, increment the count
            root->totalWords++;
            //We have a new node, increment the count.
            root->totalNodes++;
            //Create a new node equal to the current phrase.
            struct node* newlyNode=initNode(newWord, 1);
            //Initialize a node as a new word
            tempRoot->children[nextIndex]=*newlyNode;

            tempRoot->children[nextIndex].size=strlen(newWord); //Set the size
            free(newlyNode); //Free it from memory, it is already assigned.
            return;
        }
        else { //There is a conflict with the word we are trying to add.

            //If the slot that occupies the conflict happens to be the same word
            //Then we should not add it at all.
            //However, if it is not a word yet then we should make it one.
            if (strcmp(newWord, tempRoot->children[nextIndex].word)==0){
                if (tempRoot->children[nextIndex].isWord==0){
                    tempRoot->children[nextIndex].isWord=1;
                }
                return;
            }
            //If the word is a suffix(Identical up until a specific char)...
            if (checkSuffix(newWord, tempRoot->children[nextIndex].word)) {

                //We get the characters that do NOT match(The suffix)
                char* tempWord=(compareWords(tempRoot->children[nextIndex].word, newWord, 0));
                strcpy(newWord, tempWord); //Copy it out before freeing from memory
                free(tempWord); //Free the pointer from memory
                //Increment the pointer
                tempRoot = &(tempRoot->children[nextIndex]);
            }
                //If the word is a prefix...
            else{

                //The beginning of the word we are on/current word.
                root->totalWords++; //There is oen new word.
                root->totalNodes++; //If we are here, the word is splitting.
                //Thus, we must create 2 more nodes as a result.

                char *prefix = (compareWords(tempRoot->children[nextIndex].word, newWord, 1));
                //The end of our current word
                char *ourSuffix =(compareWords(tempRoot->children[nextIndex].word, newWord, 0));
                //The end of the word we are on
                char *newSuffix = (compareWords( newWord, tempRoot->children[nextIndex].word, 0));

                //Store the word we are on before altering it
                struct node tempNode = tempRoot->children[nextIndex];

                //If the current word does NOT have a suffix...
                if (ourSuffix[0]==0) {

                    struct node* prefixNode =initNode(prefix, 1);
                    //We set it equal to the shared phrase.
                    tempRoot->children[nextIndex] = *prefixNode;
                    free(prefix); //Free it from memory
                    free(prefixNode); //Free it from memory.
                }
                else { //Otherwise, we DO have a suffix.

                    //This is going to be the shared phrase
                    struct node* assignedNode = initNode(prefix, 0);
                    free(prefix); //Done, free it in memory
                    //Set it equal to the shared phrase
                    tempRoot->children[nextIndex] = *assignedNode;
                    free(assignedNode); //Done, free it in memory.
                }
                //Subtract ascii value to get next child location.
                int newIndex=(newSuffix[0]-97);
                //This will determine if we have a suffix.
                int ourIndex=0;
                if(ourSuffix[0]!=0){
                  root->totalNodes++;
                  //If we do have a suffix, set it equal to the next location.
                  ourIndex=(ourSuffix[0]-97);
                  //This is the split case, we will add a new node using the
                  //suffix of the word we are adding
                  struct node* suffixNode=initNode(ourSuffix,1);
                  tempRoot->children[nextIndex].children[ourIndex]=*suffixNode;
                  free(suffixNode); //Done, free it in memory
                }

                //Copy the suffix of the word that was conflicted .
                strcpy(tempNode.word, newSuffix);
                //Put it back together to finish the split.
                tempRoot->children[nextIndex].children[newIndex]=tempNode;
                free(ourSuffix); //Done, free it in memory
                free(newSuffix);  //Done, free it in memory
                return;
            }
        }
        nextIndex=(newWord[0]-97); //Assign a new bucket based off of ASCII val
    }
}
