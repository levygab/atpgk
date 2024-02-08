module NandGate (input x, y, output z);
    AndGate and_gate(.A(x), .B(y), .Y(and_output));
    NotGate not_gate(.C(and_output), .W(z));

    reg and_output;
endmodule

module AndGate (A, B, Y);
    input A, B;
    output Y;
    assign Y = A & B;
endmodule

module NotGate (C, W);
    input C;
    output W;
    assign W = ~C;
endmodule