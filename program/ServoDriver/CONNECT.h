// https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/
#include <esp_now.h>
#include <WiFi.h>
#include <WebServer.h>
#include "WEBPAGE.h"

#include <cstdlib>
#include <ctime>

// Create AsyncWebServer object on port 80
WebServer server(80);


// select the ID of active servo.
void activeID(int cmdInput){
  activeNumInList += cmdInput;
  if(activeNumInList >= searchNum){
    activeNumInList = 0;
  }
  else if(activeNumInList < 0){
    activeNumInList = searchNum;
  }
}


void activeSpeed(int cmdInput){
  activeServoSpeed += cmdInput;
  if (activeServoSpeed > ServoMaxSpeed){
    activeServoSpeed = ServoMaxSpeed;
  }
  else if(activeServoSpeed < 0){
    activeServoSpeed = 0;
  }
}


int rangeCtrl(int rawInput, int minInput, int maxInput){
  if(rawInput > maxInput){
    return maxInput;
  }
  else if(rawInput < minInput){
    return minInput;
  }
  else{
    return rawInput;
  }
}


int delayTime;
int setSpeed;

void relaxThumb() { // 松开虎口，松开1号和3号舵机到边位，2号舵机到中位。每一次大拇指运动前都应该先relaxThumb()。
    u16 Position[3] = {900, 500, 100};
    u16 Speed[3] = {500, 300, 500};
    st.SyncWritePos(ID, 3, Position, 0, Speed);
    delay(2000);
}

void moveThumb(u16 position1, u16 position2, u16 position3, u16 speed) {
  // move the three Thumb Servos in order ID2, ID1, ID3.
    // relaxThumb();
    u16 Position[3] = {position1, position2, position3};
    u16 Speed[3] = {speed, speed, speed};
    
    st.WritePos(2, position2, 0, speed);
    delay(1000);
    st.WritePos(1, position1, 0, speed);
    delay(1000);
    st.WritePos(3, position3, 0, speed);
    delay(1000);
}

void moveThumbSameTime(u16 position1, u16 position2, u16 position3, u16 speed) {
  // move the three Thumb Servos at the same time.
    relaxThumb();
    u16 Position[3] = {position1, position2, position3};
    u16 Speed[3] = {speed, speed, speed};
    st.SyncWritePos(ID, 3, Position, 0, Speed);
    delay(500);
}
void finger2open() {
    st.WritePos(4, 250, 0, setSpeed); //平
}

void finger2close() {
    st.WritePos(4, 600, 0, setSpeed);//曲
}

void finger3open() {
    st.WritePos(5, 150, 0, setSpeed); //平
}

void finger3close() {
    st.WritePos(5, 650, 0, setSpeed);//曲
}

void finger4open() {
    st.WritePos(6, 800, 0, setSpeed); //平
}

void finger4close() {
    st.WritePos(6, 300, 0, setSpeed);//曲
}

void finger5open() {
    st.WritePos(7, 600, 0, setSpeed); //平
}

void finger5close() {
    st.WritePos(7, 200, 0, setSpeed);//曲
}

void openHand(){
    finger2open();        
    finger3open();        
    finger4open();
    finger5open();
}
void closeHand(){
    finger2close();
    finger3close();
    finger4close();
    finger5close();
}

