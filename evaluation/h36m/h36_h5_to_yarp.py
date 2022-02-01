#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Copyright (C) 2021 Event-driven Perception for Robotics
Author: Franco Di Pietro 

LICENSE GOES HERE
"""

# %% Preliminaries
import numpy as np
import os
import sys
import h5py
from tqdm import tqdm

# Load env variables set on .bashrc
bimvee_path = os.environ.get('BIMVEE_PATH')
# Add local paths
sys.path.insert(0, bimvee_path)

from bimvee import exportIitYarp


# Directory with 
datadir = '/data/h36m/events/'
outputdir = '/data/h36m/yarp/'
# datadir = '/home/fdipietro/hpe-data/h36_server/'
dir_list = os.listdir(datadir)
if 'yarp' in dir_list: dir_list.remove('yarp')


# %% Import/Export 
for i in tqdm(range(len(dir_list))):
    filename = datadir + dir_list[i] + '/Directions.h5'
    print('\nProcessing '+filename)
    hf = h5py.File(filename, 'r')
    data = np.array(hf["events"][:]) #dataset_name is same as hdf5 object name 
    container = {}
    fileExport = outputdir + dir_list[i]
    container['info'] = {}
    container['info']['filePathOrName'] = fileExport 
    container['data'] = {}
    container['data']['ch0'] = {}
    container['data']['ch0']['dvs'] = {}
    container['data']['ch0']['dvs']['ts'] = (data[:,0]-data[0,0])*1e-6
    container['data']['ch0']['dvs']['x'] = data[:,1]
    container['data']['ch0']['dvs']['y'] = data[:,2]
    container['data']['ch0']['dvs']['pol'] = data[:,3].astype(bool)
    exportIitYarp.exportIitYarp(container, exportFilePath=fileExport, protectedWrite=False)
