/* Quick Sort does a smaller amount of operations than Heap sort in the average case
 *
 * Depending on the chosen pivot, the algorithm can perform in the best case(O(nlogn) ), or in the worst case( O(n^n) )
 *
 * Best case: the algorithm performs best when the pivot is chosen as the median element from the array, such that it is always divided in two equally sized partitions
 * Also, to reach the O(nlogn) time, the array must be sorted
 *
 * Worst case: the algorithm performs worst when each call partitions the array into an array sized n-1 and one that is empty, so that at each call performs operation on
 * an array of size smaller than the previous one with 1. We achieve this by choosing the last element as a pivot and if the array is ascending, or if we take
 * as a pivot the first element and the aray is descending.
 *
 */


#include <iostream>
#include "Profiler_linux.h"

using namespace std;

Profiler profiler ("HW3");

int heap_size;

int dimension;

void copy(int *a, int *b, int n) {
    for (int k = 0; k < n; k++) {
        b[k] = a[k];
    }
}


void max_heapify (int *a, int i, int n) {
    int largest;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    profiler.countOperation("Heapsort", n, 1);
    if (l < heap_size && a[l] > a[i])
        largest = l;
    else
        largest = i;

    profiler.countOperation("Heapsort", n, 1);
    if (r < heap_size && a[r] > a[largest])
        largest = r;

    if (largest != i) {
        int aux = a[i];
        a[i] = a[largest];
        a[largest] = aux;
        profiler.countOperation("Heapsort", n, 3);
        max_heapify(a, largest, n);
    }
}

void build_heap_bottomUp (int *a, int n) { //builds heap starting from the leaves
    heap_size = n;
    profiler.countOperation("Heapsort", n, 1);
    for (int i = n / 2 - 1; i >= 0; i--)
        max_heapify(a, i, n);
}

void heapSort (int *a, int n) {
    build_heap_bottomUp(a,n);
    for (int i = n - 1; i >= 0; i--) {
        int swap = a[0];
        a[0] = a[i];
        a[i] = swap;
        heap_size--;
        profiler.countOperation("Heapsort", n, 3);
        max_heapify(a, 0, n);
    }
}

int Partition(int *a, int p, int r) {
    int x = a[r];
    profiler.countOperation("QuickSort", dimension, 1);
    int i = p - 1;
    for (int j = p; j < r; j++) {
        profiler.countOperation("QuickSort", dimension, 1);
        if (a[j] <= x) {
            i++;
            profiler.countOperation("QuickSort", dimension, 3);
            int swap = a[i];
            a[i] = a[j];
            a[j] = swap;
        }
    }
    int swap = a[i + 1];
    a[i + 1] = a[r];
    a[r] = swap;
    profiler.countOperation("QuickSort", dimension, 3);
    return i + 1;
}

void QuickSort(int *a, int p, int r) {
    if (p < r) {
        int q = Partition(a, p, r);
        profiler.countOperation("QuickSort", dimension, 1);
        QuickSort(a, p, q - 1);
        QuickSort(a, q + 1, r);
    }
}

int PartitionBestCase(int *a, int p, int r) { // when we split by the median element of the array
    int x = a[(r + p) / 2];
    profiler.countOperation("QuickSortBest", dimension, 1);
    int swap = a[(r + p) / 2];
    a[(r + p) / 2] = a[r];
    a[r] = swap;
    int i = p - 1;
    for (int j = p; j < r; j++) {
        profiler.countOperation("QuickSortBest", dimension, 1);
        if (a[j] <= x) {
            i++;
            profiler.countOperation("QuickSortBest", dimension, 3);
            int swap = a[i];
            a[i] = a[j];
            a[j] = swap;
        }
    }
    swap = a[i + 1];
    a[i + 1] = a[r];
    a[r] = swap;
    profiler.countOperation("QuickSortBest", dimension, 3);
    return i + 1;
}

