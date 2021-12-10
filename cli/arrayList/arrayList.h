#include "../parameter.h"

typedef struct arrayList {
    Parameter* array;
    int size;
    int alloced;
} ArrayList;