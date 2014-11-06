#!/usr/bin/env bash

# args build_directory build_type
# check to see if ClangOverrides exists
# cmake -DCMAKE_USER_MAKE_RULES_OVERRIDE=ClangOverrides.txt -DCMAKE_BUILD_TYPE=Debug ..

# USAGE="Usage: $0 <build_directory> <cmake_build_type>"
SRC_DIR=$(pwd)
BUILD_DIR=$1
BUILD_TYPE=$2

function usage {
    usage=()
    if [ "$#" == "2" ]; then
        usage+=( 'ERROR:' "${2}" )
    fi
    usage+=( 'Usage:' "${1}" '<build_directory>' '<cmake_build_type>' )
    echo "${usage[@]}"
    exit 1
}

function invoke_cmake {
    # make directory if it doesn't exist and change into aka. take
    mkdir -p "${BUILD_DIR}"
    cd "${BUILD_DIR}"

    options=()
    if [ "$#" == "1" ]; then # ClangOverride is $1
        options+=( "-DCMAKE_USER_MAKE_RULES_OVERRIDE=${1}" )
    fi
    options+=( "-DCMAKE_BUILD_TYPE=${BUILD_TYPE}" )
    echo "${options[@]}"
    command=( cmake "${options[@]}" "${SRC_DIR}" )    
    "${command[@]}"
}

if [ "$#" == "0" ]; then
    usage "${0}"
fi
if [ "$#" != "2" ]; then
    usage "${0}" "illegal argument(s):"
fi

# does ClangOverrides.txt exist?
if [[ -e "ClangOverrides.txt" ]]; then
    invoke_cmake "ClangOverrides.txt"
else
    invoke_cmake
fi


