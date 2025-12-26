#pragma once
#include <UefiBaseType.h>

#define MAX_KEY_ALLOWED                 32


typedef struct {
    VOID* Buffer[MAX_KEY_ALLOWED + 1];
    UINTN         Head;
    UINTN         Tail;
    UINTN         ItemSize;
} I2C_SIMPLE_QUEUE;