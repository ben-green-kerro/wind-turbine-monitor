#include <SoftwareSerial.h>
#define SSerialRX        10  //Serial Receive pin
#define SSerialTX        11  //Serial Transmit pin
#define SSerialTxControl 3   //RS485 Direction control
#define Pin13LED         13
SoftwareSerial RS485Serial(SSerialRX, SSerialTX); // RX, TX


double highestpower = 0.0;
double highestcurrent = 0.0;
double resistance = 0.176;
//double resistance = 330.0;

long timelast = 0;
long timenow = 0;
long timediff = 0;
double wspeed = 0.0;
double timetospeed = 1.75 / 20.0;

void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  // Start the built-in serial port, probably to Serial Monitor
  Serial.begin(115200);
  Serial.println("SerialRemote");  // Can be ignored
  Serial.println(timetospeed);  // Can be ignored

  pinMode(Pin13LED, OUTPUT);

  pinMode(SSerialTxControl, OUTPUT);
  digitalWrite(SSerialTxControl, LOW);  // Init Transceiver

  // Start the software serial port, to another device
  RS485Serial.begin(4800);   // set the data rate
}


double get_speed() {
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
  return tally * timetospeed;
}


void loop() {
  double value = float(analogRead(A0));
  double voltage = value * (5.0 / 1024.0);
  double current = voltage / resistance;
  if (current > highestcurrent) {
    highestcurrent = current;
  }
  double power = voltage * current;
  if (power > highestpower) {
    highestpower = power;
  }

  double myspeed = get_speed();
  Serial.print("windspeed:"); Serial.print(myspeed); Serial.print(", ");
  //  Serial.print("analoginput:"); Serial.print(value); Serial.print(", ");
  //  Serial.print("resistance:"); Serial.print(resistance); Serial.print(", ");
  Serial.print("voltage:"); Serial.print(voltage * 11.0); Serial.print(", ");
  //  Serial.print("current:"); Serial.print(current); Serial.print(", ");
  //  Serial.print("Power:"); Serial.print(power); Serial.print(", ");
  //  Serial.print("windspeed:"); Serial.print(power); Serial.print(", ");
  Serial.println();
}
