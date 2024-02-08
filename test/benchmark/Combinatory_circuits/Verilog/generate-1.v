module Prod_one(input a,b,c, output o1, o2);
    AndGate and_gate1(.A(a), .B(b), .Y(o1));
    OrGate or_gate(.x(b), .y(c), .z(o2));
endmodule


module AndGate (A, B, Y);
    input A, B;
    output Y;
    assign Y = A & B;
endmodule

module OrGate(x, y, z);
    input x, y;
    output z;
    assign z = x | y;
endmodule