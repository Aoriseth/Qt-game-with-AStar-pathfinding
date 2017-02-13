# Interactive project using Astar pathfinding

Repository for master project industrial engineering.
This project was made using C++11 using QT-creator for developing the gui.


The main purpose of this project was to design an application where a protagonist would move around in a two dimensional environment.
Enemies and healthpacks would be generated at runtime and the protagonist would be able to find and eliminate these enemies using various pathfinding algorithms: BestFirst, BreadthFirst and A* (named Dijkstra at pathfinder class).
The algorithm should be visualized to display the workings of the pathfinding system in realtime.

In addition to the pathfinding there is a strategy in place that determines whether an enemy is a valid target by comparing two values.
The health will determine if he can defeat an enemy or has to search for a healthpack.
The energy will determine the distance the protagonist can travel and will be restored upon defeating an enemy.
The strategy was designed to kill as much enemies as possible based on the protagonist's remaining health and energy, as well as the pathfinding algorithm (A* is used in this strategy, see game.cpp). 
