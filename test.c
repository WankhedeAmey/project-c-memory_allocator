#include <stdio.h>
#include <stdlib.h>

//function prototypes for custom memalloc functions
void *malloc(size_t size);
void free(void  *block);
void print_mem_list();

int main() {
    int *arr_1 = (int*) malloc(1 * sizeof(int));

    if (arr_1) {
        arr_1[0] = 10;
        printf("arr_1[0] : %d\n", arr_1[0]);
        print_mem_list();
    }
    else {
        printf("memory allocation failed\n");
    }

    int *arr_2 = (int *) malloc(5 * sizeof(int));
    if (arr_2) {
        for (int i = 0; i < 5; i ++) {
            arr_2[i] = i * 3;
            printf("arr[%d] = %d\n", i, arr_2[i]);
        }

        print_mem_list();
    }
    else {
        printf("memory allocation failed\n");
    }

    free(arr_1);
    free(arr_2);

    print_mem_list();
}
