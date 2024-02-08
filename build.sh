#!/bin/bash

which conan &> /dev/null;
if [ $? -eq 1 ]; then
    echo "Error : Conan is not installed on your device. You must install conan before trying to install this software"
    exit 1
fi

conan_profile_file="$HOME/.conan2/profiles/default"
if [ ! -e "$conan_profile_file" ]; then
    conan profile detect
fi

conan install . -of build --build=missing --settings=build_type=Debug --settings=compiler.cppstd=gnu17 --settings=compiler.libcxx=libstdc++11

# Create the 'build' directory if it doesn't exist
# echo "running : mkdir -p build"
# mkdir -p build

# Change the working directory to 'build'
echo "running : cd build"
cd build

# Run CMake to configure the project
echo "running : cmake .."

cmake .. -G "Unix Makefiles"

make

if [ $1 ]; then
    option1=$1
    if [ $option1 == "test-all" ]; then
        echo $'\n----------- TESTING -----------\n'
        echo $'Running ctest ...\n'
        ctest
    fi
fi