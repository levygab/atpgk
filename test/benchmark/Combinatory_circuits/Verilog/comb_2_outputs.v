module comb_2_outputs (input a, b, c, d, e, f, g, h, i, output s1, s3) ;
    assign s1 = ~((a|b)&(c|d));
    assign s3 = ~(i ? s1 : ~((e&f)|(g&h)));
endmodule

