// A Compact and Lightweight Rigid-Tendon Combined Exoskeleton for Hand Rehabilitation
// Author: Kevin Stark
// Open Source: https://github.com/ksDreamer/hand_exoskeleton

// ServoDriver.ino 是主程序，void loop()是主函数，在这里写的驱动舵机转动的程序上电会自动循环执行。
// 单舵机控制：st.WritePos(2, 450, 0,400); 分别是ID，角度，未知填0即可，速度。
// 多舵机同时控制，就把舵机ID、Position和Speed存入相应数组，用st.SyncWritePos(ID, 7, Position, 0, Speed);分别是舵机ID，舵机数量，角度，未知填0即可，速度。
// 多舵机控制需要在STSCTRL.h 里定义byte ID[]、u16 Position[]、u16 Speed[]，方括号填数量。
// 如果整个ServoDriver程序包正确烧录到ESP32，只要有供电，就能在Wifi列表找到开发板的局域网热点。连接后浏览器访问192.168.4.1可以打开web调试界面。
// WEBPAGE.h 是对web前端界面的开发。例如可以在里面找到Open Hand 和Close Hand两个函数，分别对应四指的平展和弯曲。
// 要给web前端开发新功能，请在WEBPAGE.h找到toggleCheckbox函数，会发现四个参数，第二个就是case，想要什么动作方案就在CONNECT.h里的active Control函数处添加新case（建议从case 30开始），然后在WEBPAGE.h里调用toggleCheckbox函数。
// 项目重要参考资料：https://www.waveshare.net/wiki/Servo_Driver_with_ESP32 和 https://www.waveshare.net/wiki/SC09_Servo

// 开发日志Developing Log：
// v0.2 测试得到四指和大拇指2号舵机的合适转动角度，放在Loop主函数里。
// v0.3 WEB前端增加四指的Move、Open、Close按钮和动作方案。根据舵臂和装置测试效果重新调整小尾指（7号舵机）转动参数以避免干涉。修改Wifi名为ESP32_KevinStark，密码为88888888。
// v0.4 增加大拇指控制方案，先relaxThumb让1和3号舵机的线松驰，再移动；封装了很多函数，增强可维护性；增加了很多动作方案。
// v0.5 优化代码结构，封装函数。大拇指稳定，先张开再收紧。增加动作方案例如四指和大拇指的联动，play game 石头剪刀布。整理Web前端布局。于ServoDriver.ino增加串口调试程序，可接收"[ID] [Position] [Speed]"格式的单舵机转动命令，还有"[caseNumber]"格式的动作方案。
// v1.0 增加三种Speed的选择 100 700 1500 对应case 45 46 47，只改变四指运行速度。简化前端界面。

// WIFI_AP settings.
const char* AP_SSID = "ESP32_KevinStark";
const char* AP_PWD  = "88888888";

// WIFI_STA settings.
const char* STA_SSID = "OnePlus 8";
const char* STA_PWD  = "40963840";

// the MAC address of the device you want to ctrl.
uint8_t broadcastAddress[] = {0x08, 0x3A, 0xF2, 0x93, 0x5F, 0xA8};
// uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};


