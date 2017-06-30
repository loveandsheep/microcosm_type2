
void init_wifi()
{
  SPIFFS.begin();
    
  String ssid_w = "aterm-sheep";
  String pass_w = "sheepmeme";
//  String ssid_w = "ERATO-Keio";
//  String pass_w = "kakehilab";
  String host_w = "host";

  File fw = SPIFFS.open(settings, "w");
  fw.println(ssid_w);
  fw.println(pass_w);
  fw.println(host_w);
  fw.close();
  
  /* set SSID & DNS name */
  byte mac[6];
  WiFi.macAddress(mac);
  String ssid_ap = "mcsm-";
  String dns_name = "mcsm-";
  String macAdd;
  for (int i = 0;i < 6;i++)
  {
    if (mac[i] < 16) macAdd += "0";
    macAdd += String(mac[i], HEX);    
  }

  ssid_ap += macAdd;
  dns_name += macAdd;

  /* MDNS intialize */
  MDNS.begin(dns_name.c_str());
  MDNS.addService("http", "tcp", 80);
  MDNS.addService("esp", "tcp", 8080);

  File f = SPIFFS.open(settings, "r");
  String ssid_r   = f.readStringUntil('\n');
  String pass_r   = f.readStringUntil('\n');
  String host_ip  = f.readStringUntil('\n');
  f.close();
  ssid_r.trim();
  pass_r.trim();

  /* Wifi initialize */
  const char *pass_ap = "microcosm";

  delay(1000);
  Serial.print("connect to ");
  Serial.println(ssid_r);
  Serial.println(pass_r);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid_r.c_str(), pass_r.c_str());
  WiFi.softAP(ssid_ap.c_str(), pass_ap);

  waitForWifi();

  server.on("/", HTTP_GET, handle_index);
  server.on("/setting", HTTP_GET, handle_setting);
  server.on("/setting", HTTP_POST, handle_settingPost);
  server.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void server_update()
{
  server.handleClient();
  webSocket.loop();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    lastClientId = num;
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        
                // send message to client
                webSocket.sendTXT(num, "Connected");
            }
            break;
        case WStype_TEXT:
            char tx[64];
            sprintf(tx,"%s", payload);
            parseCommand(tx);
            break;
        case WStype_BIN:
            Serial.printf("[%u] get binary length: %u\n", num, length);
            hexdump(payload, length);

            // send message to client
            // webSocket.sendBIN(num, payload, length);
            break;
    }

}

void parseCommand(char* cmd)
{ 
  String sig = String(cmd);
  if (sig == "stop")
  {
    stopMotor(true);
    stopMotor(false);
  }
  else if (sig.substring(0,2) == "go")
  {
    int len = sig.substring(2,sig.length()).toInt();
    driveGoto(len);
  }
  else if (sig == "doF") drivePole(true);
  else if (sig == "doB") drivePole(false);
  
}

void waitForWifi()
{
  int timeouter = millis();
  Serial.print("wait for wifi");
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(300);
    if (millis() - timeouter > 30000) break;
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("connection failed.");
  }
  else
  {
    Serial.println("connection success.");
    Serial.println(WiFi.localIP());
  }
}
