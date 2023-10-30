//ALU: Part1
module forAlu(a, b, c, maybe[3:0], out[3:0]);
    input a, b, c;
    input [3:0] maybe;
    output [3:0] out;
    wire flag;
    
    and_3vars what(a, b, c, temp);
    and_gate obnulenie3(maybe[3], temp, out[3]);
    and_gate obnulenie2(maybe[2], temp, out[2]);
    and_gate obnulenie1(maybe[1], temp, out[1]);
    and_gate obnulenie0(maybe[0], temp, out[0]);
endmodule

module and_3vars(a, b, c, out);
    input a, b, c;
    output out;
    wire temp;
    and_gate and1(a, b, temp);
    and_gate result(temp, c, out);
endmodule

module sum_result(a1[3:0], a2[3:0], a3[3:0], a4[3:0], a5[3:0], a6[3:0], a7[3:0], a8[3:0], out[3:0]);
    input [3:0] a1, a2, a3, a4, a5, a6, a7, a8;
    output [3:0] out;
    wire [3:0] temp1, temp2, temp3, temp4, temp5, temp6;
    or_4bit op1(a1, a2, temp1);
    or_4bit op2(temp1, a3, temp2);
    or_4bit op3(temp2, a4, temp3);
    or_4bit op4(temp3, a5, temp4);
    or_4bit op5(temp4, a6, temp5);
    or_4bit op6(temp5, a7, temp6);
    or_4bit op7(temp6, a8, out);
endmodule

module alu(a[3:0], b[3:0], control[2:0], res[3:0]);
  input [3:0] a, b;
  input [2:0] control;
  output [3:0] res;
  
  wire [3:0] not_b, change_sign_b;
  wire [3:0] temp1, temp2, temp3, temp4, temp5, temp6, temp7, temp8;
  wire [3:0] result1, result2, result3, result4, result5, result6, result7, result8;
  wire not_control2, not_control1, not_control0;
  reg [3:0] o = 4'b0000;
  
  not_4bit notb(b, not_b);
  change_sign changesignb(b, change_sign_b);
  
  not_gate otr2(control[2], not_control2);
  not_gate otr1(control[1], not_control1);
  not_gate otr0(control[0], not_control0);

    and_4bit first(a, b, temp1); //000
    forAlu t1(not_control2, not_control1, not_control0, temp1, result1);
    
    or_4bit second(a, b, temp2); //001
    forAlu t2(not_control2, not_control1, control[0], temp2, result2);
    
    adder_4bit third(a, b, o[3], temp3); //010
    forAlu t3(not_control2, control[1], not_control0, temp3, result3);
    
    adder_4bit fourth(a, b, o[3], temp4); //010
    forAlu t4(not_control2, control[1], control[0], temp4, result4);
  
    and_4bit fifth(a, not_b, temp5); //100
    forAlu t5(control[2], not_control1, not_control0, temp5, result5);
    
    or_4bit sixth(a, not_b, temp6); //101
    forAlu t6(control[2], not_control1, control[0], temp6, result6);
  
    adder_4bit seventh(a, change_sign_b, o[3], temp7); //110
    forAlu t7(control[2], control[1], not_control0, temp7, result7);
    
    slt eighth(a, b, temp8); //111
    forAlu t8(control[2], control[1], control[0], temp8, result8);
    
    
    sum_result a_na_segodnya_vse(result1, result2, result3, result4, result5, result6, result7, result8, res);
endmodule
//END_ALU


//Register_file: Part2
module d_latch(clk, d, we, q);
  input clk;
  input d;
  input we;
  output reg q;
  initial begin
    q <= 0;
  end
  always @ (negedge clk) begin
    if (we) begin
      q <= d;
    end
  end
endmodule

module decoder_1_to_2(in, out1, out2);
    input in;
    output out1, out2;
        not_gate notin(in, out1);
    assign out2 = in;
endmodule


module decoder_2_to_4(we_addr, out);
    input [1:0] we_addr;
    output [3:0] out;
    wire F1, S1;
    wire F2, S2;

    decoder_1_to_2 f(we_addr[0], F1, S1);
    decoder_1_to_2 s(we_addr[1], F2, S2);

    and_gate a0(F1, F2, out[0]);
    and_gate a1(S1, F2, out[1]);
    and_gate a2(F1, S2, out[2]);
    and_gate a3(S1, S2, out[3]);
endmodule

