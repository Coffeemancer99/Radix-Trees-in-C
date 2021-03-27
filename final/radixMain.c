#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "radHeader.h"
//This file holds the main file. Here it will take user commands to
//perform various actions on the RADIX tree. It will take commands until they
//quit out properly by using the q(uit) case.

int main() {
    struct node *tree=initNode("", 0); //This is the main root of the tree.
    tree->totalWords=0; //This initializes the words at 0.
    tree->totalNodes=1; //This initializes the nodes at 1(counting the root)
    int active=1;//This is a flag that will determine if the user can still
    //make commands.
    char input = ' '; //This is a character that will be the user's input
    char *dummyWord = (char *)calloc(35, sizeof(char)); //This is a dummy
    //string used for the print method, preallocated for convenience.
    char *word = (char *)calloc(35, sizeof(char)); //This will be an allocated
    //string that the user will put their words into. Used in most functions.
    while(active) { //While we can take input...
        scanf("%c", &input); //Get the command from the user
        switch (input) { //Go to the command they provided
            case 'i': //Insert
                scanf("%s", word); //Grab the word from the user
                insert(tree, word); //Insert the word into the tree
                break;

            case 's': //Search
                scanf("%s", word); //Grab the word from the user
                char* query = (char*)(calloc(35, sizeof(char)));
                //Allocate a space for the query before alteration
                strcpy(query, word); //Copy their input over.
                if (search(tree, word)){ //If we have found the word
                    printf("%s is present\n", query); //Print it out
                }
                else { //Otherwise, it is not present within the tree.
                    printf("%s is missing\n", query);
                }
                free(query); //We are done with query, lets properly free it.
                break;

            case 'e': //Empty
                empty(tree, tree); //Call upon the function using the root
                free(tree); //Once we are done, free the root.
                tree=initNode("", 0); //We allocate the root with fresh memory.
                tree->totalWords=0; //Reset the total words
                tree->totalNodes=1; //Reset the total nodes.
                break;

            case 'p': //Print
                walk(tree, tree, dummyWord, 0); //Walk down the tree and print
                break;

            case 'n': //Trie n number of nodes
                //Print the nodes stored by the trie. (Total)
                printf("Trie contains %d nodes\n", tree->totalNodes);
                break;

            case 'w': //Trie w number of words
                //Print the words stored by the trie. (Total)
                printf("Trie contains %d words\n", tree->totalWords);
                break;

            case 'q': //Quit
                active=0; //The user can no longer enter commands.
                empty(tree, tree); //Clear everything to avoid leaks.
                free(tree); //Done with root
                free(dummyWord); //Done with dummy word
                free(word); //Done with user input string
                break;
        }
    }
    return 0;
}
