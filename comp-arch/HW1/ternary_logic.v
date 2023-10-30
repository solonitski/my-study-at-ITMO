module not_gate(in, out);
  input wire in;
  output wire out;
  supply1 vdd;
  supply0 gnd;
  pmos pmos1(out, vdd, in);
  nmos nmos1(out, gnd, in);
endmodule

///////////////////////////////////////////////////////////////////////////

module nand_gate(in_first, in_second, out);
  input wire in_first;
  input wire in_second;
  output wire out;
  supply0 gnd;
  supply1 pwr;
  wire nmos_out;
  pmos pmos1(out, pwr, in_first);
  pmos pmos2(out, pwr, in_second);
  nmos nmos1(nmos_out, gnd, in_first);
  nmos nmos2(out, nmos_out, in_second);
endmodule


module and_gate(in1, in2, out);
  input wire in1;
  input wire in2;
  output wire out;
  wire nand_out;
  nand_gate nand_gate1(in1, in2, nand_out);
  not_gate not_gate1(nand_out, out);
endmodule

///////////////////////////////////////////////////////////////////////////

module nor_gate(in1, in2, out);
  input wire in1;
  input wire in2;
  output wire out;
  supply0 gnd;
  supply1 pwr;
  wire pmos1_out;
  pmos pmos1(pmos1_out, pwr, in1);
  pmos pmos2(out, pmos1_out, in2);
  nmos nmos1(out, gnd, in1);
  nmos nmos2(out, gnd, in2);
endmodule


module or_gate(in1, in2, out);
  input wire in1;
  input wire in2;
  output wire out;
  wire nor_out;
  nor_gate nor_gate1(in1, in2, nor_out);
  not_gate not_gate1(nor_out, out);
endmodule

///////////////////////////////////////////////////////////////////////////

module xor_gate(in1, in2, out);
  input wire in1;
  input wire in2;
  output wire out;
  wire not_in1;
  wire not_in2;
  wire and_out1;
  wire and_out2;
  wire or_out1;

  not_gate not_gate1(in1, not_in1);
  not_gate not_gate2(in2, not_in2);
  and_gate and_gate1(in1, not_in2, and_out1);
  and_gate and_gate2(not_in1, in2, and_out2);
  or_gate or_gate1(and_out1, and_out2, out);
endmodule

///////////////////////////////////////////////////////////////////////////

module eq_gate(in_first, in_second, out);
  input wire in_first;
  input wire in_second;
  output wire out;
  wire xor4ik;
  
  xor_gate xor_gate1(in_first, in_second, xor4ik);
  not_gate not_gate1(xor4ik, out);
endmodule

///////////////////////////////////////////////////////////////////////////

module ternary_min(a[1:0], b[1:0], out[1:0]);
  input [1:0] a;
  input [1:0] b;
  output [1:0] out;
  wire a0orb0;
  wire a1xora0;
  wire b1xorb0;
  wire temp;

  and_gate and_gate1(a[1], b[1], out[1]);
  or_gate or_gate1(a[0], b[0], a0orb0);
  xor_gate xor_gate1(a[1], a[0], a1xora0);
  xor_gate xor_gate2(b[1], b[0], b1xorb0);
  and_gate and_gate2(a0orb0, a1xora0, temp);
  and_gate and_gate3(temp, b1xorb0, out[0]);
endmodule

module ternary_max(a[1:0], b[1:0], out[1:0]);
  input [1:0] a;
  input [1:0] b;
  output [1:0] out;
  wire c1eqd1;
  wire c0ord0;
  
  or_gate or_gate1(a[1], b[1], out[1]);
  eq_gate eq_gate1(a[1], b[1], c1eqd1);
  or_gate or_gate2(a[0], b[0], c0ord0);
  and_gate and_gate3(c1eqd1, c0ord0, out[0]);
endmodule

module ternary_consensus(a[1:0], b[1:0], out[1:0]);
  input [1:0] a;
  input [1:0] b;
  output [1:0] out;
  wire x1xory1;
  wire x0xory0;
  wire maxim;
  wire solonitski;
  
  and_gate and_gate1(a[1], b[1], out[1]);
  xor_gate xor_gate1(a[1], b[1], x1xory1);
  xor_gate xor_gate2(a[0], b[0], x0xory0);
  xor_gate xor_gate3(x1xory1, x0xory0, maxim);
  or_gate or_gate1(maxim, a[0], solonitski);
  or_gate or_gate2(solonitski, b[0], out[0]);
endmodule

