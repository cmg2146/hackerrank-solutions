# https://www.hackerrank.com/challenges/journey-to-the-moon/problem?isFullScreen=true


from typing import Iterator, Self


class Graph:
    """A simple graph"""

    def __init__(self):
        self._nodes: set[int] = set()
        self._edges: dict[int, set[int]] = {}

    def size(self) -> int:
        return len(self._nodes)

    def add_node(self, node: int):
        self._nodes.add(node)
    
    def add_edge(self, node1: int, node2: int):
        node1_edges = self._edges.get(node1, set())
        node2_edges = self._edges.get(node2, set())
        node1_edges.add(node2)
        node2_edges.add(node1)
        self._edges[node1] = node1_edges
        self._edges[node2] = node2_edges

    def get_subgraphs(self) -> Iterator[Self]:
        """Yields all the complete (fully connected) subgraphs of this graph"""
        visited: set[int] = set()

        for node in self._nodes:
            # skip this node if it was already visited (part of another subgraph already yielded)
            if node in visited:
                continue
            
            # NOTE: The implementation here is incomplete - no edges are added to the yielded graph
            # because they are not needed to solve the problem (this would be bad design in a real
            # program)
            graph = Graph()
            [graph.add_node(x) for x in self._visit_all_connected_nodes(node, visited)]
            yield graph

    def _visit_all_connected_nodes(self, node: int, visited: set[int]) -> Iterator[int]:
        # use non-recursive traversal to avoid exceeding max recursion depth

        # use a stack to record the next nodes we should visit. initialize it to the specified node
        node_stack = [node]
        while node_stack:
            # pop and yield the top node from the stack, marking it as visited so we dont visit it
            # again
            cur_node = node_stack.pop()
            visited.add(cur_node)
            yield cur_node

            # visit all of the top node's connected nodes that we have yet to visit by pushing them
            # to the stack
            for cn in self._edges[cur_node]:
                if cn not in visited:
                    node_stack.append(cn)
            

def journeyToMoon(n, astronaut):
    # Write your code here
    
    # FIRST: build the incoming astronaut pairs into a graph
    graph = Graph()
    for pair in astronaut:
        a1, a2 = pair
        graph.add_node(a1)
        graph.add_node(a2)
        graph.add_edge(a1, a2)

    # THEN:
    # total pairs of different countries = total pairs - total pairs same country
    # calculating total pairs and total pairs same country are easy once we figure out the
    # number of astronauts in each country
    # formula for combinations of pairs = n! / (2 * (n - 2)!) = n * (n - 1) / 2 
    total_pairs = int(n * (n - 1) / 2)

    # each complete subgraph in the overall graph represents a group of astronauts in the same
    # country - the size of the subgraph is the number of astronauts in that country
    total_pairs_same_country = 0
    for country in graph.get_subgraphs():
        astronauts_in_country = country.size()
        total_pairs_same_country += int(astronauts_in_country * (astronauts_in_country - 1) / 2)
    
    return total_pairs - total_pairs_same_country
