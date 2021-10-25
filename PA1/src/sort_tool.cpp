// **************************************************************************
//  File       [sort_tool.cpp]
//  Author     [Yu-Hao Ho]
//  Synopsis   [The implementation of the SortTool Class]
//  Modify     [2020/9/15 Mu-Ting Wu]
// **************************************************************************

#include "sort_tool.h"
#include<iostream>

// Constructor
SortTool::SortTool() {}

// Insertsion sort method
void SortTool::InsertionSort(vector<int>& data) {
    // Function : Insertion sort
    // TODO : Please complete insertion sort code here
    for (int j = 1; j < data.size(); j++){
        int key = data[j];
        int i = j - 1;
        while ((i >= 0) && (data[i] > key)){
            data[i+1] = data[i];
            i--;
        }
        data[i+1] = key;
    }
}

// Quick sort method
void SortTool::QuickSort(vector<int>& data){
    QuickSortSubVector(data, 0, data.size() - 1);
}
// Sort subvector (Quick sort)
void SortTool::QuickSortSubVector(vector<int>& data, int low, int high) {
    // Function : Quick sort subvector
    // TODO : Please complete QuickSortSubVector code here
    // Hint : recursively call itself
    //        Partition function is needed
    if (low < high){
        int q = Partition(data, low, high);
        QuickSortSubVector(data, low, q-1);
        QuickSortSubVector(data, q+1, high);
    }
}

int SortTool::Partition(vector<int>& data, int low, int high) {
    // Function : Partition the vector 
    // TODO : Please complete the function
    // Hint : Textbook page 171
    int pivot = data[(low+high)/2];
    swap(data[(low+high)/2], data[high]);
    int i = low - 1;
    for (int j = low ;  j < high; j++){
        if (data[j] <= pivot){
            i++;
            swap(data[i], data[j]);
        }
    }
    swap(data[i+1], data[high]);
    return i+1;
}

// Merge sort method
void SortTool::MergeSort(vector<int>& data){
    MergeSortSubVector(data, 0, data.size() - 1);
}

// Sort subvector (Merge sort)
void SortTool::MergeSortSubVector(vector<int>& data, int low, int high) {
    // Function : Merge sort subvector
    // TODO : Please complete MergeSortSubVector code here
    // Hint : recursively call itself
    //        Merge function is needed
    if (low < high){
        int mid = (low+high) / 2;
        MergeSortSubVector(data, low, mid);
        MergeSortSubVector(data, mid+1, high);
        Merge(data, low, mid, mid+1, high);
    }
}

// Merge
void SortTool::Merge(vector<int>& data, int low, int middle1, int middle2, int high) {
    // Function : Merge two sorted subvector
    // TODO : Please complete the function
    int n1 = middle1 - low + 1;
    int n2 = high - middle2 + 1;
    vector<int> L(n1+1);
    vector<int> R(n2+1);

    int i,j;
    for (i = 0; i < n1; i++){
        L[i] = data[low + i];
    }
    for (j = 0; j < n2; j++){
        R[j] = data[middle2 + j];
    }
    L[n1] = 10000000;
    R[n2] = 10000000;
    i = 0;
    j = 0;
    for (int k = low; k <= high; k++){
        if (L[i] <= R[j]){
            data[k] = L[i];
            i++;
        }else {
            data[k] = R[j];
            j++;
        }
    }
}

// Heap sort method
void SortTool::HeapSort(vector<int>& data) {
    // Build Max-Heap
    BuildMaxHeap(data);
    // 1. Swap data[0] which is max value and data[i] so that the max value will be in correct location
    // 2. Do max-heapify for data[0]
    for (int i = data.size() - 1; i >= 1; i--) {
        swap(data[0],data[i]);
        heapSize--;
        MaxHeapify(data,0);
    }
}

//Max heapify
void SortTool::MaxHeapify(vector<int>& data, int root) {
    // Function : Make tree with given root be a max-heap if both right and left sub-tree are max-heap
    // TODO : Please complete max-heapify code here
    int l = 2*root + 1;
    int r = 2*root + 2;
    int largest = 0;
    if ((l <= heapSize - 1) && (data[l] > data[root])){
        largest = l;
    }else {
        largest = root;
    }
    if ((r <= heapSize - 1) && (data[r] > data[largest])){
        largest = r;
    }
    if (largest != root){
        swap(data[root], data[largest]);
        MaxHeapify(data, largest);
    }
}

//Build max heap
void SortTool::BuildMaxHeap(vector<int>& data) {
    heapSize = data.size(); // initialize heap size
    // Function : Make input data become a max-heap
    // TODO : Please complete BuildMaxHeap code here
    for (int i = heapSize/2; i >= 0; i--){
        MaxHeapify(data, i);
    }
}
