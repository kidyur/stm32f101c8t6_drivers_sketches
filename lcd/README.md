```c
// Sample
#include "device_drivers/lcd.h"

struct lcd_pin dummy = {0, 0};
struct lcd_pin dummy_row[8] = {{0, 0}};

int main(void)
{
	lcd_init(&dummy, &dummy, &dummy, &dummy, dummy_row);

	lcd_writeLine("Feel home!");

  while (1);
}
```
