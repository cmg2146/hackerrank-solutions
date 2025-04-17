
#define TRUE (1)
#define FALSE (0)

typedef struct BinaryHeapNode_t BinaryHeapNode;
typedef struct BinaryHeap_t BinaryHeap;

struct BinaryHeapNode_t
{
    int value;
    int index;
    BinaryHeap* heap;
};

struct BinaryHeap_t
{
    int is_max_heap;
    int capacity;
    int count;
    BinaryHeapNode** heap;
};

typedef struct MovingMedian_t
{
    int max_samples;
    BinaryHeapNode** samples;
    int sample_index;
    BinaryHeap* max_heap;
    BinaryHeap* min_heap;
} MovingMedian;

// binary heap operations
static void init_heap(BinaryHeap* heap, int is_max_heap, int capacity);
static void destroy_heap(BinaryHeap* heap);
static BinaryHeapNode* add(BinaryHeap* heap, int value);
static void update(BinaryHeap* heap, BinaryHeapNode* node, int value);
static BinaryHeapNode* get_root(BinaryHeap* heap);
static void swap_root(BinaryHeap* heap, BinaryHeap* other_heap);
static void _update(BinaryHeap* heap, BinaryHeapNode* node, int should_move_up);
static BinaryHeapNode* _get_swap_up(BinaryHeap* heap, BinaryHeapNode* node);
static BinaryHeapNode* _get_swap_down(BinaryHeap* heap, BinaryHeapNode* node);
static int _should_move_up(BinaryHeap* heap, int first_value, int second_value);
static int _should_move_down(BinaryHeap* heap, int first_value, int second_value);
static void _swap_nodes(BinaryHeap* heap, BinaryHeapNode* node, BinaryHeapNode* other_node);
static BinaryHeapNode* _get_parent(BinaryHeap* heap, BinaryHeapNode* node);
static BinaryHeapNode* _get_left_child(BinaryHeap* heap, BinaryHeapNode* node);
static BinaryHeapNode* _get_right_child(BinaryHeap* heap, BinaryHeapNode* node);
static BinaryHeapNode* _get_max_or_min_child(BinaryHeap* heap, BinaryHeapNode* node);

// moving median operations
static void init_moving_median(MovingMedian* median, int max_samples);
static void destroy_moving_median(MovingMedian* median);
static void add_sample(MovingMedian* median, int value);
static int get_twice_median(MovingMedian* median);
static int get_sample_count(MovingMedian* median);

// Initialize a binary heap
static void init_heap(BinaryHeap* heap, int is_max_heap, int capacity)
{
    heap->is_max_heap = is_max_heap;
    heap->capacity = capacity;
    heap->count = 0;

    // caller must make sure capacity is greater than 0
    heap->heap = (BinaryHeapNode**)calloc(capacity, sizeof(BinaryHeapNode*));
}

static void destroy_heap(BinaryHeap* heap)
{
    // free memory allocated for each node
    for (int i = 0; i < heap->count; i++)
    {
        free(heap->heap[i]);
    }

    // free overall heap array
    free(heap->heap);
}

// adds an element to the heap
static BinaryHeapNode* add(BinaryHeap* heap, int value)
{
    BinaryHeapNode* result = 0;

    if (heap->count < heap->capacity)
    {
        // add the node as the first element in the heap array
        BinaryHeapNode* node = (BinaryHeapNode*)malloc(sizeof(BinaryHeapNode));
        node->value = value;
        node->index = heap->count;
        node->heap = heap;

        heap->heap[node->index] = node;
        heap->count += 1;

        // now fix up the heap to maintain the heap property
        _update(heap, node, TRUE);

        result = node;
    }

    return result;
}

// updates the value of a node in the heap
static void update(BinaryHeap* heap, BinaryHeapNode* node, int value)
{
    int oldValue = node->value;
    // dont need to do anything if value is the same
    if (oldValue != value)
    {
        node->value = value;
        _update(heap, node, _should_move_up(heap, value, oldValue));
    }
}

// get root node of heap or 0 if heap is empty
static BinaryHeapNode* get_root(BinaryHeap* heap)
{
    BinaryHeapNode* result = 0;

    if (heap->count > 0)
    {
        result = heap->heap[0];
    }

    return result;
}

