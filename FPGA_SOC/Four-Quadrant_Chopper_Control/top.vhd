library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity top is
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
end entity;

architecture struct of top is
	
	--instantiation of parameters calculator
	component calc_param is
		port(
			  --inputs
			  clk 	   : in std_logic;
			  clr 		: in std_logic;             
			  fin_cycle : in std_logic;             
			  per 		: in unsigned(15 downto 0); 
			  rapp_cycl : in unsigned(15 downto 0); 
			  tm 		   : in unsigned(7 downto 0);	   
			  --outputs
			  s1			: out unsigned(15 downto 0);
			  s2			: out unsigned(15 downto 0);
			  s3			: out unsigned(15 downto 0)
		);
	end component;
	
	--instantiation of outputs generator 
	component out_gen is
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
	end component;
	
	--define interconnections
	signal fin_cycle_s    :std_logic;
	signal s1_s,s2_s,s3_s :unsigned(15 downto 0);	
	signal sn_s,sp_s      :std_logic;
	
	--register outputs
	signal reg_T1, reg_T2, reg_T3, reg_T4 : std_logic;
	
begin

	calc_param_b : calc_param port map (clk=>clk,
													clr=>clr,
												   fin_cycle=>fin_cycle_s,
												   per=>per,
												   rapp_cycl=>rapp_cycl,
												   tm=>tm,
												   s1=>s1_s,
												   s2=>s2_s,
												   s3=>s3_s);
										 
	out_gen_b :out_gen		  port map (clk=>clk,										 
												   clr=>clr,
												   s1=>s1_s,
												   s2=>s2_s,
												   s3=>s3_s,	 
												   sn=>sn_s,	
												   sp=>sp_s,			
												   fin_cycle=>fin_cycle_s);	
										 

	OUTPUTS: process(clk,clr,secure)
	begin
		if (clr='1') then
			T1<='0';
			T2<='0';
			T3<='0';
			T4<='0';
		elsif(secure='1') then
			T1<='0';
			T2<='0';
			T3<='0';
			T4<='0';
		elsif (clk'event and clk='1') then
			T1<=reg_T1;
			T2<=reg_T2;
			T3<=reg_T3;
			T4<=reg_T4;
		end if;
	end process;
	
	process(clk,clr)
	begin
		if (clr='1') then
			reg_T1<='0';
			reg_T2<='0';
			reg_T3<='0';
			reg_T4<='0';
		elsif (clk'event and clk='1') then
			reg_T1<=sn_s;
			reg_T2<=sp_s;
			reg_T3<=sn_s;
			reg_T4<=sp_s;
		end if;	
	end process;
	
end struct;		 