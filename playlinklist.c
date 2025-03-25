
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

    free(curr->file);
    free(curr->path);
    // Delete the current node
    free( curr);
}



void displaylist( stPlayList *list )
{
     printf("\n displaylist \n");
     
    while(list != NULL)
    {
        printf("\n  Node %s \n", list->path );
        list = list->next;
    }
}


void displayPage( stPlayList *list)
{
    printf("\n displayPage \n");
    
    int nCount=0;
    while(list != NULL)
    {
        printf("\n Node %s \n", list->path);
        list = list->next;
        
        if( PageListLen == ++nCount)
        {
            return;
        }
    }
}

bool getPage( stPlayList *playlist , bool next,  stPlayList ** pagehead)
{
    stPlayList *list = *pagehead;
     stPlayList *head = *pagehead;
    if( !list)
    {
        list = playlist;
        head = list;
    }
    else if(next && list)      
    {
        int Count = 0;
        while(list  != NULL)
        {
            if( PageListLen == Count++)
            {
               head = list; 
               break;
            }
            list = list->next;           
        }
        if(!list)
        list = head;
    }
    stPlayList *prev = NULL;
    int nCount=0;
    while(list != NULL)
    {
        //printf("\n%s, Page Node %x \n", list->path,  list->prv);      
        if( PageListLen == nCount++)
        {
            if(next)
            {
                *pagehead = head;
            }
            else 
            *pagehead = list;
           
            return true;
        }
        prev = list; 
        if( next)
        {
            list = list->next;
        }
        else
            list = list->prv;  
    }
    if( next)
    {
        prev = head;
    }
    if( prev)
    {
        for(int i=0; i<  PageListLen - nCount ; ++i    )
        {

            if(prev->prv)
            {
                prev = prev->prv;
            }
            else break;
        }
    
        *pagehead = prev;
    }
    printf("\n End of Page \n" );
    
    return false;
}

bool addplaylist_last( stPlayList **filelist, const char *path )
{

   stPlayList *list = *filelist;

   stPlayList *prv = NULL;

  // stPlayList *prvtoPrv = NULL;

   int count = 0;

    while(list != NULL)
    {
#if UNIQUE
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
#endif 
        prv = list;

        list = list->next;
        ++count;
    }

    if(count == MaxPlayListLen )
    {
        stPlayList *tmp  = (*filelist)->next;
        free( (*filelist)->file);
        free( (*filelist)->path);
        free( *filelist);
        tmp->prv = NULL;
        *filelist = tmp;
    }


    stPlayList *l = (stPlayList *)malloc(sizeof(stPlayList));
    l->path =(char*) malloc(strlen(path)+1);
    strcpy( l->path, path);
    const char *tmp = extract_file_name(path);
    l->file =(char*) malloc(strlen(tmp)+1);
    strcpy( l->file, tmp);
    l->next = NULL;
    l->prv= prv;

    if(prv )
      prv->next = l;
    else if(*filelist )
        (*filelist)->next = l;
    else
    *filelist = l;

    return true;


}


bool addplaylist_front( stPlayList **filelist, const char *path  )
{

    stPlayList *list = *filelist;

   #if UNIQUE
    stPlayList *prv = NULL;
    #endif 
    stPlayList *prvtoPrv = NULL;
    
    int count = 0;

    while(list != NULL)
    {
        
    #if UNIQUE
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
    #endif
       // prv = list;
        
        if(list->next && list->next->next == NULL)
        {
            prvtoPrv = list;
            
        }

        list = list->next;
        ++count;
    }

    if(count == MaxPlayListLen )
    {
       
        if(prvtoPrv )
        {
            free(prvtoPrv->next->path);
            free(prvtoPrv->next->file);
            free(prvtoPrv->next);
            prvtoPrv->next = NULL;
            
        }
 
    }


    stPlayList *l = (stPlayList *)malloc(sizeof(stPlayList));
    l->path =(char*) malloc(strlen(path)+1);
    strcpy( l->path, path);
    const char *tmp = extract_file_name(path);
    l->file =(char*) malloc(strlen(tmp)+1);
    strcpy( l->file, tmp);
    l->prv =NULL;

    (*filelist)->prv = l;
    l->next = *filelist;
    
    *filelist = l;

    return true;
}

