# Ev3 Path Finder
A C program to find the path between two points in a grid and move the Ev3 robot from the start point to the end point while avoiding obstacles. 

## Steps to run 
1. Install C on Ev3 robot 
2. Start a new project in Eclipse and connect the Ev3 robot  
3. Clone, Fork, or download the files in this repository 
4. Enter the co-ordinates for the obstacles in obstacles.h
5. Enter the starting and goal points on pathFinder.c file 
6. Click run on the robot 

## Usage guidelines 
- The program uses random values to optimize the movement of ev3, please make sure that this value matches the configuration of your robot built 
- The program only supports the configuration of the test build therefore the timing for moving forward, backward, and making turns needs to be changed accordingly 

## User stories
- The Robot can be placed at any point in the grid  
- The goal and obstacles can also be placed at any points in the grid 

