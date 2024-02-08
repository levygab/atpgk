module comb1 (input a, b, c, d, e, f, g, h, i, output s3) ;
    assign s3 = ~(i ? ~((a|b)&(c|d)) : ~((e&f)|(g&h)));
endmodule