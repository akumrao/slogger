
#ifndef PLAYLINKLIST_H_
#define PLAYLINKLIST_H_

#define PageListLen  5
#define MaxPlayListLen  16

#include "playlinklist.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include <stdio.h>



typedef struct LinkList
{
    struct LinkList * next;
    char *path;
    char *file;
    
    struct LinkList *prv;
    
} stPlayList;


const char * extract_file_name(const char *path);

void displaylist( stPlayList *list );

bool addplaylist_front( stPlayList **filelist, const char *path );

bool addplaylist_last( stPlayList **filelist, const char *path );

bool removeplaylist( stPlayList **filelist, const char *path,  stPlayList **pagehead);

bool removeplayNode( stPlayList **filelist, stPlayList *node,  stPlayList **pagehead);

bool saveplaylist( stPlayList *list );
 
bool readplaylist( stPlayList **list );

void deleteList(stPlayList* curr);

bool swapplaylist( stPlayList **filelist, const char *path, bool next, stPlayList **selnode);

bool swapplayNode(  stPlayList *node, bool next , stPlayList **selnode);

bool getPage(stPlayList *list , bool next,  stPlayList **pagehead);

void displayPage( stPlayList *list);

#endif
