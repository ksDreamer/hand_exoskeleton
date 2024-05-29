#include <SCServo.h>
#include <math.h>
//
byte ID[7]; //关于舵机ID的设置，请参考https://www.waveshare.net/wiki/SC09_Servo里的“更改舵机ID”。
u16 Position[7];
u16 Speed[7];


// === SC Servo ===
#define CTRL_SC_SERVO
SCSCL st;
float ServoDigitalRange = 1023.0;
float ServoAngleRange   = 210.0;
float ServoDigitalMiddle= 511.0;
#define ServoInitACC      0
#define ServoMaxSpeed     1500
#define MaxSpeed_X        1500
#define ServoInitSpeed    1500
int SERVO_TYPE_SELECT = 2;
int MAX_MIN_OFFSET = 30;
#define SMS_STS_ID SCSCL_ID

// === ST Servo ===
// #define CTRL_ST_SERVO
// SMS_STS st;
// float ServoDigitalRange = 4095.0;
// float ServoAngleRange   = 360.0;
// float ServoDigitalMiddle= 2047.0;
// #define ServoInitACC      100
// #define ServoMaxSpeed     4000
// #define MaxSpeed_X        4000
// #define ServoInitSpeed    2000
// int SERVO_TYPE_SELECT = 1

// set the servo ID list.
byte ID_List[253];
bool Torque_List[253];

// []: the ID of the servo.
// the buffer of the information read from the active servo.
s16  loadRead[253];
s16  speedRead[253];
byte voltageRead[253];
int  currentRead[253];
s16  posRead[253];
s16  modeRead[253];
s16  temperRead[253];

// []: the num of the active servo.
// use listID[activeNumInList] to get the ID of the active servo.
byte listID[253];
byte searchNum = 0;
bool searchedStatus = false;
bool searchFinished = false;
bool searchCmd      = false;
byte activeNumInList = 0;
s16 activeServoSpeed = 100;
byte servotoSet = 0;

// linkageBuffer to save the angle.
float linkageBuffer[50];

// the buffer of the bytes read from USB-C and servos. 
int usbRead;
int stsRead;


void getFeedBack(byte servoID){
  if(st.FeedBack(servoID)!=-1){
    posRead[servoID] = st.ReadPos(-1) ; // For bigger servo, eg SC 15. If leader and follower are the same, then delete '* 1.67'.
    speedRead[servoID] = st.ReadSpeed(-1);
    loadRead[servoID] = st.ReadLoad(-1);
    voltageRead[servoID] = st.ReadVoltage(-1);
    currentRead[servoID] = st.ReadCurrent(-1);
    temperRead[servoID] = st.ReadTemper(-1);
    modeRead[servoID] = st.ReadMode(servoID);
  }else{
    // Serial.println("FeedBack err");
  }
}


void servoInit(){
  Serial1.begin(1000000, SERIAL_8N1, S_RXD, S_TXD);
  st.pSerial = &Serial1;
  while(!Serial1) {}

  for (int i = 0; i < MAX_ID; i++){
    Torque_List[i] = true;
  }
}


void setMiddle(byte InputID){
  st.CalibrationOfs(InputID);
}


void setMode(byte InputID, byte InputMode){
  st.unLockEprom(InputID);
  if(InputMode == 0){
    if(SERVO_TYPE_SELECT == 1){
      st.writeWord(InputID, 11, 4095);
      st.writeByte(InputID, SMS_STS_MODE, InputMode);
    }
    else if(SERVO_TYPE_SELECT == 2){
      st.writeWord(InputID, SCSCL_MIN_ANGLE_LIMIT_L, 20);
      // st.writeWord(InputID, SCSCL_MIN_ANGLE_LIMIT_H, 5123);
      st.writeWord(InputID, SCSCL_MAX_ANGLE_LIMIT_L, 1003);
      // st.writeWord(InputID, SCSCL_MAX_ANGLE_LIMIT_H, 60240);
    }
  }

  else if(InputMode == 3){
    if(SERVO_TYPE_SELECT == 1){
      st.writeByte(InputID, SMS_STS_MODE, InputMode);
      st.writeWord(InputID, 11, 0);
    }
    else if(SERVO_TYPE_SELECT == 2){
      st.writeWord(InputID, SCSCL_MIN_ANGLE_LIMIT_L, 0);
      // st.writeWord(InputID, SCSCL_MIN_ANGLE_LIMIT_H, 0);
      st.writeWord(InputID, SCSCL_MAX_ANGLE_LIMIT_L, 0);
      // st.writeWord(InputID, SCSCL_MAX_ANGLE_LIMIT_H, 0);
    }
  }
  st.LockEprom(InputID);
}


void setID(byte ID_select, byte ID_set){
  if(ID_set > MAX_ID){MAX_ID = ID_set;}
  st.unLockEprom(ID_select);
  st.writeByte(ID_select, SMS_STS_ID, ID_set);
  st.LockEprom(ID_set);
}


void servoStop(byte servoID){
  st.EnableTorque(servoID, 0);
  delay(10);
  st.EnableTorque(servoID, 1);
}


void servoTorque(byte servoID, u8 enableCMD){
  st.EnableTorque(servoID, enableCMD);
}
