# Examples

For each example, you will find the following files :

- A Verilog description of the circuit
- The JSON netlist generated with Yosys
- The Yosys synthesis script, that you can run witn `yosys -s synth.ys` in the dictory of the example
- An SVG illustration generated with `netlistsvg` with the following command : `netlistsvg -o circuit.svg netlist.json`
- A TXT and a JSON file containing the test pattern vectors generated with ATPGK :

```bash
# Go to the dir that contains the executable
cd ../build

# Define the directory name of the example you want to test
# example : dir_name="1_And"
dir_name=<dir_name>

# Generate the vectors.txt output file
./ATPGK -O txt -p ../examples/$dir_name/ ../examples/$dir_name/netlist.json

# Generate the verctors.json output file
./ATPGK -O json -p ../examples/$dir_name/ ../examples/$dir_name/netlist.json
```

## List of examples

- A simple `and` cell
- A 4 inputs `and-or-invert` cell performing the following operation : `Y = ~((A & B) | (C & D))`
- A design parforming a `nand` operation with multiple modules definition
- A more complex design made of different binary cells