bool removeplaylist( stPlayList **filelist, const char *path , stPlayList ** pagehead )
{
    stPlayList *list = *filelist;


    while(list != NULL)
    {   
        if(!strcmp( list->path, path))
        {
           return removeplayNode(filelist, list, pagehead);
        }
        //prv = list;

        list = list->next;
    }
    return false;

}
 
bool removeplayNode( stPlayList **filelist, stPlayList *node,  stPlayList **pagehead )
{
     stPlayList *tmp = *pagehead;
    
    if(node != NULL)
    {   
        if( node == tmp)
        {
            if(tmp->next)
            {
                *pagehead = tmp->next;
            }
            else
                *pagehead = tmp->prv;
        }
        stPlayList *prv = node->prv;

        if(prv)
        {
            prv->next = node->next;
            if(node->next)
            {
              node->next->prv =  prv;
            }
        }
        else
        {
           *filelist = node->next;
           (*filelist)->prv = NULL;
        }
        
        free( node->file);
        free( node->path);
        free( node);

        return true;

    }
    return false;

}
 
/*
 *  plase do not swap with pointers otherwsie gui will crash and we need to add syncronization locks
 */
bool swapplaylist( stPlayList **filelist, const char *path, bool next ,  stPlayList **selnode )
{
    stPlayList *list = *filelist;

   // stPlayList *prv = NULL;
    int nCount = 0;
    while(list != NULL)
    {
        if(!strcmp( list->path, path))
        {
            return swapplayNode(list, next, selnode );
        }
        
        ++nCount;
       // prv = list;
        list = list->next;
    }
    return false;
}


bool swapplayNode(  stPlayList *node, bool next ,  stPlayList **selnode )
{

    stPlayList *prv = node->prv;
    
    char *tmppath = node->path;
    char *tmpfile =  node->file; 
    if( next )
    {
        if( node->next)
        {
            if( *selnode  == node )
            {
                *selnode = node->next;
            }
            else if( *selnode  == node->next )
            {
                *selnode =  node->prv; 
            }   
            node->path = node->next->path;
            node->file  =  node->next->file;  
            node->next->path =  tmppath;
            node->next->file = tmpfile;

            return true;
        }
    }
    else if ( prv  )
    {
        if( *selnode  == node )
        {
            *selnode =  node->prv; 
        }
        else if( *selnode == node->prv )
        {
            *selnode  = node->next; 
        }

        node->path = prv->path;
        node->file =  prv->file;  
        prv->path = tmppath;
        prv->file = tmpfile;
        return true;       
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
           fputc((int)'\n', file);
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
            if(fgets(buff, 256, file))
            {
                buff[strcspn(buff, "\n")] = '\0';
                addplaylist_last( list , buff);
            }
        }

        fclose(file);
    }
    /*
    printf("begin \n");
    stPlayList *ls = *list;
    displaylist(ls);
    printf("end \n");
   */
    return true;
}

const char * extract_file_name(const char *pPath)
{
    char tmp[256];
    strcpy( tmp,  pPath);
    char *path = tmp;
    
    int len = strlen(path);
    //printf("\nlength of %s : %d",path, len);


    for(int i=len-1; i>0; i--)
    {
        if(path[i]=='\\' || path[i]=='/' )
        {
            path = path+i+1;
            break;
        }
    }
    
    path[strcspn(path, "\n")] =  '\0';
    len = strlen(path);
    if(len > 4)
    path[strlen(path) -4 ] = '\0';
    
    return path;
}
