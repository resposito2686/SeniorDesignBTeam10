int a;

void setup() {
  Serial.begin(9600);

  ADC->ADC_MR |= ADC_MR_FREERUN | ADC_MR_LOWRES;
  ADC->ADC_CR = ADC_CR_START;
  ADC->ADC_CHER = ADC_CHER_CH7;
}

void loop() {
  a = ADC->ADC_CDR[7];
  Serial.println(a);
}
