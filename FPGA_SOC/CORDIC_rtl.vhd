library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity CORDIC_rtl is
	port(
			--input
			clk:   in  std_logic;
			phi:   in  signed(15 downto 0);
			n:	     in  unsigned(7 downto 0);
			start: in  std_logic;
			rst:    in  std_logic;
			--output
			rdy:   out std_logic;
			r1:     out signed(15 downto 0);
			r2:	 out signed(15 downto 0)
		 );
end entity;

architecture rtl of CORDIC_rtl is

		
	--definition theata_i et A_n
	type type1 is array(0 to 14) of signed(15 downto 0);
	type type2 is array(0 to 14) of signed(15 downto 0);
	
	--initialisation des tables:
	constant theta: type1:=("0011111000011100","0010010010101010","0001001101011111",
										   "0000100111010101","0000010011101111","0000001001111000",
										   "0000000100111100","0000000010011110","0000000001001111",
										   "0000000000100111","0000000000010011","0000000000001001",
										   "0000000000000100","0000000000000010","0000000000000001");
	
	constant A: type2		:=("0101101010000001","0101000011110011","0100111010001000",
										    "0100110111101100","0100110111000101","0100110110111011",
											"0100110110111000","0100110110110111","0100110110110110",
										    "0100110110110101","0100110110110100","0100110110110011",
											"0100110110110010","0100110110110001","0100110110110000");
	
	--definition des états
	type   t_state is(repos,calcul);
	signal state: t_state;
	
	--declaration des signaux
	signal alpha: signed(15 downto 0);
	signal x: 	      signed(15 downto 0);
	signal y: 	      signed(15 downto 0);
	signal r_phi: signed(15 downto 0);
	signal r_n:    unsigned(7 downto 0);
	signal i:        unsigned(7 downto 0);
		
begin
	process(clk)
	begin
		--sur un reset le calcul est interrompu et on revient a l'etat initial 
		if rst='1' then
			rdy<='1';
			state<= repos;
		elsif clk'event and clk='1' then
			case state is
				when repos =>
					if(start='1') then
						rdy   <='0';				
						r_phi <=phi;		   --on stock la valeur de l'angle presente a l entree
						r_n   <=n;			   --on stock le nombre d iteration desire
						x	  <=A(to_integer(n));
						y	  <=(others=>'0');
						alpha <=(others=>'0');
						i	  <=(others=>'0');
						state <=calcul;
					end if;
					
				when calcul=>
					if(r_n>0) then
						if(alpha < r_phi) then 
							r_n   <=r_n-1;
							
							--on remplace le terme y*d de l'algorithme par "i" décalage à droite
							x	   <=x-shift_right(y, to_integer(i));
							y	   <=y+shift_right(x, to_integer(i));
							
							alpha <=alpha+theta(to_integer(i));
							i	   <=i+1;	
						else
							r_n   <=r_n-1;
							x	   <=x+shift_right(y, to_integer(i));
							y	   <=y-shift_right(x, to_integer(i));
							alpha  <=alpha-theta(to_integer(i));
							i	   <=i+1;
						end if;
					elsif(r_n=0) then
						rdy   <='1';
						r1    <=x;
						r2    <=y;
						state <=repos;
					end if;
			end case;
		end if;
	end process;
end architecture;