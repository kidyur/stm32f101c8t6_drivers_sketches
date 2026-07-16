
// Configuring RTC
while (!(RTC->CRL & RTC_CRL_RTOFF));

RTC->CRL |= RTC_CRL_CNF;
// 1 Second update
RTC->PRLL = 0x7FFF;

RTC->CRL &= ~RTC_CRL_CNF;

RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;
// off RCC->BDCR write protection
PWR->CR |= PWR_CR_DBP;

// Select Clock Source
RCC->BDCR |= RCC_BDCR_RTCSEL_LSE;
// LSE Enable
RCC->BDCR |= RCC_BDCR_LSEON;
// RTC enable
RCC->BDCR |= RCC_BDCR_RTCEN;