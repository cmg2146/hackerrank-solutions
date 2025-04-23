// Solves: https://www.hackerrank.com/challenges/fraudulent-activity-notifications/problem?isFullScreen=true

#include <cmath>
#include <stdexcept>
#include <vector>

template<typename TValue>
class BinaryHeapNode;

template<typename TValue>
class BinaryHeap;

template<typename TValue>
class BinaryHeapNode {
public:
    int index;
    TValue value;
    BinaryHeap<TValue>* heap;

    // copy and move constructor and assignment not implemented
    BinaryHeapNode(const BinaryHeapNode&)=delete;
    BinaryHeapNode& operator=(const BinaryHeapNode&)=delete;
    BinaryHeapNode(const BinaryHeapNode&&)=delete;
    BinaryHeapNode& operator=(const BinaryHeapNode&&)=delete;

    /// @brief Constructs new binary heap node
    /// @param index The index of this node in the underlying heap vector
    /// @param value The value of the node
    /// @param heap A pointer to the heap this node belongs to
    BinaryHeapNode(int index, TValue value, BinaryHeap<TValue>* heap) :
        index{index},
        value{value},
        heap{heap}
    {}
};

template<typename TValue>
class BinaryHeap {
public:
    // copy and move constructor and assignment not implemented
    BinaryHeap(const BinaryHeap&)=delete;
    BinaryHeap& operator=(const BinaryHeap&)=delete;
    BinaryHeap(const BinaryHeap&&)=delete;
    BinaryHeap& operator=(const BinaryHeap&&)=delete;

    /// @brief Constructs a binary heap. This class offers similar functionality to the STL heap
    /// algothims but allows arbitrary nodes to be updated while maintaining the heap property.
    /// When arbitrary nodes are updated, the heap is fixed up in logarithmic time.
    /// @param isMaxHeap True if max heap, false if min heap
    /// @param capacity The maximum number of elements to store in the heap
    BinaryHeap(bool isMaxHeap, int capacity) :
        isMaxHeap{isMaxHeap},
        heap{}
    {
        if (capacity < 1)
        {
            throw std::out_of_range("Capacity must be greater than 0");
        }

        heap.reserve(capacity);
    }

    /// @brief Returns the number of nodes currently in the heap
    /// @return 
    int size() const
    {
        return heap.size();
    }

    /// @brief Returns the maximum number of nodes that can be in the heap
    /// @return 
    int capacity() const
    {
        return heap.capacity();
    }

    /// @brief adds a new element to the heap
    /// @param value The value of the new element to add
    /// @return A reference to the newly added heap node
    BinaryHeapNode<TValue>& add(TValue value)
    {
        int count = size();
        if (count == capacity())
        {
            throw std::runtime_error("Heap is full!");
        }

        // allocate a new node on the heap and add reference to the end of the heap vector
        BinaryHeapNode<TValue>* node = new BinaryHeapNode<TValue>(count, value, this);
        heap.push_back(node);

        // now fix up the heap to maintain the heap property
        fixHeap(node, true);

        return *node;
    }

    /// @brief Updates the value in the specified node, fixing up the heap to maintain the heap
    /// property
    /// @param node The node to update
    /// @param value The new value
    void update(BinaryHeapNode<TValue>& node, TValue value)
    {
        auto oldValue = node.value;

        // dont need to do anything if value is the same
        if (value != oldValue)
        {
            bool moveUp = shouldMoveUp(value, oldValue);
            node.value = value;
            fixHeap(&node, moveUp);
        }
    }

    /// @brief Swaps the root of this heap with the root of the other heap, fixing up both heaps to
    /// maintain the heap property.
    /// @param otherHeap The other heap to swap roots with
    void swapRoot(BinaryHeap<TValue>& otherHeap)
    {
        auto ourRootPtr = getRoot();
        auto theirRootPtr = otherHeap.getRoot();
        if ((ourRootPtr == nullptr) || (theirRootPtr == nullptr))
        {
            throw std::runtime_error("One of the heaps is empty! Can't swap roots");
        }

        // perform the swap. first update the heap each node is pointing to, then swap the actual
        // root nodes in the underlying vector
        std::swap(ourRootPtr->heap, theirRootPtr->heap);
        std::swap(heap[0], otherHeap.heap[0]);

        // now fix up the heaps
        fixHeap(heap[0], false);
        otherHeap.fixHeap(otherHeap.heap[0], false);
    }

    /// @brief Returns the root node or a nullptr if the heap is empty
    /// @return 
    BinaryHeapNode<TValue>* getRoot() const
    {
        BinaryHeapNode<TValue>* result = nullptr;

        if (size() > 0)
        {
            result = heap[0];
        }

        return result;
    }

    /// @brief Destroys a binary heap
    ~BinaryHeap()
    {
        // need to clean up the dynamically allocated nodes
        for (auto node: heap)
        {
            delete node;
        }
    }

private:
    bool isMaxHeap;
    // TODO: use unique pointer and remove associated destructor code
    /// @brief The underlying heap vector. A vector of pointers to nodes.
    std::vector<BinaryHeapNode<TValue>*> heap;

