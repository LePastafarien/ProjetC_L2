#include <stdio.h>
#include <stdlib.h>
#include "printTree.h"
#include <search.h>
#include <time.h>
#include "tree.h"
#include "dicoParser.h"

int main() {

    time_t t;

    /* Intializes random number generator */
    srand((unsigned) time(&t));
    FILE *f;
    f = fopen("/dictionnaire.txt", "r");

    tree nTree = createEmptyTree();
    tree adjTree = createEmptyTree();
    tree advTree = createEmptyTree();
    tree verTree = createEmptyTree();

    fillTreeDictionary(&verTree, f, 1);
    fillTreeDictionary(&adjTree, f, 2);
    fillTreeDictionary(&nTree, f, 3);
    fillTreeDictionary(&advTree, f, 4);

    int mode = 0;
    while (1){
        int reponse;
        printf("1 : trouver une forme de Base\n");
        printf("2 : créer une phrase de type : nom – adjectif – verbe – nom\n");
        printf("3 : créer une phrase de type : nom – adjectif – verbe – nom\n");
        printf("4 : créer une phrase de type : nom - verbe - adverbe - nom - adjectif\n");
        printf("5 : quitter\n");
        scanf("%d",&reponse);
        if(reponse == 1){
            sizedTab tab = getBaseFormTab(f, mode);
            printf("La forme de base généré : %s\n", tab.tab[rand() % tab.size]);
        }
        if(reponse == 2){
            n_a_v_n(nTree, adjTree, verTree);
        }
        if(reponse == 3){
            n_qui_v_v_n_a(nTree, adjTree, verTree);
        }
        if(reponse == 4){
            n_v_adv_n_adj(nTree, adjTree, verTree, advTree);
        }
        if(reponse == 5){
            break;
        }
        if(reponse != 5){
            printf("sélectionner une valeur de la liste\n");
        }
    }
    fclose(f);
    return 0;
}
