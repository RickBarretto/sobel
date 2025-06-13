`define MATRIX_5x5 (0):(8*25-1)             /// 5x5 matrix flatted array indexes
`define INTEGER_8 7:0                       /// 8-bits integer indexes

module MpuOperations(
    input        [2:0] operation,
    input signed [`MATRIX_5x5] matrix_a,
    input signed [`MATRIX_5x5] matrix_b,
    input        [`INTEGER_8] size,
    input signed [`INTEGER_8] factor,

    input clock,
	 input start,
    output reg signed [`MATRIX_5x5] result
);

	 wire signed [`MATRIX_5x5] add;
	 wire signed [`MATRIX_5x5] conv;
	 wire signed [`MATRIX_5x5] imul;
	 wire signed [`MATRIX_5x5] opp;
	 wire signed [`MATRIX_5x5] trans;
	 wire signed [`MATRIX_5x5] mul;
	 wire signed [`MATRIX_5x5] determinant;
	 
	 MpuAdd(matrix_a, matrix_b, add);
	 MpuConv(matrix_a, matrix_b, clock, start, sub);
	 MpuIntMul(matrix_a, factor, imul);
	 MpuOpposite(matrix_a, opp);
	 MpuTranspose(matrix_a, trans);
	 MpuDet(matrix_a, size, determinant);
	 MpuMul(matrix_a, matrix_b, size, clock, mul);

    always @(posedge clock) begin
        case (operation)
            0: result <= add;
            1: result <= sub;
            2: result <= imul;
            3: result <= opp;
            4: result <= trans;
            5: result <= determinant;
            6: result <= mul;
            default: ;
        endcase
    end

endmodule