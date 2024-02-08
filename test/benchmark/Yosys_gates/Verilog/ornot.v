//  |---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|
//-
//-     $_ORNOT_ (A, B, Y)
//-
//- A 2-input OR-NOT gate.
//-
//- Truth table:    A B | Y
//-                -----+---
//-                 0 0 | 1
//-                 0 1 | 0
//-                 1 0 | 1
//-                 1 1 | 1
//-
module \$_ORNOT_ (A, B, Y);
input A, B;
output Y;
assign Y = A | (~B);
endmodule