# Roomgenerator
Generate random room in gazebo with Ros massage for testing.


## Structure
This work contains both random maps with certain size and occupancy and two realistic maps reconstructe follow the outlay of the library.
The random maps only happens in the size size 20, 40 and 80. And the resolution is fixed as 10, which means
100 as array size 10 meter in the gazebo world file, each value in array represent a block with 10cm*10cm.
Each map contains
* Gazebo .world file (could be open using command gazebo *.world).
* Array contain 100 and 0 (100 as occupied and 0 as free). It could be use as Ros massage in Ros structure. 

## Random map generate flow
1. In "main.cpp", modify the parameters defined on top of the code.
2. It will generate the array contain 100 and 0 that randomly putting the shapes defined in the shape list into the map.
3. The array is saved in "maparray.txt"
4. It will also constructe a generator list in "generatelist.txt" that instruct the generation of .world file.
5. In the dirctory auto_gazebo, running the script "worldGenerator.py" will automaticlly generate the world file.
6. One thing is the init.world need also be changed once we change the size of the map in "main.cpp" .
7. Running the generate_scripe.sh will automaticlly go through this procedure.

## Current exist maps
* In /bib_eg and /bib_og, the world files similar to the jpg picture of MI-library layouts.
* In /random_maps, maps with three different size and three different occupancy.
