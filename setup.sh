#!/bin/bash

# exit if any command fails
set -e

# update submodules
git submodule update --init --recursive

# set up FRUT
cd modules/FRUT/
rm -Rf build/
mkdir build && cd build/
cmake .. -DCMAKE_INSTALL_PREFIX=../prefix -DJUCE_ROOT=../../JUCE
cmake --build . --target install

# set up JUCE
# TODO
