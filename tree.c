#include "tree.h"
node* createNode(char val){ //Fonction qui remplie un noeuds avec une valeur
    node * n = malloc(sizeof(node));
    n->letter = val;
    n->right = NULL;
    n->left = NULL;
    n->EOW = 0;
    n->flexedWords = NULL;
    return n;
}

tree createEmptyTree(){ //Fonction qui crée un arbre vide
    tree t;
    t.root = NULL;
    return t;
}

int hasLetterRight(node* n, char letter){
    if (n == NULL || n->letter > letter)
        return 0;
    if(n->letter == letter)
        return 1;
    return hasLetterRight(n->right, letter);
}

//int hasLetterLeft(node* n, char letter){
//    if(n->left == NULL)
//        n->left = createNode(letter);
//    return hasLetterRight(n->left, letter);
//}

node* addLetterRight(node* n, char letter){
    if (n == NULL)
        return n;
    if (hasLetterRight(n, letter))
        return gotoLetterRight(n, letter);

    while (n->right != NULL && n->right->letter < letter)
        n = n->right;

    node* temp = n->right;
    n->right = createNode(letter);
    n->right->right = temp;
    return n->right;
}

node* addLetterLeft(node* n, char letter){
    if(n == NULL)
        return n;
    if(n->left == NULL)
        return n->left = createNode(letter);

    return addLetterRight(n->left, letter);
}

node* gotoLetterRight(node* n, char letter){
    if (n == NULL || n->letter > letter)
        return NULL;
    if(n->letter == letter)
        return n;
    return gotoLetterRight(n->right, letter);
}
node* gotoLetterLeft(node* n, char letter){
    if (n == NULL || n->letter > letter)
        return NULL;
    if(n->letter == letter)
        return n;
    return gotoLetterLeft(n->left, letter);
}


void fillTreeDictionary(tree* dicoTree, FILE* f){
    sizedTab baseFormTab = getBaseFormTab(f);
    char* word;
    dicoTree->root = createNode('a');
    node* tempNode = dicoTree->root;

    for (int i = 0; i < baseFormTab.size; ++i) {
        word = strdup(baseFormTab.tab[i]);

        tempNode = addLetterRight(dicoTree->root, word[0]);

        for (int j = 1; j < strlen(word); ++j)
            tempNode = addLetterLeft(tempNode, word[j]);

        tempNode->EOW = 1;
    }
}