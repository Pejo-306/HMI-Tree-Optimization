# HMI Tree Optimization

PoC (Proof of Concept) caching optimization algorithm for graphical tree
environment, written entirely in pure C++.

## Table of contents

* [Preface](#preface)
* [Problem description and solution](#problem-description-and-solution)
  - [The HMI tree structure](#the-hmi-tree-structure)
  - [Brief summary of the problem](#brief-summary-of-the-problem)
  - [Solution](#solution)
    * [The cache table](#the-cache-table)
    * [Frequency counting](#frequency-counting)
    * [Tree traversal](#tree-traversal)
  - [Evaluating dirtiness](#evaluating-dirtiness)
    * [The count-min sketch (CMS) data structure](#the-count-min-sketch-cms-data-structure)
    * [The Approximate Heavy Hitters problem](#the-approximate-heavy-hitters-problem)
* [Implementaion details](#implementation-details)
  - [Project structure - brief rundown](#project-structure---brief-rundown)
  - [Unit tests](#unit-tests)
  - [C++ version](#c-version)
  - [Simulation](#simulation)
* [Documentation](#documentation)
* [Build and run](#build-and-run)
  - [Building the project](#building-the-project)
  - [Running](#running)
  - [User input](#user-input)
* [License](#license)

## Preface

After I had finished the 11th grade in high school, I had 2 weeks of compulsory
internship at a legitimate software company in Sofia, Bulgaria. The firm (which
I won't name in this document) where I spent my first two weeks of July 2019,
develop some of the embedded electronics, as well as its accompanied firmware,
for well-known car companies such as __Volkswagen__, __BMW__, and __Porsche__.
Furthermore, my 2 weeks of internship took place in the firm's HMI department.
The latter is tasked with developing the graphical software environment of 
various car models' dashboards.

During my two weeks of internship, I was tasked with developing a Proof of Concept
solution to a rather difficult algorithmic optimization problem (which is 
described below). In the span of this fortnight, I had gained an understanding
of the existing HMI graphical structure and had managed to come up with
a solution made up of various algorithms, adapted to my specific problem, and
had started to implement it in C++. In the following 2-3 weeks, I had fully 
developed and documented (see [Documentation](#documentation) section below) 
my solution.

This README attempts to explain the thought process that went into solving the
given optimization problem, the algorithms that were utilized to develop this
PoC, as well as various implementation details to ease readers of the C++ code.
I have decided to publish my solution in this repository for others, who may
also have their own algorithmic problems to solve, to possibly explore. There
might be some ideas from this project that you could utilize. If you choose 
to explore this project's code, know that it is definitely not of the highest
quality - most was written within 2-3 weeks and I also do not consider myself
an experienced C++ programmer. The section on [Implementation details](#implementation-details)
may aid any readers.

## Problem description and solution

This section describes the given optimization problem and my PoC solution.

### The HMI tree structure

The underlying graphical environment of a car's dashboard, which the HMI
department was developing, is a very complex mixed tree-like/graph-like data
structure. The nodes of this tree-like structure are the various graphical
elements of the car's dashboard. Some of them are standalone elements (e.g. LEDs,
letters, etc.), whilst others both have their intrinsic properties and can also 
contain other graphical nodes within. There are also reference nodes which act
as pointers to other types of nodes, essentially providing a way to copy some
part of the graphical dashboard and paste it to another location in the same
dashboard. This is why the HMI data structure is also graph-like. However, 
these reference nodes are not taken into account within the context of the 
presented solution and will not be explored further in this document.

Essentially, the properties of the aforementioned graphical environment which 
are of most import for the presented solution can be summarized 
with the few following points:
- the graphical data structure is considered a pure tree data structure with
only 1 root node (the 'View' node) and an arbitrary amount of branches and leaf nodes;
- container nodes are henceforth also referenced as 'widgets'; these nodes do
not hold any other kind of properties but can still be updated (see below);
- leaf nodes are represented by nodes which hold some arbitrary information
(such as text) which does not affect the optimization algorithm itself;
- the dashboard occasionally refreshes its screen, after which it must
redraw all of its elements;
- there is no fixed refresh rate - the details of when this process takes place
are of no significance to the presented solution algorithm;
- any element's state may have been altered between two screen refreshes - this
process is considered as an update to the altered node; if a node has been
updated, it is marked as __dirty__ until the screen has been refreshed;
- it does __not__ matter how many times a node is updated between two frames
(two screen refreshes) - as long as it has been altered at least once it is
considered __dirty__; there aren't nodes which are considered _more_ __dirty__
than others in the span of two frame refreshes.

Considering everything said, a very simplistic exemplary HMI graphical tree
is given below:

```
V│
 └──circle
 └──W│
 │   └──LED
 └──W│
     └──W│
     │   └──square
     │   └──letter
     │   └──letter
     │   └──letter
     └──LED
     └──LED
     └──letter
     └──LED
```

The 'view' node acts as the tree's root node which has a few branches. Some
of them lead to 'widget' nodes which contain other types of nodes including
other 'widget' nodes. Leaf nodes come in many different variations such as
LEDs, letters, etc.

### Brief summary of the problem

The previously presented example tree may mislead readers to believe that
such a graphical data structure is fairly simple. In reality, it is much more
complex and contains hundreds of nodes. A particular optimization problem
arises when refreshing the screen - the dashboard system must rerender and
redraw every single element of the tree. There is no caching mechanism in
place to optimize this refreshing process. Since a tree is made up of hundreds
of nodes and the rerendering process is rather slow, refreshing the dashboard
screen leads to a substantial drop in performance and noticeable latency.
Moreover, the (average) screen refresh rate cannot be lowered as some information
displayed on the dashboard requires frequent reevaluation.

The following logical observation may be made on the graphical tree data 
structure: some items are updated very frequently, while others are changed
every so often and remain almost static. For example, the elements representing
the car's current speed must be updated almost every screen refresh, whilst
the currently playing radio's name may remain unchanged for a long amount of time.
Furthermore, it should be noted that there are facilities present to cache nodes 
inside the system's memory. By caching container nodes ('widgets') the states of
all of its children will also be saved in one chunk of memory alongside their
container node. Therefore, caching a widget may be viewed upon as a partial 
screenshot of the dashboard which may quickly and directly be redrawn when 
refreshing the screen.

Infrequently altered elements may be considered static. The problem then can be
solved by developing an optimization algorithm which dynamically determines
which nodes may be considered _static_. Once the whole tree has been evaluated
for _static_ nodes, the algorithm decides to cache the biggest static branches,
closest to the root node. Therefore, only the most shallow nodes would need
to be rerendered on-screen refresh while the cached static branches of the
tree are directly pasted (redrawn) on the screen. This way, a lot of the
rendering work is made obsolete and the performance of the system increases
substantially.

### Solution

The optimization algorithm presented has 2 aspects to its operation.

#### The cache table

The ultimate goal of the optimization algorithm can be considered the management
of a _cache table_ - a mapping between a node's unique identifier and a
pointer/reference to the same node's cache entry in memory.

#### Frequency counting

The algorithm constantly keeps count of how many times a node has been made
__dirty__ during the lifetime of the system. It does __not__ keep count of how
many times a node has been updated - as long as it has altered its state at
least once, the node is considered __dirty__. The update frequency of all nodes
is kept within a count-min sketch data structure (which is discussed later in
[The count-min sketch (CMS) data structure](#the-count-min-sketch-cms-data-structure)).

For the purposes of this solution, some additional terminology is used:
- a _very dirty_ node is one which the algorithm has decided that it is updated
very frequently; therefore it is not considered suitable for caching and is
rerendered on every screen refresh; a container node is also automatically
considered _very dirty_ if any of its _direct_ or _indirect_ children is
also _very dirty_;
- a _very clean_ node is one which is updated rather seldom in comparison to
other nodes; it is essentially the opposite of a _very dirty_ node and is thus
considered suitable for caching.

Every time the screen refreshes the solution has to decide which nodes should
be marked as _very dirty_ and the others - as _very clean_. To achieve this,
the _Heavy Hitters_ algorithm (that is discussed later in the section 
[The Approximate Heavy Hitters problem](#the-approximate-heavy-hitters-problem))
is utilized to mark a number of nodes as _very dirty_ whereas all others are 
marked as _very clean_.

#### Tree traversal

As explained in the last section, the optimization algorithm has the capability
to consider which nodes are suitable for caching. On every screen refresh, the
tree structure is traversed twice - once with a _DFS_ (depth-first search) and
the second time with a _BFS_ (breadth-first search).

The first traversal of the tree uses _DFS_ to clear the __dirty__ flag (if the
node has been updated since the last refresh), as well as mark each node as
either _very dirty_ or _very clean_. The marking decision is made based on 
frequency counting as elaborated on in the above subsection. The traversal 
method used here must be a _DFS_ to properly mark container nodes based on the 
marking of their children nodes. Therefore, this _DFS_ _must_ traverse the 
entire tree data structure.

The second tree traversal utilizes a _BFS_ to decide which nodes to cache,
which - to rerender, and which - to load from the cache. The benefit of using
a _BFS_ traversal here is that the latter naturally reaches the uppermost (closest
to the root) _very clean_ nodes first. It is thus obsolete to traverse a
_very clean_ node's children, as the whole container's branch must be _very clean_
for the widget itself to be _very clean_. The entire branch may then be cached.
This is why the _BFS_ algorithm traverses the entire tree only in the worst-case
scenario - when all the nodes are non-cacheable. In practice, the _BFS_ algorithm
will always partially execute to achieve the goals of the solution.

### Evaluating dirtiness

This section goes into more detail on the frequency counting facilities of the
optimization algorithm.

As already discussed the solution utilizes the _Approximate Heavy Hitters_ 
algorithm to determine the non-cacheable nodes. An excellent paper on this
topic can be found [here](http://timroughgarden.org/s17/l/l2.pdf).

#### The count-min sketch (CMS) data structure

This particular data structure is used by the _Approximate Heavy Hitters_
algorithm. The data structure is comprised of _b_ buckets and _l_ hash
functions and supports two operations - _Inc(x)_ and _Count(x)_. The structure
itself is a 2D array of _b_ x _l_ counters, initially set to _0_. The _Inc(x)_
operation increments the counters, located at every one of the _l_ hash functions'
results. Since a hash function has a finite amount of values for its result
(i.e. an index to one of the buckets) collisions between counted elements will
occur. However, since counters are never decremented, any one counter can only
overestimate the frequency occurrence of any one item. Therefore, the _Count(x)_
operation just returns the smallest value of any the _l_ counters, pointed to
by the computed values of the hash functions for the element _x_. This method
of counting occurrence obviously produces errors but is independent of the number
of elements which are tracked and therefore takes up a substantially smaller
amount of memory.

The linked paper more thoroughly elaborates on the CMS. The most important
takeaway points are left here as a list:
- _k_ - user-defined parameter; the maximum allowed amount of heavy hitters;
- _δ_ - the allowable error probability; set by the user;
- _ε_ - user-defined parameter; read the paper for more details; value is _1 / 2k_;
- _b_ - number of buckets; equal to _e / ε_;
- _l_ - number of hash functions; equal to _ceil(ln(1/δ))_;
In this project each hash function is generated via the following formula:
```
h = ((ax + b) mod p) mod b

where:
  a - random non-negative integer;
  b - random positive integer;
  p - random prime number
  b - number of buckets
```
These are only used to deliver a PoC solution. Other families of hash
functions may be more suitable for the purposes of the optimization algorithm,
however, they are not explored in this document.

#### The _Approximate Heavy Hitters_ problem

As previously mentioned, the _Approximate Heavy Hitters_ algorithm uses a
CMS to count the update frequency of all nodes. Since the optimization algorithm
runs in real-time, without knowing in advance the number of times each node will
be updated for the lifetime of the system, this algorithm has to run every time
the screen refreshes and needs to keep track of the number of times nodes have
collectively been marked __dirty__ - a variable denoted by _m_. Therefore, all
heavy hitters are considered nodes which have been __dirty__ at least _m / k_
times since the system started and are marked consequently as _very dirty_.

If implemented as stated above, however, a possible problem occurs. Remember
that a node's update frequency is actually the number of times it has been marked
__dirty__. Thus, its frequency counters (in the CMS) can only be increased by
1 between 2 frame refreshes. If its update frequency (theoretically assuming 
it is 100% accurate) reaches the required value of _m / k_ in a given frame and
the node has not been immediately updated in the next frame, whilst any other
node has been (i.e. the value of _m_ has increased), then the same node's update
frequency will fall below the required _m / k_ value to be considered a heavy 
hitter. This would result in unsatisfactory results where some nodes repeatedly
change states between being _very dirty_ and _very clean_. To adapt this 
algorithm to this project's problem, an additional user-defined parameter is 
required - the _leeway_ (in the range 0-1). In order for a heavy hitter to be 
'cleaned' and marked _very clean_ again, it must have been made __dirty__ less 
than _(1 + leeway) * m / k_ times. This ensures that one newly marked heavy
hitter will continue to be considered as such for several consecutive frames
even if it isn't updated.

## Implementation details

This section of the README document is meant for people who are interested in
exploring the project's code. It does not elaborate additionally on the
execution and reasoning behind the optimization algorithm.

### Project structure - brief rundown

The following output (partially modified) is produced by running the command
Linux command _'tree'_ in the project's root directory:

```
.<project root directory>
├── bin
│   ├── solution
│   └── test
│       └── ...<output ommitted>
├── docs
│   └── doxygen
│       └── ...<output ommitted>
├── Doxyfile
├── include
│   ├── catch2
│   │   └── catch.hpp
│   ├── heavy_hitters
│   │   └── ...<output ommitted>
│   ├── lib
│   ├── solution
│   │   └── ...<output ommitted>
│   ├── std_helper
│   │   └── ...<output ommitted>
│   ├── __test
│   │   └── ...<output ommitted>
│   └── tree
│       └── ...<output ommitted>
├── lib
├── LICENSE
├── Makefile
├── README.md
├── src
│   ├── heavy_hitters
│   │   ├── ...<output ommitted>
│   │   └── module.mk
│   ├── solution
│   │   ├── ...<output ommitted>
│   │   ├── main.cc
│   │   ├── module.mk
│   │   └── target.mk
│   ├── std_helper
│   │   ├── module.mk
│   │   └── ...<output ommitted>
│   ├── __test
│   │   └── ...<output ommitted>
│   └── tree
│       ├── module.mk
│       └── ...<output ommitted>
├── test
│   └── ...<output ommitted>
└── tmp
    ├── inputs
    │   ├── input0.txt
    │   ├── input1.txt
    │   └── input2.txt
    └── obj
        └── ...<output ommitted>
```

The entire project is managed via the Make utility. The project's root contains
the main _'Makefile'_ which may be used to build the project (see 
[Build and run](#build-and-run)). The project also contains some
other important files such as _'LICENSE'_, _'README\.md'_ and _'Doxyfile'_.

The C++ project is split into so-called _'modules'_. They are meant to separate
the project's code into smaller, more manageable and logically-linked pieces. 
Instructions to compile each module are found within the various smaller
make files (with extension '.mk'). Each module has its own source code files,
header files, as well as unit tests and are namespaced appropriately within the
C++ code.
This project has the following modules:
- _'heavy hitters'_: defines the CMS structure;
- _'std\_helper'_: functionality to more easily interface with some parts of
C++'s standard library;
- _'tree'_: defines a simulated stripped-down tree data structure to represent
the real HMI graphical environment;
- _'solution'_: this module holds the program's 'main' function and produces
an executable to run;
- _'\_\_test'_: ignore this module (see [Unit tests](#unit-tests)).
As mentioned above the _'solution'_ module utilizes all other modules to
compile the entire program and produce an executable, named after the same
module, for use.

The project's file structure is summarized in the following list:
- __'bin/'__: project's produced executables;
- __'src/'__: project's C++ source code (separated in modules);
- __'include/'__: project's C++ header files (separated in modules);
- __'lib/'__: third party C++ libraries/code should be stored here; since this
project does not use any such code this folder should be ignored;
- __'test/'__: project's unit tests (separated in modules); ignore this folder
(see [Unit tests](#unit-tests));
- __'docs/'__: files, related to project documentation (see [Documentation](#documentation));
- __'tmp/'__: various temporary files;
  - __'tmp/obj/'__: compiled object files from the project's source code;
  - __'tmp/inputs/'__: exemplary user input (see [Build and run](#build-and-run));
  
### Unit tests

The project's structure has facilities to write unit tests with the 
[Catch2](https://github.com/catchorg/Catch2) C++ testing framework. However,
no unit tests have been written for this project and all files and folders
related to unit testing should be ignored.

### C++ version

This project is written in modern C++, i.e. the code must be compiled with
the __C++11__ standard or above.

### Simulation

This project is only a Proof of Concept build - this is neither the final 
solution to the presented optimization problem nor is the algorithm 
run on the real HMI data structure. In a way, the project can be viewed on 
as a simulation. The HMI data structure is represented by a stripped-down
tree with an arbitrary amount of nodes. The latter only have unique ids and
various flags, indicating the dirtiness of said node, as well as some arbitrary
insignificant information (for leaf nodes). All graphical processes such as
rendering, drawing, caching, retrieving from the cache, etc. are purely 
demonstrative (e.g. the rendering process is simulated via a deliberate time 
delay).

## Documentation

This project's C++ source code is documented with [Doxygen](http://www.doxygen.nl/).
Most of the files are fully documented.

To generate the Doxygen documentation (from the project's root directory):

```bash
$ doxygen
```

Then, open the _'index.html'_ file within the __'docs/doxygen/html/'__ folder
(with Firefox, for example):

```bash
$ firefox docs/doxygen/html/index.html
```

## Build and run

This section has instructions on building and running the project with
exemplary user input.

### Building the project

First of all, clone the GitHub project:

```bash
$ git clone https://github.com/Pejo-306/HMI-Tree-Optimization.git
$ cd HMI-Tree-Optimization/
```

The Make utility is used to compile the source code and build the project.
You can fully build it like so:

```bash
$ make
```

After that, an executable named _'solution'_ will be generated in the __'bin/'__
directory which can be run (see below).

In case you wish to delete all project binaries and compiled object files, the
following command could be used:

```bash
$ make clean
```

### Running

After the project has been built, it can be executed from the command line with
a number of compulsory user-defined paramters:

```bash
$ ./bin/solution {0|1-debug} {k} {δ} {leeway}

where
  debug - set to 1 to display additional debugging information;
  k - maximum number of heavy hitters;
  δ - the allowed error probability (in the range 0-1);
  leeway - in the range 0-1.
```

This project comes with a few exemplary user input files, located in the 
__'tmp/inputs/'__ folder. Then, the program can be executed like so:

```bash
$ ./bin/solution 1 3 0.01 0.01 < tmp/inputs/input1.txt
```

### User input

The program accepts input from STDIN to build a simulation tree and perform
various operations on it. The reader is advised to take a look at any one of
the provided exemplary user input files.


#### Building a tree

First, the program reads an integral value _n_. The latter determines the number
of tree nodes (excluding the readily available root node) that the
tree has. Afterward, the next _n_ lines of input define each tree node, one
line per node. Each line is in CSV format:

```
{parent id},{type},{id},[arg1,arg2...]

where
  parent_id - this node's parent's id; integer;
  type - node type as a single letter; currently the only available types 
         are: 'W' - widget, 'T' - text;
  id - this node's unique id; integer;
  [arg1,arg2...] - additional arguments, required for node construction;
                   depends on the node's type.
```

For example, to create a text node which is directly connected to the _root_,
has an id of _'33'_ and retains the text _'Hello World'_:

```
<input ommitted>
0,T,33,Hello World
<input ommitted>
```

#### Operations

There are several operations implemented which are meant to simulate the real
HMI graphical environment with only the features that are of most import to
the optimization algorithm. The commands are inputted one per line and the
available ones are:
- _print_: displays the current state of the simulated tree. Each node is
displayed by its type (represented as a single character) and its unique id.
There are also two other symbols which may appear next to the node's type letter:
_'*'_ if the node is __dirty__; _'%'_ if the node is _very dirty_. Some exemplary
output from the _print_ command is provided below:

```
%V│0
 └──T│13
 └──W│11
 │    └──T│21
 └──%W│12
 │    └──%W│35
 │    │    └──T│44
 │    │    └──T│43
 │    │    └──T│41
 │    │    └──%*T│42
 │    └──T│34
 │    └──T│33
 │    └──T│31
 │    └──T│32
```

- _refresh_: simulates a screen refresh. As previously discussed, the optimization
algorithm runs when the screen is refreshed.
- update: this command issues and update to a node. The command itself is in
the following CSV format:

```
{id},[arg1,arg2...]

where
  id - unique id of updated node; integer;
  [arg1,arg2...] - additional arguments, required to update the node; depends
                   on node's type.
```

For example, to update the text node with id _'33'_ by replacing its text content
with _'dirty'_:

```
<input ommitted>
33,dirty
<input ommitted>
```

- _end_: exits the program.

## License

This project is distributed under the [MIT license](LICENSE)

