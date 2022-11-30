/* YourDuino SoftwareSerialExample1Remote
   - Used with YD_SoftwareSerialExampleRS485_1 on another Arduino
   - Remote: Receive data, loop it back...
   - Connect this unit Pins 10, 11, Gnd
   - To other unit Pins 11,10, Gnd  (Cross over)
   - Pin 3 used for RS485 direction control
   - Pin 13 LED blinks when data is received

   Questions: terry@yourduino.com
*/

/*-----( Import needed libraries )-----*/
#include <SoftwareSerial.h>
/*-----( Declare Constants and Pin Numbers )-----*/
#define SSerialRX        10  //Serial Receive pin
#define SSerialTX        11  //Serial Transmit pin

#define SSerialTxControl 3   //RS485 Direction control
#define RS485Transmit    HIGH
#define RS485Receive     LOW

#define Pin13LED         13

/*-----( Declare objects )-----*/
SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX

/*-----( Declare Variables )-----*/
int byteReceived;
int byteSend;

long timelast = 0;
long timenow = 0;
long timediff = 0;
double wspeed = 0.0;
double timetospeed = 1.75 / 20.0;

double get_speed2() {
  RS485Serial.flush();
  int tally = 0;
  timelast = millis();
  while (timenow < (timelast + 1000)) {
    if (RS485Serial.available()) {
      RS485Serial.read();
      tally += 1;
    }
    timenow = millis();

  }
  return tally*timetospeed;
}

double get_speed() {
  RS485Serial.flush();
  while (!RS485Serial.available())
  {
    delayMicroseconds(1);
  }
  byteReceived = RS485Serial.read();
  timelast = micros();
  while (!RS485Serial.available())
  {
    delayMicroseconds(1);
  }
  byteReceived = RS485Serial.read();
  timenow = micros();
  timediff = timenow - timelast;
  wspeed = timetospeed / timediff;
  Serial.print("time = ");
  Serial.println(timediff);
  //  Serial.print("wspeed = ");
  //  Serial.println(wspeed);
  //  Serial.print("ticks per second = ");
  //  Serial.println(1000 / timediff);
  return wspeed;
}
void setup()   /****** SETUP: RUNS ONCE ******/
{
  // Start the built-in serial port, probably to Serial Monitor
  Serial.begin(115200);
  Serial.println("SerialRemote");  // Can be ignored
  Serial.println(timetospeed);  // Can be ignored

  pinMode(Pin13LED, OUTPUT);
  pinMode(SSerialTxControl, OUTPUT);

  digitalWrite(SSerialTxControl, RS485Receive);  // Init Transceiver

  // Start the software serial port, to another device
  RS485Serial.begin(4800);   // set the data rate
}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  double myspeed = get_speed2();
  delay(10);
  Serial.println(myspeed);
}
