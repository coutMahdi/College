# Two Versions of IDDFS
## AIMA: Artificial Intelligence a Modern Approach
### Part Ⅱ Problem-solving :: 3. Solving Problems By Searching
#### Exercise 20
Implement two versions of the function for the 8-puzzle: one that copies and edits the data structure for the parent node s
and one that modifies the parent state directly (undoing the modifications as needed). Write versions of iterative deepening depth-first search that use these functions and compare their performance.
### Explanation of my Solution
---
- V1: Creates Tree using **"new"** keyword  
- V2: Only has Parent node and does not create tree (more memory-efficient)
  
the version 2 is a better one in terms of both memory and time  
  
instresting edition: adding two line to V1 make it far better  
--> checking Repeating of childs to not be made if it already exists in its ancestors/parents by treaversing up to root with the help of **parent object pointer** in struct PuzzleState.  
  
V3: it is more like version 1 in syntax but it doesn't use **pointer array** and **new** and use just *object array*. it is as fast as version 2 (even a little faster) but maybe not standard
