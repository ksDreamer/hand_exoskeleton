# hand_exoskeleton

To be continued

20240529

A low-cost hand exoskeleton using linkage and tendon mechanism under ESP32.   

低成本手部外骨骼，连杆驱动四指，线驱模拟肌腱机制驱动大拇指，ESP32主控，使用SC09总线舵机。

## Material 材料

* 机械相关
  * SC09 总线舵机 * 7
  * 鱼眼轴承 * 8
  * M2和M3规格的螺丝和螺母若干
  * 铜柱（规格TD）
  * 线 0.8mm 淘宝链接：[304不锈钢钢丝绳 晾衣绳遮阳网包塑钢丝绳 细软小包胶钢丝绳 0.8mm](https://detail.tmall.com/item.htm?id=727898238439&price=2.25-59.94&short_name=h.g3fgrSYf6gOjNsf&spm=a2159r.13376460.0.0)
  * 3D打印材料（例如PLA）和3D打印机（自备或外包）
* 电子相关
  * ESP32 微控制器 *1
  * 信号线和串线板（如果采用本项目所购买的材料商家，会在舵机里配套这两个）
  * 支持DC6-12V供电的电源
  * 或者2-3节18650电池串联（3.7v）5.5*2.1mm +DC转接头+电池座
  * 电线和锡丝若干
  * 电焊台
  * （可选）热风枪、热塑管、电源开关、
* 软件
  * 开发：支持烧录程序的电脑，已在Mac和Windows 上测试可行，Arduino或VSCode编程环境
  * 使用：任一能连接Wi-Fi且具有图形化窗口的设备

## Program 程序

ServoDriver.ino 是主程序，void loop()是主函数，在这里写的驱动舵机转动的程序上电会自动循环执行。

单舵机控制：st.WritePos(2, 450, 0,400); 分别是ID，角度，未知填0即可，速度。

多舵机同时控制，就把舵机ID、Position和Speed存入相应数组，用st.SyncWritePos(ID, 7, Position, 0, Speed);分别是舵机ID，舵机数量，角度，未知填0即可，速度。

多舵机控制需要在STSCTRL.h 里定义byte ID[]、u16 Position[]、u16 Speed[]，方括号填数量。

如果整个ServoDriver程序包正确烧录到ESP32，只要有供电，就能在Wifi列表找到开发板的局域网热点。连接后浏览器访问192.168.4.1可以打开web调试界面。

WEBPAGE.h 是对web前端界面的开发。例如可以在里面找到Open Hand 和Close Hand两个函数，分别对应四指的平展和弯曲。

要给web前端开发新功能，请在WEBPAGE.h找到toggleCheckbox函数，会发现四个参数，第二个就是case，想要什么动作方案就在CONNECT.h里的active Control函数处添加新case（建议从case 30开始），然后在WEBPAGE.h里调用toggleCheckbox函数。

项目重要参考资料：https://www.waveshare.net/wiki/Servo_Driver_with_ESP32 和 https://www.waveshare.net/wiki/SC09_Servo

### 开发日志Developing Log

* v0.2 测试得到四指和大拇指2号舵机的合适转动角度，放在Loop主函数里。

* v0.3 WEB前端增加四指的Move、Open、Close按钮和动作方案。根据舵臂和装置测试效果重新调整小尾指（7号舵机）转动参数以避免干涉。修改Wifi名为ESP32_KevinStark，密码为88888888。

* v0.4 增加大拇指控制方案，先relaxThumb让1和3号舵机的线松驰，再移动；封装了很多函数，增强可维护性；增加了很多动作方案。

* v0.5 优化代码结构，封装函数。大拇指稳定，先张开再收紧。增加动作方案例如四指和大拇指的联动，play game 石头剪刀布。整理Web前端布局。于ServoDriver.ino增加串口调试程序，可接收"[ID] [Position] [Speed]"格式的单舵机转动命令，还有"[caseNumber]"格式的动作方案。

* v1.0 增加三种Speed的选择 100 700 1500 对应case 45 46 47，只改变四指运行速度。简化前端界面。
