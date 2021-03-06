# 校电赛项目

小车题+视觉题

设计制作智能小车，要求至少具有循迹功能（白色轨迹，黑色底色），鼓励设计具有无线充电、避障、跟踪、远程控制等功能的智能小车；

设计视觉识别系统，要求至少具有颜色与数字识别功能（硬件平台限于嵌入式视觉设备：OPENMV、树莓派、Jetson nano/nx），鼓励设计具有低训练成本、多目标、高速、高准确率的智能视觉识别系统；

**手头设备**：半成品小车、openmv、k210、esp-cam、esp-eye、stm32h750、h743、f407、f103核心板、zynq核心板、xc7a50t开发板、gd32vf407核心板、树莓派zero、树莓派3、或许可以借到jetson nano

（待定）小车基本功能：

* 使用wifi（esp32、esp8266）/无线模块（蓝牙透传、无线通信模块）远程控制前进、后退、原地转向
* 固定角度转向、固定速度前进/后退、可控制转向/进退时间
* 搭载openmv或k210云台进行巡线
* 搭载九轴模块、温湿度传感器、oled、lcd等不知道有什么用的东西
* 使用光电传感器巡线
* openmv跟踪小球、手写/打印数字识别
* 超声波避障
* openmv视觉避障
* 基于树莓派或nano实现多目标检测（基于yolo）
* esp32图传+人脸识别（使用esp-eye套件或esp-cam）
* 基于zynq或fpga实现小车基本控制
* 直接基于zynq（感觉不太可能）或移植pynq实现机器视觉功能
* wifi图传（基于esp-idf）

**目标**：

1. 基本运动逻辑(角度转弯、前行、后退)+遥控（蓝牙，有时间进军wifi）
2. 巡线
3. 自动避障
4. 上下坡（来得及的话加控制）
5. 半路上回位

# 初步准备

**主控**：stm32f407vb/zgt6

**缺少部件**：

1. 超声波
2. 光电传感器
3. GPS
4. ~~供电板~~
5. 主控板（如果有时间自己打）
6. 主控板（洞洞板）
7. 固定件

**实现思路**：

1. 基本运动逻辑(角度转弯、前行、后退)+遥控（蓝牙，有时间进军wifi）

    用mpu9250控制角度，光电编码器控制前进距离

    遥控使用蓝牙模块或esp8266透传

2. 巡线

    光电传感器或openmv（如果有时间就做）

3. 自动避障

    把openmv当成距离传感器或直接控制超声波传感器

4. 上下坡（来得及的话加控制）

    上坡-仰角大于某值（可以考虑加PI）-加速

    下坡-仰角小于某值-减速

5. 半路上回位

    GPS

    ==不确定怎么搞==

主控需要提前规划的资源：

如cubemx所示
