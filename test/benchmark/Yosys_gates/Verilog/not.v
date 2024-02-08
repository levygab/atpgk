//  |---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|
//-
//-     $_NOT_ (A, Y)
//-
//- An inverter gate.
//-
//- Truth table:    A | Y
//-                ---+---
//-                 0 | 1
//-                 1 | 0
//-
module \$_NOT_ (A, Y);
input A;
output Y;
assign Y = ~A;
endmodule