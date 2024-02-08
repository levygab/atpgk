module comb_2_outputs (input a, b, c, d, e, f, output s1, s2) ;
    AndGate and_gate1(.A(a), .B(b), .Y(AndNotInput1));
    AndGate and_gate2(.A(b), .B(c), .Y(AndNotInput2)); 
    AndNotGate and_not_gate(.x(AndNotInput1), .y(AndNotInput2), .z(s1));
    OAI3Gate oai3_gate(.A(c), .B(d), .C(e), .Y(orInput1));
    OrGate or_gate(.x(orInput1), .y(f), .z(s2));
endmodule

module AndGate (A, B, Y);
    input A, B;
    output Y;
    assign Y = A & B;
endmodule

module AndNotGate(x, y, z);
    input x, y;
    output z;
    assign z = x & ~y;
endmodule

module OAI3Gate (A, B, C, Y);
    input A, B, C;
    output Y;
    assign Y = ~((A | B) & C);
endmodule

module OrGate(x, y, z);
    input x, y;
    output z;
    assign z = x | y;
endmodule