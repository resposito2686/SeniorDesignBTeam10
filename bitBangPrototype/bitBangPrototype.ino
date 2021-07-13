int clockCount = 17;
int dataCount = 7;
uint8_t data[8] = {0,0,0,0,0,0,0,0};
uint8_t temp[8];

void setup() {
  ADC->ADC_MR |= ADC_MR_FREERUN | ADC_MR_LOWRES;
  ADC->ADC_CR = ADC_CR_START;
  ADC->ADC_CHER = ADC_CHER_CH7 | ADC_CHER_CH6 | ADC_CHER_CH5 | ADC_CHER_CH4 | ADC_CHER_CH3 | ADC_CHER_CH2 | ADC_CHER_CH1 | ADC_CHER_CH0;

  //PA15(P24):CLOCK, 
  PIOA->PIO_OER = PIO_OER_P15;
  PIOA->PIO_OWER = PIO_OWER_P15;
  
  //PC22(P8):LATCH, PC28(P3):SR3, PC26(P4):SR4, PC25(P5): SR5, PC24(P6): SR6, PC23(P7):SR7
  PIOC->PIO_OER = PIO_OER_P22 | PIO_OER_P28 | PIO_OER_P26 | PIO_OER_P25 | PIO_OER_P24 | PIO_OER_P23;
  PIOC->PIO_OWER = PIO_OWER_P22 | PIO_OWER_P28 | PIO_OWER_P26 | PIO_OWER_P25 | PIO_OWER_P24 | PIO_OWER_P23;
  
  //PD7(P11):SR0, PD8(P12):SR1
  PIOD->PIO_OER = PIO_OER_P7 | PIO_OER_P8;
  PIOD->PIO_OWER = PIO_OER_P7 | PIO_OER_P8;
  
  //PB25(P2):SR2
  PIOB->PIO_OER = PIO_OER_P25;
  PIOB->PIO_OWER = PIO_OER_P25;
  
  PMC->PMC_PCR = PMC_PCR_EN | PMC_PCR_CMD | (ID_TC0 & 0x7F);
  TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4;
  TC0->TC_CHANNEL[0].TC_RC = 219;
  TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
  NVIC_EnableIRQ(TC0_IRQn);
  TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;

  PIOA->PIO_ODSR |= PIO_ODSR_P15;
}

void loop() {
  if (clockCount < 0){
    clockCount = 17;
  }
  
  if (dataCount < 0){
    dataCount = 7;
    data[0] = 0xFF >> 8 - (ADC->ADC_CDR[7]/114);
    data[1] = 0xFF >> 8 - (ADC->ADC_CDR[6]/114);
    data[2] = 0xFF >> 8 - (ADC->ADC_CDR[5]/114);
    data[3] = 0xFF >> 8 - (ADC->ADC_CDR[4]/114);
    data[4] = 0xFF >> 8 - (ADC->ADC_CDR[3]/114);
    data[5] = 0xFF >> 8 - (ADC->ADC_CDR[2]/114);
    data[6] = 0xFF >> 8 - (ADC->ADC_CDR[1]/114);
    data[7] = 0xFF >> 8 - (ADC->ADC_CDR[0]/114);
  }
}

void TC0_Handler(){

  TC0->TC_CHANNEL[0].TC_SR;
  PIOA->PIO_ODSR ^= PIO_ODSR_P15;
  
  if (clockCount > 2) {
    PIOC->PIO_ODSR &= ~PIO_ODSR_P22;
    
    if ((clockCount & 0x01) != 0) {
      for (int i = 0; i < 8; i++){
        temp[i] = data[i] & (0x01 << dataCount);
        temp[i] >>= dataCount;
      }
      dataCount--;
      if (temp[0] == 0x01){
        PIOD->PIO_ODSR |= PIO_ODSR_P7;
      }
      else
      {
        PIOD->PIO_ODSR &= ~PIO_ODSR_P7;
      }
      if (temp[1] == 0x01){
        PIOD->PIO_ODSR |= PIO_ODSR_P8;
      }
      else
      {
        PIOD->PIO_ODSR &= ~PIO_ODSR_P8;
      }
      if (temp[2] == 0x01){
        PIOB->PIO_ODSR |= PIO_ODSR_P25;
      }
      else
      {
        PIOB->PIO_ODSR &= ~PIO_ODSR_P25;
      }
      if (temp[3] == 0x01){
        PIOC->PIO_ODSR |= PIO_ODSR_P28;
      }
      else
      {
        PIOC->PIO_ODSR &= ~PIO_ODSR_P28;
      }
      if (temp[4] == 0x01){
        PIOC->PIO_ODSR |= PIO_ODSR_P26;
      }
      else
      {
        PIOC->PIO_ODSR &= ~PIO_ODSR_P26;
      }      
      if (temp[5] == 0x01){
        PIOC->PIO_ODSR |= PIO_ODSR_P25;
      }
      else
      {
        PIOC->PIO_ODSR &= ~PIO_ODSR_P25;
      }      
      if (temp[6] == 0x01){
        PIOC->PIO_ODSR |= PIO_ODSR_P24;
      }
      else
      {
        PIOC->PIO_ODSR &= ~PIO_ODSR_P24;
      }      
      if (temp[7] == 0x01){
        PIOC->PIO_ODSR |= PIO_ODSR_P23;
      }
      else
      {
        PIOC->PIO_ODSR &= ~PIO_ODSR_P23;
      }
    }
  }
  else {
    PIOC->PIO_ODSR |= PIO_ODSR_P22;
  }
  clockCount--;
}
