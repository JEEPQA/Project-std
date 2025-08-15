#include <corecrt_malloc.h>

#include "handle.h"

struct _MyStruct {

    int data;
    int data2;
    int data3;
    int data4;
};
typedef struct _MyStruct* MyHandle;


//struct HWND__ {
//    int unused;
//}; 
//typedef struct HWND__* HWND;


MyHandle CreateHandle() 
{


    MyHandle handle = (MyHandle)malloc(sizeof(struct _MyStruct));
    handle->data = 0;
    handle->data2 = 0;
    handle->data3 = 0;
    handle->data4 = 0;

    return handle;
}

void DestroyHandle(MyHandle handle) 
{
    free(handle);
}

int GetData(MyHandle handle) 
{
    return handle->data;
}
int GetData2(MyHandle handle) 
{
    return handle->data2;
}
void SetData(MyHandle handle, int data) 
{
    handle->data = data;
}
void SetData2(MyHandle handle, int data) 
{
    handle->data2 = data;
}
