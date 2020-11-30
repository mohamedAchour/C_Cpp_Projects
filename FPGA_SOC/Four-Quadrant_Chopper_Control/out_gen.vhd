library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity out_gen is
		port(
			  --inputs
			  clk 		: in std_logic;
			  clr			: in std_logic;
			  s1			: in unsigned(15 downto 0);
			  s2			: in unsigned(15 downto 0);
			  s3			: in unsigned(15 downto 0);	  
			  --outputs
			  sn			: out std_logic;
			  sp			: out std_logic;
			  fin_cycle : out std_logic
			);
end entity;

architecture arch of out_gen is

	--states
	type state_t is (T0,T1,T2,T3,T4,T5);
	signal state : state_t;

--counter signal
signal count : unsigned (15 downto 0);

begin
	--state evolution
	EVOL:process(clk,clr)
	begin
		if (clr='1') then
			state<= T0;
			fin_cycle<='0';
		elsif (clk'event and clk='1') then
			case state is
				when T0 =>
					count <= (others => '0');
					fin_cycle<='1';       --set fin_cycle for first period
					state <= T1;
				when T1 =>	
					if (count<(s1-1)) then
						count <= count+1;
					elsif (count=(s1-1)) then									
						fin_cycle<='0';
						count <= count+1;
						state <= T2;
					end if;
				when T2 =>
					if (count<(s2-1)) then
						count <= count+1;
					elsif (count=(s2-1)) then
						count <= count+1;
						state <= T3;
					end if;
				when T3 =>
					if (count<(2*s3-s2-1)) then
						count <= count+1;
					elsif (count=(2*s3-s2-1)) then
						count <= count+1;
						state <= T4;
					end if;
				when T4 =>
					if (count<2*s3-s1-1) then
						count <= count+1;
					elsif (count=(2*s3-s1-1)) then
						count <= count+1;
						state <= T5;
					end if;
				when T5 =>	
					if (count<2*s3-1) then
						count <= count+1;
					elsif (count=2*s3-1) then
	 					--generate a flag 
						fin_cycle<='1';
						count    <=(others => '0');
						state    <=T1;
					end if;
			end case;			
		end if;
	end process;
	
	--assign outputs
	OUTPUTS:process(state)
	begin
		case state is
			when T0 => sn <='0'; sp <='0';
			when T1 => sn <='1'; sp <='0'; 
			when T2 => sn <='0'; sp <='0';
			when T3 => sn <='0'; sp <='1';
			when T4 => sn <='0'; sp <='0';
			when T5 => sn <='1'; sp <='0';
		end case;	
	end process;	
end arch;