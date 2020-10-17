library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity tb is
end entity;

architecture seq of tb is

	--definition des facteurs multiplicateur
	constant FACT_MUL_1: real:= 32767.0; 
	constant FACT_MUL_2: real:= 353.35;
	
	--les fonctions de conversion fp2fix() et fix2fp()
	function fp2fix(x: real; M: real) return signed is
	begin
		return to_signed(integer(x*M),16);
	end;
	
	function fix2fp(x: signed(15 downto 0);M: real) return real is 
	begin
		return real(to_integer(x))/M;
	end;	
	
	component CORDIC_rtl is
		port(
				clk:   in  std_logic;
				phi:   in  signed(15 downto 0);
				n:	     in  unsigned(7 downto 0);--natural range 0 to 14;
				start: in  std_logic;
				rst:    in  std_logic;
				rdy:   out std_logic;
				r1:     out signed(15 downto 0);
				r2:	 out signed(15 downto 0)
		   );
	end component;
	
	--definition des signaux
	signal clk,start,rst,rdy : std_logic;
	signal phi,r1,r2	       :  signed(15 downto 0);
	signal n				       :  unsigned(7 downto 0);
	
	--on déclare ces signaux pour juste visualiser les résultat en virgule flottante
	signal r1_f,r2_f,phi_f: real;
	
begin

	--instantiation du composant 
	DUT: CORDIC_rtl port map(clk,phi,n,start,rst,rdy,r1,r2);
	
	CLOCK:process
	begin
		clk<='1'; wait for 5 ns;
		clk<='0'; wait for 5 ns;
	end process;
	
	STIM:process
	begin
		rst<='1'; wait for 2 ns;
		--on choisi ici 11 itérations
		rst<='0';start<='1'; phi<=fp2fix(60.0,FACT_MUL_2);n<="00001011"; wait for 10 ns;	
		start<='0';wait for 200 ns;
		
		start<='1'; phi<=fp2fix(38.0,FACT_MUL_2); wait for 10 ns;
		start<='0';wait for 200 ns;		
		wait;
	end process;	
		
		--En parallèle au process de calcul, les signaux prend 
		--les valeures courantes des résultats de cos et sin
		phi_f <=fix2fp(phi,FACT_MUL_2);
		r1_f  <=fix2fp(r1,FACT_MUL_1);
		r2_f  <=fix2fp(r2,FACT_MUL_1);
	
end architecture;

