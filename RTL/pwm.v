// PWM controller for KRV-32
//4 channel PWM with programmable duty cycle 0(0) and 100(255)
//individual channel control through enable pin
module pwm_4_channel (
    input clk,          // System clock
    input rst_n,        // Asynchronous reset (active low)
    input [3:0] enable, //start signal to generate pwm. 4bit for 4 individual channel
    input [7:0] duty_cycle_ch1, // Duty cycle for channel 1 (0 to 255)
    input [7:0] duty_cycle_ch2, // Duty cycle for channel 2 (0 to 255)
    input [7:0] duty_cycle_ch3, // Duty cycle for channel 3 (0 to 255)
    input [7:0] duty_cycle_ch4, // Duty cycle for channel 4 (0 to 255)
    output reg pwm_out_ch1,   // PWM output for channel 1
    output reg pwm_out_ch2,   // PWM output for channel 2
    output reg pwm_out_ch3,   // PWM output for channel 3
    output reg pwm_out_ch4    // PWM output for channel 4
  );

  parameter CLK_FREQ = 100_000_000; // Example 50 MHz clock frequency
  parameter PWM_FREQ = 500;       // Desired PWM frequency

  localparam COUNT_MAX = (CLK_FREQ / PWM_FREQ) - 1; // Calculate the maximum count value
  reg [19:0] counter;

  always @(posedge clk or negedge rst_n)
  begin
    if (!rst_n)
    begin
      counter <= 16'd0;
      pwm_out_ch1 <= 1'b0;
      pwm_out_ch2 <= 1'b0;
      pwm_out_ch3 <= 1'b0;
      pwm_out_ch4 <= 1'b0;
    end
    else
    begin
      if (counter == COUNT_MAX)
      begin
        counter <= 16'd0;
      end
      else
      begin
        counter <= counter + 1'b1;
        //$display(counter);
      end

      // Generate PWM output for channel 1
      if (counter < (duty_cycle_ch1 * (COUNT_MAX + 1)) / 256)
      begin
        pwm_out_ch1 <= 1'b1 & enable[0] ;
      end
      else
      begin
        pwm_out_ch1 <= 1'b0;
      end

      // Generate PWM output for channel 2
      if (counter < (duty_cycle_ch2 * (COUNT_MAX + 1)) / 256)
      begin
        pwm_out_ch2 <= 1'b1 & enable[1] ;
      end
      else
      begin
        pwm_out_ch2 <= 1'b0;
      end

      // Generate PWM output for channel 3
      if (counter < (duty_cycle_ch3 * (COUNT_MAX + 1)) / 256)
      begin
        pwm_out_ch3 <= 1'b1 & enable[2];
      end
      else
      begin
        pwm_out_ch3 <= 1'b0;
      end

      // Generate PWM output for channel 4
      if (counter < (duty_cycle_ch4 * (COUNT_MAX + 1)) / 256)
      begin
        pwm_out_ch4 <= 1'b1 & enable[3];
      end
      else
      begin
        pwm_out_ch4 <= 1'b0;
      end

    end
  end

endmodule
