const char* settings = "/wifi_settings.txt";
WiFiClient       phoneClient;
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
uint8_t          lastClientId;
