#include <stdio.h>
#include <stdlib.h>
#include "flexarray.h"

struct flexarrayrec {
    int capacity;
    int itemcount;
    int *items;
};

void *emalloc(size_t s) {
    void *result = malloc(s);
    if (NULL == result) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

void *erealloc(void *p, size_t s) {
    void *result = realloc(p, s);
    if (NULL == result) {
        fprintf(stderr, "Memory re-allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

/*
 * Start from the 2nd item,
 * everytime you do a sort,
 * bump the started item first,
 * then compare all the previous items one by one,
 * from left to right, if the checked item is bigger
 * than the bumped item, copy its value to the bumped value,
 * then move one step back to do the checking again,
 * until you are outOfIndex, then insert the bumped value
 * to the (stopped position+1) to avoid the outOfIndex.
 */
void insertion_sort(int *arr, int size) {
    int i, previousIndex, insertValue, j;

    for (i = 1; i < size; i++) {
        insertValue = arr[i];
        previousIndex = i - 1;

        while (previousIndex >= 0 && arr[previousIndex] > insertValue) {
            arr[previousIndex + 1] = arr[previousIndex];
            previousIndex--;
        }
        arr[previousIndex + 1] = insertValue;

        if (i == ((size - 1) / 2)) {
            for (j = 0; j < size; j++) {
                fprintf(stderr, "%d\n", arr[j]);
            }
        }
    }
}

flexarray flexarray_new() {
    flexarray result = emalloc(sizeof *result);
    result->capacity = 2;
    result->itemcount = 0;
    result->items = emalloc(result->capacity * sizeof result->items[0]);
    return result;
}

void flexarray_append(flexarray f, int num) {
    if (f->itemcount == f->capacity) {
        f->capacity += f->capacity;
        f->items = erealloc(f->items, f->capacity * sizeof f->items[0]);
    }
    f->items[f->itemcount++] = num;
}

void flexarray_print(flexarray f) {
    int i;
    for (i = 0; i < f->itemcount; i++) {
        printf("%d\n", f->items[i]);
    }
}

void flexarray_sort(flexarray f) { insertion_sort(f->items, f->itemcount); }

void flexarray_free(flexarray f) {
    free(f->items);
    free(f);
}
