library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity controller is
	port ( 
	     clock           : in  std_logic;
	     reset 	     : in  std_logic; 
	
	     --end cycle
	     end_cycle_in    : in  std_logic;
	     end_cycle_out   : out std_logic;

	     --secure
	     secure_in 	     : in  std_logic;  
			
	     --avmm signals
	     avmm_address    : in  std_logic_vector(1  downto 0); 			 
	     avmm_byteenable : in  std_logic_vector(1  downto 0);
	     avmm_read       : in  std_logic;			 
	     avmm_readdata   : out std_logic_vector(15 downto 0);
	     avmm_write      : in  std_logic;
	     avmm_writedata  : in  std_logic_vector(15 downto 0);
			
	     per	     : out unsigned(15 downto 0);
	     rapp_cyc        : out unsigned(15 downto 0);
	     tm	             : out unsigned(7  downto 0);
			
	     --the controller bloc assigns reset signals to "calc_param" and "out_gen" blocs
	     clr             : out std_logic
	    );
end entity;

architecture Behavior of controller is

	--define registers
	signal state_reg    : std_logic_vector(15 downto 0); --@00 : state_reg(0) -->M/A, state_reg(1) -->secure,
																				  --state_reg(2) -->interrupt enable,state_reg(3) -->interrupt flag.
	signal per_reg      : unsigned(15 downto 0);         --@01
	signal rapp_cyc_reg : unsigned(15 downto 0);	     --@10
	signal tm_reg       : unsigned(7  downto 0);         --@11
	
	signal control_sign : std_logic_vector(5 downto 0);
	
begin
	
	--concatenate signals (to ease the control)
	control_sign <=   avmm_read & avmm_write & avmm_byteenable & avmm_address;
	
	process(clock,reset)
	begin
	
		if clock'event and clock = '1'then
		
			if reset =  '1' then	
							
				end_cycle_out <= '0';				
				--set clear to one
				clr           <= '1';
			else
				--unset clear signal
				clr           <= '0';
				case control_sign is 
				
					--this corresponds to state register writing 
					when "011100"   => 
						--state_reg(3) -->interrupt flag & state_reg(1)-->secure are read only(we use mask)
						state_reg   <= avmm_writedata and "0000000000000101"; 
					
					--this corresponds to period register writing 
					when "011101"   => 
						per_reg      <= unsigned(avmm_writedata);
					
					--this corresponds duty cycle register writing 
					when "011110"   => 
						rapp_cyc_reg <= unsigned(avmm_writedata);
						
					--this corresponds dead time register writing 	
					when "011111"   => 
						tm_reg       <= unsigned(avmm_writedata(7 downto 0));  
					
					--this corresponds to state register reading 
					when "101100"   => 
						avmm_readdata<= state_reg;
					
					--this corresponds to period register reading
					when "101101"   => 
						avmm_readdata<= std_logic_vector(per_reg);
						
					--this corresponds duty cycle register reading
					when "101110"   => 
						avmm_readdata<= std_logic_vector(rapp_cyc_reg);
					
					--this corresponds dead time register reading 
					when "101111"   => 
						avmm_readdata<= (15 downto 8 => '0') & std_logic_vector(tm_reg);
					when others =>	
						avmm_readdata <= (others => '0');						
					
				end case;		
					
				--handle state registre bits
				state_reg(1) <= secure_in; --send secure signal to hps
				
				if state_reg(2)= '1' then --if Interrupt flag is enabled
				
					end_cycle_out <=end_cycle_in; --set interrupt
					state_reg(3)  <=end_cycle_in;
					
				elsif (state_reg(2)= '0') then --if Interrupt flag is disabled
					state_reg(3)  <='0';   --set flag bit to 0 (as default value)
				end if;	
				
			end if;		
		end if;
	end process;	
	
	
	--assign outputs process
	process(clock,reset)
	begin
		if clock'event and clock = '1'then
		
			if reset =  '1' then			
				per      <= (others => '0');
				rapp_cyc <= (others => '0');
				tm       <= (others => '0');
				
			elsif state_reg(0)= '1' then --if M/A bit =1 --> system is down
				per      <= (others => '0');
				rapp_cyc <= (others => '0');
				tm       <= (others => '0');
			else			
				per      <= per_reg ;
				rapp_cyc <= rapp_cyc_reg;
				tm       <= tm_reg;		
			end if;
		end if;
	end process;
end architecture;
