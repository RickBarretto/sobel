`define MATRIX_5x5  199:0
`define INTEGER_8   7:0	

module MpuConv (
  input [`MATRIX_5x5],
  input signed [`MATRIX_5x5] kernel,
  input clock,
  input start,
  output [`INTEGER_8] result, 
  output signal
);
	
  reg signed [15:0] multiplication [0:24];
  reg signed [16:0] partial_1 [0:12];
  reg signed [17:0] partial_2 [0:6];
  reg signed [18:0] partial_3 [0:3];
  reg signed [19:0] partial_4 [0:1];
  reg signed [20:0] sum;
  

  reg [2:0]  current;
  reg [20:0] mod;
  
  assign result = (|mod[20:8])?8'hff:mod[7:0];
  assign signal = sum[20]; 
  
  integer i;

  always @(posedge clock) begin
    if (!start) begin
      current <= 0;
      done <= 0;
    end else begin
      case (current)
        0: begin
          for (i = 0; i < 25; i = i + 1)
            multiplication[i] <= $signed(kernel[i*8 +: 8]) * matrix[i*8 +: 8];
			
          current <= 1;
          done <= 0;
        end

        1: begin
          for (i = 0; i < 12; i = i + 1) 
				partial_1[i] <= multiplication[2*i] + multiplication[2*i+1];
          
			 partial_1[12] <= multiplication[24]; 
          current <= 2;
        end

        2: begin
          for (i = 0; i < 6; i = i + 1)
            partial_2[i] <= partial_1[2*i] + partial_1[2*i+1];
          
			 partial_2[6] <= partial_1[12];
          current <= 3;
        end

        3: begin
          for (i = 0; i < 3; i = i + 1)
				partial_3[i] <= partial_2[2*i] + partial_2[2*i+1];
				
				partial_3[3] <= partial_2[6];
          current <= 4;
        end

        4: begin
          partial_4[0] <= partial_3[0] + partial_3[1];
          partial_4[1] <= partial_3[2] + partial_3[3];
          current <= 5;
        end

        5: begin
          sum <= partial_4[0] + partial_4[1];
          current <= 6;
        end

        6: begin
          mod <= sum[20] ? (~sum + 1'b1) : sum;
          current <= 6;
        end
      endcase
    end
  end

endmodule