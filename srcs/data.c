#include "data.h"

// 작업 큐 초기화
TaskQueue *createTaskQueue(int capacity) {
    TaskQueue *queue = (TaskQueue *)malloc(sizeof(TaskQueue));
    queue->tasks = (Task *)malloc(sizeof(Task) * capacity);
    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
    queue->capacity = capacity;
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond, NULL);
    return queue;
}

// 작업 큐에 작업 추가
void enqueueTask(TaskQueue *queue, Task task) {
    pthread_mutex_lock(&queue->mutex);
    while (queue->size == queue->capacity)
        pthread_cond_wait(&queue->cond, &queue->mutex);
    queue->tasks[queue->rear] = task;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->size++;
    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

// 작업 큐에서 작업 제거
Task dequeueTask(TaskQueue *queue) {
    pthread_mutex_lock(&queue->mutex);
    while (queue->size == 0)
        pthread_cond_wait(&queue->cond, &queue->mutex);
    Task task = queue->tasks[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
    return task;
}

void freeTaskQueue(TaskQueue *queue) {
    if (queue == NULL)
        return;

    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->cond);

    if (queue->tasks != NULL)
        free(queue->tasks);
    free(queue);
}
