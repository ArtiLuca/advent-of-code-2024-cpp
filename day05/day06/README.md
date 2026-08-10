# Advent of Code 2024 - Day 6: Guard Gallivant

## Part 1
In Part 1 we are given as input a map of our surroundings, a map of a laboratory layout to track a single guard patrolling the area. The map is provided as a 2D grid where obstructed spaces are marked with `#` and empty spaces are marked with `.`. Our objective is to avoid the patrol route of the guard, whose starting position and initial orientation of the guard are indicated by a direction symbol: `^` (up), `>` (right), `v` (down), or `<` (left).

The patrolling guard follows a strict protocol at each step:  

 1. if there is an obstacle (`#`) directly in front of them, they turn 90 degrees right in place.
 2. otherwise, they take a single step forward in their current direction.

In order to avoid the patrolling guard, we want to simulate the guard's complete patrol route and determine the total number of distinct positions grid positions the guard visits before they step out of the mapped area (our 2D grid). The total number of distinct positions is Part 1's solution.

## Idea
Since we want to keep track of the positions visited by the guard without duplicating them, we can store visited locations as `(row,col)` coordinate pairs in a unique hash container, for example `std::set<std::pair<int,int>>` or `std::unordered_set<std::pair<int,int>>` in C++.
Since the simulation naturally terminates when the guard first falls out of the mapped area, adding extra padding to out grid is not necessary.

Assuming the `G[0...R, 0...C]` contains the 2D grid with our map read from input, we want to first scan the entire grid for the guard's starting position coordinates and map their symbol to an initial direction index using a `dir` variable (0 = up, 1 = right, 2 = down, 3 = left). We store this initial position in our `visited` container.  

We can simplify the simulation of the guard's movements using directional arrays:  

```text
dr = {-1, 0, 1, 0}
dc = {0, 1, 0, -1}
```  

At each step of the simulation, we determine the coordinates of the next position using `nr = r + dr[dir]` and `nc = c + dc[dir]`. We then have three cases:

* Case 1: if `(nr,nc)` falls outside of the mapped area we can stop the simulation as the guard has left the mapped area.  
* Case 2: if `G[nr][nc]` contains an obstacle (`#`) the guard does not move but turns in place 90 degrees to the right. We can simulate this using our direction index `dir = (dir + 1) % 4`.
* Case 3: if the path is empty (`.`) we update the guard's new position to `r = nr` and `c = nc`, and insert this new position into the `visited` container (under the assumption that it does not allow duplicate values to be inserted).

Once the simulation ends (because of Case 1) we can simply return the size of the `visited` container as Part 1's solution.


## Pseudocode

Assuming `G[0...R, 0...C]` contains the 2D grid with our map read from input, where `R` is the total number of rows and `C` is the total number of columns. And assuming that we use a container `visited` that does not allow duplicate values for storing the distinct positions the guard visits during his patrol.  

```text
solvePart1(G)

    CREATE visited as empty set of (int,int)

    // find starting position of guard
    for i=0 to R-1
        for j=0 to C-1
            if G[i,j] contains either '^', '>', 'v', '<'
                r = i
                c = j
                if G[i,j] == '^' then dir = 0
                if G[i,j] == '>' then dir = 1
                if G[i,j] == 'v' then dir = 2
                if G[i,j] == '<' then dir = 3
                // break

        // end inner for        
    // end outer for

    // insert initial position
    insert (r,c) into visited

    // simulate guard's patrol path

    dr = {-1, 0, 1, 0}
    dc = {0, 1, 0, -1}
    simulate = true

    while simulate == true
        nr = r + dr[dir]
        nc = c + dc[dir]

        // if next position is outside mapped area, simulation terminates
        if nr < 0 or nr >= R or nc < 0 or nc >= C
            simulate = false
            // break

        // if next position contains obstacle, turn right 90 degrees
        if G[nr,nc] == '#'
            dir = (dir + 1) % 4

        // otheriwse, path is clear, guard moves
        else
            r = nr
            c = nc
            insert (r,c) into visited

    // end while
    // once simulation stops, the size of our visited set is the solution
    return visited.size                
```

## Complexity    
The *Time Complexity* is $\mathcal{O}(R \times C \log(R \times C))$.
Scanning the grid has a linear cost relative to its area $\mathcal{O}(R \times C)$ and the simulation only traverses the grid partially. However, the insertion of unique elements into a standard tree container (e.g. `std::set`) has an average cost of $\mathcal{O}(\log N)$ per insertion, where $N$ is the number of visited positions.
The *Space Complexity* is $\mathcal{O}(R \times C)$ since we load and preserve the entire 2D grid from input.

