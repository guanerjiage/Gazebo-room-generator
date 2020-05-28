#!/usr/bin/env python3
import matplotlib.pyplot as plt
import random
import copy
import numpy as np
import shape_list

####################################################################################################################
#   map_length: the height or width of the map
#   occupancy: percentage of shapes occupied the room
#   max_size_obstacle: the largest size of obstacles, the largest shape exist is 200*200
#   path_name: relative path to save the picture and picture name
#   random_seed: same seed to reproduce same picture and get new one from different seeds
####################################################################################################################


def room_generate(map_length=200, occupancy=0.4, max_size_obstacle=100, path_name="./haha.png", random_seed=1):
    # initialize some parameters
    bound = 100
    robot_size = 4
    map_size = map_length*map_length
    # initialize the room and calculate init occupancy with four walls
    room = np.ones((map_length, map_length))
    for i in range(map_length):
        room[i][0] = 1
        room[0][i] = 1
        room[i][map_length-1] = 1
        room[map_length-1][i] = 1
    current_occupancy = 4*(map_length-1)/map_size
    random.seed(random_seed)
    # count search time to bound infinite search cause of improper setting
    search = 0
    while current_occupancy <= occupancy and search < bound:
        search += 1
        x = random.randint(0, map_length-1)
        y = random.randint(0, map_length-1)
        s = random.randint(0, len(shape_list.shapeSize)-1)
        # check if the random shape violate the max obstacle size
        if shape_list.shapeSize[s][0] > max_size_obstacle or shape_list.shapeSize[s][1] > max_size_obstacle:
            continue
        # check if the obstacle fit the position that robot could walk through
        if check_validation(x, y, current_occupancy, occupancy, s, robot_size, map_length, room):
            # if the check is passed, put it in the room
            current_occupancy = (current_occupancy * map_size + shape_list.shapeSize[s][0] * shape_list.shapeSize[s][1])/map_size
            for i in range(shape_list.shapeSize[s][0]):
                for j in range(shape_list.shapeSize[s][1]):
                    if shape_list.shapeList[s][i][j] != 0:
                        room[x+i][y+j] = 0
    # save the room as a png file
    plt.imsave(path_name, room, cmap='gray')


def check_validation(x, y, curro, maxo, s, robot_size, map_height, room):
    # check if the there is path around the shape
    map_size = map_height * map_height
    t = copy.deepcopy(room)
    if x<robot_size or y<robot_size or x+shape_list.shapeSize[s][0]>map_height-robot_size or y+shape_list.shapeSize[s][1]>map_height-robot_size:
        return False
    # check if the occupancy beyond the max
    if (curro * map_size + shape_list.shapeSize[s][0] * shape_list.shapeSize[s][1])/map_size > maxo:
        return False
    # copy the shape in the room check for intersection
    for i in range(shape_list.shapeSize[s][0]):
        for j in range(shape_list.shapeSize[s][1]):
            if shape_list.shapeList[s][i][j] != 0:
                if t[x + i][y + j] == 0:
                    return False
                else:
                    t[x + i][y + j] = 0
    # check if the road way of robot still exist
    for i in range(x-robot_size, x+shape_list.shapeSize[s][0]+robot_size):
        for j in range(1, robot_size+1):
            if t[i][y-j] == 0 or (y+shape_list.shapeSize[s][1]-1+j < map_height and t[i][y+shape_list.shapeSize[s][1]-1+j] == 0):
                return False
    for i in range(y-robot_size, y+shape_list.shapeSize[s][1]+robot_size):
        for j in range(1, robot_size+1):
            if t[x-j][i] == 0 or (x+shape_list.shapeSize[s][0]-1+j < map_height and t[x+shape_list.shapeSize[s][0]-1+j][i] == 0):
                return False
    return True


room_generate(500, 0.6, 200, "../../Desktop/haha.png", 2)