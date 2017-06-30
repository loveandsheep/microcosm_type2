WiFiUDP Udp;
const unsigned int outPort = 12400;          // remote port (not needed for receive)
const unsigned int localPort = 12500;        // local port to listen for UDP packets (here's where we send the packets)
OSCErrorCode error;
bool zero_lock = false;

float motorRatio = 0.25;
float motorRatio_ext = 0.2;

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
      bundle.dispatch("/ratio", ratio_osc);
      bundle.dispatch("/zeroLock", zeroLock_osc);
      bundle.dispatch("/tension", tension_osc);
      bundle.dispatch("/roll", roll_osc);
      
    }
  }
}

void ratio_osc(OSCMessage &msg)
{
  motorRatio = msg.getFloat(0);
  motorRatio_ext = msg.getFloat(1);
}

void roll_osc(OSCMessage &msg)
{
  driveRoll(msg.getInt(0));
}

void tension_osc(OSCMessage &msg)
{
  drivePole(msg.getInt(0));
}

void zeroLock_osc(OSCMessage &msg)
{
  zero_lock = msg.getInt(0);
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
  stopMotor(false);
}

void run_osc(OSCMessage &msg)
{
  int targ = msg.getInt(0);
  int dir = msg.getInt(1);
  driveMotor(targ, dir);
}
