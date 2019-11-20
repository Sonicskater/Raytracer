## README.md

Based on TA provided Code

program was modified to be mroe object oriented and used inheritance to simplify behaviour.

e.g. triangles are children of planes and use their intersection code for before checking if the  resultant intersection is withiin the tri.

I decided to genreate 1 complicated scene.
This scene contains 4 reflective sphers, 1 matte sphere, 1 reflective triangle and 1 reflective infinite plane.

I rendered it from 6 different directions, to demonstrate different shadows and reflections.

For instance, the sphere in the center of 1-4 has a hard line shadow cast from the green triangle up and behind the camera.

I also increased the reflectivenss of the scene for 1-recursive.png in order to better show the recursive reflections of the scene.
Further done to make it even more apparent in 1-extra-recursive.png

All objects in the scene use the same reflection ratio, If reflection is enabled. This is the case for all objects except for the dark red sphere.

The program can bbe run from command line with ./RayTracing_Simple <width> <height> in pixels, or with no args for 1000*1000.

## Compilation ##
navigate to the source directory, with CMakeLists.txt

Run:
cmake ./
make

The build will be located in the source directory on linux, or 
potentially in out/build/<target> if compiled on windows depending on your visual studio settings. 


## Other ##
Developed on WSL-Based Ubuntu using visual studio, and using the bash command line to make sure it is linux compatible c++.

tested on main lab machines

pics are dark due to my error, were brighter on home monitors.
