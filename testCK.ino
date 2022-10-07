// Them thu vien can thiet cho chuong trinh
#include <ESP32Servo.h> 
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

LiquidCrystal_I2C lcd(0x27,16,2);
Servo myservo;

int ir_enter = 18;
int ir_back = 19;
int SV = 13;

const int place[]={16,17,32,33,26,27};
int S[7];
String temp ="";
int k = 0;
int flag1=0, flag2=0; 
int slot = 6;  
int total = 0;

// Ten wifi và mat khau wifi
const char* ssid = "Khu E Bach Khoa";
const char* password = "Bachkhoa@2019";
//const char* ssid = "aaaaa";
//const char* password = "abalahkn";


// Cong hoat dong cua server
AsyncWebServer server(80);

// Ham truyen du lieu len web
String processor(const String& var){
  Serial.println(var);
 if (var == "COUNT"){
    return String(total);
  }
  else if (var == "SLOT"){
    return String(slot);
  }
  else if (var == "S1"){
    return String(S[1]);
  }
  else if (var == "S2"){
    return String(S[2]);
  }
  else if (var == "S3"){
    return String(S[3]);
  }
  else if (var == "S4"){
    return String(S[4]);
  }
  else if (var == "S5"){
    return String(S[5]);
  }
  else if (var == "S6"){
    return String(S[6]);
  }
  return String();
}
 
void setup(){
  Serial.begin(115200);

  for(int i=0;i<6;i++){
    pinMode(place[i], INPUT);
  } 
       
  for(int i=1;i<=6;i++){
    S[i] = 0;
  }
  
  pinMode(ir_enter, INPUT);
  pinMode(ir_back, INPUT);
    
  myservo.attach(SV);
  myservo.write(90);

  lcd.init(); 
  lcd.backlight();  
  lcd.setCursor (0,1);
  lcd.print("  Car  parking  ");
  lcd.setCursor (1,0);
  lcd.print("    System    ");
  delay (2000);
  lcd.clear();   
  
  Read_Sensor();

  // Kiem tra SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Ket noi wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // In dia chi IP cua ESP32
  Serial.println(WiFi.localIP());
  
  // Gui tep signup.html den may khach
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/signup.html", String(), false, processor);
  });
  
  // Gui cac file css
  server.on("/css/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/css/style.css");
  });
  server.on("/css/shorcode.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/css/shorcode.css");
  });
  server.on("/css/reponsive.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/css/reponsive.css");
  });
  server.on("/css/bootstrap.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/css/bootstrap.css");
  });
  server.on("/css/main.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/css/main.js");
  });
  server.on("/css/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/css/jquery.min.js");
  });
  server.on("/css/oto.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/css/oto.png");
  });
  server.on("/css/icon.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/css/icon.png");
  });

  server.on("/css/favicon.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/css/favicon.png");
  });

  // Nhan hanh dong tu web ve dieu khien mo servo 
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    myservo.write(0);   
    request->send(SPIFFS, "/signup.html", String(), false, processor);
  });
  
  // Nhan hanh dong tu web ve dieu khien mo servo 
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    myservo.write(100);   
    request->send(SPIFFS, "/signup.html", String(), false, processor);
  });
  
  server.on("/count", HTTP_GET, [](AsyncWebServerRequest *request){
       
    request->send_P(200, "text/plain", String(total).c_str());
  });
  server.on("/slot", HTTP_GET, [](AsyncWebServerRequest *request){
       
    request->send_P(200, "text/plain", String(slot).c_str());
  });
  
  server.on("/s1", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(S[1]).c_str());
  });

  server.on("/s2", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(S[2]).c_str());
  });

  server.on("/s3", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(S[3]).c_str());
  });

  server.on("/s4", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(S[4]).c_str());
  });

  server.on("/s5", HTTP_GET, [](AsyncWebServerRequest *request){  
    request->send(200, "text/plain", String(S[5]).c_str());
  });
  
  server.on("/s6", HTTP_GET, [](AsyncWebServerRequest *request){ 
    request->send(200, "text/plain", String(S[6]).c_str());
  });

  // Start server
  server.begin();
}
 
void loop(){
Read_Sensor();

lcd.setCursor (0,0);
lcd.print("   Have Slot: "); 
lcd.print(slot);
lcd.print("    ");  

k=0;
for (int i = 1 ; i <= 6; i++) {
  lcd.setCursor (k, 1);

  if(S[i]==1){
    lcd.print(String(i)+ "N");
  }
     
  else{
    lcd.print(String(i)+ "Y");
  }
  k = k + 2;
}

// Dong mo servo theo cam bien
if(digitalRead (ir_enter) == 0 && flag1==0){
  if(slot>0){
    flag1=1;
    if(flag2==0 && myservo.read()==0){
      slot = slot-1;
    }
    else if(flag2==0 && myservo.read()==99){
      myservo.write(0);
      slot = slot-1;
    }
    Serial.println(myservo.read());
  }
  else
  {
    lcd.setCursor (0,0);
    lcd.print("Sorry Full");  
    delay(1500);
  } 
}

if(digitalRead (ir_back) == 0 && flag2==0){
  flag2=1;
  if(flag1==0 && myservo.read()==99){
    myservo.write(0); 
    slot = slot+1;
    Read_Sensor();
    Serial.println(myservo.read());
  }
  else if (flag1==0 && myservo.read()==0){
    myservo.write(0); 
    slot = slot+1;
    Read_Sensor();
  }
  
}

if(flag1==1 && flag2==1 && myservo.read()==0){
  delay (1000);
  myservo.write(100);
  flag1=0, flag2=0;
  Serial.println(myservo.read());
}
else if(flag1==1 && flag2==1 && myservo.read()==99){
  delay (1000);
  flag1=0, flag2=0;
  Serial.println(myservo.read());
}


delay(1);
  
}

void Read_Sensor(){
  slot=0;
  total=0;
  for (int i  = 1; i <= 6; i++){
    S[i] = 0;
  }
  
  for (int i = 1; i <= 6; i++) {
    if(digitalRead(place[i-1]) == 0){
      S[i]=1; 
    }
    else{
      S[i]=0;
    }
  }

  for (int i = 1; i <= 6 ; i++ ){
    total += S[i];
  }

  slot = 6-total; 
}
