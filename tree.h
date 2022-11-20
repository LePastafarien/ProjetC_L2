/** \file tree.h
    \brief the definiton of the tree data structure and its associated functions
    \authors Antoine Ribot
    \authors Jacques Soghomonyan
    \authors Nicolas Chalumeau
*/
#ifndef PROJETC_2022_TREE_H
#define PROJETC_2022_TREE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dicoParser.h"

typedef struct node_s{ //Création de la structure pour créer un noeud
    char letter;
    struct node_s* right, *left;
    int EOW; // End Of Word
    flexed flexedWords;
} node;

typedef struct{ //Création de la structure pour créer l'arbre
    node* root;
} tree;

node* createNode(char);
tree createEmptyTree();
int hasLetterRight(node*, char);
void fillTreeDictionary(tree*, FILE*, int);
node* addLetterRight(node* n, char letter);
node* gotoLetterRight(node* n, char letter);
node* gotoLetterLeft(node* n, char letter);
char* randomWord(tree);
void n_a_v_n(tree nom, tree adjectif, tree verbe);
void n_qui_v_v_n_a(tree nom, tree adjectif, tree verbe);
void n_v_adj_n_adv(tree nom, tree adjectif, tree verbe, tree adverbe);
#endif //PROJETC_2022_TREE_H