    /// @brief fixes up the heap when a node's value is changed
    /// @param node The node that was updated
    /// @param moveUp Indicates if the node should be sifted up or down
    void fixHeap(BinaryHeapNode<TValue>* node, bool moveUp)
    {
        bool continueSwapping = true;
        while (continueSwapping)
        {
            auto swapWith = moveUp
                ? getSwapUp(node)
                : getSwapDown(node);
            
            if (swapWith != nullptr)
            {
                swapNodes(node, swapWith);
            }
            else
            {
                continueSwapping = false;
            }
        }
    }

    /// @brief Swaps the position of two nodes in the heap
    /// @param node 
    /// @param otherNode 
    void swapNodes(BinaryHeapNode<TValue>* node, BinaryHeapNode<TValue>* otherNode)
    {
        // update the indexes first then swap the underlying pointers in the vector
        std::swap(node->index, otherNode->index);
        std::swap(heap[node->index], heap[otherNode->index]);
    }

    /// @brief Returns the parent node of the specified node if the node needs to be moved up to satisfy
    /// the heap property. Returns nullptr if the node does not need to be moved up.
    /// @param node 
    /// @return 
    BinaryHeapNode<TValue>* getSwapUp(BinaryHeapNode<TValue>* node)
    {
        BinaryHeapNode<TValue>* result = nullptr;

        auto parent = getParent(node);
        if (parent != nullptr)
        {
            bool shouldMove = shouldMoveUp(node->value, parent->value);
            if (shouldMove)
            {
                result = parent;
            }
        }

        return result;
    }

    /// @brief Returns the child node of the specified node if the node needs to be moved down to
    /// satisfy the heap property. Returns nullptr if the node does not need to be moved down.
    /// @param node 
    /// @return 
    BinaryHeapNode<TValue>* getSwapDown(BinaryHeapNode<TValue>* node)
    {
        BinaryHeapNode<TValue>* result = nullptr;

        auto candidateChild = getMaxOrMinChild(node);
        if (candidateChild != nullptr)
        {
            bool shouldMove = shouldMoveDown(node->value, candidateChild->value);
            if (shouldMove)
            {
                result = candidateChild;
            }
        }

        return result;
    }
    
    /// @brief Indicates if the first value should be moved up in the heap, relative to the second value, to
    /// satisfy the heap property.
    /// @param firstValue 
    /// @param secondValue 
    /// @return 
    bool shouldMoveUp(TValue firstValue, TValue secondValue)
    {
        return isMaxHeap
            ? (firstValue > secondValue)
            : (firstValue < secondValue);
    }

    /// @brief Indicates if the first value should be moved down in the heap, relative to the second value, to
    /// satisfy the heap property.
    /// @param firstValue 
    /// @param secondValue 
    /// @return 
    bool shouldMoveDown(TValue firstValue, TValue secondValue)
    {
        return isMaxHeap
            ? (firstValue < secondValue)
            : (firstValue > secondValue);
    }

    /// @brief Returns the parent of a given node, or nullptr if the node is the root node
    /// @param node The node to get the parent for
    /// @return Pointer to parent node or nullptr if no parent
    BinaryHeapNode<TValue>* getParent(BinaryHeapNode<TValue>* node)
    {
        BinaryHeapNode<TValue>* result = nullptr;
        int nodeIndex = node->index;

        if (nodeIndex > 0)
        {
            // even indexed nodes are always righthand children.
            bool isEven = ((nodeIndex % 2) == 0);
            int offset = isEven ? 1 : 0;
            int parentIndex = (nodeIndex - offset - 1) / 2;

            result = heap[parentIndex];
        }

        return result;
    }
    
    /// @brief Returns the left child of a given node, or nullptr if the node doesnt have a left child
    /// @param node The node to get the left child of
    /// @return Pointer to left child node or nullptr if no left child
    BinaryHeapNode<TValue>* getLeftChild(BinaryHeapNode<TValue>* node)
    {
        BinaryHeapNode<TValue>* result = nullptr;
        int nodeIndex = node->index;
        int halfCapacity = capacity() / 2;

        // a crude check to avoid integer overflow
        if (nodeIndex < halfCapacity)
        {
            // odd indexed nodes are always lefthand children.
            int childIndex = (nodeIndex * 2) + 1;
            if (childIndex < size())
            {
                result = heap[childIndex];
            }
        }

        return result;
    }

    /// @brief Returns the right child of a given node, or nullptr if the node doesnt have a right child
    /// @param node The node to get the right child of
    /// @return Pointer to right child node or nullptr if no right child
    BinaryHeapNode<TValue>* getRightChild(BinaryHeapNode<TValue>* node)
    {
        BinaryHeapNode<TValue>* result = nullptr;
        int nodeIndex = node->index;
        int halfCapacity = capacity() / 2;

        // a crude check to avoid integer overflow
        if (nodeIndex < halfCapacity)
        {
            // even indexed nodes are always righthand children.
            int childIndex = (nodeIndex * 2) + 2;
            if (childIndex < size())
            {
                result = heap[childIndex];
            }
        }

        return result;
    }
    
