// Solves: https://www.hackerrank.com/challenges/fraudulent-activity-notifications/problem?isFullScreen=true

import java.util.List;
import java.util.ArrayList;

class BinaryHeapNode<T extends Comparable<T>> {
    private T value;
    private int index;
    private BinaryHeap<T> heap;

    public BinaryHeapNode(T value, int index, BinaryHeap<T> heap) {
        this.value = value;
        this.index = index;
        this.heap = heap;
    }

    public T getValue() {
        return value;
    }

    void setValue(T value) {
        this.value = value;
    }

    public int getIndex() {
        return index;
    }

    void setIndex(int index) {
        this.index = index;
    }

    public BinaryHeap<T> getHeap() {
        return heap;
    }

    void setHeap(BinaryHeap<T> heap) {
        this.heap = heap;
    }
}

class BinaryHeap<T extends Comparable<T>> {
    private boolean isMaxHeap;
    private int capacity;
    private ArrayList<BinaryHeapNode<T>> heap;

    /**
    * Constructs a new binary heap
    * <p>
    * @param  isMaxHeap Indicates if this heap is a max or min heap
    * @param  capacity  The max number of nodes that can be stored in the heap
    * @throws IllegalArgumentException If the capacity is less than 1
    */
    public BinaryHeap(boolean isMaxHeap, int capacity) {
        if (capacity < 1) {
            throw new IllegalArgumentException("capacity should be greater than 0");
        }

        this.isMaxHeap = isMaxHeap;
        this.capacity = capacity;

        // initialize the underlying heap storage
        heap = new ArrayList<>(this.capacity);
    }

    /**
    * Add a node to the heap
    * <p>
    * @param  value The value for the new node
    * @throws IllegalStateException If the heap has reached capacity
    */
    public BinaryHeapNode<T> add(T value) {
        int size = getSize();

        if (size >= capacity) {
            throw new IllegalStateException("Heap is full! Can't add any more nodes.");
        }

        // create a new node and add it to the end of the heap storage
        BinaryHeapNode<T> node = new BinaryHeapNode<>(value, size, this);
        heap.add(node);

        // fix up the heap to maintain the heap property
        update(node, true);

        return node;
    }

    /**
    * Update the value of the specified node in the heap. This will also rearrange nodes to maitain
    * the heap property.
    * <p>
    * @param  node  The node to update
    * @param  value The new value for the node
    */
    public void update(BinaryHeapNode<T> node, T value) {
        T oldValue = node.getValue();

        // if the value is the same, do nothing
        if (!oldValue.equals(value)) {
            node.setValue(value);
            update(node, shouldMoveUp(value, oldValue));
        }
    }

    /**
    * Swap the root node in this heap with another heap's root node
    * <p>
    * @param  otherHeap  The other heap to swap roots with
    */
    public void swapRoot(BinaryHeap<T> otherHeap) {
        BinaryHeapNode<T> ourRoot = getRoot();
        BinaryHeapNode<T> theirRoot = otherHeap.getRoot();

        if ((ourRoot == null) || (theirRoot == null)) {
            throw new IllegalStateException("One of the heaps is empty! Can't swap roots");
        }

        // now perform the swap
        BinaryHeapNode<T> tempOurRoot = ourRoot;
        ourRoot = theirRoot;
        theirRoot = tempOurRoot;
        ourRoot.setHeap(this);
        theirRoot.setHeap(otherHeap);
        heap.set(0, ourRoot);
        otherHeap.heap.set(0, theirRoot);

        // make sure to fix up the heaps
        update(ourRoot, false);
        otherHeap.update(theirRoot, false);
    }

    /**
    * Get the current size of the heap
    * <p>
    * @return  The number of nodes currently in the heap
    */
    public int getSize() {
        return heap.size();
    }

    /**
    * Returns the root node in the heap
    * <p>
    * @return  The root node or null if the heap is empty
    */
    public BinaryHeapNode<T> getRoot() {
        if (heap.isEmpty()) {
            return null;
        } else {
            return heap.get(0);
        }
    }

    /**
    * Updates the heap to maintain the heap property when a node's value is changed.
    * <p>
    * @param  node      The node that was updated
    * @param  moveUp    Indicates if the node should be moved up or down when updating
    */
    private void update(BinaryHeapNode<T> node, boolean moveUp) {
        boolean continueSwapping = true;
        while (continueSwapping) {
            var swapWith = moveUp
                    ? getSwapUp(node)
                    : getSwapDown(node);

            if (swapWith != null) {
                swapNodes(node, swapWith);
            } else {
                continueSwapping = false;
            }
        }
    }

