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

#### Instructions
---
the initial state (starting PuzzleState namely start) is in depth 62 or a vey high one and it __only__ works out for improved version 1 that is far faster than others.  
to compare performance of two version you need:
1.  more simple example
2.  make sure to pass the same maxdepth to both IDDFS1 and IDDFS2 (in "main")
3.  comment/delete two line which improves version 1:
    -  line 83: `if(isRepeating_state(current_state)) continue;`
    -  line 86: `successors[count]->parent = &current_state;`
<br>

##### Analysis on maxdepth changes
---
Imagine the **Goal Depth** is 60. In this case:  

1. In less than 60, the entire tree must necessarily be traversed.
2. In 60 or more, depending on where its position within the depth of 60:
    - If the __leftmost__ node at depth 60, it shouldn't make much difference.
    - However, if it's the __rightmost__ node at depth 60, it can significantly differ.

Generally, increasing the maxdepth increases the search time. However, if maxdepth set more than Goal Depth, the increase has less effect on average cases  

###### next appliable updates: can be printing path especially in version 2
