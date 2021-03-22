library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity calc_param is
		port(
	 	    --inputs
		    clk       : in std_logic;
		    clr       : in std_logic;             
		    fin_cycle : in std_logic;             --from out_gen block
		    per       : in unsigned(15 downto 0); --in [4000,20000]
		    rapp_cycl : in unsigned(15 downto 0); --given in the range [0,1023]
		    tm        : in unsigned(7  downto 0); --dead time given between 0 and 255 	  
		    --outputs
		    s1	: out unsigned(15 downto 0);
		    s2	: out unsigned(15 downto 0);
		    s3	: out unsigned(15 downto 0)
		);
end entity;

architecture arch of calc_param is

	--register input signals (good practice!!)
	signal reg_per 	     : unsigned(15 downto 0); 
	signal reg_rapp_cycl : unsigned(15 downto 0);
	signal reg_tm        : unsigned(7 downto 0);	
	
	--register output signals
	signal reg_s1 :  unsigned(15 downto 0);
	signal reg_s2 :  unsigned(15 downto 0);
	signal reg_s3 :  unsigned(15 downto 0);
	
begin
	
	--capture inputs signals
	CAPTURE:process(clk,clr)
	begin
	   --asynchronous reset
		if (clr='1') then
			reg_per       <= (others => '0');
			reg_rapp_cycl <= (others => '0');
			reg_tm	      <= (others => '0');
			
		elsif (clk'event and clk='1') then
			reg_per       <= per;
			reg_rapp_cycl <= rapp_cycl;
			reg_tm	      <= tm;
		end if;	
	end process;
	
	--assigning registred outputs signals
	ASSIGN_OUTPUTS: process(clk,clr,fin_cycle)
	begin	
	
		if(clr = '1') then 
			s1<= (others => '0');
			s2<= (others => '0');
			s3<= (others => '0');
		elsif(clk'event and clk='1') then	
		       if (fin_cycle='1') then
		 	   s1 <= reg_s1;
			   s2 <= reg_s2;
			   s3 <= reg_s3;
		       end if;			
		end if;
	end process;
	
	--compute parametrs 
	CALCUL_PARAM: process(clk,clr)
	begin	
		if (clr='1') then
		
			reg_s1<= (others => '0');
			reg_s2<= (others => '0');
			reg_s3<= (others => '0');
			
		elsif(clk'event and clk='1') then	
		
			reg_s1 <= resize(shift_right(reg_rapp_cycl*reg_per,11),s1'length); --duty cycle in range [0,1024] corresponds to [0,1] in hps 
																									 --S1=(duty_cycle/10)*periode/2
			reg_s2 <= resize((shift_right((reg_rapp_cycl*reg_per),11)+shift_right(reg_tm,1)),s2'length); --S2=S1+tm/2
			reg_s3 <= resize(shift_right(reg_per,1),s3'length); --S3=per/2
		end if;
		
	end process;

end arch;
