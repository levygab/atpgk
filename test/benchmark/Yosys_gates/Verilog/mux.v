//  |---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|
//-
//-     $_MUX_ (A, B, S, Y)
//-
//- A 2-input MUX gate.
//-
//- Truth table:    A B S | Y
//-                -------+---
//-                 a - 0 | a
//-                 - b 1 | b
//-
module \$_MUX_ (A, B, S, Y);
input A, B, S;
output Y;
assign Y = S ? B : A;
endmodule