WiFiUDP Udp;
const unsigned int outPort = 12400;          // remote port (not needed for receive)
const unsigned int localPort = 12500;        // local port to listen for UDP packets (here's where we send the packets)
OSCErrorCode error;

void init_osc()
{
  Udp.begin(localPort);
}

void update_osc()
{
  OSCMessage bundle;
  int size = Udp.parsePacket();
  if (size > 0)
  {    
    while (size--)
    {
      bundle.fill(Udp.read());
    }
    if (!bundle.hasError())
    {
      bundle.dispatch("/run",  run_osc);
      bundle.dispatch("/stop", stop_osc);
      bundle.dispatch("/goto", goto_osc);
      bundle.dispatch("/power", pwr_osc);
    }
  }
}

void pwr_osc(OSCMessage &msg)
{
  setPower(msg.getInt(0));
}

void goto_osc(OSCMessage &msg)
{
  int tg = msg.getInt(0);
  driveGoto(tg);
}

void stop_osc(OSCMessage &msg)
{
  stopMotor(true);
}

void run_osc(OSCMessage &msg)
{
  int dir = msg.getInt(0);
  driveMotor(true, dir);
}
