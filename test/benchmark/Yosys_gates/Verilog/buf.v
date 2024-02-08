//  |---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|---v---|
//-
//-     $_BUF_ (A, Y)
//-
//- A buffer. This cell type is always optimized away by the opt_clean pass.
//-
//- Truth table:    A | Y
//-                ---+---
//-                 0 | 0
//-                 1 | 1
//-
module \$_BUF_ (A, Y);
input A;
output Y;
assign Y = A;
endmodule