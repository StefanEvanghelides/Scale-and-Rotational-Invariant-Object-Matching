# Scale and Rotational Invariant Object Matching

This Read-Me file contains the project description and the description of the code provided as solution to this problem, as well as the reasoning behind it. I will also use this file for Backlogging, including my breakdown of the tasks, estimated time and completion time. This will have the added benefit of learning to accurately estimate the time requried for different tasks/subtasks, an important skill needed for improving self efficiency in managing projects.

Superviser: Dr. Arnold Meijster

Starting Date: November 13 2017

Deadline Date: January  31 2018

---

# CHAPTER 1: Project Description
    
The aim of this project is to find a solution to the problem of pattern recognition when the comparable objects have different sizes and/or angles of rotation. This will be achieve by correlating a test object (mask) to an image containing multiple objects. In this case, direct correlation (pixel by pixel) is not possible as it will fail most of the times. Here the Matching Squares algorithm will be used to create isolines (i.e contour lines) of the objects, deriving mathematical functions whose values could be compared sensibly, even though the objects have different scales and angles of rotations.
    
    
# CHAPTER 2: Backlog

    1. Introduction - COMPLETED Nov 14 2017 - ESTIMATED Nov 17 2017
        o Reading PGM files - COMPLETED - Nov 13 2017
        o Writing PGM files - COMPLETED - Nov 14 2017
        
    2. Drawing the contour - ESTIMATED Nov 26 2017 
        o Research: Isoline algorithm and Marching Squares
        o Application: Drawing the contour of a given object (mask)
        o Application 2: Drawing the contour of all objects on an image
        
    3. Correlation
        o Same scale and rotation
        o Different scales
        o Different rotation degrees
        
    4. Testing
        o Rates of success/failure (current accuracy)
        o Improving accuracy
        o Statistical measures
