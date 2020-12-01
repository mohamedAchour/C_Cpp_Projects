library ieee;
use ieee.std_logic_1164.all;

entity reg_file is
	    port(
		clk		: in  std_logic;
		clr 		: in  std_logic;
		avmm_byteenable : in  std_logic_vector(3  downto 0);
		avmm_write_data : in  std_logic_vector(31 downto 0);
		avmm_read_data  : out std_logic_vector(31 downto 0)
	        );
end entity;

architecture beh of reg_file is
begin
	
	process(clk,clr)
	begin
		if (clr='1') then
			avmm_read_data <=(others=>'0');
		elsif (clk'event and clk='1') then
		
			case avmm_byteenable is
				when "0001" =>
					avmm_read_data(7  downto 0) <= avmm_write_data(7  downto 0);
				when "0011" =>
					avmm_read_data(15 downto 0) <= avmm_write_data(15 downto 0);
				when "0111" =>
					avmm_read_data(23 downto 0) <= avmm_write_data(23 downto 0);
				when "1111" =>
					avmm_read_data(31 downto 0) <= avmm_write_data(31 downto 0);
			end case;
		end if;
	end process;
end beh;
