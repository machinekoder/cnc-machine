#include "button.h"

CircularBuffer buttonBuffer;
volatile ButtonValue buttonBufferData[BUTTON_BUFFER_SIZE];

typedef struct {
    uint8 id;
    uint8 port;
    uint8 pin;
    uint8 unset;
    uint8 type;
} Button;

volatile Button  buttons[BUTTON_BUFFER_SIZE];
volatile ButtonValue val[BUTTON_BUFFER_SIZE];
volatile uint8   buttonCount = 0;

volatile uint32 maxunset;

/** reads the button values
 */
void valueButton(void);

/** stores the values and counts the matches.
 *@param i is the number of the row of buttons.
 *@param var is the value of the button.
 */
void putVal(uint8 i, uint8 pressed);

uint8 Button_initialize(uint32 khz, uint32 sampleInterval, uint32 timeoutInterval)
{
    if (Cb_initialize(&buttonBuffer, BUTTON_BUFFER_SIZE, sizeof(ButtonValue), (void*)(&buttonBufferData)) == -1)
        return -1;
    
	if (Timer_initialize(Timer2, khz, sampleInterval) == -1)
        return -1;
    
    maxunset = (uint32)(timeoutInterval/sampleInterval);
    
    Timer_connectFunction(Timer2, valueButton);
    Timer_start(Timer2);

    return 0;
}

void Button_initializeButton(uint8 id, uint8 port, uint8 pin,ButtonType type)
{
    Gpio_setDirection(port, pin, Gpio_Direction_Input );    //direction 0=input
    if(type == ButtonTypeLowActive)
      Pin_setMode(port, pin, Pin_Mode_PullUp);
    if(type == ButtonTypeHighActive)
       Pin_setMode(port, pin, Pin_Mode_PullDown);
    
    buttons[buttonCount].id     = id;
    buttons[buttonCount].port   = port;
    buttons[buttonCount].pin    = pin;
    buttons[buttonCount].type   = type;
    buttons[buttonCount].unset  = 0;
    
    val[buttonCount].count = 0;
    val[buttonCount].id = id;

    buttonCount++;
}

void valueButton(void){
	uint8 i;
    
	for(i = 0; i < buttonCount; i++)
    {
        if(buttons[i].type == ButtonTypeLowActive)
        {
            Gpio_read(buttons[i].port,buttons[i].pin) ? putVal(i,0): putVal(i,1);
        }
        else if(buttons[i].type == ButtonTypeLowActive)
        {
            Gpio_read(buttons[i].port,buttons[i].pin) ? putVal(i,1): putVal(i,0);
        }
	}
}

void putVal(uint8 i,uint8 pressed)
{
    
	if(pressed && (buttons[i].unset != 0))
    {
		val[i].count = val[i].count +1;
        buttons[i].unset = 0;
	}
	else
    {
		if(!pressed && buttons[i].unset <= maxunset) 
        {
            buttons[i].unset = buttons[i].unset +1;
        }
		if((buttons[i].unset > maxunset) && (val[i].count != 0) )
        {
			Cb_put(&buttonBuffer,(void*)(&val[i]));
			val[i].count = 0;
			buttons[i].unset =0;
		}
	}
}

int8 Button_getPress(ButtonValue *value)
{
    return Cb_get(&buttonBuffer, (void*)value);
}
