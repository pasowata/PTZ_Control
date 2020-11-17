#include <EthernetClient.h>

byte macAddress[6] = {0xA8, 0x61, 0x0A, 0xAE, 0x80, 0x2B};
EthernetClient ethernetClient;

bool sendPtzCommand(String ipAddress, String command, int timeOut);

void setup() {
  Serial.begin(9600);
  Serial.print("Start Ethernet... ");
  if(Ethernet.begin(macAddress)){
    Serial.println(Ethernet.localIP());
  }else{
    Serial.println("Failed");
  }
  delay(1000);
  Serial.println("Ready");
  sendPtzCommand("192.168.11.22", "APS70008000102", 1000);
}

void loop() {

}

bool sendPtzCommand(String ipAddress, String command, int timeOut){
  Serial.print(String(String("Connect '") + ipAddress + "' ... "));
  if(ethernetClient.connect(ipAddress.c_str(), 80)){
    Serial.println("Success");
    ethernetClient.println("GET /cgi-bin/aw_ptz?cmd=%23" + command + "&res=1 HTTP/1.1");
    ethernetClient.println("HOST: " + ipAddress);
    ethernetClient.println("Connection: close");
    ethernetClient.println();
  }else{
    Serial.println("Failed");
    return false;
  }
  int elapsed_time = 0;
  while(ethernetClient.connected()){
    while(ethernetClient.available()){
      int clientRead =  ethernetClient.read();
      Serial.write(clientRead);
      elapsed_time = 0;
    }
    delay(1);
    elapsed_time ++;
    if(timeOut < elapsed_time){
      Serial.println();
      Serial.println("Timeout");
      ethernetClient.stop();
    }
  }
  Serial.println();
  Serial.println("Disconneted");
  ethernetClient.stop();
  return true;
}
