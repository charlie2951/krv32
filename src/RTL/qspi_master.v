module qspi_master #(
    parameter CPOL = 0,
    parameter CPHA = 0,
    parameter CLK_DIV = 10   // system clk / (2*CLK_DIV) = SPI clk
)(
    input  wire        clk,
    input  wire        rst_n,

    input  wire        start,
    input  wire [7:0]  cmd,
    input  wire [23:0] addr,
    input  wire [31:0] data_in,
    output reg  [31:0] data_out,

    input  wire        mode,        // 0=read, 1=write
    input  wire [3:0]  dummy_len,
    input  wire        quad_addr,   // 1 = quad address phase
    input  wire        burst_en,

    output reg         busy,
    output reg         done,

    output reg         qspi_cs_n,
    output wire        qspi_clk,
    inout  wire [3:0]  qspi_io
);

    // -------------------------------
    // SPI CLOCK GENERATION
    // -------------------------------
    reg [15:0] clk_cnt;
    reg spi_clk_int;
    reg spi_clk_en;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            clk_cnt <= 0;
            spi_clk_int <= CPOL;
        end else if (spi_clk_en) begin
            if (clk_cnt == CLK_DIV-1) begin
                clk_cnt <= 0;
                spi_clk_int <= ~spi_clk_int;
            end else begin
                clk_cnt <= clk_cnt + 1;
            end
        end else begin
            spi_clk_int <= CPOL;
            clk_cnt <= 0;
        end
    end

    assign qspi_clk = (!qspi_cs_n) ? spi_clk_int : CPOL;

    wire sample_edge = (CPHA==0) ? (spi_clk_int==1) : (spi_clk_int==0);
    wire drive_edge  = (CPHA==0) ? (spi_clk_int==0) : (spi_clk_int==1);

    // -------------------------------
    // IO CONTROL
    // -------------------------------
    reg [3:0] io_out;
    reg [3:0] io_oe; // 1 = output

    assign qspi_io = {
        io_oe[3] ? io_out[3] : 1'bz,
        io_oe[2] ? io_out[2] : 1'bz,
        io_oe[1] ? io_out[1] : 1'bz,
        io_oe[0] ? io_out[0] : 1'bz
    };

    wire [3:0] io_in = qspi_io;

    // -------------------------------
    // FSM
    // -------------------------------
    localparam IDLE=0, CMD=1, ADDR=2, DUMMY=3, DATA=4, STOP=5;

    reg [2:0] state;
    reg [5:0] bit_cnt;
    reg [31:0] shift_reg;

    // -------------------------------
    // MAIN FSM
    // -------------------------------
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            state <= IDLE;
            qspi_cs_n <= 1;
            busy <= 0;
            done <= 0;
            spi_clk_en <= 0;
            io_oe <= 4'b0000;
        end else begin
            done <= 0;

            case (state)
            // ---------------------------
            IDLE: begin
                if (start) begin
                    busy <= 1;
                    qspi_cs_n <= 0;
                    spi_clk_en <= 1;
                    shift_reg <= {cmd,24'd0};
                    bit_cnt <= 7;
                    state <= CMD;
                end
            end

            // ---------------------------
            CMD: if (drive_edge) begin
                io_oe <= 4'b0001;
                io_out <= {3'b000, shift_reg[31]};
                shift_reg <= {shift_reg[30:0],1'b0};

                if (bit_cnt == 0) begin
                    shift_reg <= {addr,8'd0};
                    bit_cnt <= 23;
                    state <= ADDR;
                end else bit_cnt <= bit_cnt - 1;
            end

            // ---------------------------
            ADDR: if (drive_edge) begin
                if (quad_addr) begin
                    io_oe <= 4'b1111;
                    io_out <= shift_reg[31:28];
                    shift_reg <= {shift_reg[27:0],4'd0};
                    if (bit_cnt <= 3) begin
                        bit_cnt <= 0;
                        state <= (dummy_len==0) ? DATA : DUMMY;
                    end else bit_cnt <= bit_cnt - 4;
                end else begin
                    io_oe <= 4'b0001;
                    io_out <= {3'b000, shift_reg[31]};
                    shift_reg <= {shift_reg[30:0],1'b0};
                    if (bit_cnt == 0) begin
                        state <= (dummy_len==0) ? DATA : DUMMY;
                    end else bit_cnt <= bit_cnt - 1;
                end
            end

            // ---------------------------
            DUMMY: if (drive_edge) begin
                io_oe <= 4'b0000;
                if (bit_cnt == dummy_len-1) begin
                    bit_cnt <= 7;
                    state <= DATA;
                end else bit_cnt <= bit_cnt + 1;
            end

            // ---------------------------
            DATA:
            if (mode) begin // WRITE
                if (drive_edge) begin
                    io_oe <= 4'b1111;
                    io_out <= shift_reg[31:28];
                    shift_reg <= {shift_reg[27:0],4'd0};
                    if (bit_cnt == 0) state <= STOP;
                    else bit_cnt <= bit_cnt - 1;
                end
            end else begin // READ
                if (sample_edge) begin
                    io_oe <= 4'b0000;
                    shift_reg <= {shift_reg[27:0], io_in};

                    if (bit_cnt == 0) begin
                        data_out <= {shift_reg[27:0], io_in};
                        if (burst_en) begin
                            bit_cnt <= 7;
                        end else begin
                            state <= STOP;
                        end
                    end else bit_cnt <= bit_cnt - 1;
                end
            end

            // ---------------------------
            STOP: begin
                qspi_cs_n <= 1;
                spi_clk_en <= 0;
                busy <= 0;
                done <= 1;
                state <= IDLE;
            end
            endcase
        end
    end
endmodule