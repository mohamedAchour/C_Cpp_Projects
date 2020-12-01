library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity tb_calc_param is
end entity;

architecture arch of tb_calc_param is

	component calc_param is
	port(
	    --inputs
	    clk       : in std_logic;
	    clr       : in std_logic; 
	    fin_cycle : in std_logic;
	    per       : in unsigned(15 downto 0);
	    rapp_cycl : in unsigned(15 downto 0);
	    tm 	      : in unsigned(7  downto 0);			  
	    --outputs
	    s1	      : out unsigned(15 downto 0);
	    s2	      : out unsigned(15 downto 0);
	    s3	      : out unsigned(15 downto 0)
	    );  
	end component;
	
	signal clk,clr,fin_cycle        : std_logic;	
	signal per, rapp_cycl, s1,s2,s3 : unsigned(15 downto 0);
	signal tm                       : unsigned(7  downto 0);	
	
begin

	DUT : calc_param port map (clk,clr,fin_cycle,per,rapp_cycl,tm,s1,s2,s3);
	
	CLOCK: process
	begin
		clk <= '1'; wait for 5ns;
		clk <= '0'; wait for 5ns;
	end process;	
		
	STIM: process
	begin
		--initialization clear
		clr <= '0'; wait for 4ns;
		clr <= '1'; wait for 10ns;
		clr <= '0';			
		
		per <= to_unsigned(20000, per'length);   
		rapp_cycl <=to_unsigned(560,rapp_cycl'length); 
		tm<=to_unsigned(127, tm'length); 
		
		fin_cycle <= '0'; wait for 4ns;
		fin_cycle <= '1'; wait for 25ns;
		fin_cycle <= '0';
		
		wait for 50ns;			
		
		per <= to_unsigned(6000, per'length);   
		rapp_cycl <=to_unsigned(1000, rapp_cycl'length); 
		tm<=to_unsigned(120,tm'length); 
		wait for 50ns;	
		
		fin_cycle <= '0'; wait for 4ns;
		fin_cycle <= '1'; wait for 10ns;
		fin_cycle <= '0';
		
		wait;
	end process;
	
end architecture;
