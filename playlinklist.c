
#include "playlinklist.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include <stdio.h>


void deleteList(stPlayList* curr) {

    // Base case: If the list is empty, return
    if (curr == NULL) {
        return;
    }

    // Recursively delete the next node
    deleteList(curr->next);

    // Delete the current node
    free( curr);
}



void displaylist( stPlayList *list )
{
    while(list != NULL)
    {
        printf("\n%s\n", list->path);
        list = list->next;
    }
}

bool addplaylist_last( stPlayList **filelist, const char *path )
{

   stPlayList *list = *filelist;

   stPlayList *prv = NULL;
   stPlayList *prvtoPrv = NULL;

   int count = 0;

    while(list != NULL)
    {
        if(!strcmp( list->path, path))
        {
             stPlayList * tmp =  list->next;

            if(prv)
              prv->next = tmp;
            else
              *filelist = tmp;

            free( list);

            list = tmp;
            continue;
        }
        prv = list;

        list = list->next;
        ++count;
    }

    if(count == PlayListLen )
    {
        stPlayList *tmp  = (*filelist)->next;
        free( *filelist);
        *filelist = tmp;

    }


    stPlayList *l = (stPlayList *)malloc(sizeof(stPlayList));
    strcpy( l->path, path);
  //  strcpy( l->file, extract_file_name(path));
    l->next = NULL;

    if(prv )
      prv->next = l;
    else if(*filelist )
        prv->next = l;
    else
    *filelist = l;

    return true;


}


bool addplaylist_front( stPlayList **filelist, const char *path  )
{

   stPlayList *list = *filelist;

   stPlayList *prv = NULL;
   stPlayList *prvtoPrv = NULL;
    
   int count = 0;

    while(list != NULL)
    {
        if(!strcmp( list->path, path))
        {
             stPlayList * tmp =  list->next;

            if(prv)
              prv->next = tmp;
            else
              *filelist = tmp;

            free( list);

            list = tmp;
            continue;
        }
        prv = list;
        
        if(list->next && list->next->next == NULL)
        {
            prvtoPrv = list;
            
        }

        list = list->next;
        ++count;
    }

   if(count == PlayListLen )
   {
       
        if(prvtoPrv )
        {
            
            free(prvtoPrv->next);
            prvtoPrv->next = NULL;
            
        }

   }


    stPlayList *l = (stPlayList *)malloc(sizeof(stPlayList));
    strcpy( l->path, path);
  //  strcpy( l->file, extract_file_name(path));
    l->next = NULL;

    l->next = *filelist;
      *filelist = l;

    return true;
}

 bool removeplaylist( stPlayList **filelist, const char *path )
{
    stPlayList *list = *filelist;

    stPlayList *prv = NULL;

    while(list != NULL)
    {
        if(!strcmp( list->path, path))
        {
            if(prv)
             prv->next = list->next;
            else
             *filelist = list->next;

            free( list);

            return true;
        }
        prv = list;

        list = list->next;

    }

    return false;

}


 bool saveplaylist( stPlayList *list )
{

    FILE* file = fopen("./skin/playlist.txt", "w");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file %s \n", "./skin/playlist.txt");
        return false;
    }
    else
    {

        while(list != NULL)
        {
           fwrite(list->path, 1, strlen(list->path), file);
           list = list->next;
        }

    }

    fclose(file);
    return true;
}



bool readplaylist( stPlayList **list )
{

   FILE* file = fopen("./skin/playlist.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file %s \n", "./skin/playlist.txt");
        return false;
    }
    else
    {
        char buff[256] = {'\0'};

        while (!feof(file))
        {
            fgets(buff, 256, file);
             addplaylist_last( list , buff);
        }

        fclose(file);
    }

    return true;
}

const char * extract_file_name(const char *pPath)
{
    char tmp[256];
    strcpy( tmp,  pPath);
    char *path = tmp;
    
    int len = strlen(path);
    printf("\nlength of %s : %d",path, len);


    for(int i=len-1; i>0; i--)
    {
        if(path[i]=='\\' || path[i]=='//' || path[i]=='/' )
        {
            path = path+i+1;
            break;
        }
    }
    
    path[strcspn(path, "\n")] = 0;
   // path[strlen(path) -4 ] = 0;
    
    return path;
}
