#include "centity.h"

#include <string.h>

#define LIST_SIZE 32
#define DICT_BITS 6

typedef struct cpertor Cpertor;

Cpertor *cpertor_createRoot(const char *name);
Cpertor *cpertor_createListEntry(Cpertor *parent, size_t index);
Cpertor *cpertor_createDictEntry(
	Cpertor *parent, size_t keylen, const void *key);

struct centity
{
    CeType type;
    Cpertor *ptor;
};

typedef struct
{
    CeType type;
    Cpertor *ptor;
    char content[1];
} CeString;

typedef struct
{
    CeType type;
    Cpertor *ptor;
    size_t entries;
    size_t size;
    Centity *memb[1];
} CeList;

typedef struct ceDictEntry CeDictEntry;
struct ceDictEntry
{
    CeDictEntry *next;
    Centity *ent;
    size_t keylen;
    char key[1];
};

typedef struct
{
    CeType type;
    Cpertor *ptor;
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
    self->size = initialSize;
    self->entries = 0U;
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

static void
ceDictEntry_destroy(CeDictEntry *self)
{
    if (!self) return;

    free(self->key);
    centity_destroy(self->ent);
    ceDictEntry_destroy(self->next);
    free(self);
}

void
centity_destroy(Centity *self)
{
    if (!self) return;

    switch (self->type)
    {
	case CE_STRING:
	    free(self);
	    break;

	case CE_LIST:
	    {
		CeList *list = (CeList *)self;
		size_t i;

		for (i = 0; i < list->entries; ++i)
		{
		    centity_destroy(list->memb[i]);
		}
	    }
	    break;
	
	case CE_DICT:
	    {
		CeDict *dict = (CeDict *)self;
		size_t i;

		for (i = 0; i <= dict->hashmask; ++i)
		{
		    ceDictEntry_destroy(dict->memb[i]);
		}
	    }
	    break;
    }
    free(self);
}

const char *
centity_string(const Centity *self)
{
    if (self->type == CE_STRING)
    {
	const CeString *string = (const CeString *)self;
	return &(string->content[0]);
    }
    return 0;
}

