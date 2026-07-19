

# [19.07.2026] - 'wait' functions

## Description
I tried to realize waitMicros(us) and waitMillis functions with SYSCLK

## Problem
Functions waited four times more than it was meant to be. 
```C
// this must wait for 1 second
waitMicros(1e9ul);
// but about 4 second ellapsed
```

## Solution
I thought that it had been connected with clocking speed, but I checked 
it several times and everything was as I wanted it to be: 
using HSI, 8MHz with AHB prescaler = 1 and without /8 division on AHB clock

Actually, the devil was in the details, in the details of realization:
```C
// It doesn't work, because:
// 1) we need to set VAL-1 LOAD, to raise the CNTFLAG in time
// 2) MCU spends clock cycles on other stuff in 'for' loop 
void waitMicros(const uint32_t micros) {
	...
  // 8  : with 8Mhz we need 8 clock cycles to get 1us
	SysTick->VAL = 8;
  for (int i = 0; i < micros; i++) {
    SysTick->CTRL = 0;
  	while (!(SysTick->CTRL & SYSTICK_CTRL_CNTFLAG));
  }
  ...
}

// It works. Notice: we got rid of 'for' loop with 
// 1) increment, : i++;
// 2) comparison : i < micros;
// 3) reading    : SysTick->VAL
void waitMicros(const uint32_t micros) {
	...
  // -1 : we count from LOAD to zero and it raises flag on 1 -> 0 transition 
  SysTick->LOAD = 8 * micros - 1; 
	SysTick->VAL = 0;
	while (!(SysTick->CTRL & SYSTICK_CTRL_CNTFLAG));
  ...
}
```

### Appendix
Proof of my solution using MCU Debug tool - DWT:
```C
CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

DWT->CYCCNT = 0;
DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

waitMillis(1000);

uint32_t stamp = DWT->CYCCNT; 
// Debug: stamp = 8'000'058 what is appropriate
//        for 1 second 
``` 

