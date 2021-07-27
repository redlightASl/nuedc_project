# 硬件设计

一个通用型的STM32开发板

## 开发板

### 需求

* 8路PWM
* 4路ADC
* 2路编码器接口
* 按键模块接口（4*4按键矩阵）
* LCD显示接口
* 2路串口，其中一路是PA9、PA10，从开发板上引出
* SWD调试接口

### 设计

#### PWM

* 引出TIM1、TIM8两路高级定时器

  TIM1：PA9、11、13、14

  TIM8：PC6、7、8、9

* 引出通用定时器TIM2、TIM3、TIM4

  TIM2：PA0、1、2、3A

  TIM3：PB0、1

  TIM4：PD14、15

* 其余定时器预留

#### ADC/DAC

* 引出ADC1的IN1、3、6、7

  IN1：PA1

  IN3：PA3

  PA6：IN6

  PA7：IN7

* 引出ADC2的IN10、11、12、13

  IN10：PC0

  IN11：PC1

  IN12：PC2

  IN13：PC3

* 引出ADC3的IN14、IN15

  IN14：PF4

  IN15：PF5

* 引出两路DAC OUT

  OUT1：PA4

  OUT2：PA5

#### 编码器接口

* 引出TIM3的CHANNEL_1、CHANNEL_2

  ENCODER：PB4、5

* 引出TIM4的CHANNEL_1、CHANNEL_2

  ENCODER：PD12、13

#### 按键模块接口、LED接口

引出GPIO，如下所示：

PC13、PD11、PF11、PG1

引出两个LED控制脚

PF2、PF3

#### LCD显示接口

使用stm32f4的FSMC

接口如下

* PD0：FSMC_D2
* PD1：FSMC_D3
* PD4：FSMC_NOE
* PD5：FSMC_NWE
* PD8：FSMC_D13
* PD9：FSMC_D14
* PD10：FSMC_D15
* PD14：FSMC_D0
* PD15：FSMC_D1
* PE7：FSMC_D4
* PE8：FSMC_D5
* PE9：FSMC_D6
* PE10：FSMC_D7
* PE11：FSMC_D8
* PE12：FSMC_D9
* PE13：FSMC_D10
* PE14：FSMC_D11
* PE15：FSMC_D12
* PF12：FSMC_A6
* PG12：FSMC_NE4

#### 串口与其他通讯接口

* 引出USART1、USART2

  USART1：PB6（TX）、PB7（RX）

  USART2：PD5（TX）、PD6（RX）

* 引出SPI3，配置为四线全双工

  NSS：PA15

  SCLK：PC10

  MISO：PC11

  MOSI：PC12

* 引出IIC2

  SDA：PF0

  SCL：PF1

* 引出CAN2

  PB12（RX）、PB13（TX）

#### SWD调试接口

引出SWD接口

#### 其他接口

全部引脚引出