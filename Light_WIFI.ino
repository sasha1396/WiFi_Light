
/* управление светом по WiFi*/

#include <WiFiClient.h> 
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// настройка порта вывода
uint8_t PowerPin = 0; // GPIO0
bool    PowerOn  = false;

// Имя хоста
const char* host = "WiFi Light";
// Вместо звездочек в ssid - пишем название вашей сети WiFi который раздает роутер 
// в password - пароль вашей сети WiFi
const char* ssid = "********";
const char* password = "********";

ESP8266WebServer server(80);

void setup() {
  // Вывод данных в порт (скорость 115200 бод)
  Serial.begin(115200);

  // Инициализация порта на выход
  pinMode(PowerPin , OUTPUT);
  digitalWrite(PowerPin , PowerOn);

  // Подлючение к WiFi
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  if(WiFi.waitForConnectResult() == WL_CONNECTED){
     // Запуск WEB-сервера
     MDNS.begin(host);
     server.on ( "/", HTTP_handleRoot );
     server.onNotFound ( HTTP_handleRoot );
     server.begin();    
     Serial.print( "\nHTTP server ok! Open ");
     Serial.print(WiFi.localIP());
     Serial.print( " in your browser");
  }
  else {
     Serial.printf("WiFi started is fail");
  }    
}

void loop() {
  server.handleClient();
   delay(50);
}

/*
 * Оработчик главной страницы сервера
 */
void HTTP_handleRoot(void) {
  bool stat = false;

  if( server.hasArg("stat") ){
     if( strncmp(server.arg("stat").c_str(),"1",1) == 0 )stat = true;
  }
  else {
     stat = PowerOn;
  }
  
  String out = "";

  out =
"<html>\
  <head>\
    <meta charset=\"utf-8\" />\
    <title>Свет по WiFi </title>\
    <style>\
      body { background-color: #f2f2f2; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }\
    </style>\
  </head>\
  <body>\
    <h1>Управление светом по WiFi</h1>\n";

  if( stat ){
      out+="\
    <h2>Состояние: Выключено</br>\</br>\
    <a href=\"/?stat=0\">Включить</a></h1>\
    ";
  }
  else {
      out+="\
    <h2>Состояние: Включено</br>\</br>\
    <a href=\"/?stat=1\">Выключить</a><h1>\
    ";            
  }
   out+= "\
  </body>\
</html>";
   server.send ( 200, "text/html", out );
   if( stat != PowerOn ){
      PowerOn = stat;
      digitalWrite(PowerPin , PowerOn);
      if( PowerOn )Serial.println("Power is OFF");
      else Serial.println("Power is ON");                                                             
   }
   
}
