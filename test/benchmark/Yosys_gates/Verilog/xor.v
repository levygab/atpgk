//  |---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|
//-
//-     $_XOR_ (A, B, Y)
//-
//- A 2-input XOR gate.
//-
//- Truth table:    A B | Y
//-                -----+---
//-                 0 0 | 0
//-                 0 1 | 1
//-                 1 0 | 1
//-                 1 1 | 0
//-
module \$_XOR_ (A, B, Y);
input A, B;
output Y;
assign Y = A ^ B;
endmodule