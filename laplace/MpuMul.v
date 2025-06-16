`define i8(x) 8'sd``x
`define INTEGER_8 7:0
`define MATRIX_5x5 (8*25-1):0
`define at(row, col) (8 * (col + 5*row))

module MpuMul (
    input      signed [`MATRIX_5x5] matrix_a, // Flattened 5x5 matrix (each element 8 bits)
    input      signed [`MATRIX_5x5] matrix_b, // Flattened 5x5 matrix (each element 8 bits)
	 input [7:0] size,
	 
	 input clock,
	 
    output reg signed [`MATRIX_5x5] result   // Flattened 5x5 result matrix (each element 16 bits)
);
    
	/// This module works with 5 cycles of clock for operate a matrix of any size,
	/// this is agnostic to size, and will process 5 elements per cycle.
	/// Since the matrix is 5x5, this will take 5 cycles to complete the whole task.
	///
	/// Note
	/// ----
	/// `row` works as a counter and will be reset when reach the higher state.

	 reg [`INTEGER_8] row;
	 
    always @(posedge clock) begin
		result[`at(row, 0) +: 8] <= 
			  matrix_a[`at(row, 0) +: 8] * matrix_b[`at(0, 0) +: 8]
			+ matrix_a[`at(row, 1) +: 8] * matrix_b[`at(1, 0) +: 8]
			+ matrix_a[`at(row, 2) +: 8] * matrix_b[`at(2, 0) +: 8]
			+ matrix_a[`at(row, 3) +: 8] * matrix_b[`at(3, 0) +: 8]
			+ matrix_a[`at(row, 4) +: 8] * matrix_b[`at(4, 0) +: 8];
			
			result[`at(row, 1) +: 8] <= 
			  matrix_a[`at(row, 0) +: 8] * matrix_b[`at(0, 1) +: 8]
			+ matrix_a[`at(row, 1) +: 8] * matrix_b[`at(1, 1) +: 8]
			+ matrix_a[`at(row, 2) +: 8] * matrix_b[`at(2, 1) +: 8]
			+ matrix_a[`at(row, 3) +: 8] * matrix_b[`at(3, 1) +: 8]
			+ matrix_a[`at(row, 4) +: 8] * matrix_b[`at(4, 1) +: 8];
			
		result[`at(row, 2) +: 8] <= 
			  matrix_a[`at(row, 0) +: 8] * matrix_b[`at(0, 2) +: 8]
			+ matrix_a[`at(row, 1) +: 8] * matrix_b[`at(1, 2) +: 8]
			+ matrix_a[`at(row, 2) +: 8] * matrix_b[`at(2, 2) +: 8]
			+ matrix_a[`at(row, 3) +: 8] * matrix_b[`at(3, 2) +: 8]
			+ matrix_a[`at(row, 4) +: 8] * matrix_b[`at(4, 2) +: 8];
			
		result[`at(row, 3) +: 8] <= 
			  matrix_a[`at(row, 0) +: 8] * matrix_b[`at(0, 3) +: 8]
			+ matrix_a[`at(row, 1) +: 8] * matrix_b[`at(1, 3) +: 8]
			+ matrix_a[`at(row, 2) +: 8] * matrix_b[`at(2, 3) +: 8]
			+ matrix_a[`at(row, 3) +: 8] * matrix_b[`at(3, 3) +: 8]
			+ matrix_a[`at(row, 4) +: 8] * matrix_b[`at(4, 3) +: 8];
			
		result[`at(row, 4) +: 8] <= 
			  matrix_a[`at(row, 0) +: 8] * matrix_b[`at(0, 4) +: 8]
			+ matrix_a[`at(row, 1) +: 8] * matrix_b[`at(1, 4) +: 8]
			+ matrix_a[`at(row, 2) +: 8] * matrix_b[`at(2, 4) +: 8]
			+ matrix_a[`at(row, 3) +: 8] * matrix_b[`at(3, 4) +: 8]
			+ matrix_a[`at(row, 4) +: 8] * matrix_b[`at(4, 4) +: 8];

		if (row == 4) begin
		  row <= 0;
		end else begin
		  row <= row + 1;
		end
    end
endmodule