    /// @brief Gets the child with the greater value (if max heap) or smaller value (min heap). Returns
    /// nullptr if the node does not have any children.
    /// @param node The node to get the max child for
    /// @return 
    BinaryHeapNode<TValue>* getMaxOrMinChild(BinaryHeapNode<TValue>* node)
    {
        BinaryHeapNode<TValue>* result = nullptr;
        auto leftChild = getLeftChild(node);
        auto rightChild = getRightChild(node);

        if (leftChild == nullptr)
        {
            // right child may still be null, but thats ok, returning nullptr indicates no children
            result = rightChild;
        }
        else if (rightChild == nullptr)
        {
            // left child may still be null, but thats ok, returning nullptr indicates no children
            result = leftChild;
        }
        else
        {
            // use the largest child for max heap and smallest for min heap
            bool useLeftChild = shouldMoveUp(leftChild->value, rightChild->value);
            result = useLeftChild ? leftChild : rightChild;
        }

        return result;
    }
};

template<typename TValue>
class MovingMedian {
public:
    /// @brief Constructs an instance of a moving median calculator which calculates a median in
    /// logarithmic time by combining equal, or as close as possible, sized max and min heaps.
    /// @param maxSamples Maximum number of samples to use in calculating the median 
    MovingMedian(int maxSamples) :
        // when number of samples is odd, the max heap size is 1 greater than the min heap size
        MovingMedian(maxSamples, (maxSamples / 2) + (maxSamples % 2))
    { }

    /// @brief Returns current number of samples used in the median calculation
    /// @return 
    int getCount() const
    {
        return maxHeap.size() + minHeap.size();
    }

    /// @brief Returns twice the current median value. If the current number of samples is odd,
    /// the median value is the middle value, if the current number of samples is even, the
    /// median value is the average of the two middle values.
    /// @return 
    int getTwiceMedian() const
    {
        if (getCount() == 0)
        {
            throw std::runtime_error("No samples yet!");
        }
        
        // Use the max heap root if the number of samples is odd or both heap roots if the number of
        // samples is even.
        int low_median = maxHeap.getRoot()->value;
        int high_median = low_median;
        if (maxHeap.size() == minHeap.size())
        {
            high_median = minHeap.getRoot()->value;
        }

        return low_median + high_median;
    }

    /// @brief Add a sample to the moving median calculation
    /// @param value 
    void add(TValue value)
    {
        int count = getCount();

        if (count == maxSamples)
        {
            // we have reached the max number of samples. replace the value in the oldest sample
            BinaryHeapNode<TValue>& nodeToUpdate = *samples[sampleIndex];
            nodeToUpdate.heap->update(nodeToUpdate, value);
        }
        else
        {
            // we havent reached the max sample size yet.
            // alternate between adding to min and max heap to keep the heaps the same size or
            // the max heap one size larger to ensure the median value(s) are always stored in
            // the roots.
            bool isEven = (count % 2) == 0;
            BinaryHeapNode<TValue>& node = isEven ? maxHeap.add(value) : minHeap.add(value);
            samples.push_back(&node);
        }

        /*
        at this point, the min and max heaps both satisfy their heap property indepedently,
        but, it's possible the heap we added/updated the value in wasnt the correct heap to
        ensure the two middle values are at the heap roots. So swap the roots then fix up the
        heaps again, if necessary.
        */
        auto minHeapRoot = minHeap.getRoot();
        auto maxHeapRoot = maxHeap.getRoot();
        if ((minHeapRoot != nullptr) && (maxHeapRoot != nullptr))
        {
            // min heap root value should be greater than max heap root value
            if (minHeapRoot->value < maxHeapRoot->value)
            {
                minHeap.swapRoot(maxHeap);
            }
        }

        sampleIndex += 1;
        sampleIndex %= maxSamples;
    }

private:
    // the heaps are declared before the samples vector so the samples vector is destroyed first.
    // this isnt required but it's more correct because the samples vector contains pointers to
    // nodes in the heaps.
    BinaryHeap<TValue> maxHeap;
    BinaryHeap<TValue> minHeap;
    int maxSamples;
    int sampleIndex;
    std::vector<BinaryHeapNode<TValue>*> samples;

    MovingMedian(int maxSamples, int maxHeapCapacity) :
        maxHeap(true, std::max(1, maxHeapCapacity)),
        minHeap(false, std::max(1, maxSamples - maxHeapCapacity)),
        maxSamples{maxSamples},
        sampleIndex{0},
        samples{}
    {
        if (maxSamples < 1)
        {
            throw std::out_of_range("Max samples must be greater than 0");
        }
    }
};

int activityNotifications(std::vector<int> expenditure, int d)
{
    int result = 0;
    MovingMedian<int> medianCalculator{d};

    for (auto curDaySpending : expenditure)
    {
        if (medianCalculator.getCount() == d)
        {
            if (curDaySpending >= medianCalculator.getTwiceMedian())
            {
                result++;
            }
        }
        medianCalculator.add(curDaySpending);
    }
    
    return result;
}

int main(void)
{
    return 0;
}
