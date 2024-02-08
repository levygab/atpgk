//  |---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|
//-
//-     $_NMUX_ (A, B, S, Y)
//-
//- A 2-input inverting MUX gate.
//-
//- Truth table:    A B S | Y
//-                -------+---
//-                 0 - 0 | 1
//-                 1 - 0 | 0
//-                 - 0 1 | 1
//-                 - 1 1 | 0
//-
module \$_NMUX_ (A, B, S, Y);
input A, B, S;
output Y;
assign Y = S ? !B : !A;
endmodule