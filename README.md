# Threaded-Binary-Search-Tree
THREADED BINARY SEARCH TREE

The aim of the project is:

-implement a binary search tree in a threaded manner
- to save space the program re-uses Right pointer as the thread when nullptr by adding  a boolean “isThreaded” field to every node.
	bool isThreaded returns true if threaded and false if not
- the program traverses the tree from top to bottom
-Threads are added during insertion. If new node is added to the left of its parent node P, then new node thread points to P. If the new node is added to the right of parent node, then new node inherits the thread of its parent.
-functions that enable traversal are begin() and next(). 
