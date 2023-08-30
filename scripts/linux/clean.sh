#!/bin/bash

source config.sh

# Remove the binary directory.
if [ -d $KBHELL_BIN_DIR ]
then
    echo "removing '$KBHELL_BIN_DIR'"
    rm -r $KBHELL_BIN_DIR
fi

# Remove the CMake build directory.
if [ -d $KBHELL_BUILD_DIR ]
then
    echo "removing '$KBHELL_BUILD_DIR'"
    rm -r $KBHELL_BUILD_DIR
fi
