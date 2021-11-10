# 针对电赛控制题和仪器仪表题设计的核心板

主控：stm32h750vbt6

协处理器：gw-1n

网络模块：ESP32-wroom-32E

### 外设模块

* stm32外设

    spi flash A：存储片外程序或RTOS

    spi flash B：存储ESP32固件或比特流

    UART-USB：直连PA9、PA10，用于打印调试信息

    6路直流电机驱动

    舵机驱动

    JTAG

* ESP32-W0WD

    spi flash C：适配ESP32模组，用于存储NVS与固件

    摄像头接口：需要通过FPGA连接

    板载天线/外接天线

    彩色LED*2：指示运行状态

* FPGA

    共32可用IO

    ch552t：8051单片机，用于下载数据

    摄像头接口：兼容ov5640，可由stm32或esp32驱动

    高速ADC：AD9280

    JTAG

    FPC排线：可引出到外部，接入其他传感器，与高速DDS和ADC冲突

    与stm32的12个引脚相连，与esp32的18个引脚相连

