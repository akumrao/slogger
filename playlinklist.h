
#ifndef PLAYLINKLIST_H_
#define PLAYLINKLIST_H_

#define PlayListLen  4


typedef struct LinkList
{
    struct LinkList * next;
    char path[256];
   
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





void displaylist( stPlayList *list );

bool addplaylist( stPlayList **filelist, const char *path );

bool removeplaylist( stPlayList **filelist, const char *path );

bool saveplaylist( stPlayList *list );
 
bool readplaylist( stPlayList **list );




#endif
