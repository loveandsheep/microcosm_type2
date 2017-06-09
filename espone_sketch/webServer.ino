
void handle_index()
{
  String html = "";
  html = "<meta http-equiv='Content-Type' content='text/html; charset=UTF-8'><html><head><script>var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);connection.onopen = function (){connection.send('Connect ' + new Date());}; connection.onerror = function (error){    console.log('WebSocket Error ', error);};connection.onmessage = function (e) { var command = e.data.split(':'); document.getElementById(command[0]).innerHTML = command[1];};function reflesh(){document.getElementById('step').value = parseInt(document.getElementById('stepRange').value) * 100;}function reftext(){document.getElementById('stepRange').value = parseInt(document.getElementById('step').value) / 100;}function sendGo(){var cmd = 'go' + parseInt(document.getElementById('step').value).toString();connection.send(cmd);}function doFoward(){connection.send('doF');}function doBackward(){connection.send('doB');}function stop(){connection.send('stop');}</script></head><body style='background-color:#102542; color:#D6FFF6'><h2>microcosm manual operation</h2>長さ指定<br><input type='range' id='stepRange' oninput='reflesh();' value='0' style='background-color:#D6FFF6'><br><input id='step' type='text' size=6 value='0' oninput='reftext();' style='background-color:#D6FFF6'><input type='button' value='go' onclick='sendGo();' style='background-color:#D6FFF6'><br><br>マニュアル駆動<br><input type='button' value='foward'  onclick='doFoward();' style='background-color:#3BA99C'><input type='button' value='backward' onclick='doBackward();' style='background-color:#69D1C5'><input type='button' value='stop'  onclick='stop();' style='background-color:#F87060'><br><br><p>現在位置：<b id='pos'></b></p><p>状態：<b id='status'></b></p><p>メッセージ：<b id='mes'></b></p></body></html>";
  server.send(200, "text/html", html);
}

void handle_setting()
{
  String html = "";
  html = "<meta http-equiv='Content-Type' content='text/html; charset=UTF-8'><html><head></head><body style='background-color:#102542; color:#D6FFF6'><h2>microcosm settings</h2>Wi-Fi設定<br><form method='post'><p>SSID：<input type='text' name='ssid' value='' size='30' style='background-color:#D6FFF6'/></p><p>Pass：<input type='text' name='pass' value='' size='30' style='background-color:#D6FFF6'/></p><p><input type='submit' name='submit' value='ok' /></p></form></body></html>";
  server.send(200, "text/html", html);
}

void handle_settingPost()
{
  String ssid_w = server.arg("ssid");
  String pass_w = server.arg("pass");

  
  
  String html = "";
  char html_c[1024];  
  sprintf(html_c, "<meta http-equiv='Content-Type' content='text/html; charset=UTF-8'><html><head></head><body style='background-color:#102542; color:#D6FFF6'><h2>Wi-Fi setting succeed.</h2><p>SSID：%s</p><p>Pass：%s</p></body></html>", ssid_w.c_str(), pass_w.c_str());

  html = html_c;
  server.send(200, "text/html", html);
}


void sendWebsockMessage(String message)
{
  webSocket.sendTXT(lastClientId, message);
}

