#include "adc.h"

volatile uint8 adcBurstEnabled  = 0;
volatile uint8 adcPin           = 0;
volatile uint16 adcValue        = 0;
volatile uint8 adcIntDone       = 0;

int8 Adc_initialize(uint32 clk, Adc_Pin pin, Adc_BurstMode burstMode)
{
    ADC_ENABLE_POWER();           // Power on the ADC
    ADC_SET_CORE_CLK();           // Set the ADC core clock
    
    ADC_CLEAR_PINSEL();           // clear pin selection bits
    switch (pin)
    {
        case 0: ADC_ENABLE_PIN0();  // Enable pin 0
                ADC_SET_PIN(0);     // set sample pin
                break;
        case 1: ADC_ENABLE_PIN1();  //Enable pin 1
                ADC_SET_PIN(1);     // set sample pin
                break;
        case 2: ADC_ENABLE_PIN2();  //Enable pin 2
                ADC_SET_PIN(2);     // set sample pin
                break;
        case 3: ADC_ENABLE_PIN3();  //Enable pin 3
                ADC_SET_PIN(3);     // set sample pin
                break;
        case 4: ADC_ENABLE_PIN4();  //Enable pin 4
                ADC_SET_PIN(4);     // set sample pin
                break;
        case 5: ADC_ENABLE_PIN5();  //Enable pin 5
                ADC_SET_PIN(5);     // set sample pin
                break;
        case 6: ADC_ENABLE_PIN6();  //Enable pin 6
                ADC_SET_PIN(6);     // set sample pin
                break;
        case 7: ADC_ENABLE_PIN7();  //Enable pin 7
                ADC_SET_PIN(7);     // set sample pin
                break;
        default: break;
    }
    adcPin = pin;
    
    ADC_SET_CLK(clk);           // Set ADC clock
    ADC_CONFIGURE();            // Configure ADC specific settings
    ADC_ENABLE();               // Enable the ADC
    
    adcBurstEnabled = burstMode;
    if (burstMode == Adc_BurstMode_Enabled)
    {
        ADC_SET_BURSTMODE();
    }

    ADC_ENABLE_IRQ();               // Enable ADC interrupt
 
    if (burstMode == Adc_BurstMode_Enabled)
    {
        ADC_SET_ALL_IRQS_BURST();   // Enable all interrupts
    }
    else 
    {
        ADC_SET_ALL_IRQS();         // Enable all interrupts
    }
    
   
    return 0;
}

int8 Adc_deinitialize(void)
{ 
    ADC_DISABLE();          // Disable the ADC
    ADC_DISABLE_POWER();    // Power off the ADC
    ADC_DISABLE_IRQ();      // Disable ADC interrupt
    
    return 0;
}

int8 Adc_read(uint16 *value)
{
    if (adcBurstEnabled == 0)
    {
        adcIntDone = 0;
        ADC_START();                    // start conversion now
    }
    
    while (adcIntDone != 1)             // wait until finished
        ;
    
    if (ADC_HAS_OVERRUN(adcValue))      // overrun error
        return -1;
    
    *value = ADC_GET_VALUE(adcValue);
    
    return 0;
}

void ADC_IRQHandler(void)
{   
    switch (adcPin) // Read ADC will clear the interrupt
    {
        case 0: adcValue = ADC_READ_PIN0();
                break;
        case 1: adcValue = ADC_READ_PIN1();
                break;
        case 2: adcValue = ADC_READ_PIN2();
                break;
        case 3: adcValue = ADC_READ_PIN3();
                break;
        case 4: adcValue = ADC_READ_PIN4();
                break;
        case 5: adcValue = ADC_READ_PIN5();
                break;
        case 6: adcValue = ADC_READ_PIN6();
                break;
        case 7: adcValue = ADC_READ_PIN7();
                break;
    }
    
    ADC_STOP();     // stop adc
    
    adcIntDone = 1;
}
