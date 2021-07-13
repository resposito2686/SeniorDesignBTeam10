void setup() {
  /* ----- DIGITAL PIN WRITE CONFIG -----
   * Configures the selected pin as a digital output (Pin 4 in this example). 
   * The file "Arduino - PinMappingSAM3X.pdf" shows how the pins are mapped.
   * 
   * - PIOx, x=[A,B,C]: Parallel Input/Output Controller mapped to selected pin (page 618).
   * 
   * - OER: Output Enable Register. Sets the selected pin as an output (page 636).
   * 
   * - OWER: Output Write Enable Register. Write enable for selected pin (page 661).
   */
  PIOC->PIO_OER |= PIO_OER_P26;
  PIOC->PIO_OWER |= PIO_OWER_P26;
  
  /* ----- TIMER/INTERRUPT CONFIG -----
   *  Configures the selected timer to trigger an interrupt at a specified
   *  frequency.
   *  
   *  - PMC: Power Management Controller (page 526).
   *  
   *  - PCR: Paripheral Control Register. Enables the selected timer (page 566). 
   *    The file "TC_Property_Table.png" shows how each timer is mapped to it's 
   *    corresponding Nested Vector Interupt Controller (NVIC) ID, 
   *    Interrupt service routine handler, and Power Managment Controller ID.
   *    
   *  - TCx, x=[0,1,2]: Timer Counter 0, 1, or 2 (page 856). 
   *  
   *  - TC_CHANNEL[x], x=[0,1,2]: Channel for the selected timer (page 860).
   *  
   *  - CMR: Channel Mode Register. Configures the mode the timer will operate in along
   *    with it's pre-scale (page 881).
   *    
   *  - RC: Register C. Contains the value that will generate the timer's interrupt.
   *    Can be calculated by using the following:
   *    (TIMER_CLOCK_x / Desired Frequency). Maximum value = 2^32 - 1.
   *    
   *  - IER: Interrupt Enable Register. Enables the selected interrupts for the timer (page 894).
   *  
   *  - NVIC_EnableIRQ(): Enables the Nested Vector Interrupt Controller for the timer id in it's 
   *    argument. The file "TC_Property_Table.png" shows each timer's ID.
   *    
   *  - CCR: Channel Control Register. Starts the clock (page 880). 
   */

  PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_CMD | (ID_TC0 & 0x7F);
  TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4;
  TC0->TC_CHANNEL[0].TC_RC = 131200;
  TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
  NVIC_EnableIRQ(TC0_IRQn);
  TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
  
}

void loop() {}

/* ----- INTERRUPT SERVICE ROUTINE -----
 *  Interrupt service routine function for selected timer.
 *  
 *  - SR: Status Register. Stores the status of each timer interrupt.
 *    Clears interrupt status when read (page 892).
 *    
 *  - ODSR: Output Data Status Register. Outputs the value of each I/O line
 *    in the register (page 644).
 */
void TC0_Handler()
{
  TC0->TC_CHANNEL[0].TC_SR;
  PIOC->PIO_ODSR ^= PIO_ODSR_P26;
}
