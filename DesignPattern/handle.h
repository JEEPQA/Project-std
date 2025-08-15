#pragma once
typedef struct _MyStruct* MyHandle;

MyHandle CreateHandle();

void DestroyHandle(MyHandle handle);

int GetData(MyHandle handle);

int GetData2(MyHandle handle);

void SetData(MyHandle handle, int data);

void SetData2(MyHandle handle, int data);


