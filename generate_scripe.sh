#!/usr/bin/env bash
make
./Roomgenerator
cd auto_gazebo
python worldGenerator.py
gazebo output.world