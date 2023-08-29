#!/bin/bash

CWD=$(pwd)

# Root directory.
KBHELL_PROJECT_PATH=$(dirname ${CWD})

# Scripts directory.
KBHELL_SCRIPTS_PATH="${KBHELL_PROJECT_PATH}/scripts"

# Binary directory.
KBHELL_BIN_DIR="${KBHELL_PROJECT_PATH}/bin"

# Resource directory.
KBHELL_RES_DIR="${KBHELL_PROJECT_PATH}/resources"

# CMake build files and cache.
KBHELL_BUILD_DIR="${KBHELL_PROJECT_PATH}/build"