module multiplexer(a, b, c, d, control[1:0], out);
  input [1:0] control;
  input a, b, c, d;
  output out;
  wire not_control0, not_control1, out1, out2, temp1, temp2, temp3, temp4;
  not_gate n1(control[0], not_control0);
  not_gate n2(control[1], not_control1);
  
  and_3vars a1(not_control1, not_control0, a, temp1);
  and_3vars a2(not_control1, control[0], b, temp2);
  and_3vars a3(control[1], not_control0, c, temp3);
  and_3vars a4(control[1], control[0], d, temp4);
  
  or_gate o1(temp1, temp2, out1);
  or_gate o2(temp3, temp4, out2);
  or_gate o3(out1, out2, out);
endmodule

module register(clk, d, we, q);
  input clk; 
  input [3:0] d;
  input we;
  output wire [3:0] q; 
  d_latch latch1(clk, d[0], we, q[0]);
  d_latch latch2(clk, d[1], we, q[1]);
  d_latch latch3(clk, d[2], we, q[2]); 
  d_latch latch4(clk, d[3], we, q[3]);
endmodule

module register_file(clk, rd_addr, we_addr, we_data, rd_data);
  input clk;
  input [1:0] rd_addr, we_addr;
  input [3:0] we_data;
  output [3:0] rd_data;
  wire [3:0] r;
  wire [3:0] rd1, rd2, rd3, rd4;
  decoder_2_to_4 d1(we_addr, r);
  register r1(clk, we_data, r[0], rd1);
  register r2(clk, we_data, r[1], rd2);
  register r3(clk, we_data, r[2], rd3);
  register r4(clk, we_data, r[3], rd4);
  multiplexer m1(rd1[0], rd2[0], rd3[0], rd4[0], rd_addr, rd_data[0]);  
  multiplexer m2(rd1[1], rd2[1], rd3[1], rd4[1], rd_addr, rd_data[1]);
  multiplexer m3(rd1[2], rd2[2], rd3[2], rd4[2], rd_addr, rd_data[2]);
  multiplexer m4(rd1[3], rd2[3], rd3[3], rd4[3], rd_addr, rd_data[3]);
endmodule

//END_Register_file


//Calculator: Part3
module calculator(clk, rd_addr, immediate, we_addr, control, rd_data);
  input clk;
  input [1:0] rd_addr;
  input [3:0] immediate;
  input [1:0] we_addr;
  input [2:0] control;
  output [3:0] rd_data;
  wire [3:0] we_data;
  register_file reg_file1(clk, rd_addr, we_addr, we_data, rd_data);
  alu alu1(rd_data, immediate, control, we_data); 
endmodule
//END_Calculator


//Вспомогательные модули
//////////////////////////////////////////////////////////////////////////////////////////
module and_4bit(in1[3:0], in2[3:0], out[3:0]);
  input [3:0] in1, in2;
  output [3:0] out;
  and_gate and_gate0(in1[0], in2[0], out[0]);
  and_gate and_gate1(in1[1], in2[1], out[1]);
  and_gate and_gate2(in1[2], in2[2], out[2]);
  and_gate and_gate3(in1[3], in2[3], out[3]);
endmodule

module or_4bit(in1[3:0], in2[3:0], out[3:0]);
  input [3:0] in1, in2;
  output [3:0] out;
  or_gate or_gate0(in1[0], in2[0], out[0]);
  or_gate or_gate1(in1[1], in2[1], out[1]);
  or_gate or_gate2(in1[2], in2[2], out[2]);
  or_gate or_gate3(in1[3], in2[3], out[3]);
endmodule

module not_4bit(in[3:0], out[3:0]);
  input [3:0] in;
  output [3:0] out;
  not_gate not_gate0(in[0], out[0]);
  not_gate not_gate1(in[1], out[1]);
  not_gate not_gate2(in[2], out[2]);
  not_gate not_gate3(in[3], out[3]);
endmodule
//////////////////////////////////////////////////////////////////////////////////////////
module half_adder(a, b, c_out, s);
  input wire a, b;
  output wire c_out, s;
  and_gate and_gate1(a, b, c_out);
  xor_gate xor_gate1(a, b, s);
endmodule

module adder_1bit(a, b, c_in, c_out, sum);
  input a, b, c_in;
  output c_out, sum;
  wire temp1, temp2, temp3;
  xor_gate x0(a, b, temp1);
  xor_gate x1(temp1, c_in, sum);
  and_gate a0(temp1, c_in, temp2);
  and_gate a1(a, b, temp3);
  or_gate  o0(temp2, temp3, c_out);
