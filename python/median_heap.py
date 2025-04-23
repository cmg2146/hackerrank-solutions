"""
Solves: https://www.hackerrank.com/challenges/fraudulent-activity-notifications/problem?isFullScreen=true
"""

from dataclasses import dataclass
from typing import Generic, Optional, TypeVar

TValue = TypeVar("TValue", int, float)

@dataclass
class BinaryHeapNode(Generic[TValue]):
    """A node in a BinaryHeap"""

    value: TValue
    """The value stored in the node"""
    index: int
    """The index of the node in the underlying heap array"""
    heap: "BinaryHeap"
    """The heap this node belongs to"""


class BinaryHeap(Generic[TValue]):
    """A binary heap structure supporting min and max heaps"""

    def __init__(self, is_max_heap: bool, capacity: int):
        """Constructs an instance of a binary heap"""
        if capacity < 1:
            raise ValueError("capacity must be greater than 0")

        self._is_max_heap = is_max_heap
        self._capacity = capacity
        self._count = 0
        self._heap: list[Optional[BinaryHeapNode[TValue]]] = [None] * capacity

    def get_count(self) -> int:
        """Gets current number of items in the heap"""
        return self._count

    def get_root(self) -> Optional[BinaryHeapNode[TValue]]:
        """Gets the root node in the heap, or None if heap is empty"""
        try:
            return self._heap[0]
        except:
            return None

    def add(self, value: TValue) -> BinaryHeapNode[TValue]:
        """Adds a new element to the heap"""
        if self._count == self._capacity:
            raise RuntimeError("Heap is full!")

        # add the node as the first element in the heap array
        node = BinaryHeapNode(value, self._count, self)
        self._heap[node.index] = node
        self._count += 1

        # now fix up the heap to maintain the heap property
        self._update(node, True)

        return node

    def update(self, node: BinaryHeapNode[TValue], value: TValue):
        """Updates the value in a node and fixes up the heap to maintain the heap property"""
        oldValue = node.value

        # dont need to do anything if value is the same
        if value != oldValue:
            node.value = value
            self._update(node, self._should_move_up(value, oldValue))

    def swap_root(self, other_heap: "BinaryHeap[TValue]"):
        """Swaps the root of this heap with the root of the other heap, fixing up both heaps to
        maintain the heap property.
        """
        ourRoot = self.get_root()
        theirRoot = other_heap.get_root()
        if (ourRoot is None) or (theirRoot is None):
            raise RuntimeError("One of the heaps is empty! Can't swap roots")

        tempOurRoot = ourRoot
        ourRoot = theirRoot
        theirRoot = tempOurRoot
        ourRoot.heap = self
        theirRoot.heap = other_heap
        self._heap[0] = ourRoot
        other_heap._heap[0] = theirRoot

        # now fix up the heaps
        self._update(ourRoot, False)
        other_heap._update(theirRoot, False)

    def _update(self, node: BinaryHeapNode[TValue], move_up: bool):
        """Updates the heap to maintain the heap property when a node's value changes.
        
        :param node: The node that was updated
        :param move_up: Indicates which direction to move the node (up or down the tree)
        """
        continue_swapping = True
        while continue_swapping:
            swap_with = self._get_swap_up(node) if move_up else self._get_swap_down(node)
            if swap_with is not None:
                self._swap_nodes(node, swap_with)
            else:
                continue_swapping = False
    
    def _get_swap_up(self, node: BinaryHeapNode[TValue]) -> Optional[BinaryHeapNode[TValue]]:
        """Returns the parent node of the specified node if the node needs to be moved up to satisfy
        the heap property. Returns None if the node does not need to be moved up.
        """
        result: Optional[BinaryHeapNode[TValue]] = None

        parent = self._get_parent(node)
        if (parent is not None) and self._should_move_up(node.value, parent.value):
            result = parent

        return result

    def _get_swap_down(self, node: BinaryHeapNode[TValue]) -> Optional[BinaryHeapNode[TValue]]:
        """Returns a child node of the specified node if the node needs to be moved down to satisfy
        the heap property. Returns None if the node does not need to be moved down.
        """
        result: Optional[BinaryHeapNode[TValue]] = None

        candidate_child = self._get_max_or_min_child(node)
        if (candidate_child is not None) and self._should_move_down(node.value, candidate_child.value):
            result = candidate_child
        
        return result
    
    def _should_move_up(self, first_value: TValue, second_value: TValue) -> bool:
        """Indicates if the first value should be moved up in the heap, relative to the second value, to
        satisfy the heap property.
        """
        if self._is_max_heap:
            result = first_value > second_value
        else:
            result = first_value < second_value
        
        return result

    def _should_move_down(self, first_value: TValue, second_value: TValue) -> bool:
        """Indicates if the first value should be moved down in the heap, relative to the second value, to
        satisfy the heap property.
        """
        if self._is_max_heap:
            result = first_value < second_value
        else:
            result = first_value > second_value
        
        return result

    def _swap_nodes(self, node: BinaryHeapNode[TValue], other_node: BinaryHeapNode[TValue]):
        """Swaps two connected nodes in the heap"""
        old_index = node.index
        old_other_index = other_node.index

        self._heap[old_index] = other_node
        other_node.index = old_index
        self._heap[old_other_index] = node
        node.index = old_other_index

    def _get_parent(self, node: BinaryHeapNode[TValue]) -> Optional[BinaryHeapNode[TValue]]:
        """Returns the parent of a given node, or None if the node is the root node"""
        if node.index < 1:
            return None
        
        # even indexed nodes are always righthand children.
        offset = 1 if ((node.index % 2) == 0) else 0
        parent_index = (node.index - offset - 1) // 2

        return self._heap[parent_index]
    
    def _get_left_child(self, node: BinaryHeapNode[TValue]) -> Optional[BinaryHeapNode[TValue]]:
        """Returns the left child of a given node, or None if the node doesnt have a left child"""
        result: BinaryHeapNode[TValue] = None

        # odd indexed nodes are always lefthand children.
        child_index = (node.index * 2) + 1
        if child_index < self._capacity:
            result = self._heap[child_index]

        return result

    def _get_right_child(self, node: BinaryHeapNode[TValue]) -> Optional[BinaryHeapNode[TValue]]:
        """Returns the right child of a given node, or None if the node doesnt have a right child"""
        result: BinaryHeapNode[TValue] = None

        # even indexed nodes are always righthand children.
        child_index = (node.index * 2) + 2
        if child_index < self._capacity:
            result = self._heap[child_index]

        return result
    
    def _get_max_or_min_child(self, node: BinaryHeapNode[TValue]) -> Optional[BinaryHeapNode[TValue]]:
        """Gets the child with the greater value (if max heap) or smaller value (min heap). Returns
        None if the node does not have any children."""
        left_child = self._get_left_child(node)
        right_child = self._get_right_child(node)

        if left_child is None:
            # right child may still be null, but thats ok, returning null indicates no children
            result = right_child
        elif right_child is None:
            # left child may still be null, but thats ok, returning null indicates no children
            result = left_child
        else:
            # use the largest child for max heap and smallest for min heap
            result = left_child if self._should_move_up(left_child.value, right_child.value) else right_child

        return result

