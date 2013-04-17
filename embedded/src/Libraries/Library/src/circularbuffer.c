#include "circularbuffer.h"

int8 Cb_initialize(CircularBuffer *buffer, uint16 bufferSize, uint16 dataSize, void *data)
{
    buffer->dataSize = dataSize;
    buffer->bufferSize = bufferSize * dataSize;
    
    if (data == NULL)
    {
        buffer->startPointer = calloc(bufferSize, dataSize);
    }
    else
    {
        buffer->startPointer = data;
    }
    
    if (buffer->startPointer == NULL)    //memory full
        return -1;
    
    buffer->inPointer = buffer->startPointer;// + buffer->dataSize;
    buffer->outPointer = buffer->startPointer;   //empty buffer
    
    return 0;
}

int8 Cb_put(CircularBuffer *buffer, void *item)
{
    if ((buffer->inPointer == (buffer->outPointer - buffer->dataSize)) || 
        ((buffer->inPointer == (buffer->startPointer+buffer->bufferSize)) && (buffer->outPointer == buffer->startPointer)))  //buffer full
    {
        return -1;
    }
    
    memcpy(buffer->inPointer, item, buffer->dataSize);
    
    //increase the pointer
    if (buffer->inPointer == (buffer->startPointer+buffer->bufferSize))
    {
        buffer->inPointer = buffer->startPointer;
    }
    else
    {
        buffer->inPointer += buffer->dataSize;
    }
    
    return 0;
}

int8 Cb_get(CircularBuffer *buffer, void *item)
{
    if (buffer->outPointer == buffer->inPointer)   //buffer empty
        return -1;
    
    memcpy(item, buffer->outPointer, buffer->dataSize);
     
    //increase the pointer
    if (buffer->outPointer == (buffer->startPointer+buffer->bufferSize))
    {
        buffer->outPointer = buffer->startPointer;
    }
    else
    {
        buffer->outPointer += buffer->dataSize;
    }
    
    return 0;
}
