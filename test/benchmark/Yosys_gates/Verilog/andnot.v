//  |---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|
//-
//-     $_ANDNOT_ (A, B, Y)
//-
//- A 2-input AND-NOT gate.
//-
//- Truth table:    A B | Y
//-                -----+---
//-                 0 0 | 0
//-                 0 1 | 0
//-                 1 0 | 1
//-                 1 1 | 0
//-
module \$_ANDNOT_ (A, B, Y);
input A, B;
output Y;
assign Y = A & (~B);
endmodule