// swaps the root node of the two heaps
static void swap_root(BinaryHeap* heap, BinaryHeap* other_heap)
{
    BinaryHeapNode* our_root = get_root(heap);
    BinaryHeapNode* their_root = get_root(other_heap);

    if ((our_root != 0) && (their_root != 0))
    {
        BinaryHeapNode* our_root_temp = our_root;
        our_root = their_root;
        their_root = our_root_temp;
        our_root->heap = heap;
        their_root->heap = other_heap;
        heap->heap[0] = our_root;
        other_heap->heap[0] = their_root;

        // now fix up the heaps
        _update(heap, our_root, FALSE);
        _update(other_heap, their_root, FALSE);
    }
}

// updates the heap by moving the node up or down until heap property is satisfied
static void _update(BinaryHeap* heap, BinaryHeapNode* node, int should_move_up)
{
    int continue_swapping = TRUE;

    while(continue_swapping)
    {
        BinaryHeapNode* swap_with = should_move_up
            ? _get_swap_up(heap, node)
            : _get_swap_down(heap, node);
        if (swap_with != 0)
        {
            _swap_nodes(heap, node, swap_with);
        }
        else
        {
            continue_swapping = FALSE;
        }
    }
}

static BinaryHeapNode* _get_swap_up(BinaryHeap* heap, BinaryHeapNode* node)
{
    BinaryHeapNode* result = 0;

    BinaryHeapNode* parent = _get_parent(heap, node);
    if (parent != 0)
    {
        int should_swap = _should_move_up(heap, node->value, parent->value);
        if (should_swap)
        {
            result = parent;
        }
    }

    return result;
}

static BinaryHeapNode* _get_swap_down(BinaryHeap* heap, BinaryHeapNode* node)
{
    BinaryHeapNode* result = 0;

    BinaryHeapNode* candidate_child = _get_max_or_min_child(heap, node);
    if (candidate_child != 0)
    {
        int should_swap = _should_move_down(heap, node->value, candidate_child->value);
        if (should_swap)
        {
            result = candidate_child;
        }
    }

    return result;
}

static int _should_move_up(BinaryHeap* heap, int first_value, int second_value)
{
    int result;

    if (heap->is_max_heap)
    {
        result = (first_value > second_value);
    }
    else
    {
        result = (first_value < second_value);
    }

    return result;
}

static int _should_move_down(BinaryHeap* heap, int first_value, int second_value)
{
    int result;

    if (heap->is_max_heap)
    {
        result = (first_value < second_value);
    }
    else
    {
        result = (first_value > second_value);
    }

    return result;
}

static void _swap_nodes(BinaryHeap* heap, BinaryHeapNode* node, BinaryHeapNode* other_node)
{
    int old_index = node->index;
    int old_other_index = other_node->index;

    heap->heap[old_index] = other_node;
    other_node->index = old_index;
    heap->heap[old_other_index] = node;
    node->index = old_other_index;
}

static BinaryHeapNode* _get_parent(BinaryHeap* heap, BinaryHeapNode* node)
{
    BinaryHeapNode* result = 0;

    // if node is the root then it has no parent
    if (node->index > 0)
    {
        // even indexed nodes are always righthand children.
        int is_even_node = ((node->index % 2) == 0);
        int offset = is_even_node ? 1 : 0;
        int parent_index = (node->index - offset - 1) / 2;

        result = heap->heap[parent_index];
    }
    
    return result;
}

static BinaryHeapNode* _get_left_child(BinaryHeap* heap, BinaryHeapNode* node)
{
    BinaryHeapNode* result = 0;

    // odd indexed nodes are always lefthand children.
    // NOTE: Could have overflow here if the heap is huge
    int child_index = (node->index * 2) + 1;
    if (child_index < heap->capacity)
    {
        result = heap->heap[child_index];
    }

    return result;
}

static BinaryHeapNode* _get_right_child(BinaryHeap* heap, BinaryHeapNode* node)
{
    BinaryHeapNode* result = 0;

    // even indexed nodes are always righthand children.
    // NOTE: Could have overflow here if the heap is huge
    int child_index = (node->index * 2) + 2;
    if (child_index < heap->capacity)
    {
        result = heap->heap[child_index];
    }

    return result;
}

