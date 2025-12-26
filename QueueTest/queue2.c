#include "queue2.h"

/**
  Create the queue.

  @param  Queue     Points to the queue.
  @param  ItemSize  Size of the single item.

**/
VOID
InitQueue(
    IN OUT  I2C_SIMPLE_QUEUE* Queue,
    IN      UINTN              ItemSize
)
{
    UINTN                      Index;

    Queue->ItemSize = ItemSize;
    Queue->Head = 0;
    Queue->Tail = 0;

    H2OFreePool((VOID**)&(Queue->Buffer[0]));

    Queue->Buffer[0] = AllocatePool(sizeof(Queue->Buffer) / sizeof(Queue->Buffer[0]) * ItemSize);
    ASSERT(Queue->Buffer[0] != NULL);

    for (Index = 1; Index < sizeof(Queue->Buffer) / sizeof(Queue->Buffer[0]); Index++) {
        Queue->Buffer[Index] = ((UINT8*)Queue->Buffer[Index - 1]) + ItemSize;
    }
}

/**
  Destroy the queue

  @param Queue    Points to the queue.
**/
VOID
DestroyQueue(
    IN OUT I2C_SIMPLE_QUEUE* Queue
)
{
    H2OFreePool((VOID**)&(Queue->Buffer[0]));
}

/**
  Check whether the queue is empty.

  @param  Queue     Points to the queue.

  @retval TRUE      Queue is empty.
  @retval FALSE     Queue is not empty.

**/
BOOLEAN
IsQueueEmpty(
    IN  I2C_SIMPLE_QUEUE* Queue
)
{
    //
    // Meet FIFO empty condition
    //
    return (BOOLEAN)(Queue->Head == Queue->Tail);
}

/**
  Check whether the queue is full.

  @param  Queue     Points to the queue.

  @retval TRUE      Queue is full.
  @retval FALSE     Queue is not full.

**/
BOOLEAN
IsQueueFull(
    IN  I2C_SIMPLE_QUEUE* Queue
)
{
    return (BOOLEAN)(((Queue->Tail + 1) % (MAX_KEY_ALLOWED + 1)) == Queue->Head);
}

/**
  Enqueue the item to the queue.

  @param  Queue     Points to the queue.
  @param  Item      Points to the item to be enqueued.
  @param  ItemSize  Size of the item.
**/
VOID
Enqueue(
    IN OUT  I2C_SIMPLE_QUEUE* Queue,
    IN      VOID* Item,
    IN      UINTN            ItemSize
)
{
    ASSERT(ItemSize == Queue->ItemSize);
    //
    // If keyboard buffer is full, throw the
    // first key out of the keyboard buffer.
    //
    if (IsQueueFull(Queue)) {
        Queue->Head = (Queue->Head + 1) % (MAX_KEY_ALLOWED + 1);
    }

    CopyMem(Queue->Buffer[Queue->Tail], Item, ItemSize);

    //
    // Adjust the tail pointer of the FIFO keyboard buffer.
    //
    Queue->Tail = (Queue->Tail + 1) % (MAX_KEY_ALLOWED + 1);
}

/**
  Dequeue a item from the queue.

  @param  Queue     Points to the queue.
  @param  Item      Receives the item.
  @param  ItemSize  Size of the item.

  @retval EFI_SUCCESS        Item was successfully dequeued.
  @retval EFI_DEVICE_ERROR   The queue is empty.

**/
EFI_STATUS
Dequeue(
    IN OUT  I2C_SIMPLE_QUEUE* Queue,
    OUT  VOID* Item,
    IN      UINTN            ItemSize
)
{
    ASSERT(Queue->ItemSize == ItemSize);

    if (IsQueueEmpty(Queue)) {
        return EFI_DEVICE_ERROR;
    }

    CopyMem(Item, Queue->Buffer[Queue->Head], ItemSize);
    //[-start-251003-IB23770023-add]//
    ZeroMem(Queue->Buffer[Queue->Head], ItemSize);
    //[-end-251003-IB23770023-add]//
      //
      // Adjust the head pointer of the FIFO keyboard buffer.
      //
    Queue->Head = (Queue->Head + 1) % (MAX_KEY_ALLOWED + 1);

    return EFI_SUCCESS;
}