## Part 2
In Part 2 we are told that we want to trap the guard in an infinite time loop by placing exactly one obstruction (`#`) somewhere on the 2D grid. We can place the obstruction anywhere on the map except for the guard's starting position.
Part 2's solution asks how many unique empty spaces across the 2D grid could cause the guard to become trapped in an infinite time loop if altered.

## Idea
We can use a brute-force approach by simulating the guard's movement after placing a single obstruction (`#`) somewhere on the grid. However, since we have the guard's patrol route from Part 1, running the simulation after placing an obstruction on any position that the guard does not actually visit wouldn't make sense.
Therefore, we can limit running our simulation only after placing an obstruction in a position that the guard actually visits during his normal patrol route found from Part 1.

For each position of guard's normal patrol route considered, we temporarily update it so that it contains an obstruction (`#`), reset the guard to their initial starting position/direction and then run the simulation.
Since we want to detect if a specific obstruction creates an infinite loop, we track both position and direction of the guard. To optimize the simulation and avoid massive memory allocation overhead, we flatten this state into a 1D boolean array `stateVisited` of size `R * C * 4` allocated just once in the main loop. For each simulation we run, we check the state table:
* if `stateVisited[idx]` is already `true` this means we have detected a cycle. Therefore, we update our global loop counter `infiniteLoopCount`, restore the original grid character, and move on to the next candidate position.
* if the guard manages to step out of the mapped area, then no loop was found, so we restore the original grid character and move on.

## Pseudocode

Assuming `G[0...R, 0...C]` contains our 2D grid read from input, and `initialGuard` contains the initial stored state (position and direction) of the guard, we can use a helper to simulate a loop:  


```text
simulateLoop(G, initialGuard, stateVisited)
    // create working copy from initial coordinates
    guard = initialGuard

    // helper to calculate 1D index
    getIndex(r, c, d) = (r * C * 4) + (c * 4) + d

    stateVisited[getIndex(guard.row, guard.col, guard.dir)] = true

    dr = {-1, 0, 1, 0}
    dc = {0, 1, 0, -1}

    while (true)
        nr = guard.row + dr[guard.dir]
        nc = guard.col + dc[guard.dir]

        // if guard leaves mapped area, no loop was found
        if nr < 0 or nr >= R or nc < 0 or nc >= C
            return false

        // if obstruction found, turn guard right 90 degrees
        if G[nr,nc] == '#'
            guard.dir = (guard.dir + 1) % 4

        // otherwise, move guard to next position he is facing    
        else
            guard.row = nr
            guard.col = nc    

        idx = getIndex(guard.row, guard.col, guard.dir)

        // if this exact position and direction was already logged, we found a loop
        if stateVisited[idx] == true
            return true

        // log current state
        stateVisited[idx] = true        
```

We can then use this helper to find Part 2's solution by running all possible simulations after placing a single obstruction in a position visited during the guard's normal patrol route (positions visited from Part 1) and keep a counter `infiniteLoopCount` to keep track of the total:  

```text  
solvePart2(G, initialGuard)
    // find all candidate positions from Part 1 (guard's normal patrol route)
    CREATE pathCandidates as empty set of (int,int)

    // run Part 1 simulation logic
    r = initialGuard.row
    c = initialGuard.col
    dir = initialGuard.dir
    dr = {-1, 0, 1, 0}
    dc = {0, 1, 0, -1}

    while (true)
        nr = r + dr[dir]
        nc = c + dc[dir]

        if nr < 0 or nr >= C or nc < 0 or nc >= 0
            // break
        if G[nr, nc] == '#'
            dir = (dir + 1) % 4
        else
            r = nr
            c = nc
            insert (r, c) into pathCandidates

    // remove starting position from candidate positions for obstruction placement
    REMOVE (initialGuard.row, initialGuard.col) from pathCandidates

    infiniteLoopCount = 0

    // allocate state array ONCE
    CREATE stateVisited as 1D boolean array of size (R * C * 4)

    // iterate and test candidate positions sequentially
    FOR EACH (r, c) in pathCandidates
        // place obstruction
        G[r,c] = '#'

        FILL stateVisited with false

        // run simulation with loop detection helper
        if simulateLoop(G, initialGuard, stateVisited) == true
            infiniteLoopCount = infiniteLoopCount + 1

        // restore grid to original
        G[r,c] = '.'

    return infiniteLoopCount
```    

## Complexity
The *Time Complexity* is $\mathcal{O}(V \times (R \times C))$, where $V$ is the total number of distinct positions visited by the guard in Part 1. In the absolute worst case, $V$ can scale up towards the maximum area limit of the map, $\mathcal{O}(R \times C)$, pushing the aggregate upper bound of the simulation routine to $\mathcal{O}((R \times C)^2)$.
The *Space Complexity* is $\mathcal{O}(R \times C)$ to maintain the original map allocation alongside our flat $R \times C \times 4$ boolean state array allocated just once for loop detection.
