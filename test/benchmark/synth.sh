#!/bin/bash

if [ $2 ]; then
    yosys -p "read_verilog ./$1/Verilog/$2.v; opt; synth; flatten; opt; write_json ./$1/JSON_Netlist/$2.json"

    netlistsvg -o ./$1/SVG/$2.svg ./$1/JSON_Netlist/$2.json
else
    echo "Usage :"
    echo ""
    echo "    > ./synth.sh <subdirectory_name> <module_name>"
fi