module ternary_any(a[1:0], b[1:0], out[1:0]);
  input [1:0] a;
  input [1:0] b;
  output [1:0] out;
  wire zero = 0;
  wire one = 1;
  wire x0eq0;
  wire x0eq1;
  wire x1eq0;
  wire x1eq1;
  wire y0eq0;
  wire y0eq1;
  wire y1eq0;
  wire y1eq1;
  wire and1;
  wire and2;
  wire xor1;
  wire xor2;
  wire xor1eq1;
  wire xor2eq1;
  eq_gate eq_gate1(a[0], zero, x0eq0);
  eq_gate eq_gate2(a[0], one, x0eq1);
  eq_gate eq_gate3(a[1], zero, x1eq0);
  eq_gate eq_gate4(a[1], one, x1eq1);
  eq_gate eq_gate5(b[0], zero, y0eq0);
  eq_gate eq_gate6(b[0], one, y0eq1);
  eq_gate eq_gate7(b[1], zero, y1eq0);
  eq_gate eq_gate8(b[1], one, y1eq1);
  
  and_gate and_gate1(x1eq1, y1eq1, and1);
  xor_gate xor_gate1(a[1], b[1], xor1);
  xor_gate xor_gate2(a[0], b[0], xor2);
  eq_gate eq_gate9(xor1, one, xor1eq1);
  eq_gate eq_gate10(xor2, one, xor2eq1);
  and_gate and_gate2(xor1eq1, xor2eq1, and2);
  or_gate or_gate1(and1, and2, out[1]);
  
  wire maxim1;
  wire maxim2;
  wire first;
  wire maxim3;
  wire maxim4;
  wire second;
  wire maxim5;
  wire maxim6;
  wire third;
  wire t1nol;
  
  and_gate and_gate3(x1eq1, x0eq0, maxim1);
  and_gate and_gate4(maxim1, y1eq0, maxim2); 
  and_gate and_gate5(maxim2, y0eq0, first);
  and_gate and_gate6(x1eq0, x0eq0, maxim3);
  and_gate and_gate7(maxim3, y1eq1, maxim4);
  and_gate and_gate8(maxim4, y0eq0, second);
  and_gate and_gate9(x1eq0, x0eq1, maxim5);
  and_gate and_gate10(maxim5, y1eq0, maxim6);
  and_gate and_gate11(maxim6, y0eq1, third);
  or_gate or_gate2(first, second, t1nol);
  or_gate or_gate3(t1nol, third, out[0]);
endmodule

//тесты решил сохранить, а то мало ли что :)
//и да, тесты все прошли :)

/*module testMin();
    reg [1:0] a, b;
    wire [1:0] out;
    ternary_min test_min(a, b, out);
    initial begin
        $display("ternary_min");
        a = 2'b00;
        b = 2'b00;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b00;
        b = 2'b01;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b00;
        b = 2'b10;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b01;
        b = 2'b00;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b01;
        b = 2'b01;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b01;
        b = 2'b10;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b10;
        b = 2'b00;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b10;
        b = 2'b01;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b10;
        b = 2'b10;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        $display("");
    end

endmodule*/

/*module testMax();
    reg [1:0] a, b;
    wire [1:0] out;
    ternary_max test_max(a, b, out);
    initial begin
        $display("ternary_max");
        a = 2'b00;
        b = 2'b00;
        #2;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b00;
        b = 2'b01;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b00;
        b = 2'b10;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b01;
        b = 2'b00;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b01;
        b = 2'b01;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b01;
        b = 2'b10;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b10;
        b = 2'b00;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b10;
        b = 2'b01;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b10;
        b = 2'b10;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
    end

endmodule*/

/*module testConsensus();
    reg [1:0] a, b;
    wire [1:0] out;
    ternary_consensus test_con(a, b, out);
    initial begin
        $display("ternary_consensus");
        a = 2'b00;
        b = 2'b00;
        #2;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b00;
        b = 2'b01;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b00;
        b = 2'b10;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b01;
        b = 2'b00;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b01;
        b = 2'b01;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b01;
        b = 2'b10;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b10;
        b = 2'b00;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b10;
        b = 2'b01;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b10;
        b = 2'b10;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
    end

endmodule*/

/*module testAny();
    reg [1:0] a, b;
    wire [1:0] out;
    ternary_any test_any(a, b, out);
    initial begin
        $display("ternary_any");
        a = 2'b00;
        b = 2'b00;
        #2;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b00;
        b = 2'b01;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b00;
        b = 2'b10;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b01;
        b = 2'b00;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b01;
        b = 2'b01;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b01;
        b = 2'b10;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b10;
        b = 2'b00;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b10;
        b = 2'b01;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
        
        a = 2'b10;
        b = 2'b10;
        #1;
        $display("a = %b, b = %b => out = %b", a, b, out);
    end

endmodule*/