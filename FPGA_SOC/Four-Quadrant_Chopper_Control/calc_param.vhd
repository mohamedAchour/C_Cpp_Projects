library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity calc_param is
		port(
			  --inputs
			  clk 	   : in std_logic;
			  clr 		: in std_logic;             --set outputs to 0!
			  fin_cycle : in std_logic;             --from out_gen block
			  per 		: in unsigned(15 downto 0); --k in [4000,20000]
			  rapp_cycl : in unsigned(15 downto 0); --at this stage we give it in [0,1023]
			  tm 		   : in unsigned(7  downto 0); --give values in [0,255] 	  
			  --outputs
			  s1			: out unsigned(15 downto 0);
			  s2			: out unsigned(15 downto 0);
			  s3			: out unsigned(15 downto 0)
		);
end entity;

architecture arch of calc_param is

	--capture inputs signals
--	signal reg_per 		: unsigned(15 downto 0); 
--	signal reg_rapp_cycl : unsigned(15 downto 0);
--	signal reg_tm        : unsigned(7 downto 0);	
--	
	--signals for registering outputs
	signal reg_s1 :  unsigned(15 downto 0);
	signal reg_s2 :  unsigned(15 downto 0);
	signal reg_s3 :  unsigned(15 downto 0);
	
begin
	
	--capture inputs signals
--	CAPTURE:process(clk,clr)
--	begin
--		if (clr='1') then
--			reg_per 		  <= (others => '0');
--			reg_rapp_cycl <= (others => '0');
--			reg_tm		  <= (others => '0');
--		elsif (clk'event and clk='1') then
--			reg_per 		  <= per;
--			reg_rapp_cycl <= rapp_cycl;
--			reg_tm		  <= tm;
--		end if;	
--	end process;
	
	--calcul parametrs 
	
	ASSIGN_OUTPUTS: process(clk,clr,fin_cycle)
	begin				
		if(clk'event and clk='1') then
		
			if(clr = '1') then 
				s1<= (others => '0');
				s2<= (others => '0');
				s3<= (others => '0');
			elsif (fin_cycle='1') then
			--right shift to calcul the division
				s1 <= reg_s1;
				s2 <= reg_s2;
				s3 <= reg_s3;
			end if;
			
		end if;
	end process;
	
	CALCUL_PARAM: process(clk,clr)
	begin				
		if(clk'event and clk='1') then
		
			--synchrone clear
			if(clr = '1') then 
				reg_s1<= (others => '0');
				reg_s2<= (others => '0');
				reg_s3<= (others => '0');
			
			--right shift to calcul the division
			else 
				reg_s1 <= resize(shift_right(rapp_cycl*per,11),s1'length); --11 because we devide by /10 and /2
				reg_s2 <= resize((shift_right((rapp_cycl*per),11)+shift_right(tm,1)),s2'length);
				reg_s3 <= resize(shift_right(per,1),s3'length);
			end if;	
		end if;
	end process;

end arch;