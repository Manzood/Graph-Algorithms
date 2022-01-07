# Implementation of both Reverse-Delete algorithm and Boruvka's algorithm

Input:
    A graph with 9 vertices and 14 edges.
    The edges are described in subsequent lines.
    The edge weight for each edge is also specified in each line.

Output:
    The edges of the MST, along with the sum of all edges in the MST


Running instructions (Linux):

    - Reverse-Delete Algorithm:
        - To run, compile the file with g++ Reverse-Delete.cpp -o Reverse-Delete
        - Following this, run it with ./Reverse-Delete < Sample_Input > Sample_Output

    - Boruvka's algorithm:
        - Change the line in the main function (line 198) to:
            ``` bool testing_boruvka = true; ```
        - Now, follow the same steps to run:
            - To run, compile the file with g++ Reverse-Delete.cpp -o Reverse-Delete
            - Following this, run it with ./Reverse-Delete < Sample_Input > Sample_Output