typedef struct struct_message {
  int ID_send;
  int POS_send;
  int Spd_send;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// set the default role here.
// 0 as normal mode.
// 1 as leader, ctrl other device via ESP-NOW.
// 2 as follower, can be controled via ESP-NOW.
#define DEFAULT_ROLE 0

// set the default wifi mode here.
// 1 as [AP] mode, it will not connect other wifi.
// 2 as [STA] mode, it will connect to know wifi.
#define DEFAULT_WIFI_MODE 1

// the uart used to control servos.
// GPIO 18 - S_RXD, GPIO 19 - S_TXD, as default.
#define S_RXD 18
#define S_TXD 19

// the IIC used to control OLED screen.
// GPIO 21 - S_SDA, GPIO 22 - S_SCL, as default.
#define S_SCL 22
#define S_SDA 21

// the GPIO used to control RGB LEDs.
// GPIO 23, as default.
#define RGB_LED   23
#define NUMPIXELS 10

// set the max ID.
int MAX_ID = 20;

// modeSelected.
// set the SERIAL_FORWARDING as true to control the servos with USB.
bool SERIAL_FORWARDING = false;

// OLED Screen Dispaly.
// Row1: MAC address.
// Row2: VCC --- IP address.
// Row3: MODE:Leader/Follower  [AP]/[STA][RSSI]
//       DEFAULT_ROLE: 1-Leader(L)/ 2-Follower(F).
//       DEFAULT_WIFI_MODE: 1-[AP]/ 2-[STA][RSSI] / 3-[TRY:SSID].
//       (no matter what wifi mode you select, you can always ctrl it via ESP-NOW.)
// Row4: the position of servo 1, 2 and 3.
String MAC_ADDRESS;
IPAddress IP_ADDRESS;
byte   SERVO_NUMBER;
byte   DEV_ROLE;
byte   WIFI_MODE;
int    WIFI_RSSI;




// set the interval of the threading.
#define threadingInterval 600
#define clientInterval    10

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#include "RGB_CTRL.h"
#include "STSCTRL.h"
#include "CONNECT.h"
#include "BOARD_DEV.h"



void setup() {
  Serial.begin(115200);
  while(!Serial) {}

  espNowInit();

  getMAC();
  
  boardDevInit();

  RGBcolor(0, 64, 255);

  servoInit();

  wifiInit();

  webServerSetup();

  RGBoff();

  ID[0] = 1; //匹配舵机ID，大拇指分别为1,2,3,二到五指为4,5,6,7。有多少个就要在STSCTRL.h的byte ID[]，Position[]，Speed[]里对应修改。
  ID[1] = 2;
  ID[2] = 3;
  ID[3] = 4;
  ID[4] = 5;
  ID[5] = 6;
  ID[6] = 7;

  delay(1000);
  pingAll(true);

  threadInit();
}


void loop() {
//  delay(300000);

// 四指串口输入的语法: ID[空格]Position[空格]ID[空格]Position[空格]...[空格]Speed;
    if (Serial.available() > 0) { // 检查是否有可用的数据
        int firstNumber = Serial.parseInt(); // 读取第一个数字

        if (Serial.peek() == ' ') { // 检查下一个字符是否是空格
            // 如果是空格，那么读取后面的数字并控制舵机
            Serial.read(); // 跳过空格
            int ID = firstNumber;
            int Position = Serial.parseInt(); // 读取舵机的角度
            if (Serial.read() == ' ') { // 跳过空格
                int Speed = Serial.parseInt(); // 读取舵机的速度
                st.WritePos(ID, Position, 0, Speed); // 控制舵机
                Serial.write("Servo ");
                Serial.print(ID);
                Serial.write(" moves to Position ");
                Serial.print(Position);
                Serial.write(" with Speed ");
                Serial.println(Speed);
                delay(7000);
            }
        }
        else {
            // 如果只输入单个数字，没有空格，那么根据第一个数字执行相应的 case
            activeCtrl(firstNumber);
            }
    }
}


// 舵机顺时针转，角度由小变大。四号和五号舵机在舵机左边旋转，所以其变化与在舵机右边旋转的六号和七号舵机相反。
// 舵臂拉到远端是平，所以平的时候4、5号舵机是小角度，6、7号舵机是大角度。曲的时候4、5号舵机是大角度，6、7号舵机是小角度。

// activeCtrl(25); // Open and Close four fingers one by one, ID order: 4,5,6,7,4,5,6,7
// activeCtrl(26); // Open and Close four fingers one by one, ID order: 4,5,6,7,7,6,5,4
// activeCtrl(27); // 'Yeah', Simulatously Open ID 4 and 5, Close ID 6 and 7.
// activeCtrl(30); // 四指张开
// activeCtrl(31); // 四指合拢

// 官方示例代码
//   st.WritePosEx(1, 0, 600, 0);
//   delay(3000);
//   st.WritePosEx(1, 180, 600, 0);
  // st.WritePosEx(3, 25, 600, 0);
  // delay(2000);

  // st.WritePosEx(1, 1000, 600, 0);
  // st.WritePosEx(2, 1000, 600, 0);
  // st.WritePosEx(3, 1000, 600, 0);
  // delay(2000);
  
// > > > > > > > > > DOC < < < < < < < < <
// === Develop Board Ctrl ===
// get the MAC address and save it in MAC_ADDRESS;
// getMAC();

// Init GPIO.
// pinMode(PIN_NUM, OUTPUT);
// pinMode(PIN_NUM, INPUT_PULLUP);

// set the level of GPIO.
// digitalWrite(PIN_NUM, LOW);
// digitalWrite(PIN_NUM, HIGH);

// PWM output(GPIO).
// int freq = 50;
// resolution = 12;
// ledcSetup(PWM_NUM, frep, resolution);
// ledcAttachPin(PIN_NUM, PWM_NUM);
// ledcWrite(PWM_NUM, PWM);


// === Servo Ctrl ===
// GPIO 18 as RX, GPIO 19 as TX, init the serial and the servos.
// servoInit();

// set the position as middle pos.
// setMiddle(servoID);
// st.WritePosEx(ID, position, speed, acc);



// === Devices Ctrl ===
// ctrl the RGB.
// 0 < (R, G, B) <= 255
// setSingleLED(LED_ID, matrix.Color(R, G, B));

// init the OLED screen, RGB_LED.
// boardDevInit();

// dispaly the newest information on the screen.
// screenUpdate();