    /**
    * Returns the parent node of the specified node if the node needs to be moved up to satisfy
    * the heap property. Returns null if the node does not need to be moved up.
    * <p>
    * @param  node The specified node to test
    * @return      The parent node the node should be swapped with, or null if no swap necessary.
    */
    private BinaryHeapNode<T> getSwapUp(BinaryHeapNode<T> node) {
        BinaryHeapNode<T> result = null;

        var parent = getParent(node);
        if ((parent != null) && shouldMoveUp(node.getValue(), parent.getValue())) {
            result = parent;
        }

        return result;
    }

    /**
    * Returns a child node of the specified node if the node needs to be moved down to satisfy
    * the heap property. Returns null if the node does not need to be moved down.
    * <p>
    * @param  node The specified node to test
    * @return      The child node the node should be swapped with, or null if no swap necessary.
    */
    private BinaryHeapNode<T> getSwapDown(BinaryHeapNode<T> node) {
        BinaryHeapNode<T> result = null;

        var candidateChild = getMaxOrMinChild(node);
        if ((candidateChild != null) && shouldMoveDown(node.getValue(), candidateChild.getValue())) {
            result = candidateChild;
        }

        return result;
    }

    /**
    * Indicates if the first value should be moved up in the heap, relative to the second value, to
    * satisfy the heap property.
    * <p>
    * @param  firstValue    The first value to compare
    * @param  secondValue   The second value to compare
    * @return               True if the first value should be moved up
    */
    private boolean shouldMoveUp(T firstValue, T secondValue) {
        var valComparison = firstValue.compareTo(secondValue);
        return isMaxHeap
                ? (valComparison > 0)
                : (valComparison < 0);
    }

    /**
    * Indicates if the first value should be moved down in the heap, relative to the second value, to
    * satisfy the heap property.
    * <p>
    * @param  firstValue    The first value to compare
    * @param  secondValue   The second value to compare
    * @return               True if the first value should be moved down
    */
    private boolean shouldMoveDown(T firstValue, T secondValue) {
        var valComparison = firstValue.compareTo(secondValue);
        return isMaxHeap
                ? (valComparison < 0)
                : (valComparison > 0);
    }

    /**
    * Swaps two connected nodes in the heap
    * <p>
    * @param  node      The first node to swap
    * @param  otherNode The other node to swap with
    */
    private void swapNodes(BinaryHeapNode<T> node, BinaryHeapNode<T> otherNode) {
        var oldIndex = node.getIndex();
        var oldOtherIndex = otherNode.getIndex();

        heap.set(oldIndex, otherNode);
        otherNode.setIndex(oldIndex);
        heap.set(oldOtherIndex, node);
        node.setIndex(oldOtherIndex);
    }

    /**
    * Returns the parent of a given node, or null if the node is the root node
    * <p>
    * @param  node      The given node to return the parent for
    * @return           The parent node or null if the node is the root node in the heap
    */
    private BinaryHeapNode<T> getParent(BinaryHeapNode<T> node) {
        var nodeIndex = node.getIndex();
        if (nodeIndex < 1) {
            return null;
        }

        // even indexed nodes are always righthand children.
        var offset = ((nodeIndex % 2) == 0) ? 1 : 0;
        var parentIndex = (nodeIndex - offset - 1) / 2;

        return heap.get(parentIndex);
    }

    /**
    *  Returns the left child of a given node, or null if the node doesnt have a left child
    * <p>
    * @param  node      The given node to return the left child for
    * @return           The child node or null if the node has no left child
    */
    private BinaryHeapNode<T> getLeftChild(BinaryHeapNode<T> node) {
        BinaryHeapNode<T> result = null;

        // odd indexed nodes are always lefthand children.
        var childIndex = (node.getIndex() * 2) + 1;
        if (childIndex < getSize()) {
            result = heap.get(childIndex);
        }

        return result;
    }

    /**
    *  Returns the right child of a given node, or null if the node doesnt have a right child
    * <p>
    * @param  node      The given node to return the right child for
    * @return           The child node or null if the node has no right child
    */
    private BinaryHeapNode<T> getRightChild(BinaryHeapNode<T> node) {
        BinaryHeapNode<T> result = null;

        // even indexed nodes are always righthand children.
        var childIndex = (node.getIndex() * 2) + 2;
        if (childIndex < getSize()) {
            result = heap.get(childIndex);
        }

        return result;
    }