void activeCtrl(int cmdInput){
  switch(cmdInput){
    case 1:st.WritePosEx(listID[activeNumInList], ServoDigitalMiddle, activeServoSpeed, ServoInitACC);break;
    case 2:
      if(modeRead[listID[activeNumInList]] == 0) {
        servoStop(listID[activeNumInList]);
      }
      else if(modeRead[listID[activeNumInList]] == 3){
        st.WritePos(listID[activeNumInList], 0, 0, 0);
      }
      break;
    case 3:servoTorque(listID[activeNumInList],0);Torque_List[activeNumInList] = false;break;
    case 4:servoTorque(listID[activeNumInList],1);Torque_List[activeNumInList] = true;break;
    case 5:
      if(modeRead[listID[activeNumInList]] == 0){
        if(SERVO_TYPE_SELECT == 1){
          st.WritePosEx(listID[activeNumInList], ServoDigitalRange - 1, activeServoSpeed, ServoInitACC);
        }
        else if(SERVO_TYPE_SELECT == 2){
          st.WritePosEx(listID[activeNumInList], ServoDigitalRange - MAX_MIN_OFFSET, activeServoSpeed, ServoInitACC);
        }
      }


      else if(modeRead[listID[activeNumInList]] == 3){
        if(SERVO_TYPE_SELECT == 1){
          st.WritePosEx(listID[activeNumInList], 10000, activeServoSpeed, ServoInitACC);
        }
        else if(SERVO_TYPE_SELECT == 2){
          st.WritePos(listID[activeNumInList], 0, rangeCtrl(activeServoSpeed,200,999), 0);
        }
      }
      break;
    case 6:
      if(modeRead[listID[activeNumInList]] == 0){
        if(SERVO_TYPE_SELECT == 1){
          st.WritePosEx(listID[activeNumInList], 0, activeServoSpeed, ServoInitACC);
        }
        else if(SERVO_TYPE_SELECT == 2){
          st.WritePosEx(listID[activeNumInList], MAX_MIN_OFFSET, activeServoSpeed, ServoInitACC);
        }
      }


      else if(modeRead[listID[activeNumInList]] == 3){
        if(SERVO_TYPE_SELECT == 1){
          st.WritePosEx(listID[activeNumInList], -10000, activeServoSpeed, ServoInitACC);
        }
        else if(SERVO_TYPE_SELECT == 2){
          st.WritePos(listID[activeNumInList], 0, rangeCtrl(activeServoSpeed,200,999)+1024, 0);
        }
      }
      break;
    case 7:activeSpeed(100);break;
    case 8:activeSpeed(-100);break;
    case 9:servotoSet += 1;if(servotoSet > 250){servotoSet = 0;}break;
    case 10:servotoSet -= 1;if(servotoSet < 0){servotoSet = 0;}break;
    case 11:setMiddle(listID[activeNumInList]);break;
    case 12:setMode(listID[activeNumInList], 0);break;
    case 13:setMode(listID[activeNumInList], 3);break;
    case 14:SERIAL_FORWARDING = true;break;
    case 15:SERIAL_FORWARDING = false;break;
    case 16:setID(listID[activeNumInList], servotoSet);break;

    case 17:DEV_ROLE = 0;break;
    case 18:DEV_ROLE = 1;break;
    case 19:DEV_ROLE = 2;break;

    case 20:RAINBOW_STATUS = 1;break;
    case 21:RAINBOW_STATUS = 0;break;

    case 23: // Open Hand
        openHand();
        moveThumbSameTime(1000, 100, 0, setSpeed);
        break;
    case 24: // Cose Hand
        moveThumbSameTime(100, 500, 200, setSpeed);
        moveThumbSameTime(1000, 1000, 0, setSpeed);
        closeHand();
        break;
    case 25: // Open and Close
        openHand();
        delay(delayTime);
        closeHand();
        delay(delayTime);
        break;
    case 26: // Show 4,5,6,7,7,6,5,4
        finger2open();
        delay(200);
        finger3open();
        delay(200);
        finger4open();
        delay(200);
        finger5open();
        delay(200);
        finger5close();
        delay(200);
        finger4close();
        delay(200);
        finger3close();
        delay(200);
        finger2close();
        delay(200);
        break;

    case 27: // 'Yeah', Simulatously Open ID 4 and 5, Close ID 6 and 7.
        finger2open();
        finger3open();
        finger4close();
        finger5close();
        delay(delayTime);
        break;
    case 28: // play game 剪刀石头布
        // 准备动作
        for(int i = 0; i < 2; i++) {
            openHand();
            delay(500);
            closeHand();
            delay(500);
        }

        // 生成随机数
        srand(time(0));
        { // 这个花括号是因为在switch的case里定义random，而c++里switch case里不能定义新的变量，于是添加一个新的作用域{}
        int random = rand() % 3;
        // 根据随机数决定出剪刀、石头还是布
        if(random == 0) { // 剪刀
            finger2open();
            finger3open();
            finger4close();
            finger5close();
        } else if(random == 1) { // 石头
            closeHand();
        } else { // 布
            openHand();
        }}
        delay(4000);
        break;

    case 29: // Thumb 紧贴手背
        moveThumbSameTime(800, 500, 800, 500);
        break;
    case 30: // Thumb 远离手背
        moveThumbSameTime(100, 500, 200, 500);
        break;
    case 31: // Thumb 张开
        moveThumbSameTime(1000, 100, 0, 500);
        break;
    case 32: // Thumb 收紧 (先远离手背，然后收紧)
        moveThumbSameTime(100, 500, 200, 1000);
        moveThumbSameTime(1000, 1000, 0, 1000);
        break;

    case 33: // Finger 2 Move 食指动
        finger2open();
        delay(delayTime);
        finger2close();
        delay(delayTime);
        break;
    case 34: // Finger 2 Open 食指平
        finger2open();
        delay(delayTime);
        break;
    case 35: // Finger 2 Close 食指动
        finger2close();
        delay(delayTime);
        break;
    case 36: // Finger 3 Move
        finger3open();
        delay(delayTime);
        finger3close();
        delay(delayTime);
        break;
    case 37: // Finger 3 Open
        finger3open();
        delay(delayTime);
        break;
    case 38: // Finger 3 Close
        finger3close();
        delay(delayTime);
        break;
    case 39: // Finger 4 Move
        finger4open();
        delay(delayTime);
        finger4close();
        delay(delayTime);
        break;
    case 40: // Finger 4 Open
        finger4open();
        delay(delayTime);
        break;
    case 41: // Finger 4 Close
        finger4close();
        delay(delayTime);
        break;
    case 42: // Finger 5 Move
        finger5open();
        delay(delayTime);
        finger5close();
        delay(delayTime);
        break;
    case 43: // Finger 5 Open
        finger5open();
        delay(delayTime);
        break;
    case 44: // Finger 5 Close
        finger5close();
        delay(delayTime);
        break;
    case 45:
        setSpeed = 100;
        delayTime = 5000;
        break;
    case 46:
        setSpeed = 700;
        delayTime = 2000;
        break;
    case 47:
        setSpeed = 1500;
        delayTime = 1000;
        break;
  }
}


