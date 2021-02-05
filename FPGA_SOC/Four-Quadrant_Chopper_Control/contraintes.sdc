# constrain 50 MHz clock
create_clock -name "clk_50MHz" -period 20.000ns [get_ports clk_clk ]

#constrain PLL and other generated clocks
derive_pll_clocks -create_base_clocks

#Automatically calculate clock uncertainty to jitter and other effects
derive_clock_uncertainty