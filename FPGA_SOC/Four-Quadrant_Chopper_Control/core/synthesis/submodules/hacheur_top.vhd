library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity hacheur_top is
		port(
			 --clk & rst inputs
			 csi_clock  	 : in  std_logic; --clock sink input
			 rsi_reset  	 : in  std_logic; --reset sink input		 
			
			 --secure
			 secure 	 		 : in std_logic;  
			 
			 --pwm outpus			 
			 A 				 : out std_logic;
			 B				    : out std_logic;
			 
			 --fin_cycle flag output
			 ins_end_cycle  : out std_logic;	--interrupt sende
			 
			 --avmm interface
			 avs_address    : in  std_logic_vector(1  downto 0); 			 
			 avs_byteenable : in  std_logic_vector(1  downto 0);
			 avs_read   	 : in  std_logic;			 
			 avs_readdata   : out std_logic_vector(15 downto 0);
			 avs_write   	 : in  std_logic;
			 avs_writedata  : in  std_logic_vector(15 downto 0)
			 );
end entity;

architecture struct of hacheur_top is

	--instantiation of the controller
	component controller is
		port(
				clock           : in  std_logic;
				reset 	       : in  std_logic;

				--end cycle
				end_cycle_in    : in  std_logic;
				end_cycle_out   : out std_logic;

				--secure
				secure_in 		 : in  std_logic;  

				--avmm interface
				avmm_address    : in  std_logic_vector(1  downto 0); 			 
				avmm_byteenable : in  std_logic_vector(1  downto 0);
				avmm_read   	 : in  std_logic;			 
				avmm_readdata   : out std_logic_vector(15 downto 0);
				avmm_write   	 : in  std_logic;
				avmm_writedata  : in  std_logic_vector(15 downto 0);

				per			    : out unsigned(15 downto 0);
				rapp_cyc        : out unsigned(15 downto 0);
				tm	             : out unsigned(7  downto 0);
				clr             : out std_logic
			);
	end component;

		
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
	
	--define interconnections signals
	signal fin_cycle_s      : std_logic;
	signal s1_s,s2_s,s3_s   : unsigned(15 downto 0);	
	signal sn_s,sp_s        : std_logic;
	signal clr_s            : std_logic;
	signal per_s 				: unsigned(15 downto 0);
	signal rapp_cyc_s 		: unsigned(15 downto 0);
	signal tm_s             : unsigned(7  downto 0);
	
	--register outputs
	signal reg_A, reg_B   : std_logic;
	
begin
	
	controller_b : controller port map (
													clock           => csi_clock,          
													reset 	       => rsi_reset,
													end_cycle_in    => fin_cycle_s, 
													end_cycle_out   => ins_end_cycle,
													secure_in 		 => secure,
													avmm_address    => avs_address, 		 
													avmm_byteenable => avs_byteenable,
													avmm_read   	 => avs_read,	 
													avmm_readdata   => avs_readdata,
													avmm_write   	 => avs_write,
													avmm_writedata  => avs_writedata,
													per			    => per_s,
													rapp_cyc        => rapp_cyc_s,
													tm	             => tm_s,
													clr             => clr_s     
													);

	calc_param_b : calc_param port map (clk        => csi_clock,
													clr        => clr_s,
												   fin_cycle  => fin_cycle_s,
												   per        => per_s,      
												   rapp_cycl  => rapp_cyc_s, 
												   tm			  => tm_s,       
												   s1			  => s1_s,
												   s2         => s2_s,
												   s3         => s3_s
													);
										 
	out_gen_b    : out_gen   port map  (clk		 => csi_clock,										 
									 			   clr		 => clr_s,
												   s1			 => s1_s,
												   s2			 => s2_s,
												   s3		    => s3_s,	 
												   sn        => sn_s,	
												   sp        => sp_s,			
												   fin_cycle => fin_cycle_s
													);	
		
	--process for registring outputs signals
	REGISTER_OUTPUTS:process(csi_clock,rsi_reset)
	begin		
		if (rsi_reset='1') then
			reg_A <= '0';
			reg_B <= '0';
		elsif (csi_clock'event and csi_clock='1') then 
			reg_A <= sn_s;
			reg_B <= sp_s;
		end if;	
	end process;	


	ASSIGN_OUTPUTS: process(csi_clock,rsi_reset,secure)
	begin
		if (rsi_reset='1') then
			A<='0';
			B<='0';
		elsif (secure='1') then
			A<='0';
			B<='0';
		elsif (csi_clock'event and csi_clock='1') then 
				A<=reg_A;
				B<=reg_B;
		end if;
	end process;

	
end struct;		 