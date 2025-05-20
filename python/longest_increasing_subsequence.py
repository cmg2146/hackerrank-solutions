# Solves: https://www.hackerrank.com/challenges/longest-increasing-subsequent/problem?isFullScreen=true


def replace_element(sorted_arr: list[int], element: int):
    # use a binary search to find the index in the sorted array to replace the new element with
    lo = 0
    hi = len(sorted_arr) - 1
    while hi > lo:
        index = lo + ((hi - lo) // 2)
        val = sorted_arr[index]
        if val < element:
            lo = index + 1
        else:
            hi = index

    # now replace the smallest item that is greater than the element
    sorted_arr[lo] = element


def longestIncreasingSubsequence(arr: list[int]) -> int:
    # assumes arr has at least one element

    # initialization: the first element is currently the longest sequence
    longest = [arr[0]]

    for i in range(1, len(arr)):
        val = arr[i]
        if val > longest[-1]:
            # if element is greater than largest item currently in the longest sequence then append it
            # to increase the longest sequence by 1
            longest.append(val)
        else:
            # otherwise, replace the smallest item in the longest sequence that is greater than the element.
            replace_element(longest, val)

    return len(longest)


def longest_increasing_subsequence_n_squared(arr: list[int]) -> int:
    num_elements = len(arr)

    # initialization: we know each element can form a sequence of length 1
    longest = [1] * num_elements

    # work from the end of the array, determining the longest subsequence from each element to end of array
    i = num_elements - 2
    while i >= 0:
        j = i + 1
        while j < num_elements:
            # calculate the length of the longest subsequence starting at element "i" and continuing
            # with element j (add 1 for current element "i")
            new_longest = longest[j] + 1

            # update the current longest length for element i if the value at j is greater and the
            # longest subsequence continuing through j is longer
            if (arr[j] > arr[i]) and (new_longest > longest[i]):
                longest[i] = new_longest
            j += 1
        i -= 1

    return max(longest)
