#!/usr/bin/env bash
make
./Roomgenerator
cd auto_gazebo
python worldGenerator.py
gazebo 30*30_occu=20_2.world