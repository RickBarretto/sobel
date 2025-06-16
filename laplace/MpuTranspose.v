`define i8(x) 8'sd``x                /// defined a 8-bit integer
`define MATRIX_5x5 (0):(8*25-1)     /// defines a 5x5 matrix flatted array indexes
`define at(col, row) (8 * (row + 5*col))    /// Access each 8-bit element in the 5x5 matrix

module MpuTranspose (
    input  wire signed [`MATRIX_5x5] matrix,  // 5x5 8-bit matrix
    output wire signed [`MATRIX_5x5] result   // 5x5 8-bit matrix
);

    genvar i, j;
    generate
        for (i = 0; i < 5; i = i + 1) begin : row_loop
            for (j = 0; j < 5; j = j + 1) begin : col_loop
                assign result[`at(i, j) +: 8] = matrix[`at(j, i) +: 8];
            end
        end
    endgenerate

endmodule