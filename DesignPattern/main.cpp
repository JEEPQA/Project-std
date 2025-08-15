#include "MyPimpl.h"
#include "handle.h"
#include <stdio.h>

int main()
{
    MyInterface instance;

    instance.publicApi1();
    instance.publicApi2(2);


    MyHandle handle = CreateHandle();
    SetData(handle,2);

    int data = GetData(handle);
    printf("data:%d", data);

    DestroyHandle(handle);
    return 0;
}