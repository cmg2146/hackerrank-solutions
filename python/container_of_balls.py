# Solves: https://www.hackerrank.com/challenges/organizing-containers-of-balls/problem?isFullScreen=true


def organizingContainers(container):
    # Write your code here

    # The size of each container remains the same because only swap operations are involved.
    # so to check if the solution is possible or not, just check if there is a
    # combination of balls with the correct amount for each container.

    n = len(container)  # total number of containers/colors
    container_size = [0] * n  # the total number of balls in each container
    num_balls_by_color = [0] * n  # the total number of balls of each color

    for container_index, ball_list in enumerate(container):
        for color_index, amount in enumerate(ball_list):
            container_size[container_index] += amount
            num_balls_by_color[color_index] += amount

    # TODO: instead of sorting, a dictionary could be used for fast lookups
    container_size = sorted(container_size)
    num_balls_by_color = sorted(num_balls_by_color)

    return "Possible" if (container_size == num_balls_by_color) else "Impossible"
