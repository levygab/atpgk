//  |---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|
//-
//-     $_AOI3_ (A, B, C, Y)
//-
//- A 3-input And-Or-Invert gate.
//-
//- Truth table:    A B C | Y
//-                -------+---
//-                 0 0 0 | 1
//-                 0 0 1 | 0
//-                 0 1 0 | 1
//-                 0 1 1 | 0
//-                 1 0 0 | 1
//-                 1 0 1 | 0
//-                 1 1 0 | 0
//-                 1 1 1 | 0
//-
module \$_AOI3_ (A, B, C, Y);
input A, B, C;
output Y;
assign Y = ~((A & B) | C);
endmodule