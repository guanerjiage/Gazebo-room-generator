#!/usr/bin/env bash
make
./Roomgenerator
cd auto_gazebo
python worldGenerator.py
gazebo 80*80_occu=30_1.world