void QuickSortBestCase(int *a, int p, int r) {
    if (p < r) {
        int q = PartitionBestCase(a, p, r);
        profiler.countOperation("QuickSortBest", dimension, 1);
        QuickSortBestCase(a, p, q - 1);
        QuickSortBestCase(a, q + 1, r);
    }
}

int PartitionWorstCase(int *a, int p, int r) { //when we split each time into an array of one element and an array of n-1 elements
    int x = a[r];
    profiler.countOperation("QuickSortWorst", dimension, 1);
    int i = p - 1;
    for (int j = p; j < r; j++) {
        profiler.countOperation("QuickSortWorst", dimension, 1);
        if (a[j] <= x) {
            i++;
            profiler.countOperation("QuickSortWorst", dimension, 3);
            int swap = a[i];
            a[i] = a[j];
            a[j] = swap;
        }
    }
    int swap = a[i + 1];
    a[i + 1] = a[r];
    a[r] = swap;
    profiler.countOperation("QuickSortWorst", dimension, 3);
    return i + 1;
}

void QuickSortWorstCase(int *a, int p, int r) {
    if (p < r) {
        int q = PartitionWorstCase(a, p, r);
        profiler.countOperation("QuickSortWorst", dimension, 1);
        QuickSortWorstCase(a, p, q - 1);
        QuickSortWorstCase(a, q + 1, r);
    }
}

int RandomizedSelect(int *a, int p, int r, int i) {
    if (p == r) {
        return a[p];
    }
    int q = Partition(a, p, r);
    int length = q - p + 1;
    if (length == i) {
        return a[q];
    } else if (i < length) {
        return RandomizedSelect(a, p, q - 1, i);
    }
    return RandomizedSelect(a, q + 1, r, i - length);
}

int main() {

    int randomArray[10000], copyOfRandomArray[10000], quickWorst[10000], quickBest[10000];

    //Corectness of heapsort
    int a[] = {4, 1, 3, 2, 16, 9, 10, 14, 8, 7};
    heapSort(a, 10);
    for (int i = 0; i < 10; i++) {
        cout<<a[i]<<" ";
    }

    cout<<"\n";

    //Corectness of QuickSort
    int b[] = {2, 8, 7, 1, 3, 5, 6, 4};
    QuickSort(b, 0, 7);
    for (int i = 0; i < 8; i++) {
        cout<<b[i]<<" ";
    }
    cout<<"\n";

    //Corectness of Randomized select
    int c[] = {3, 4, 5, 2, 6, 7, 15, 23, 79, 123, 445, 21};
    int rezRandomizedSelect[30];
    for (int i = 0; i < 12; i++) {
        rezRandomizedSelect[i] = RandomizedSelect(c, 0, 11, i + 1);
    }
    for (int i = 0; i < 12; i++) {
        cout<<rezRandomizedSelect[i]<<" ";
    }
    cout<<"\n";
//
//    for (int m = 0; m < 5; m++) {
//
//        FillRandomArray(randomArray, 10000);
//
//        for (dimension = 100; dimension <= 10000; dimension = dimension + 500) {
//
//            copy(randomArray, copyOfRandomArray, dimension);
//            heapSort(copyOfRandomArray, dimension);
//
//            copy(randomArray, copyOfRandomArray, dimension);
//            QuickSort(copyOfRandomArray, 0, dimension - 1);
//        }
//    }
//
//    profiler.createGroup("Heapsort vs QuickSort", "Heapsort", "QuickSort");
//
//    FillRandomArray(quickBest, 10000);
//
//    for (dimension = 100; dimension <= 10000; dimension = dimension + 500) {
//
//        for (int i = 0; i < dimension; i++) {
//            quickWorst[i] = i; //array in ascending order
//        }
//        QuickSortWorstCase(quickWorst, 0, dimension - 1);
//
//        QuickSortBestCase(quickBest, 0, dimension - 1);
//
//    }
//
//    profiler.createGroup("QuickSort: Worst, Best", "QuickSortWorst", "QuickSortBest");
//
//    profiler.showReport();

    return 0;
}