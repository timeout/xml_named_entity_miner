#!/usr/bin/env bash

# check to see that xmltest.zip exists
# usage: prep_tests.sh <build_dir>

function usage {
    usage=() 
    usage+=( 'Usage:' "${1}" '<build_directory>' )
    echo "${usage[@]}"
    exit 1
}

function create_dummy_directory {
    mkdir -pv "${1}"/tests/dummy_directory/sub_directory_{1,2}
    touch "${1}"/tests/dummy_directory/file_{1,2,3}
}

function unzip_tests {
    echo "unzipping ${1}..."
    unzip assets/xmltest.zip -d "${1}/tests"
}

function move_xml_schemas {
    echo "moving schemas..."
    cp -Rv assets/schemas "${1}/tests"
}

if [ "$#" -eq "1" ]  || { [ "$#" -eq "0" ] && [ -n "${BUILD_DIR}" ]; } then
    [[ -n "${BUILD_DIR}" ]] && TARGET="${BUILD_DIR}" || TARGET="${1}"
    create_dummy_directory "${TARGET}"
    unzip_tests "${TARGET}"
    move_xml_schemas "${TARGET}"
else
    usage "${0}"
fi

