library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity tb_out_gen is
end entity;

architecture beh of tb_out_gen is

	component out_gen is
			port(
			    --inputs
			    clk       : in std_logic;
			    clr	      : in std_logic;
			    s1	      : in unsigned(15 downto 0);
			    s2	      : in unsigned(15 downto 0);
			    s3	      : in unsigned(15 downto 0);	  
			    --outputs
			    sn	      : out std_logic;
			    sp	      : out std_logic;
			    fin_cycle : out std_logic
			);
	end component;
	
	signal clk,clr,sn,sp,fin_cycle: std_logic;
	signal s1,s2,s3 : unsigned(15 downto 0);
	
begin

	DUT : out_gen port map(clk, clr,s1,s2,s3,sn,sp,fin_cycle);
	
	CLOCK: process
	begin
		clk <= '1'; wait for 5ns;
		clk <= '0'; wait for 5ns;
	end process;	
		
	STIM:process
	begin
		clr <= '0'; wait for 5ns;
		clr <= '1'; wait for 8ns;
		clr <= '0';
		s3 <= to_unsigned(100, 16); 
		s2 <= to_unsigned(55, 16); 
		s1 <= to_unsigned(54, 16); 		
		wait for 5000ns;
		s3 <= to_unsigned(0, 16); 
		s2 <= to_unsigned(0, 16); 
		s1 <= to_unsigned(0, 16); 	
		wait;
	end process;
end beh;
