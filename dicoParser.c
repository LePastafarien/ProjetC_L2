#include "dicoParser.h"
#include <err.h>

unsigned int getCount(FILE* f, char delimiter, char valToCount){
    if (f == NULL)
        err(EXIT_FAILURE, "in `getCount`");

    rewind(f);

    char buff = (char) fgetc(f);
    if (buff == EOF)
        return 0;

    unsigned int count = 1;

    while (buff != delimiter){
        buff = (char) fgetc(f);
        if(buff == valToCount)
            count++;
    }

    rewind(f);

    return count;
}

sizedTab getColumnTab(FILE* f, int column, int noDupes, int mode){
    if(column <= 0 || getColumnCount(f) < column)
        err(EXIT_FAILURE, "Index out of range, trying to access non existing column\n");

    rewind(f);

    int bufferSize = 255; // arbitrary, we assume that no lines are over 255 in length
    char* buff = malloc(bufferSize * sizeof(char));

    unsigned int nbLine = getLineCount(f);

    sizedTab t;
    t.tab = malloc(nbLine * sizeof(char*));
    t.maxSize = (int) nbLine;

    char *temp, *temp2 = NULL, *string;

    int idx = 0;

    // we just blindly fill an array with everything we come across
    for (int i = 0; i < t.maxSize; ++i) {
        fgets(buff, bufferSize, f);

        // to skip the '\n' at the EOL
        string = strndup(buff, strlen(buff) - 1);

        for (int j = 0; j < column; ++j)
            temp = strsep(&string, "\t");

        t.size = idx;

        int shouldBeStored = 0;
        for (int j = 0; j < 3 - column; ++j)
            temp2 = strsep(&string, "\t");
        if(temp2 == NULL) temp2 = strdup(temp);

        if(mode == 0)
            shouldBeStored = 1;
        else if (mode == 1){
            if(temp2[0] == 'V' && temp2[1] == 'e' && temp2[2] == 'r')
                shouldBeStored = 1;
        }
        else if (mode == 2) {
            if (temp2[0] == 'A' && temp2[1] == 'd' && temp2[2] == 'j')
                shouldBeStored = 1;
        }
        else if (mode == 3) {
            if (temp2[0] == 'N' && temp2[1] == 'o' && temp2[2] == 'm')
                shouldBeStored = 1;
        }
        else if (mode == 4) {
            if (temp2[0] == 'A' && temp2[1] == 'd' && temp2[2] == 'v')
                shouldBeStored = 1;
        }

//        printf("%c%c%c   %s  %d - %d\n", temp2[0], temp2[1], temp2[2], temp, shouldBeStored, mode);
        if(shouldBeStored) t.tab[idx++] = strdup(temp);
    }
    //free(buff);

    if(noDupes){
        int size = 0;
        // It is quicker to sort and delete duplicates with the binary search (O(n))
        // tanks stdlib
        qsort(t.tab, t.size, sizeof(char*), strGreater);

        // The point of the duplicated code is to optimize space
        // we first calculate size to allocate the right memory space to tab2
        for (int i = 0; i < t.size; ++i) {
            while (i + 1 < t.size && !strcmp(t.tab[i], t.tab[i + 1]))
                i++;
            size++;
        }

        char** tab2 = malloc(size * sizeof(char*));

        // and only when we have the right size we fill the array with non duplicates
        size = 0;
        for (int i = 0; i < t.size; ++i) {
            // we skip duplicates (they are in order => one after the other)
            while ((i + 1 < t.size) && (!strcmp(t.tab[i], t.tab[i + 1])))
                i++;
            tab2[size++] = t.tab[i];
        }

        free(t.tab);
        t.tab = tab2;
        t.size = size;
    }
    rewind(f);
    return t;
}

sizedTab getLineTab(FILE* f, int line){
    if(line <= 0 || getLineCount(f) < line)
        err(EXIT_FAILURE, "Index out of range, trying to access non existing line\n");

    rewind(f);

    int bufferSize = 255;
    char* buff = malloc(bufferSize * sizeof(char));

    int nbCol = (int) getColumnCount(f);

    sizedTab t;
    t.tab = malloc(nbCol * sizeof(char*));
    t.maxSize = nbCol;
    t.size = nbCol;

    // we iterate to the specified line
    for (int i = 0; i < line; ++i)
        fgets(buff, bufferSize, f);

    char *temp = strndup(buff, strlen(buff) - 1);

    for (int i = 0; i < nbCol; ++i)
        t.tab[i] = strdup(strsep(&temp, "\t"));

    free(buff);
    free(temp);

    rewind(f);
    return t;
}

