
#include "playlinklist.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include <stdio.h>





void displaylist( stPlayList *list )
{
    while(list != NULL) 
    {
        printf("\n%s\n", list->path);
        list = list->next;
    }
}


bool addplaylist( stPlayList **filelist, const char *path )
{
   
   stPlayList *list = *filelist;
   
   stPlayList *prv = NULL;
   
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
     l->next = NULL;
    
    if(prv )
      prv->next = l;
    else if(*filelist )
        prv->next = l;
    else
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
             addplaylist( list , buff);
        }
        
        fclose(file);    
    }
       
    return true;
}

