# Solves: https://www.hackerrank.com/challenges/matrix-rotation-algo/problem?isFullScreen=true

from dataclasses import dataclass

@dataclass
class MatrixRing:
    row_min: int
    col_min: int
    row_max: int
    col_max: int

    def num_elements(self) -> int:
        rows = self.row_max - self.row_min + 1
        cols = self.col_max - self.col_min + 1
        return (rows * 2) + ((cols - 2) * 2)


def getNextCoordinate(row, col, ring: MatrixRing) -> tuple[int, int]:
    """Given an input coordinate, returns the next coordinate for one counter-clockwise
    rotation.
    """
    # assumes row and col are valid coordinates
    if (col == ring.col_min) and (row < ring.row_max):
        return (row + 1, col)
    elif (row == ring.row_max) and (col < ring.col_max):
        return (row, col + 1)
    elif (col == ring.col_max) and (row > ring.row_min):
        return (row - 1, col)
    else:
        return (row, col - 1)


def rotateRing(ring: MatrixRing, matrix, original_matrix, r):
    num_elements = ring.num_elements()
    r = r % num_elements  # normalize r to reduce number of operations
    
    if r != 0:
        # start with the top left element
        i, j = ring.row_min, ring.col_min
        new_i, new_j = i, j

        # get the new coordinate for the top left element
        for _ in range(r):
            new_i, new_j = getNextCoordinate(new_i, new_j, ring)

        # now perform the rotation on every element
        for _ in range(num_elements):
            matrix[new_i][new_j] = original_matrix[i][j]
            # move to next element
            i, j = getNextCoordinate(i, j, ring)
            new_i, new_j = getNextCoordinate(new_i, new_j, ring)


def matrixRotation(matrix, r):
    num_rows = len(matrix)
    num_cols = len(matrix[0])

    # make a copy of the original matrix
    original_matrix = [[y for y in x] for x in matrix]

    # rotate the outer ring of matrix first then move inward until done
    ring = MatrixRing(0, 0, num_rows - 1, num_cols - 1)
    while (ring.row_min < ring.row_max) and (ring.col_min < ring.col_max):
        rotateRing(ring, matrix, original_matrix, r)

        # move to the next ring
        ring.row_min += 1
        ring.col_min += 1
        ring.row_max -= 1
        ring.col_max -= 1
    
    for row in matrix:
        print(" ".join((str(x) for x in row)))
