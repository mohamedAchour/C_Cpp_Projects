library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity tb is
end entity;

architecture beh of tb is 

	component top is
	port(
		 --inputs
		 clk 		  : in std_logic;
		 clr 		  : in std_logic;
		 secure 	  : in std_logic;
		 per 		  : in unsigned(15 downto 0);
		 rapp_cycl : in unsigned(15 downto 0);
		 tm 		  : in unsigned(7 downto 0);
		 --outpus
		 
		 T1 : out std_logic;
		 T2 : out std_logic;
		 T3 : out std_logic;
		 T4 : out std_logic
		 );
	end component;
	
	signal  clk,clr,secure,T1,T2,T3,T4 : std_logic;
	signal  per, rapp_cycl : unsigned(15 downto 0);
	signal  tm : unsigned(7 downto 0);
	
begin

	top_b : top port map(clk,clr,secure,per,rapp_cycl,tm,T1,T2,T3,T4);
		
	CLOCK: process
	begin
		clk <= '1'; wait for 5ns;
		clk <= '0'; wait for 5ns;
	end process;
	
	STIM:process
	begin
		clr <= '0'; wait for 4ns;
		clr <= '1'; wait for 20ns;
		clr <= '0';
		
		per <= to_unsigned(200, per'length);   
		rapp_cycl <=to_unsigned(566, rapp_cycl'length); 
		tm<=to_unsigned(12,tm'length); 
		wait for 3000ns;
		
		secure<='1';
		
		per <= to_unsigned(100, per'length);   
		rapp_cycl <=to_unsigned(266, rapp_cycl'length); 
		tm<=to_unsigned(12,tm'length); 
		wait;
	end process;
	
end beh;