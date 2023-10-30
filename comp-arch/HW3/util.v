// модуль, который реализует расширенение
// 16-битной знаковой константы до 32-битной
module sign_extend(in, out);
  input [15:0] in;
  output [31:0] out;

  assign out = {{16{in[15]}}, in};
endmodule

// модуль, который реализует побитовый сдвиг числа
// влево на 2 бита
module shl_2(in, out);
  input [31:0] in;
  output [31:0] out;

  assign out = {in[29:0], 2'b00};
endmodule

// 32 битный сумматор
module adder(a, b, out);
  input [31:0] a, b;
  output [31:0] out;

  assign out = a + b;
endmodule

// 32-битный мультиплексор
module mux2_32(d0, d1, a, out);
  input [31:0] d0, d1;
  input a;
  output [31:0] out;
  assign out = a ? d1 : d0;
endmodule

// 5 - битный мультиплексор
module mux2_5(d0, d1, a, out);
  input [4:0] d0, d1;
  input a;
  output [4:0] out;
  assign out = a ? d1 : d0;
endmodule

module for_control_unit(openCode, regWrite, regDST, alusrc, branch, memWrite, memToReg, aluOP);
  input [5:0] openCode;
  
  output reg regWrite, regDST, alusrc, branch, memWrite, memToReg;
  output reg [1:0] aluOP;

  always @* begin
  if (openCode == 6'b000000) begin
      regWrite = 1'b1;
      regDST = 1'b1;
      alusrc = 1'b0;
      branch = 1'b0;
      memWrite = 1'b0;
      memToReg = 1'b0;
      aluOP = 2'b10;
  end else if (openCode == 6'b000100) begin
      regWrite = 1'b0;
      alusrc = 1'b0;
      branch = 1'b1;
      memWrite = 1'b0;
      aluOP = 2'b01; 
  end else if (openCode == 6'b001000) begin
      regWrite = 1'b1;
      regDST = 1'b0;
      alusrc = 1'b1;
      branch = 1'b0;
      memWrite = 1'b0;
      memToReg = 1'b0;
      aluOP = 2'b00; 
  end else if (openCode == 6'b001100) begin
      regWrite = 1'b1;
      regDST = 1'b0;
      alusrc = 1'b1;
      branch = 1'b0;
      memWrite = 1'b0;
      memToReg = 1'b0;
      aluOP = 2'b11; 
  end else if (openCode == 6'b100011) begin
      regWrite = 1'b1;
      regDST = 1'b0;
      alusrc = 1'b1;
      branch = 1'b0;
      memWrite = 1'b0;
      memToReg = 1'b1;
      aluOP = 2'b00;
  end else if (openCode == 6'b101011) begin
      regWrite = 1'b0;
      alusrc = 1'b1;
      branch = 1'b0;
      memWrite = 1'b1;
      aluOP = 2'b00;
  end 
  end
endmodule

module control_unit(openCode, fun, regWrite, regDST, alusrc, branch, memWrite, memToReg, aluControl);
  input [5:0] openCode, fun;
  wire [1:0] aluop;
  output wire regWrite, regDST, alusrc, branch, memWrite, memToReg;
  output reg [2:0] aluControl;

  for_control_unit contramater(openCode, regWrite, regDST, alusrc, branch, memWrite, memToReg, aluop);

  always @* begin
    if (aluop == 2'b00) begin
        aluControl = 3'b010;
    end else if (aluop == 2'b01) begin
        aluControl = 3'b110;
    end else if (aluop == 2'b10) begin
      if (fun == 6'b100000) begin
        aluControl = 3'b010;
      end else if (fun == 6'b100010) begin
        aluControl = 3'b110;
      end else if (fun == 6'b100100) begin
        aluControl = 3'b000;
      end else if (fun == 6'b100101) begin
        aluControl = 3'b001;
      end else if (fun == 6'b101010) begin
        aluControl = 3'b111;
      end
    end
  end
endmodule

module alu(a, b, alucontrol, aluresult, zero);
    input signed [31:0] a, b;
    input [2:0] alucontrol; 
    output reg signed [31:0] aluresult;
    output reg zero;
    always @(alucontrol or a or b) begin
	case (alucontrol)
	3'b000:
	    aluresult <= a & b;
	3'b001:
	    aluresult <= a | b;
	3'b010:
	    aluresult <= a + b;
	3'b011:
	    aluresult <= a & b;
	3'b100:
	    aluresult <= a & ~(b);
	3'b101:
	    aluresult <= a | ~(b);
	3'b110:
	    aluresult <= a - b;
	3'b111:
	    aluresult <= a < b ? 32'b1 : 32'b0;	
	endcase
    end
	
    always @(aluresult) begin
	if (aluresult == 32'b0) begin
	    zero <= 1'b1;
	end else begin
	    zero <= 1'b0;
	end
    end
endmodule