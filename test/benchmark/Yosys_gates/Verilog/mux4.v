//  |---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|
//-
//-     $_MUX4_ (A, B, C, D, S, T, Y)
//-
//- A 4-input MUX gate.
//-
//- Truth table:    A B C D S T | Y
//-                -------------+---
//-                 a - - - 0 0 | a
//-                 - b - - 1 0 | b
//-                 - - c - 0 1 | c
//-                 - - - d 1 1 | d
//-
module \$_MUX4_ (A, B, C, D, S, T, Y);
input A, B, C, D, S, T;
output Y;
assign Y = T ? (S ? D : C) :
               (S ? B : A);
endmodule