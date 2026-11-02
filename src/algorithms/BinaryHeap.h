#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <algorithm>
#include <utility>

/**
 * @file BinaryHeap.h
 * @brief Custom Generic Indexed Binary Min-Heap in pure C++.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPT: TEMPLATES & DATA STRUCTURES
 * - Generic template class supporting any comparable key type.
 * - Supports decreaseKey operation in O(log N) time via internal index mapping.
 * - Used in Dijkstra's shortest path and A* algorithms.
 */
template<typename T, typename KeyType = double, typename Compare = std::less<KeyType>>
class IndexedBinaryHeap {
private:
    struct HeapElement {
        T item;
        KeyType key;
        size_t id;
    };

    std::vector<HeapElement> m_heap;
    std::unordered_map<T, size_t> m_positionMap; // item -> heap index
    Compare m_compare;

    void swapElements(size_t i, size_t j) {
        std::swap(m_heap[i], m_heap[j]);
        m_positionMap[m_heap[i].item] = i;
        m_positionMap[m_heap[j].item] = j;
    }

    void siftUp(size_t index) {
        while (index > 0) {
            size_t parent = (index - 1) / 2;
            if (m_compare(m_heap[index].key, m_heap[parent].key)) {
                swapElements(index, parent);
                index = parent;
            } else {
                break;
            }
        }
    }

    void siftDown(size_t index) {
        size_t size = m_heap.size();
        while (2 * index + 1 < size) {
            size_t leftChild = 2 * index + 1;
            size_t rightChild = 2 * index + 2;
            size_t bestChild = leftChild;

            if (rightChild < size && m_compare(m_heap[rightChild].key, m_heap[leftChild].key)) {
                bestChild = rightChild;
            }

            if (m_compare(m_heap[bestChild].key, m_heap[index].key)) {
                swapElements(index, bestChild);
                index = bestChild;
            } else {
                break;
            }
        }
    }

public:
    IndexedBinaryHeap() = default;
    ~IndexedBinaryHeap() = default;

    bool empty() const {
        return m_heap.empty();
    }

    size_t size() const {
        return m_heap.size();
    }

    bool contains(const T& item) const {
        return m_positionMap.find(item) != m_positionMap.end();
    }

    void push(const T& item, KeyType key) {
        if (contains(item)) {
            decreaseKey(item, key);
            return;
        }

        size_t newIndex = m_heap.size();
        m_heap.push_back({item, key, newIndex});
        m_positionMap[item] = newIndex;
        siftUp(newIndex);
    }

    std::pair<T, KeyType> top() const {
        if (empty()) {
            throw std::underflow_error("IndexedBinaryHeap::top: Heap is empty");
        }
        return {m_heap[0].item, m_heap[0].key};
    }

    std::pair<T, KeyType> pop() {
        if (empty()) {
            throw std::underflow_error("IndexedBinaryHeap::pop: Heap is empty");
        }

        auto root = m_heap[0];
        m_positionMap.erase(root.item);

        if (m_heap.size() == 1) {
            m_heap.pop_back();
            return {root.item, root.key};
        }

        m_heap[0] = m_heap.back();
        m_positionMap[m_heap[0].item] = 0;
        m_heap.pop_back();

        siftDown(0);
        return {root.item, root.key};
    }

    void decreaseKey(const T& item, KeyType newKey) {
        auto it = m_positionMap.find(item);
        if (it == m_positionMap.end()) {
            push(item, newKey);
            return;
        }

        size_t index = it->second;
        if (m_compare(newKey, m_heap[index].key)) {
            m_heap[index].key = newKey;
            siftUp(index);
        }
    }

    void clear() {
        m_heap.clear();
        m_positionMap.clear();
    }
};

#endif // BINARY_HEAP_H