endmodule

module adder_4bit(a, b, c_in, res);
  input [3:0] a, b;
  input c_in;
  output [3:0] res;
  wire c_out0, c_out1, c_out2, c_out3;
  adder_1bit a0(a[0], b[0], c_in, c_out0, res[0]);
  adder_1bit a1(a[1], b[1], c_out0, c_out1, res[1]);
  adder_1bit a2(a[2], b[2], c_out1, c_out2, res[2]);
  adder_1bit a3(a[3], b[3], c_out2, c_out3, res[3]);
endmodule

module change_sign(num[3:0], out[3:0]);
  input [3:0] num;
  output [3:0] out;
  wire [3:0] not_num;
  reg [3:0] one = 4'b0001; 
  not_4bit ne(num, not_num);
  adder_4bit res(not_num, one, one[3], out);
endmodule

module slt(a[3:0], b[3:0], out[3:0]);
  input [3:0] a, b;
  output [3:0] out;
  reg [3:0] o = 4'b0000; 
  reg [3:0] o1 = 4'b0000;
  wire temp1, temp2, temp3, temp4, temp5, temp6, temp7, temp8, not_b3, not_a3;
  wire[3:0] a_raznost_b, change_sign_b;
  
  change_sign changesignb(b, change_sign_b);
  not_gate nb3(b[3], not_b3);
  not_gate na3(a[3], not_a3);
  and_gate a1(not_b3, a[3], temp1);
  nand_gate na2(not_a3, b[3], temp2);
  adder_4bit add(a, change_sign_b, o[3], a_raznost_b);
  and_gate a2(temp2, a_raznost_b[3], temp3);
  or_gate o11(a_raznost_b[2], a_raznost_b[3], temp4);
  or_gate o22(a_raznost_b[0], a_raznost_b[1], temp5);
  or_gate o33(temp4, temp5, temp6);
  and_gate a4(temp3, temp6, temp7);
  or_gate or1(temp1, temp7, out[0]);
  or_gate or2(o[1], o1[1], out[1]);
  or_gate or3(o[2], o1[2], out[2]);
  or_gate or4(o[3], o1[3], out[3]);
endmodule

//////////////////////////////////////////////////////////////////////////////////////////
module not_gate(in, out);
  input wire in;
  output wire out;
  supply1 vdd; 
  supply0 gnd; 
  pmos pmos1(out, vdd, in); 
  nmos nmos1(out, gnd, in);
endmodule

module nand_gate(in1, in2, out);
  input wire in1, in2;
  output wire out;
  supply0 gnd;
  supply1 pwr;
  wire nmos1_out;
  pmos pmos1(out, pwr, in1);
  pmos pmos2(out, pwr, in2);
  nmos nmos1(nmos1_out, gnd, in1);
  nmos nmos2(out, nmos1_out, in2);
endmodule

module nor_gate(in1, in2, out);
  input wire in1, in2;
  output wire out;
  supply0 gnd;
  supply1 pwr;
  wire pmos1_out;
  pmos pmos1(pmos1_out, pwr, in1);
  pmos pmos2(out, pmos1_out, in2);
  nmos nmos1(out, gnd, in1);
  nmos nmos2(out, gnd, in2);
endmodule

module and_gate(in1, in2, out);
  input wire in1, in2;
  output wire out;
  wire nand_out;
  nand_gate nand_gate1(in1, in2, nand_out);
  not_gate not_gate1(nand_out, out);
endmodule

module or_gate(in1, in2, out);
  input wire in1, in2;
  output wire out;
  wire nor_out;
  nor_gate nor_gate1(in1, in2, nor_out);
  not_gate not_gate1(nor_out, out);
endmodule

module xor_gate(in1, in2, out);
  input wire in1, in2;
  output wire out;
  wire not_in1, not_in2, and_out1, and_out2, or_out1;
  not_gate not_gate1(in1, not_in1);
  not_gate not_gate2(in2, not_in2);
  and_gate and_gate1(in1, not_in2, and_out1);
  and_gate and_gate2(not_in1, in2, and_out2);
  or_gate or_gate1(and_out1, and_out2, out);
endmodule
//////////////////////////////////////////////////////////////////////////////////////////