
#ifndef PLAYLINKLIST_H_
#define PLAYLINKLIST_H_

#define PlayListLen  4


typedef struct LinkList
{
    struct LinkList * next;
    char path[256];
    char file[256];
} stPlayList;



//typedef struct 
//{
//    stPlayList *filelist;
//    int count;
//   
//} stFileLinkList;
//
//



#include "playlinklist.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include <stdio.h>




const char * extract_file_name(const char *path);

void displaylist( stPlayList *list );

bool addplaylist_front( stPlayList **filelist, const char *path );

bool addplaylist_last( stPlayList **filelist, const char *path );

bool removeplaylist( stPlayList **filelist, const char *path );

bool saveplaylist( stPlayList *list );
 
bool readplaylist( stPlayList **list );

void deleteList(stPlayList* curr);


#endif
