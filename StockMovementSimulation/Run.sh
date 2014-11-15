 #!/bin/bash

 gcc -o 1 BrownianStockMovement1.c -lm -fopenmp
 gcc -o 2 BrownianStockMovement2.c -lm -fopenmp
 gcc -o 3 BrownianStockMovement3.c -lm -fopenmp
./1
./2
./3
gcc -o RunSim Simulation.c -lglut -lGL -lGLU -lm
./RunSim
