//
// Created by wangrl on 2021/3/22.
//

#pragma once

/**
 * Sifts a broken heap. The input array is a heap from root to bottom
 * except that the root entry may be out of place.
 *
 * Sinks a hole from array[root] to leaf and then sifts the original array[root]
 * element from the leaf level up.
 *
 * This version does extra work, in that it copies child to parent on the way down,
 * then copies parent to child on the way back up. When copies are inexpensive,
 * this ia an optimization as this sift variant should only be used when
 * the potentially out of place root entry value is expected to be small.
 *
 * @param root      The one based index into array of the out-of-place root of the heap
 *                  以1开始的数组起始点
 * @param bottom    The one based index in the array of the last entry in the heap
 *                  以1结束的数组结束点
 */
template<typename T, typename C>
void HeapSortSiftUp(T array[], size_t root, size_t bottom, const C& lessThan) {
    T x = array[root - 1];
    size_t start = root;
}


/**
 * Sorts the array of size count using comparator lessThan using a Heap Sort algorithm.
 * Be sure to specialize swap if T has an efficient swap operation.
 *
 * @tparam T
 * @tparam C
 * @param array         the array to be sorted
 * @param count         the number of elements in the array
 * @param lessThan      a functor with bool operator()(T a, T b) which returns true if a comes before b
 */
template<typename T, typename C>
void HeapSort(T array[], size_t count, const C& lessThan) {
    for (size_t i = count >> 1; i > 0; --i) {

    }
}

/**
 * Sorts the array of size count using comparator lessThan using an insertion sort algorithm.
 *
 * @tparam T
 * @tparam C
 * @param left          the array pointer
 * @param count         the number of elements in the array
 * @param lessThan      a functor with bool operator()(T a, T b) which returns true if a comes before b
 */
template<typename T, typename C>
static void InsertionSort(T* left, int count, const C& lessThan) {
    T* right = left + count - 1;
    for (T* next = left + 1; next <= right; next++) {
        if (!lessThan(*next, *(next - 1))) {
            continue;
        }
        // 需要插入的元素
        T insert = std::move(*next);
        // 存放临时数据
        T* hole = next;
        // 找到需要插入的位置
        do {
            *hole = std::move(*(hole - 1));
            hole--;
        } while (left < hole && lessThan(insert, *(hole - 1)));
        *hole = std::move(insert);
    }
}

/**
 * @param left      left pointer (inclusive)
 * @param right     right pointer (exclusive)
 */
template<typename T, typename C>
static void InsertionSort(T* left, T* right, const C& lessThan) {
    InsertionSort(left, right - left - 1, lessThan);
}
