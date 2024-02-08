//  |---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|
//-
//-     $_TBUF_ (A, E, Y)
//-
//- A tri-state buffer.
//-
//- Truth table:    A E | Y
//-                -----+---
//-                 a 1 | a
//-                 - 0 | z
//-
module \$_TBUF_ (A, E, Y);
input A, E;
output Y;
assign Y = E ? A : 1'bz;
endmodule