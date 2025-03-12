#ifndef TABLE_H
# define TABLE_H

# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <string.h>
# include <stdlib.h>

typedef struct {
    int PARTKEY;
    char NAME[25];
    char MFGR[25];
    char BRAND[10];
    char TYPE[25];
    int SIZE;
    char CONTAINER[10];
    float RETAILPRICE;
    char COMMENT[200];
} PART;

typedef struct {
    int PARTKEY;
    int SUPPKEY;
    int AVAILQTY;
    float SUPPLYCOST;
    char COMMENT[200];
} PARTSUPP;

// 데이터 파싱
void parsePartRecord(char *line, PART *part);
void parsePartSuppRecord(char *line, PARTSUPP *partsupp);

#endif