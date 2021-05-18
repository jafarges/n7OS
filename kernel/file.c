#include <file.h>
#include <malloc.h>

void create(File* file){
    file->first =  0;
    file->last =  0;
}

int length_rec(File_Element* el){
    if(el != 0)
        return 1+length_rec(el->next);
    else
        return 0;
}

int length(File file){
    return length_rec(file.first);
}

void enqueue(File* file,int value){
    File_Element* lastOne = (File_Element*) malloc(sizeof(File_Element));
    lastOne->value = value;
    lastOne->next = 0;

    if(file->last != 0)
        file->last->next = lastOne;
    else
        file->first = lastOne;
    
    file->last = lastOne;
    
}

int dequeue(File* file){
    if(file->first != 0){
        int ret = file->first->value;
        File_Element* next = file->first->next;
        
        if(next==0)
            file->last = 0;
        
        free(file->first);
        file->first = next;
        return ret;
    }else{
        return -1;
    }
}

void foreach_rec(File_Element* el,executor mFunc){
    if(el != 0)
    {     
        mFunc(el->value);
        foreach_rec(el->next,mFunc);
    }
}

void foreach(File file, executor mFunc){
    foreach_rec(file.first,mFunc);
}

void del(File* file, int value){
    File_Element* crrEl = file->first;
    if(crrEl != 0){
        if(crrEl->value == value){
            // Head must be deleted
            File_Element* next = crrEl->next;
            free(crrEl);
            file->first=next;
            if(next==0){
                // If head is last element, update last element too
                file->last = 0;
            }
        }else{
            while((crrEl->next != 0) && (crrEl->next->value !=value)){ 
                crrEl = crrEl->next;
            }

            if(crrEl->next !=0 && crrEl->next->value==value){
                File_Element* bridgeEl = crrEl->next->next;
                free(crrEl->next);
                crrEl->next = bridgeEl;
                if(bridgeEl==0){
                    // Last element has been deleted
                    file->last = crrEl;
                }
            }
        }
    }
}
