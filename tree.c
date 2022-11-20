#include "tree.h"
node* createNode(char val){ //Fonction qui remplie un noeuds avec une valeur
    node * n = malloc(sizeof(node));
    n->letter = val;
    n->right = NULL;
    n->left = NULL;
    n->EOW = 0;
    flexed f;
    n->flexedWords = f;
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


void fillTreeDictionary(tree* dicoTree, FILE* f, int mode){
    sizedTab baseFormTab = getBaseFormTab(f, mode);
    char* word;
    dicoTree->root = createNode('a');
    node* tempNode = dicoTree->root;

    for (int i = 0; i < baseFormTab.size; ++i) {
        word = strdup(baseFormTab.tab[i]);

        tempNode = addLetterRight(dicoTree->root, word[0]);

        for (int j = 1; j < strlen(word); ++j)
            tempNode = addLetterLeft(tempNode, word[j]);

        tempNode->EOW = 1;
//        tempNode->flexedWords = getFlexedWords(f, word);
    }
}

char* randomWord(tree Tree){
    char* mot = malloc(25);
    node* noeud = Tree.root;
    int fin = 0;
    int nbLettre = 0;

    int chance = 20;

    while (fin != 1){
        int n = rand()%100 + 1;
        if (n<=chance){
            mot[nbLettre++] = noeud->letter;

            if(n <= chance / 10 && noeud->EOW == 1)
                fin = 1;
            if (noeud->left == NULL)
                fin = 1;
            else
                noeud = noeud->left;
        }
        else{
            if(noeud->right == NULL && noeud->EOW == 1) {
                fin = 1;
                mot[nbLettre++] = noeud->letter;
            }
            if(noeud->right == NULL && noeud->EOW == 0) {
                mot[nbLettre++] = noeud->letter;
                noeud = noeud->left;
            }
            else
                noeud = noeud->right;
        }
    }
    mot[nbLettre]='\0';
    return mot;
}

void n_a_v_n(tree nom, tree adjectif, tree verbe){
    char* nom1 = randomWord(nom);
    char* adj = randomWord(adjectif);
    char* verb = randomWord(verbe);
    char* nom2 = randomWord(nom);
    printf("%s %s %s %s.\n",nom1,adj,verb,nom2);
}

void n_qui_v_v_n_a(tree nom, tree adjectif, tree verbe){
    char* nom1 = randomWord(nom);
    char* adj = randomWord(adjectif);
    char* verbe1 = randomWord(verbe);
    char* verbe2 = randomWord(verbe);
    char* nom2 = randomWord(nom);
    printf("%s, qui %s %s, %s %s.\n",nom1,verbe1,verbe2,nom2, adj);
}

void n_v_adj_n_adv(tree nom, tree adjectif, tree verbe, tree adverbe){
    char* nom1 = randomWord(nom);
    char* verbe1 = randomWord(verbe);
    char* adj = randomWord(adjectif);
    char* nom2 = randomWord(nom);
    char* adv = randomWord(adverbe);
    printf("%s %s %s %s %s.\n",nom1,verbe1,adv,nom2, adj);
}