"""Solves https://www.hackerrank.com/contests/coding-gym-mi0420/challenges/find-the-maximum-difference"""

from typing import TypeVar

Number = TypeVar("Number", int, float)

def get_max_difference(numbers: list[Number]) -> Number:
    """Returns the maximum possible difference between any two subsequent numbers in the list."""
    try:
        # initialize max difference to 0 (keeping the type of the first element)
        max_difference = numbers[0] - numbers[0]
    except IndexError as exc:
        raise ValueError("no numbers provided!") from exc

    min_number: Number | None = None  # minimum value observed so far

    for number in numbers:
        if min_number is None:
            # it takes two numbers to calculate a difference so on the first iteration just update
            # the minimum number observed so far
            min_number = number
        else:
            # the difference is the current number minus the minimum of all the previous numbers
            difference = number - min_number
            max_difference = max(max_difference, difference)
            min_number = min(min_number, number)

    return max_difference


if __name__ == "__main__":
    n = input().strip()
    vals = [int(x) for x in input().strip().split(" ")]
    print(get_max_difference(vals))
