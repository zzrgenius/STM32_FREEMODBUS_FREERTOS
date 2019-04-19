#include "delay.h"
#include "stm32f4xx.h"
static unsigned int cpuclkfeq; 

void DelayInit(void)
{
    cpuclkfeq = SystemCoreClock;
    DEM_CR         |=  DEM_CR_TRCENA; 
		//DWT_CYCCNT      = 0u; 
    DWT_CR         |= DWT_CR_CYCCNTENA;
}


void Delayus(unsigned int usec)
{
     unsigned int startts,endts,ts;
     startts = DWT_CYCCNT;
     ts =  usec * (cpuclkfeq /(1000*1000) ); 
     endts = startts + ts;          
      if(endts > startts)        
      {
          while(DWT_CYCCNT < endts);      
       }
       else
      {
           while(DWT_CYCCNT > endts);    
           while(DWT_CYCCNT < endts); 
      }

}
void dwt_delay_init(uint32_t clk)
{
    cpuclkfeq = clk;
    DEM_CR         |=  DEM_CR_TRCENA;
    DWT_CYCCNT      = 0u;
    DWT_CR         |= DWT_CR_CYCCNTENA;
}

 

void delay_us(unsigned int us)
{
        unsigned int temp;
        temp= DWT_CYCCNT;
        us *=  SystemCoreClock/1000000;
        while((unsigned int)( DWT_CYCCNT - temp)< us);
}

void delay_ms(unsigned int ms)
{
        unsigned int temp;
        temp= DWT_CYCCNT;
        ms *=  SystemCoreClock/1000; 
        while((unsigned int)( DWT_CYCCNT - temp)< ms);
}
