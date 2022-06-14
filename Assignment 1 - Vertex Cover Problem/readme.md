**Vertex Cover problem** -
The idea is for the police to be able to minimize the number of cameras they need to install, and still be as effective as possible with their monitoring.

There are 4 kinds of commands -
(1) add a street (add), (2) modify a street(mod), (3) remove a street(rm), and, (4) generate a graph (gg)

Output Graph -
1. There is a vertex corresponding to: (a) each intersection, and, (b) the end-point of a line segment of a street that intersects with another street.
2. There is an edge between two vertices if: (a) at least one of them is an intersection, (b) both
lie on the same street, and, (c) one is reachable from the other without traversing another vertex.
