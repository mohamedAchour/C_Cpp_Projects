#include <stdlib.h> 
#include <time.h>   //to generate random numbers

#define LWH_to_F_BRIDGE_BASE 0xFF200000 
#define FPGA_REGISTERS_BASE  0x00030000
#define FPGA_REGISTERS_BASE_IN_HPS LWH_to_F_BRIDGE_BASE + FPGA_REGISTERS_BASE

#define LOW_FREQ  4000  //correponds to 5Khz
#define HIGH_FREQ 20000 //correponds to 25Khz


uint32_t generateRandom(int lower, int upper);
void disable_A9_interrupts (void);
void set_A9_IRQ_stack (void);
void config_GIC (void);void config_KEYs (void);
void enable_A9_interrupts (void);


int main(void)
{
	volatile int *fpga_registers = (int*)(FPGA_REGISTERS_BASE_IN_HPS);

	float duty_cycle=0;      //given by the user [0,1]

	int state_reg;     
	int per_reg;       //range [4000,20000]
	int duty_c_reg;    //range [0,1024]
	int dead_time_reg;	

	disable_A9_interrupts ();   // disable interrupts in the A9 processor
	set_A9_IRQ_stack ();        // initialize the stack pointer for IRQ mode
	config_GIC ();              // configure the general interrupt controller
	enable_A9_interrupts ();    // enable interrupts in the A9 processor

	/*initialize registers with defaults values*/
	duty_cycle=0.5; //50%

	state_reg    = 0x0000; //interrupt flage is disabled
	per_reg      = 0x0FA0; //4000--> 5Khz
	duty_c_reg   = 0x000A;//(uint32_t)(duty_cycle*1024); //512-->50%
	dead_time_reg= 0x000A; //dead time = 10

	/*seed for random generator*/
    srand(time(0)); 


	/*0xdead_time_reg_duty_cyle_reg_periode_reg_state_reg*/
	*fpga_registers  =(dead_time_reg<<24) | (duty_c_reg <<16)| (per_reg <<8) | state_reg;

    while(1)
    {
		per_reg=generateRandom(LOW_FREQ, HIGH_FREQ);

		/*change periode register with random values*/
		*fpga_registers  =(dead_time_reg<<24) | (duty_c_reg <<16)| (per_reg <<8) | state_reg;
    }

	return 0;
}


uint32_t generateRandom(int lower, int upper)
{ 
	uint32_t rand_number=0;
     
     rand_number  = (rand() % (upper - lower + 1)) + lower; 

     return rand_number;
} 

/**Turn off interrupts in the ARM processor*/
void disable_A9_interrupts(void)
{
	int status = 0b11010011;
	asm("msr cpsr, %[ps]" : : [ps]"r"(status));
}

/**Initialize the banked stack pointer register for IRQ mode*/
void set_A9_IRQ_stack(void)
{
	int stack, mode;stack = 0xFFFFFFFF - 7; // top of A9 onchip memory, aligned to 8 bytes
	/*change processor to IRQ mode with interrupts disabled*/
	mode = 0b11010010;asm("msr cpsr, %[ps]" : : [ps] "r" (mode));
	/*set banked stack pointer*/
	asm("mov sp, %[ps]" : : [ps] "r" (stack));
	/*go back to SVC mode before executing subroutine return!*/
	mode = 0b11010011;asm("msr cpsr, %[ps]" : : [ps] "r" (mode));
}

/**Configure the Generic Interrupt Controller (GIC)*/
void config_GIC(void)
{
	config_interrupt (73, 1);    // configure the FPGA KEYs interrupt (73)
	// Set Interrupt Priority Mask Register (ICCPMR). Enable all priorities
	*((int*) 0xFFFEC104) = 0xFFFF;
	// Set the enable in the CPU Interface Control Register (ICCICR)
	*((int*) 0xFFFEC100) = 1;
	// Set the enable in the Distributor Control Register (ICDDCR)
	*((int*) 0xFFFED000) = 1;
}

/**Turn on interrupts in the ARM processor*/
void enable_A9_interrupts(void)
{
	int status = 0b01010011;
	asm("msr cpsr, %[ps]" : : [ps]"r"(status));
}