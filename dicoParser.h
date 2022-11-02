/** \file dicoParser.h
    \brief tsv .txt dictionary parser
    \authors Antoine Ribot
    \authors Jacques Soghomonyan
    \authors Nicolas Chalumeau
*/
#ifndef PROJETC_2022_DICOPARSER_H
#define PROJETC_2022_DICOPARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * \struct s_t
 * \brief a simple sized string array
 * \param tab the tab
 * \param size the logical size of the array
 * \param maxSize the maximum size wished
 * */
struct s_t{
    char** tab;
    int size;
    int maxSize;
};

/**
 * \typedef struct s_t sizedTab
 * \brief declaration of sized tab
 */
typedef struct s_t sizedTab;

/**
 * \typedef char* string;
 * \brief replaces char* by string for convenience
 */
typedef char* string;

/**
 * \def MIN(a,b)
 * \brief gives the smallest (in length) string of the two
 */
#define MIN(a,b) (strlen(a) < strlen(b) ? a : b)

/**
 * \fn unsigned int getCount(FILE* f, char delimiter, char valToCount);
 * \brief counts occurrences of \a valtoCount in the \a f file, until it meats \a delimiter
 * \param f the file to search in
 * \param delimiter the char at witch the function should stop
 * \param valToCount the char we are trying to match
 * \returns the number of occurrences of \a valCount
 */
unsigned int getCount(FILE*, char, char);

/**
 * \def getLineCount(f)
 * \brief counts appearances of lines in the specified file
 */
#define getLineCount(f) getCount(f, EOF, '\n')

/**
 * \def getColumnCount(f)
 * \brief counts appearances of columns in the specified file
 */
#define getColumnCount(f) getCount(f, '\n', '\t')

/**
 * \fn sizedTab getColumnTab(FILE*, int, int);
 * \brief gives the tabs of all the elements in a column
 * \warning throws an error if \code{c}getColumnCount(f) > column\endcode
 * \param f the file to search in
 * \param column the index of the column
 * \param noDupes whether or not there should duplicates in the tab
 * \returns a tab of all the strings in the column
 */
sizedTab getColumnTab(FILE*, int, int);

/**
 * \fn sizedTab getLineTab(FILE*, int);
 * \brief gives the tabs of all the elements in a line
 * \warning throws an error if \code{c}getLineCount(f) < line\endcode
 * \param f the file to search in
 * \param line the index of the line
 * \returns a tab of all the strings in the line
 */
sizedTab getLineTab(FILE*, int);

/**
 * \fn int inCharTab(char*, sizedTab);
 * \brief checks if a string is in a string array
 * \note binary search based algorithm (O(log(n))
 * \param val the value to find
 * \param tab the \b sorted tab in which to search
 * \warning \b tab needs to be sorted
 * \returns a boolean representing if a string is in the given array
 */
int inCharTab(string, string*, int);

/**
 * \fn int strGreater(const void*, const void*);
 * \brief basically the \a compare function for \a qsort
 * \param _str1 a pointer to a string
 * \param _str2 the same as \a _str1
 * \note \a 1 if \a str1 > \a str2 \n \a 0 if \a str1 = \a str2 \n \a -1 otherwise
 * \returns the lexicographic order of \a str1 and \a str2
 */
int strGreater(const void*, const void*);

#endif //PROJETC_2022_DICOPARSER_H