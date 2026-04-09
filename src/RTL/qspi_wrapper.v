module riscv_qspi_wrapper #(
    parameter BASE_ADDR = 32'hC2000000
)(
    input  wire        clk,
    input  wire        rst_n,

    input  wire [31:0] cpu_addr,
    input  wire [31:0] cpu_data_in,
    output reg  [31:0] cpu_data_out,
    input  wire        cpu_rd_strobe,
    input  wire [3:0]  cpu_wr_strobe,
    //output wire        cpu_ready,

    output wire        qspi_cs_n,
    output wire        qspi_clk,
    inout  wire [3:0]  qspi_io
);

    // ---------------------------
    // REGISTER FILE
    // ---------------------------
    reg [7:0]  reg_cmd;
    reg [23:0] reg_addr;
    reg [31:0] reg_wdata;

    reg        reg_mode;
    reg        reg_quad_addr;
    reg        reg_burst_en;
    reg [3:0]  reg_dummy;

    reg        start_req;
    reg        start_pulse;

    wire       qspi_busy;
    wire       qspi_done;
    wire [31:0] qspi_rdata;

    wire base_hit = (cpu_addr[31:24] == BASE_ADDR[31:24]);

    // ---------------------------
    // WRITE LOGIC
    // ---------------------------
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            reg_cmd       <= 0;
            reg_addr      <= 0;
            reg_wdata     <= 0;
            reg_mode      <= 0;
            reg_quad_addr <= 0;
            reg_burst_en  <= 0;
            reg_dummy     <= 0;
            start_req     <= 0;
        end else begin
            start_req <= 0;

            if (base_hit && |cpu_wr_strobe) begin
                case (cpu_addr[7:0])

                    // 0x00: WRITE DATA
                    8'h00: reg_wdata <= cpu_data_in;

                    // 0x04: CONTROL
                    // [0] start
                    // [1] mode
                    // [2] quad_addr
                    // [3] burst_en
                    // [7:4] dummy cycles
                    // [15:8] cmd
                    8'h04: begin
                        reg_mode      <= cpu_data_in[1];
                        reg_quad_addr <= cpu_data_in[2];
                        reg_burst_en  <= cpu_data_in[3];
                        reg_dummy     <= cpu_data_in[7:4];
                        reg_cmd       <= cpu_data_in[15:8];
                        //start_req     <= cpu_data_in[0];
			            start_req <= cpu_data_in[0] & ~qspi_busy;
                    end

                    // 0x08: ADDRESS
                    8'h08: reg_addr <= cpu_data_in[23:0];

                endcase
            end
        end
    end

    // ---------------------------
    // START PULSE GENERATION
    // ---------------------------
   reg start_d;

always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        start_d <= 0;
        start_pulse <= 0;
    end else begin
        start_d <= start_req;
        start_pulse <= start_req & ~start_d; // rising edge detect
    end
end

    // ---------------------------
    // QSPI CORE
    // ---------------------------
    qspi_master core (
        .clk(clk),
        .rst_n(rst_n),

        .start(start_pulse),
        .cmd(reg_cmd),
        .addr(reg_addr),
        .data_in(reg_wdata),
        .data_out(qspi_rdata),

        .mode(reg_mode),
        .dummy_len(reg_dummy),
        .quad_addr(reg_quad_addr),
        .burst_en(reg_burst_en),

        .busy(qspi_busy),
        .done(qspi_done),

        .qspi_cs_n(qspi_cs_n),
        .qspi_clk(qspi_clk),
        .qspi_io(qspi_io)
    );

    // ---------------------------
    // READ LOGIC
    // ---------------------------
    always @(posedge clk) begin
        cpu_data_out <= 32'd0;

        if (base_hit && cpu_rd_strobe) begin
            case (cpu_addr[7:0])

                // 0x00: READ DATA
                8'h00: cpu_data_out <= qspi_rdata;

                // 0x0C: STATUS
                // [0] busy
                // [1] done
                8'h0C: cpu_data_out <= {30'd0, qspi_done, qspi_busy};

                // 0x10: DEBUG (optional)
                8'h10: cpu_data_out <= {8'd0, reg_cmd, reg_addr};

                default: cpu_data_out <= 32'd0;
            endcase
        end
    end

    // ---------------------------
    // READY SIGNAL
    // ---------------------------
    // Immediate ready (non-blocking MMIO)
   // assign cpu_ready = 1'b1;

endmodule