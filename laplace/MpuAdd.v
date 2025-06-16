`define i8(x) 8'sd``x                       /// defined a 8-bit integer
`define MATRIX_5x5 (0):(8*25-1)             /// defines a 5x5 matrix flatted array indexes
`define at(col, row) (8 * (row + 5*col))    /// Access each 8-bit element in the 5x5 matrix

module MpuAdd (
    input      signed [`MATRIX_5x5] matrix_a,  // 5x5 8-bits matrix
    input      signed [`MATRIX_5x5] matrix_b,  // 5x5 8-bits matrix
    output reg signed [`MATRIX_5x5] result     // 5x5 8-bits matrix
);

    genvar col, row;
    generate
        for (col = 0; col < 5; col = col + 1) begin : col_loop
            for (row = 0; row < 5; row = row + 1) begin : row_loop
                always @(*) begin
                    result[`at(col, row) +: 8] = 
                        matrix_a[`at(col, row) +: 8] + matrix_b[`at(col, row) +: 8];
                end
            end
        end
    endgenerate

endmodule