void handleRoot() {
 server.send(200, "text/html", index_html); //Send web page
}


void handleID() {
  if(!searchedStatus && searchFinished){
    String IDmessage = "ID:";
    for(int i = 0; i< searchNum; i++){
      IDmessage += String(listID[i]) + " ";
    }
    server.send(200, "text/plane", IDmessage);
  }
  else if(searchedStatus){
    String IDmessage = "Searching...";
    server.send(200, "text/plane", IDmessage);
  }
}


void handleSTS() {
  String stsValue = "Active ID:" + String(listID[activeNumInList]);
  if(voltageRead[listID[activeNumInList]] != -1){
    stsValue += "  Position:" + String(posRead[listID[activeNumInList]]);
    // if(DEV_ROLE == 0){
    //   stsValue += "<p>Device Mode: Normal";
    // }
    // else if(DEV_ROLE == 1){
    //   stsValue += "<p>Device Mode: Leader";
    // }
    // else if(DEV_ROLE == 2){
    //   stsValue += "<p>Device Mode: Follower";
    // }
    stsValue += "<p>Voltage:" + String(float(voltageRead[listID[activeNumInList]])/10);
    stsValue += "  Load:" + String(loadRead[listID[activeNumInList]]);
    // stsValue += "<p>Speed:" + String(speedRead[listID[activeNumInList]]);

    // stsValue += "  Temper:" + String(temperRead[listID[activeNumInList]]);
    // stsValue += "<p>Speed Set:" + String(activeServoSpeed);
    // stsValue += "<p>ID to Set:" + String(servotoSet);
    // stsValue += "<p>Mode:";
    // if(modeRead[listID[activeNumInList]] == 0){
    //   stsValue += "Servo Mode";
    // }
    // else if(modeRead[listID[activeNumInList]] == 3){
    //   stsValue += "Motor Mode";
    // }

    // if(Torque_List[activeNumInList]){
    //   stsValue += "<p>Torque On";
    // }
    // else{
    //   stsValue += "<p>Torque Off";
    // }
  }
  else{
    stsValue += " FeedBack err";
  }
  server.send(200, "text/plane", stsValue); //Send ADC value only to client ajax request
}


void webCtrlServer(){
    server.on("/", handleRoot);
    server.on("/readID", handleID);
    server.on("/readSTS", handleSTS);

    server.on("/cmd", [](){
    int cmdT = server.arg(0).toInt();
    int cmdI = server.arg(1).toInt();
    int cmdA = server.arg(2).toInt();
    int cmdB = server.arg(3).toInt();

    switch(cmdT){
      case 0:activeID(cmdI);break;
      case 1:activeCtrl(cmdI);break;
      case 9:searchCmd = true;break;
    }
  });

  // Start server
  server.begin();
  Serial.println("Server Starts.");
}


void webServerSetup(){
  webCtrlServer();
}


void getMAC(){
  WiFi.mode(WIFI_AP_STA);
  MAC_ADDRESS = WiFi.macAddress();
  Serial.print("MAC:");
  Serial.println(WiFi.macAddress());
}


void getIP(){
  IP_ADDRESS = WiFi.localIP();
}


void setAP(){
  WiFi.softAP(AP_SSID, AP_PWD);
  IPAddress myIP = WiFi.softAPIP();
  IP_ADDRESS = myIP;
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  WIFI_MODE = 1;
}


void setSTA(){
  WIFI_MODE = 3;
  WiFi.begin(STA_SSID, STA_PWD);
}


void getWifiStatus(){
  if(WiFi.status() == WL_CONNECTED){
    WIFI_MODE = 2;
    getIP();
    WIFI_RSSI = WiFi.RSSI();
  }
  else if(WiFi.status() == WL_CONNECTION_LOST && DEFAULT_WIFI_MODE == 2){
    WIFI_MODE = 3;
    // WiFi.disconnect();
    WiFi.reconnect();
  }
}


void wifiInit(){
  DEV_ROLE  = DEFAULT_ROLE;
  WIFI_MODE = DEFAULT_WIFI_MODE;
  if(WIFI_MODE == 1){setAP();}
  else if(WIFI_MODE == 2){setSTA();}
}


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  if(DEV_ROLE == 2){
    memcpy(&myData, incomingData, sizeof(myData));
    myData.Spd_send = abs(myData.Spd_send);
    if(myData.Spd_send < 50){
      myData.Spd_send = 200;
    }
    st.WritePosEx(myData.ID_send, myData.POS_send, abs(myData.Spd_send), 0);

    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.print("POS: ");
    Serial.println(myData.POS_send);
    Serial.print("SPEED: ");
    Serial.println(abs(myData.Spd_send));
  }
}


void espNowInit(){
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  MAC_ADDRESS = WiFi.macAddress();
  Serial.print("MAC:");
  Serial.println(WiFi.macAddress());
}
