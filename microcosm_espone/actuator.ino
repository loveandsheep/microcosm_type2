#define STAT_IDLE 0
#define STAT_FOWARD 1
#define STAT_BACKWARD 2
#define STAT_GOFOWARD 3
#define STAT_GOBACKWARD 4
#define THR_REFLECT 550

long rotPos = 0;
int  statbef = 0;
int driveStat = STAT_IDLE;
int power = 255;


int targPos = -1;

void setPower(int pwr)
{
  power = pwr;
}

void actuate()
{
//  getRotary();
  if (zero_lock) overRunCheck();
}

void overRunCheck()
{
  int tipVal = readAdc(ADC_PHOTO_TIP);
  int rootVal = readAdc(ADC_PHOTO_ROOT);

  if ((driveStat == STAT_BACKWARD || driveStat == STAT_GOBACKWARD) && (tipVal < THR_REFLECT))
  {
    stopMotor(true);
    stopMotor(false);
    rotPos = 0;
    sendWebsockMessage("pos:"+String(rotPos));
  }

  if ((driveStat == STAT_GOBACKWARD) && (rotPos  <= targPos))
  {
    stopMotor(true);
    stopMotor(false);
  }

  if ((driveStat == STAT_GOFOWARD) && (rotPos  >= targPos))
  {
    stopMotor(true);
    stopMotor(false);
  }

}

void getRotary()
{
  bool pa = readAdc(ADC_ROT_A) > 512;
  bool pb = readAdc(ADC_ROT_B) > 512;
  int stat = pa ? (pb ? 2 : 1) : (pb ? 3 : 0);

  bool flip = abs(statbef - stat) > 1;
  if (statbef < stat)
  {
    rotPos += flip ? 1 : -1;
  }
  else if (statbef > stat)
  {
    rotPos += flip ? -1 : 1;    
  }
  
  statbef = stat;
}

void driveGoto(int pos)
{
    targPos = pos;

    if (rotPos < targPos)
    {
      drivePole(true);
    }
    else if (rotPos > targPos)
    {
      drivePole(false);
    }

}

void driveRoll(bool isExtend)
{
  driveMotor(false, isExtend);
}

void drivePole(bool isExtend)
{
  driveStat = isExtend ? STAT_FOWARD : STAT_BACKWARD;
  if (isExtend)
  {
    driveMotor(true, false);
    driveMotor(false, false);
  }
  else
  {
    driveMotor(true, true);
    driveMotor(false, true);
  }
  sendWebsockMessage(String("status:")+(isExtend ? "Extend" : "Rewind"));
  sendWebsockMessage("pos:moving...");
}

void driveMotor(bool isM1, bool isFoward)
{
  int pw = power;
  if (!isM1)
  {
    pw *= motorRatio;
    if (!isFoward) pw *= motorRatio_ext;
  }
  
  digitalWrite(PIN_EN, HIGH);
  analogWrite(isM1 ? PIN_M1_D1 : PIN_M2_D1, isFoward ? 0 : pw);
  analogWrite(isM1 ? PIN_M1_D2 : PIN_M2_D2, isFoward ? pw  : 0);
}

void stopMotor(bool isM1)
{
  driveStat = STAT_IDLE;
  analogWrite(isM1 ? PIN_M1_D1 : PIN_M2_D1, 0);
  analogWrite(isM1 ? PIN_M1_D2 : PIN_M2_D2, 0);
  digitalWrite(PIN_EN, HIGH);
  digitalWrite(isM1 ? PIN_M1_D1 : PIN_M2_D1, LOW);
  digitalWrite(isM1 ? PIN_M1_D2 : PIN_M2_D2, LOW);
  sendWebsockMessage("status:Stay");
  sendWebsockMessage("pos:"+String(rotPos));
}

void disableMotor()
{
  digitalWrite(PIN_EN, LOW);
}

unsigned int readAdc(byte ch)
{
  SPI.begin();
  digitalWrite(15, LOW);
  SPI.transfer(0x01);
  byte highbyte = SPI.transfer(((8+ch)<<4));
  byte lowbyte = SPI.transfer(0x00);
  digitalWrite(15, HIGH);
  SPI.end();

  unsigned int data = ((highbyte << 8) + lowbyte) & 0x03FF;
  return data;
}

