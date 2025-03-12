#ifndef DATA_H
# define DATA_H

# include "table.h"

typedef struct {
    PART *partBlock;
    int partCount;
    PARTSUPP partsuppRecord;
    FILE *outputFile;
    int isEndTask;
} Task;

// 작업 큐 구조체
typedef struct {
    Task *tasks;
    int front;
    int rear;
    int size;
    int capacity;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} TaskQueue;

TaskQueue *taskQueue;

TaskQueue *createTaskQueue(int capacity);
void enqueueTask(TaskQueue *queue, Task task);
Task dequeueTask(TaskQueue *queue);
void freeTaskQueue(TaskQueue *queue);

#endif