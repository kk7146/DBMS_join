#include "table.h"

// 데이터 파싱
void parsePartRecord(char *line, PART *part) {
    char *token = strtok(line, "|");
    part->PARTKEY = atoi(token);

    token = strtok(NULL, "|");
    snprintf(part->NAME, sizeof(part->NAME), "%s", token);

    token = strtok(NULL, "|");
    snprintf(part->MFGR, sizeof(part->MFGR), "%s", token);

    token = strtok(NULL, "|");
    snprintf(part->BRAND, sizeof(part->BRAND), "%s", token);

    token = strtok(NULL, "|");
    snprintf(part->TYPE, sizeof(part->TYPE), "%s", token);

    token = strtok(NULL, "|");
    part->SIZE = atoi(token);

    token = strtok(NULL, "|");
    snprintf(part->CONTAINER, sizeof(part->CONTAINER), "%s", token);

    token = strtok(NULL, "|");
    part->RETAILPRICE = atof(token);

    token = strtok(NULL, "|");
    snprintf(part->COMMENT, sizeof(part->COMMENT), "%s", token);
}

void parsePartSuppRecord(char *line, PARTSUPP *partsupp) {
    char *token = strtok(line, "|");
    partsupp->PARTKEY = atoi(token);

    token = strtok(NULL, "|");
    partsupp->SUPPKEY = atoi(token);

    token = strtok(NULL, "|");
    partsupp->AVAILQTY = atoi(token);

    token = strtok(NULL, "|");
    partsupp->SUPPLYCOST = atof(token);

    token = strtok(NULL, "|");
    snprintf(partsupp->COMMENT, sizeof(partsupp->COMMENT), "%s", token);
}