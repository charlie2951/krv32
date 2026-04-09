module seven_seg_mmio (
    input  wire        clk,      
    input  wire        resetn,   
    // MMIO Interface (Base: 0x11000000)
    input  wire [31:0] addr,
    input  wire [31:0] data_in,
    output reg  [31:0] data_out,
    input  wire        rstrb,
    input  wire [3:0]  wstrb,
    // Physical Pins
    output reg  [3:0]  an,       
    output reg  [7:0]  seg       
);

    reg [15:0] display_data;
    reg        mode_select; // Bit 17: 0 for Hex, 1 for Special Words

    wire addr_match = (addr == 32'h1100_0000);
    wire write_en   = addr_match && (|wstrb);

    // MMIO Write Logic
    always @(posedge clk or negedge resetn) begin
        if (!resetn) begin
            display_data <= 16'h0000;
            mode_select  <= 1'b0;
        end else if (write_en) begin
            if (wstrb[0]) display_data[7:0]   <= data_in[7:0];
            if (wstrb[1]) display_data[15:8]  <= data_in[15:8];
            if (wstrb[2]) mode_select         <= data_in[17]; 
        end
    end

    // MMIO Read Logic
    always @(*) begin
        if (addr_match && rstrb)
            data_out = {14'h0, mode_select, 1'b0, display_data};
        else
            data_out = 32'h0;
    end

    // Multiplexing logic
    reg [18:0] refresh_counter;
    always @(posedge clk or negedge resetn) begin
        if (!resetn) refresh_counter <= 0;
        else         refresh_counter <= refresh_counter + 1;
    end

    wire [1:0] active_digit = refresh_counter[18:17];
    reg  [3:0] current_hex;
    
    // Anode and Digit selection
    always @(*) begin
        case(active_digit)
            2'b00: begin an = 4'b1110; current_hex = display_data[3:0];   end
            2'b01: begin an = 4'b1101; current_hex = display_data[7:4];   end
            2'b10: begin an = 4'b1011; current_hex = display_data[11:8];  end
            2'b11: begin an = 4'b0111; current_hex = display_data[15:12]; end
            default: begin an = 4'b1111; current_hex = 4'h0; end
        endcase
    end

    // Character Decoder
    always @(*) begin
        if (mode_select) begin
            // Special Word Mode (based on display_data[3:0])
            case(display_data[3:0])
                4'h1: begin // "DONE"
                    case(active_digit)
                        2'b11: seg = 8'hA1; // d
                        2'b10: seg = 8'hC0; // O
                        2'b01: seg = 8'hAB; // n
                        2'b00: seg = 8'h86; // E
                        default: seg = 8'hFF;
                    endcase
                end
                4'h2: begin // "BOOT"
                    case(active_digit)
                        2'b11: seg = 8'h83; // b
                        2'b10: seg = 8'hC0; // O
                        2'b01: seg = 8'hC0; // O
                        2'b00: seg = 8'h87; // t
                        default: seg = 8'hFF;
		     endcase
                end
		4'h3: begin // "FAIL"
                    case(active_digit)
                        2'b11: seg = 8'h8E; // F
                        2'b10: seg = 8'h88; // A
                        2'b01: seg = 8'hF9; // I 
                        2'b00: seg = 8'hC7; // L
                        default: seg = 8'hFF;
                    endcase
                end
                default: seg = 8'hBF; // Dash "-" for unknown word
            endcase
        end else begin
            // Standard Hex Mode
            case(current_hex)
                4'h0: seg = 8'hC0; 4'h1: seg = 8'hF9; 4'h2: seg = 8'hA4; 4'h3: seg = 8'hB0;
                4'h4: seg = 8'h99; 4'h5: seg = 8'h92; 4'h6: seg = 8'h82; 4'h7: seg = 8'hF8;
                4'h8: seg = 8'h80; 4'h9: seg = 8'h90; 4'hA: seg = 8'h88; 4'hB: seg = 8'h83;
                4'hC: seg = 8'hC6; 4'hD: seg = 8'hA1; 4'hE: seg = 8'h86; 4'hF: seg = 8'h8E;
                default: seg = 8'hFF;
            endcase
        end
    end

endmodule