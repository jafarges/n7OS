#ifndef FILE_H
#define FILE_H

typedef struct File_Element {
    int value;
    struct File_Element* next;
} File_Element;

typedef struct File {
    File_Element* first;
    File_Element* last;
} File;


typedef void(*executor)(int);

void create(File* file);
void enqueue(File* file,int value);
int dequeue(File* file);
void del(File* file, int value);

void foreach(File file, executor mFunc);
int length(File file);


#endif