int strGreater(const void* _str1, const void* _str2){
    string str1 = *(string *) _str1;
    string str2 = *(string *) _str2;

    //printf("%s, %s\n", str1, str2);
    if(strcmp(str1, str2) == 0)
        return 0;

    for (int i = 0; i < strlen(MIN(str1, str2)); ++i){
        if(str1[i] > str2[i])
            return 1;
        else if(str1[i] < str2[i])
            return -1;
    }

    return !strcmp(str1, MIN(str1, str2)) ? -1 : 1;
}

int strGreater2(string str1, string str2){
    //printf("%s, %s\n", str1, str2);
    if(strcmp(str1, str2) == 0)
        return 0;

    for (int i = 0; i < strlen(MIN(str1, str2)); ++i){
        if(str1[i] > str2[i])
            return 1;
        else if(str1[i] < str2[i])
            return -1;
    }

    return !strcmp(str1, MIN(str1, str2)) ? -1 : 1;
}

int inCharTab(string val, string* tab, int size, int getIdx){
    if(size <= 0)
        return -1;

    int left = 0, right = size - 1;
    int middle;

    // binary search (with ascii char)
    while (left <= right){
        middle = (right + left) / 2;

//        for (int i = 0; i < size; ++i)
//            printf("%s, ", tab[i]);

        if(!strcmp(tab[middle], val))
            return getIdx ? middle : 1;

        if(strGreater2(val, tab[middle]))
            left = middle + 1;
        else
            right = middle - 1;
    }

    return 0;
}

toHash getFlexedWords(FILE* f, int mode){
    rewind(f);

    toHash hash;
    hash.keys = getColumnTab(f, 2, 1, mode);
    hash.fl = malloc(hash.keys.size * sizeof(flexed));
    for (int i = 0; i < hash.keys.size; ++i) {
        hash.fl[i].flexedTab = malloc(sizeof(char *));
        hash.fl[i].size = 1;
    }

    sizedTab flexedWordsTab = getColumnTab(f, 1, 0, mode);
    sizedTab rawBaseWordsTab = getColumnTab(f, 2, 0, mode);

    int bufferSize = 255;
    char* buff = malloc(bufferSize * sizeof(char));
    int nbCol = (int) getColumnCount(f);
    char** tab = malloc(nbCol * sizeof(char*));
    char* temp;
    int lines = getLineCount(f);
    int shouldBeStored = 0;

    for (int i = 0; i < lines; ++i) {
        fgets(buff, bufferSize, f);

        shouldBeStored = 0;

        temp = strndup(buff, strlen(buff) - 1);
        for (int j = 0; j < nbCol; ++j)
            tab[j] = strdup(strsep(&temp, "\t"));

        if (mode == 0)
            shouldBeStored = 1;
        else if (mode == 1) {
            if (tab[2][0] == 'V' && tab[2][1] == 'e' && tab[2][2] == 'r')
                shouldBeStored = 1;
        } else if (mode == 2) {
            if (tab[2][0] == 'A' && tab[2][1] == 'd' && tab[2][2] == 'j')
                shouldBeStored = 1;
        } else if (mode == 3) {
            if (tab[2][0] == 'N' && tab[2][1] == 'o' && tab[2][2] == 'm')
                shouldBeStored = 1;
        } else if (mode == 4) {
            if (tab[2][0] == 'A' && tab[2][1] == 'd' && tab[2][2] == 'v')
                shouldBeStored = 1;
        }

        if (shouldBeStored) {
            int idx = inCharTab(tab[1], hash.keys.tab, hash.keys.size, 1);
            //printf("%d %s\n", idx, tab[1]);
            if(idx >= 0){
                hash.fl[idx].flexedTab[hash.fl[idx].size - 1] = tab[1];
                hash.fl[idx].flexedTab = realloc(hash.fl[idx].flexedTab, ++hash.fl[idx].size * sizeof(char *));
            }
        }
    }

    free(buff);
    //free(temp);
//    printf("%s\n", baseWord);
    rewind(f);
    return hash;
}