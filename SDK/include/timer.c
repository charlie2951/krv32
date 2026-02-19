#include "timer.h"


/* =========================================================
   Set Timer Value
   ========================================================= */
void timer_set_value(uint32_t value)
{
    TIMER_TIMER_VAL = value;
}

/* =========================================================
   Configure Timer as Timer/Counter mode
   mode = 0 -> timer mode
   mode = 1 -> counter mode
   ========================================================= */
void timer_init(uint32_t mode)
{
    uint32_t ctrl = 0;

    if(mode)
        ctrl |= TIMER_CTRL_MODE; //1-> counter, default-> Timer mode

   // ctrl |= TIMER_CTRL_START;

    TIMER_CTRL = ctrl;
}

    /* START / STOP Timer/Counter 
       1-> Enable, 2-> Disable
   */
  void timer_enable(uint32_t en){
     uint32_t ctrl = 0;
     if(en)
     ctrl |= TIMER_CTRL_START;//if en-> high, timer will start, 0-> stop 
    TIMER_CTRL = ctrl;
}
/* =========================================================
   Clear Done (Write-0-to-clear)
   ========================================================= 
   */
void timer_clear_done(void)
{
    TIMER_STATUS = 0;
}

void timer_start(void){ //send a high to low pulse to start timer
     timer_enable(1);//enable
     timer_enable(0);//disable
}
/* =========================================================
   Wait Until Done- Return 1 upon completion
   ========================================================= */
  uint32_t timer_status(void)
{  //return 0;
    while((TIMER_STATUS) == 0);
    timer_clear_done();
    return 1;
}



/* =========================================================
   Get Current Count Value
   ========================================================= */
void timer_getcount(uint32_t *count)
{
    *count = TIMER_COUNT_VAL;
}

//**Hardware Delay based on Timer **//
/*
void timer_delay_us(uint32_t val) //val in micro sec
{
 timer_set_value(val*73); //1micro sec gives 73 count
 timer_init(0);
 timer_start();
while(timer_status()==0);//wait until count expires
}
*/

//Alternate implimentation without multiplication
void timer_delay_us(uint32_t val) 
{
    // val * 100 is (val << 6) + (val << 5) + (val << 2)
    // 64 + 32 + 4 = 100
    uint32_t counts = (val << 6) + (val << 5) + (val << 2);
    
    timer_set_value(counts);
    timer_init(0);
    timer_start();
    
    while(timer_status() == 0); // Polling wait
}

/**
 *  Hardware Delay based on Milliseconds
 * ms The number of milliseconds to delay
 */
void timer_delay_ms(uint32_t ms) 
{
    while (ms--) 
    {
        // 1000 microseconds = 1 millisecond
        timer_delay_us(1000); 
    }
}