class MedianHeap(Generic[TValue]):
    """A data structure that combines equal sized (or within 1) max and min heaps to calculate a
    moving median over N samples with logirithmic time complexity.
    """

    def __init__(self, capacity: int):
        """Construct an instance of a moving median heap"""
        if capacity < 1:
            raise ValueError("capacity must be greater than 0")
        
        self._capacity = capacity
        self._rolling_values: list[Optional[BinaryHeapNode[TValue]]] = [None] * capacity
        self._rolling_index = 0

        # if the sample size is odd, ensure the max heap has a size that is one greater
        min_heap_capacity = self._capacity // 2
        max_heap_capacity = min_heap_capacity
        if (self._capacity % 2) == 1:
            max_heap_capacity += 1
    
        self._max_heap = BinaryHeap[TValue](True, max_heap_capacity)
        self._min_heap = BinaryHeap[TValue](False, min_heap_capacity)

    def get_count(self) -> int:
        """Returns current number of samples"""
        return self._max_heap.get_count() + self._min_heap.get_count()

    def get_twice_median(self):
        """Returns twice the median value of the samples"""
        if self.get_count() == 0:
            raise RuntimeError("No samples yet!")
        
        # The logic has been designed to
        # 1) keep the heaps the same size if the number of samples is even
        # 2) make the max heap one element larger if the number of samples is odd.
        # when the number of samples is odd, the median value is stored in the root of the max
        # heap and when the number of samples is even, the median value is the average of the
        # values stored in the two heap roots.
        low_median = self._max_heap.get_root().value
        high_median = self._max_heap.get_root().value
        if self._max_heap.get_count() == self._min_heap.get_count():
            high_median = self._min_heap.get_root().value
        
        return low_median + high_median

    def add(self, value: TValue):
        """Adds a new sample to the moving median calculation, removing the oldest sample if necessary"""
        if self.get_count() == self._capacity:
            # we have reached the max number of samples
            # replace the value in the oldest sample
            node_to_update = self._rolling_values[self._rolling_index]
            node_to_update.heap.update(node_to_update, value)
        else:
            # we havent reached the max sample size yet.
            # alternate between adding to min and max heap to keep the heaps the same size or
            # the max heap one size larger to ensure the median value(s) are always stored in
            # the roots.
            is_even = (self.get_count() % 2) == 0
            heap_to_add = self._max_heap if is_even else self._min_heap
            self._rolling_values[self._rolling_index] = heap_to_add.add(value)
        
        # at this point, the min and max heaps both satisfy their heap property indepedently,
        # but, it's possible the heap we added/updated the value in wasnt the correct heap to
        # ensure the two middle values are at the heap roots. So swap the roots then fix up the
        # heaps again, if necessary.
        min_heap_root = self._min_heap.get_root()
        max_heap_root = self._max_heap.get_root()
        if (min_heap_root is not None) and (max_heap_root is not None):
            # min heap root value should be greater than max heap root value
            if min_heap_root.value < max_heap_root.value:
                self._min_heap.swap_root(self._max_heap)

        self._rolling_index += 1
        self._rolling_index %= self._capacity
