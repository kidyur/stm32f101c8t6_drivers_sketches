`Sample`

```c

kp_SPI1_Init(kp_SPI1_GPIO_LAYOUT_SCND,
    kp_SPI1_GPIO_Speed_10MHz,
    kp_SPI1_ENDIANNESS_MSB_FIRST,
    kp_SPI1_BAUDRATE_SCALER_2);

kp_SPI1_Resume();

while (1)
{
  kp_SPI1_SendBytes_1(0x02);
}

```