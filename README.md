# Scale and Rotational Invariant Object Matching

This Read-Me file contains the project description and the description of the code provided as solution to this problem, as well as the reasoning behind it. I will also use this file for Backlogging, including my breakdown of the tasks, estimated time and completion time. This will have the added benefit of learning to accurately estimate the time requried for different tasks/subtasks, an important skill needed for improving self efficiency in managing projects.

Superviser: Dr. Arnold Meijster

Starting Date: November 13 2017

Deadline Date: January  31 2018

---

# CHAPTER 1: Project Description
    
The aim of this project is to find a solution to the problem of pattern recognition when the comparable objects have different sizes and/or angles of rotation. This will be achieve by correlating a reference object (mask) to an image containing multiple objects. In this case, direct correlation is not possible as it will fail most of the times when the two comparable objects have different sizes and/or angles of rotation.

A solution to this issue is to flatten a 2D object into a 1D object, which can then be compared to another 1D object to determine their similarity. The flattening will be done using the Marching Squares algorithm, which will be used to determine the angles of the edges involved in the contour of the object. Having a list of angles of the contour (i.e. the 1D representation of a 2D object), one can use such a list to compare it with another list of angles from another object (so, the 1D representation of another object). Thus, their level of similarity can be determined using the Pearson Correlation.
    
    
# CHAPTER 2: Backlog

    1. Introduction - ETA Nov 17 2017 - Completed on Nov 14 2017 
        o Reading PGM files - Completed on Nov 13 2017
        o Writing PGM files - Completed on Nov 14 2017
        
    2. Drawing the contour - ETA Nov 26 2017 - Completed on Dec 24 2017
        o Research: Isoline algorithm and Marching Squares - Completed on Dec 3 2017
        o Application: Drawing the contour of a given object (mask) - Completed on Dec 24 2017
        
    3. Correlation - ETA Jan 7 2018 - Completed Jan 19 2018
        o Same scale and rotation - Completed on Jan 18 2018
        o Different rotation degrees - Completed on Jan 18 2018
        o Different scales - Completed on Jan 19 2018
        
    4. Testing - ETA Jan 31 2018 - Completed Feb 4 2018
        o Automatization - Completed on Jan 22 2018
        o Plotting - Completed on Jan 24 2018
        o Inputs, Outputs, Bug checks - Completed on Jan 24 2018 + Updated on Feb 4 2018
        o Final results based on different inputs - Completed on Feb 4 2018
