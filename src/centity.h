#ifndef CSNAKE_CENTITY_H
#define CSNAKE_CENTITY_H

#include "common.h"

typedef enum
{
    CE_STRING,
    CE_LIST,
    CE_DICT
} CeType;

struct centity;
typedef struct centity Centity;

Centity *centity_createString(const char *string);
Centity *centity_createSizedList(size_t initialSize);
Centity *centity_createList(void);
Centity *centity_ceateBitsDict(unsigned int hashbits);
Centity *centity_createDict(void);
void centity_destroy(Centity *self);

#endif
