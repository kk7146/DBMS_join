#include "data.h"

#define THREAD_NUM 10
#ifndef PART_BLOCK_SIZE
# define PART_BLOCK_SIZE 50000
#endif
#ifndef PARTSUPP_BLOCK_SIZE
# define PARTSUPP_BLOCK_SIZE 100
#endif

pthread_mutex_t fileMutex = PTHREAD_MUTEX_INITIALIZER;

// 작업 처리 워커 스레드
void *workerThread(void *arg) {
    while (1) {
        Task task = dequeueTask(taskQueue);
        printf("%d ", taskQueue->size);

        if (task.isEndTask)
            return NULL;

        char buffer[1024];
        int bufferSize = 0;

        for (int i = 0; i < task.partCount; i++)
            if (task.partBlock[i].PARTKEY == task.partsuppRecord.PARTKEY) {
                bufferSize = snprintf(buffer, sizeof(buffer), "%d|%s|%s|%d|%d|%f|%s\n",
                                      task.partBlock[i].PARTKEY, task.partBlock[i].NAME,
                                      task.partBlock[i].MFGR, task.partsuppRecord.SUPPKEY,
                                      task.partsuppRecord.AVAILQTY, task.partsuppRecord.SUPPLYCOST,
                                      task.partsuppRecord.COMMENT);

                pthread_mutex_lock(&fileMutex);
                fwrite(buffer, 1, bufferSize, task.outputFile);
                pthread_mutex_unlock(&fileMutex);
            }
    }
}

void joinAndOutput(PART *partBlock, int partCount, FILE *partsuppFile, FILE *outputFile) {
    char *psLine = NULL;
    size_t psLen = 0;
    ssize_t read;
    PARTSUPP partsuppRecord;

    rewind(partsuppFile);

    while ((read = getline(&psLine, &psLen, partsuppFile)) != -1) {
        parsePartSuppRecord(psLine, &partsuppRecord);
        Task task = { .partBlock = partBlock, .partCount = partCount,
                      .partsuppRecord = partsuppRecord, .outputFile = outputFile, .isEndTask = 0 };
        enqueueTask(taskQueue, task);
        //printf("%d ", taskQueue->size);
    }
    free(psLine);
}

void add_task(FILE *partFile, FILE *partsuppFile, FILE *outputFile) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int use_part = 1;

    PART partBlock1[PART_BLOCK_SIZE];
    PART partBlock2[PART_BLOCK_SIZE];
    int partIndex = 0;

    while ((read = getline(&line, &len, partFile)) != -1) {
        if (use_part)
            parsePartRecord(line, &partBlock1[partIndex]);
        else
            parsePartRecord(line, &partBlock2[partIndex]);
        partIndex++;
        if (partIndex == PART_BLOCK_SIZE) {
            if (use_part)
                joinAndOutput(partBlock1, partIndex, partsuppFile, outputFile);
            else
                joinAndOutput(partBlock2, partIndex, partsuppFile, outputFile);
            partIndex = 0;
            use_part = !use_part;
            printf("\n");
        }
    }

    if (partIndex > 0) {
        if (use_part)
                joinAndOutput(partBlock1, partIndex, partsuppFile, outputFile);
            else
                joinAndOutput(partBlock2, partIndex, partsuppFile, outputFile);
    }
    free(line);
}

void check_leak(void)
{
	system("leaks a.out");
}

int main() {
    FILE *partFile = fopen("./tbl/part.tbl", "r");
    FILE *partsuppFile = fopen("./tbl/partsupp.tbl", "r");
    FILE *outputFile = fopen("./joinResult.tbl", "w");

    if (!partFile || !partsuppFile || !outputFile) {
        perror("Error opening files");
        exit(EXIT_FAILURE);
    }

    taskQueue = createTaskQueue(PARTSUPP_BLOCK_SIZE);

    pthread_t threads[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; i++) {
        pthread_create(&threads[i], NULL, workerThread, NULL);
    }
    add_task(partFile, partsuppFile, outputFile);

    for (int i = 0; i < THREAD_NUM; i++) {
        Task endTask = { .isEndTask = 1 };
        enqueueTask(taskQueue, endTask);
    }

    for (int i = 0; i < THREAD_NUM; i++) {
        pthread_join(threads[i], NULL);
    }

    fclose(partFile);
    fclose(partsuppFile);
    fclose(outputFile);

    freeTaskQueue(taskQueue);
    atexit(check_leak);
    return 0;
}
