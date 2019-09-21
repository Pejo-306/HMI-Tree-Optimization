# HMI Tree Optimization

PoC (Proof of Concept) caching optimization algorithm for graphical tree
environment, written entirely in pure C++.

## Table of contents

* [Preface](#preface)
* [Problem description and solution](#problem-description-and-solution)
  - [The HMI tree structure](#the-hmi-tree-structure)
  - [Brief summary of the problem](#brief-summary-of-the-problem)
  - [Brief summary of the solution](#brief-summary-of-the-solution)
  - [Evaluating dirtiness](#evaluating-dirtiness)
    * [The count-min sketch (CMS) data structure](#the-count-min-sketch-cms-data-structure)
    * [The Heavy Hitters problem](#the-heavy-hitters-problem)
  - [Caching procedure](#caching-procedure)
    * [The cache table](#the-cache-table)
    * [Selection of caching candidates](#selection-of-caching-candidates)
    * [Refreshing the screen - preloading or rerendering](#refreshing-the-screen-preloading-or-rerendering)
    * [Cleaning up the cache](#cleaning-up-the-cache)
* [Implementaion details](#implementation-details)
  - [Project structure - brief rundown](#project-structure-brief-rundown)
* [Documentation](#documentation)
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

### Brief summary of the solution


