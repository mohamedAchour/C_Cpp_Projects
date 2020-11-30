library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity hps_control is
	port(
		  clk  	 : in std_logic;
		  clr  	 : in std_logic;
		  etat	 : in std_logic_vector(15 downto 0); --master is 32bits ==> byetenable : 0011
		  periode : in std_logic_vector(15 downto 0);
		  th 		 : in std_logic_vector(15 downto 0);
		  tm		 : in std_logic_vector(15 downto 0);		  
		 );
end entity;

architecture beh of hps_control is
begin
end architecture;