    /**
    *  Gets the child with the greater value (if max heap) or smaller value (min heap). Returns
    * null if the node does not have any children.
    * <p>
    * @param  node      The given node to return the max child for
    * @return           The child node or null if the node has no children
    */
    private BinaryHeapNode<T> getMaxOrMinChild(BinaryHeapNode<T> node) {
        BinaryHeapNode<T> result;

        var leftChild = getLeftChild(node);
        var rightChild = getRightChild(node);

        if (leftChild == null) {
            // right child may still be null, but thats ok, returning null indicates no
            // children
            result = rightChild;
        } else if (rightChild == null) {
            // left child may still be null, but thats ok, returning null indicates no
            // children
            result = leftChild;
        } else {
            // use the largest child for max heap and smallest for min heap
            result = shouldMoveUp(leftChild.getValue(), rightChild.getValue())
                    ? leftChild
                    : rightChild;
        }

        return result;
    }
}

class MovingMedian<T extends Comparable<T>> {
    private int maxSamples;
    private BinaryHeap<T> minHeap;
    private BinaryHeap<T> maxHeap;
    private ArrayList<BinaryHeapNode<T>> rollingMap;
    private int rollingIndex;

    public MovingMedian(int maxSamples) {
        if (maxSamples < 1) {
            throw new IllegalArgumentException("Max samples must be greater than 0");
        }

        this.maxSamples = maxSamples;
        // the rolling map keeps track of the current samples that are used in the
        // moving median
        rollingIndex = 0;
        rollingMap = new ArrayList<>(maxSamples);

        // initialize the heaps such that they are the same size or the max heap size is
        // 1 greater
        var minHeapCapacity = this.maxSamples / 2;
        var maxHeapCapacity = minHeapCapacity;
        if ((this.maxSamples % 2) == 1) {
            maxHeapCapacity += 1;
        }
        maxHeap = new BinaryHeap<>(true, maxHeapCapacity);
        minHeap = new BinaryHeap<>(false, minHeapCapacity);
    }

    public void add(T value) {
        var size = numSamples();

        if (size < maxSamples) {
            // if number of samples is less than the max, alternate adding to max and min
            // heap
            var isEven = (size % 2) == 0;
            var heapToAdd = isEven ? maxHeap : minHeap;
            var node = heapToAdd.add(value);
            rollingMap.add(node);
        } else {
            // we have reached maximum number of samples so replace oldest sample
            var node = rollingMap.get(rollingIndex);
            node.getHeap().update(node, value);
        }

        // at this point, the min and max heaps both satisfy their heap property indepedently,
        // but, it's possible the heap we added/updated the value in wasnt the correct heap to
        // ensure the two middle values are at the heap roots. So swap the roots then fix up the
        // heaps again, if necessary.
        var minHeapRoot = minHeap.getRoot();
        var maxHeapRoot = maxHeap.getRoot();
        if ((minHeapRoot != null)
            && (maxHeapRoot != null)
            && (minHeapRoot.getValue().compareTo(maxHeapRoot.getValue()) < 0)
        ) {
            // min heap root value should be greater than max heap root value
            minHeap.swapRoot(maxHeap);
        }

        rollingIndex += 1;
        rollingIndex %= maxSamples;
    }

    public int numSamples() {
        return minHeap.getSize() + maxHeap.getSize();
    }

    public T getFirstMedian() {
        if (numSamples() == 0) {
            throw new IllegalStateException("No samples added yet!");
        }

        // the first median value is the value stored in the max heap root
        return maxHeap.getRoot().getValue();
    }

    public T getSecondMedian() {
        if (numSamples() == 0) {
            throw new IllegalStateException("No samples added yet!");
        }

        // if the number of samples is even then the second median value is the value
        // stored in the
        // min heap root
        if (minHeap.getSize() == maxHeap.getSize()) {
            return minHeap.getRoot().getValue();
        }

        // if the number of samples is odd, the second median is the same as the first
        // median value
        // (the value stored in the max heap root)
        return getFirstMedian();
    }
}

class Result {
    /*
     * Complete the 'activityNotifications' function below.
     *
     * The function is expected to return an INTEGER.
     * The function accepts following parameters:
     * 1. INTEGER_ARRAY expenditure
     * 2. INTEGER d
     */

    public static int activityNotifications(List<Integer> expenditure, int d) {
        // Write your code here
        var medianCalc = new MovingMedian<Integer>(d);
        int numNotices = 0;

        for (var curDaySpending : expenditure) {
            // notices are only sent out once number of previous days has reached "d"
            if (medianCalc.numSamples() == d) {
                var twiceMedian = medianCalc.getFirstMedian() + medianCalc.getSecondMedian();
                if (curDaySpending >= twiceMedian) {
                    numNotices += 1;
                }
            }

            // add the current day's spending to the new moving median calculation
            medianCalc.add(curDaySpending);
        }

        return numNotices;
    }
}
