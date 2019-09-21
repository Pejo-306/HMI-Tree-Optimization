# HMI Tree Optimization

PoC (Proof of Concept) caching optimization algorithm for graphical tree
environment, written entirely in pure C++.

## Table of contents

* [Preface](#preface)
* [Problem description and solution](#problem-description-and-solution)
  - [The HMI tree structure](#the-hmi-tree-structure)
  - [Brief summary of problem](#brief-summary-of-problem)
  - [Brief summary of solution](#brief-summary-of-solution)
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
for well-known car companies such as **Volkswagen**, **BMW**, and **Porsche**.
Furthermore, my 2 weeks of internship took place in the firm's HMI department.
The latter is tasked with developing the graphical software environment of 
various car models' dashboards.

During my two weeks of internship, I was tasked with developing a Proof of Concept
solution to a rather difficult algorithmic optimization problem (which is 
described below). In the span of this fortnight, I had managed to come up with
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

