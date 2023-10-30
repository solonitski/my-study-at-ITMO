`include "util.v"

module mips_cpu(clk, pc, pc_new, instruction_memory_a, instruction_memory_rd, data_memory_a, data_memory_rd, data_memory_we, data_memory_wd,
                register_a1, register_a2, register_a3, register_we3, register_wd3, register_rd1, register_rd2);
  input clk;
  inout [31:0] pc;
  output [31:0] pc_new;
  output data_memory_we;
  output [31:0] instruction_memory_a, data_memory_a, data_memory_wd;
  inout [31:0] instruction_memory_rd, data_memory_rd;
  output register_we3;
  output [4:0] register_a1, register_a2, register_a3;
  output [31:0] register_wd3;
  inout [31:0] register_rd1, register_rd2;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  wire [1:0] aluop;
  wire [2:0] alucontrol;
  wire [31:0] b, sign, temp, pcBranch, temp2;
  wire regwrite, regdst, alusrc, branch, memwrite, memtoreg, zero, pcsrc;
  
  assign instruction_memory_a = pc;
  assign register_a1 = instruction_memory_rd[25:21];
  assign register_a2 = instruction_memory_rd[20:16];    
  assign register_we3 = regwrite;
  assign data_memory_wd = register_rd2;
  assign data_memory_we = memwrite;

  sign_extend siiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiign_extand(instruction_memory_rd[15:0], sign);
  
  control_unit my_control_unit(instruction_memory_rd[31:26], instruction_memory_rd[5:0], regwrite, regdst, alusrc, branch, memwrite, memtoreg, alucontrol);
  
  mux2_32 m1(data_memory_a, data_memory_rd, memtoreg, register_wd3);
  mux2_5 m2(instruction_memory_rd[20:16], instruction_memory_rd[15:11], regdst, register_a3);
  mux2_32 m3(register_rd2, sign, alusrc, b);

  assign pcsrc = (zero & branch);   
  mux2_32 m4(temp2, pcBranch, pcsrc, pc_new);
  
  adder sum1(pc, 4, temp2);
  adder sum2(temp, temp2, pcBranch);
  
  alu alushka(register_rd1, b, alucontrol, data_memory_a, zero);
  
  shl_2 need(sign, temp);

endmodule