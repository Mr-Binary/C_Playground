#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_MAX 2000000
#define DROP_IN_SIZE 50
int my_array[ARRAY_MAX];
int w[ARRAY_MAX];

void insertion_sort(int *arr, int size) {
    int start,previousIndex,insertValue;

    for (start = 1; start < size; start++) {
        insertValue = arr[start];
        previousIndex = start - 1;

        while (previousIndex >= 0 && arr[previousIndex] > insertValue) {
            arr[previousIndex + 1] = arr[previousIndex];
            previousIndex = previousIndex - 1;
        }
        arr[previousIndex + 1] = insertValue;
    }
}

void merge(int *arr, int *w, int length) {
    int middle = length/2;
    int i = 0, j = middle, k = 0;
    while (i < middle && j < length)
        {
            if (arr[i] < arr[j]){
                w[k] = arr[i];
                k+=1;
                i+=1;
            } else {
                w[k] = arr[j];
                k+=1;
                j+=1;
            }
        }

    while (i < middle) {
        w[k] = arr[i];
        k+=1;
        i+=1;
    } 

    while (j < length) {
        w[k] = arr[j];
        k+=1;
        j+=1;
    }  
}

/*
 *
 */
void mix_merge_sort(int *arr, int *w, int length){
    int i = 0, middle = (length/2);

    if (sizeof(arr)<2){
        return;
    }

    if (length<DROP_IN_SIZE){
        insertion_sort(arr, length);
    } else {
        mix_merge_sort(arr, w, middle);
        mix_merge_sort(arr + middle, w + middle, length-middle);
        merge(arr, w, length);
   
        for (i = 0; i < length; i++){
            arr[i] = w[i];
        }
    }
    
}



int main(void) {

    
    clock_t start, end;
    int i, count = 0;
    
    while (count < ARRAY_MAX && 1 == scanf("%d", &my_array[count])) {
        count++;
    }

    start = clock();
    mix_merge_sort(my_array, w, count);
    end = clock();

    for (i = 0; i < count; i++) {
        printf("%d\n", my_array[i]);
    }

    fprintf(stderr, "%d %f\n", count, (end - start) / (double)CLOCKS_PER_SEC);
    return EXIT_SUCCESS;
}
