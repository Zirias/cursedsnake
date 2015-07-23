#include "centity.h"

#include <string.h>

#define LIST_SIZE 32
#define DICT_BITS 6

typedef enum
{
    CE_STRING,
    CE_LIST,
    CE_DICT
} CeType;

struct centity
{
    CeType type;
};

typedef struct
{
    CeType type;
    char content[1];
} CeString;

typedef struct
{
    CeType type;
    Centity *memb[1];
} CeList;

typedef struct
{
    Centity *next;
    Centity *ent;
    size_t keylen;
    char key[1];
} CeDictEntry;

typedef struct
{
    CeType type;
    unsigned int hashmask;
    CeDictEntry *memb[1];
} CeDict;

Centity *
centity_createString(const char *string)
{
    size_t len = strlen(string);
    CeString *self = malloc(sizeof(CeString) + len - 1U);
    self->type = CE_STRING;
    memcpy(&(self->content), string, len + 1U);
    return (Centity *)self;
}

Centity *
centity_createSizedList(size_t initialSize)
{
    CeList *self = malloc(sizeof(CeList) +
	    (initialSize - 1U) * sizeof(Centity *));
    self->type = CE_LIST;
    memset(&(self->memb), 0, initialSize * sizeof(Centity *));
    return (Centity *)self;
}

Centity *
centity_createList(void)
{
    return centity_createSizedList(LIST_SIZE);
}

Centity *
centity_createBitsDict(unsigned int hashbits)
{
    unsigned int hashmask = (1U << hashbits) - 1U;
    CeDict *self = malloc(sizeof(CeList) + hashmask * sizeof(Centity *));
    self->type = CE_DICT;
    self->hashmask = hashmask;
    memset(&(self->memb), 0, (hashmask + 1U) * sizeof(CeDictEntry *));
    return (Centity *)self;
}

Centity *
centity_createDict(void)
{
    return centity_createBitsDict(DICT_BITS);
}