static BinaryHeapNode* _get_max_or_min_child(BinaryHeap* heap, BinaryHeapNode* node)
{
    BinaryHeapNode* result = 0;
    BinaryHeapNode* left_child = _get_left_child(heap, node);
    BinaryHeapNode* right_child = _get_right_child(heap, node);

    if (left_child == 0)
    {
        // right child may still be null, but thats ok, returning null indicates no children
        result = right_child;
    }
    else if (right_child == 0)
    {
        // left child may still be null, but thats ok, returning null indicates no children
        result = left_child;
    }
    else
    {
        // use the largest child for max heap and smallest for min heap.
        int use_left_child = _should_move_up(heap, left_child->value, right_child->value);
        result = use_left_child ? left_child : right_child;
    }

    return result;
}

// initialize moving median calculator
static void init_moving_median(MovingMedian* median, int max_samples)
{
    median->max_samples = max_samples;
    median->samples = (BinaryHeapNode**)calloc(max_samples, sizeof(BinaryHeapNode*));
    median->sample_index = 0;

    // if the sample size is odd, ensure the max heap has a size that is one greater
    int min_heap_capacity = max_samples / 2;
    int max_heap_capacity = min_heap_capacity;
    int is_odd = (max_samples % 2) == 1;
    if (is_odd)
    {
        max_heap_capacity += 1;
    }

    median->max_heap = (BinaryHeap*)malloc(sizeof(BinaryHeap));
    median->min_heap = (BinaryHeap*)malloc(sizeof(BinaryHeap));
    init_heap(median->max_heap, TRUE, max_heap_capacity);
    init_heap(median->min_heap, FALSE, min_heap_capacity);
}

static void destroy_moving_median(MovingMedian* median)
{
    free(median->samples);
    destroy_heap(median->max_heap);
    destroy_heap(median->min_heap);
    free(median->max_heap);
    free(median->min_heap);
}

// adds a new sample to the moving median calculation. if the sample size has reached the capacity,
// the oldest sample will be replaced.
static void add_sample(MovingMedian* median, int value)
{
    int num_samples = get_sample_count(median);
    BinaryHeap* min_heap = median->min_heap;
    BinaryHeap* max_heap = median->max_heap;

    if  (num_samples == median->max_samples)
    {
        // we have reached the max number of samples. replace the value in the oldest sample
        BinaryHeapNode* node_to_update = median->samples[median->sample_index];
        update(node_to_update->heap, node_to_update, value);
    }
    else
    {
        /* 
            we havent reached the max sample size yet.
            alternate between adding to min and max heap to keep the heaps the same size or
            the max heap one size larger to ensure the median value(s) are always stored in
            the roots.
        */
        int is_even = (num_samples % 2) == 0;
        BinaryHeap* heap_to_add = is_even ? max_heap : min_heap;
        BinaryHeapNode* new_node = add(heap_to_add, value);
        median->samples[median->sample_index] = new_node;
    }

    /*
        at this point, the min and max heaps both satisfy their heap property indepedently,
        but, it's possible the heap we added/updated the value in wasnt the correct heap to
        ensure the two middle values are at the heap roots. So swap the roots then fix up the
        heaps again, if necessary
    */
    BinaryHeapNode* min_heap_root = get_root(min_heap);
    BinaryHeapNode* max_heap_root = get_root(max_heap);
    if ((min_heap_root != 0) && (max_heap_root != 0))
    {
        // min heap root value should be greater than max heap root value
        if ((min_heap_root->value) < (max_heap_root->value))
        {
            swap_root(min_heap, max_heap);
        }
    }

    median->sample_index += 1;
    median->sample_index %= median->max_samples;
}

// returns twice the current median value. returns 0 if there are no samples yet
static int get_twice_median(MovingMedian* median)
{
    int num_max_samples = median->max_heap->count;
    int num_min_samples = median->min_heap->count;
    int result = 0;

    /* 
        The logic has been designed to
        1) keep the heaps the same size if the number of samples is even
        2) make the max heap one element larger if the number of samples is odd.
        when the number of samples is odd, the median value is stored in the root of the max
        heap and when the number of samples is even, the median value is the average of the
        values stored in the two heap roots.
    */
    if ((num_max_samples > 0) || (num_min_samples > 0))
    {
        BinaryHeapNode* max_heap_root = get_root(median->max_heap);
        int low_median = max_heap_root->value;
        int high_median = low_median;
        
        if (num_max_samples == num_min_samples)
        {
            BinaryHeapNode* min_heap_root = get_root(median->min_heap);
            high_median = min_heap_root->value;
        }

        result = low_median + high_median;
    }

    return result;
}

// returns current number of samples in the moving median calculation
static int get_sample_count(MovingMedian* median)
{
    return median->min_heap->count + median->max_heap->count;
}
