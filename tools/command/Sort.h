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
