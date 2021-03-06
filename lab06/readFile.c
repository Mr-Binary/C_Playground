#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 10000

int search(int *arr, int target, int index, int size){
    if ( arr[index] == target ){
        return 1;
    }
    if ( index == size-1 ){
        return 0;
    }
    return search(arr, target, ++index, size);
}

int readFileToArray(int *arr, char *url){
    FILE *infile;
    int itemNums=0;

    if ( NULL == ( infile=fopen(url,"r") ) ){
        fprintf(stderr, "no file %s found!\n", url);
        return EXIT_FAILURE;
    }

    while( itemNums < ARRAY_SIZE &&
           1==fscanf(infile,"%d",&arr[itemNums])){
        printf("%d\n",arr[itemNums]);
        itemNums++;
    }
    fclose(infile);
    return 0;
}

int main(int argc, char **argv) {
    int sourceArray[ARRAY_SIZE];
    int queryArray[ARRAY_SIZE];
    int result;

    printf("start reading data:\n");
    if ( EXIT_FAILURE == readFileToArray(sourceArray, argv[1]) ){
        printf("%d\n",argc);
        return EXIT_FAILURE;
    }
    
    printf("start reading query:\n");
    if ( EXIT_FAILURE == readFileToArray(queryArray, argv[2]) ){
        printf("%d\n",argc);
        return EXIT_FAILURE;
    }

    printf("start searching:\n");
    for (int i=0; i<7; i++){
        result = search(sourceArray, queryArray[i], i, 9);
        if (result==1){
            printf("+\n");
        } else {
            printf("-\n");
        }
    }
    return 0;
}
