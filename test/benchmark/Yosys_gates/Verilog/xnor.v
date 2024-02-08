//  |---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|
//-
//-     $_XNOR_ (A, B, Y)
//-
//- A 2-input XNOR gate.
//-
//- Truth table:    A B | Y
//-                -----+---
//-                 0 0 | 1
//-                 0 1 | 0
//-                 1 0 | 0
//-                 1 1 | 1
//-
module \$_XNOR_ (A, B, Y);
input A, B;
output Y;
assign Y = ~(A ^ B);
endmodule