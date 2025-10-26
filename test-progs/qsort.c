#include "print.h"

void swap(int *a, int *b) {
    int tmp = *a;
    *a      = *b;
    *b      = tmp;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i     = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quicksort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high); // partition index
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

int main() {
    int arr[] = { 10, 7, 8, 9, 1, 5 };
    int n     = sizeof(arr) / sizeof(arr[0]);

    my_printf("Original array: ");
    for (int i = 0; i < n; i++) {
        my_printf("%d ", arr[i]);
    }
    my_printf("\n");

    quicksort(arr, 0, n - 1);

    my_printf("Sorted array:   ");
    for (int i = 0; i < n; i++) {
        my_printf("%d ", arr[i]);
    }
    my_printf("\n");

    return 0;
}
