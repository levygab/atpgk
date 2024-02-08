//  |---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|
//-
//-     $_AND_ (A, B, Y)
//-
//- A 2-input AND gate.
//-
//- Truth table:    A B | Y
//-                -----+---
//-                 0 0 | 0
//-                 0 1 | 0
//-                 1 0 | 0
//-                 1 1 | 1
//-
module _AND_ (A, B, Y);
input A, B;
output Y;
assign Y = A & B;
endmodule