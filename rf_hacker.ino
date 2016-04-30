#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();
 
// CONS
int power_led = 3;
int func1_led = 4;
int func2_led = 5;
int func3_led = 6;
int act_led = 7;
int select_switch = 8;
int start_switch = 9;
// Sender = 2
// Empfänger = 10
int mode = 1;

int reading_select_switch;
int previous_select_switch = LOW;
int reading_start_switch;
int previous_start_switch = LOW; 
long time = 0;
long debounce = 200;
char* binary;
char* ping;
char* pong;

// SETUP
void setup() {
  Serial.begin(9600);
  pinMode(power_led, OUTPUT);
  pinMode(func1_led, OUTPUT);
  pinMode(func2_led, OUTPUT);
  pinMode(func3_led, OUTPUT);
  pinMode(act_led, OUTPUT);
  pinMode(select_switch, INPUT);
  pinMode(start_switch, INPUT);
  mySwitch.enableTransmit(10);
  mySwitch.enableReceive(0);
  int_complete();
}

// the loop routine runs over and over again forever:
void loop() {
  reading_select_switch = digitalRead(select_switch);
  if (reading_select_switch == HIGH && previous_select_switch == LOW && millis() - time > debounce) {
    time = millis();
    mode = mode + 1;
    switch_func();
  }
  previous_select_switch = reading_select_switch;
  
  reading_start_switch = digitalRead(start_switch);
  if (reading_start_switch == HIGH && previous_start_switch == LOW && millis() - time > debounce) {
    time = millis();
    Serial.println(mode);
    start_func();
  }
  previous_start_switch = reading_start_switch;
}

int int_complete()
{
  digitalWrite(power_led, HIGH);
  delay(100);
  digitalWrite(power_led, LOW);
  digitalWrite(func1_led, HIGH);
  delay(100);
  digitalWrite(func1_led, LOW);
  digitalWrite(func2_led, HIGH);
  delay(100);
  digitalWrite(func2_led, LOW);
  digitalWrite(func3_led, HIGH);
  delay(100);
  digitalWrite(func3_led, LOW);
  digitalWrite(act_led, HIGH);
  delay(100);
  digitalWrite(act_led, LOW);
  delay(500);
  digitalWrite(power_led, HIGH);
  digitalWrite(func1_led, HIGH);  
}

int switch_func()
{
  if (mode == 4) {
    mode = 1;
  }
  digitalWrite(func1_led, LOW);
  digitalWrite(func2_led, LOW);
  digitalWrite(func3_led, LOW);
  
  if (mode == 1) {
  digitalWrite(func1_led, HIGH);
  }
  
  if (mode == 2) {
  digitalWrite(func2_led, HIGH);
  }
  
  if (mode == 3) {
  digitalWrite(func3_led, HIGH);
  }
  
}

int start_func()
{
  digitalWrite(act_led, HIGH);
  
  if (mode == 1) {
      String onoff = "0F";
      String syscodebin = int2bin(0);
      for (int x = 0; x <= 31; x++) {
        syscodebin = int2bin(x);
        syscodebin.replace("1", "F");
        char charBuf[13];
        String sendcode = syscodebin + "0FFFF" + onoff;
        sendcode.toCharArray(charBuf, 13);
        mySwitch.sendTriState(charBuf);
        act();
        sendcode = syscodebin + "F0FFF" + onoff;
        sendcode.toCharArray(charBuf, 13);
        mySwitch.sendTriState(charBuf);
        act();
        sendcode = syscodebin + "FF0FF" + onoff;
        sendcode.toCharArray(charBuf, 13);
        mySwitch.sendTriState(charBuf);
        act();
        sendcode = syscodebin + "FFF0F" + onoff;
        sendcode.toCharArray(charBuf, 13);
        mySwitch.sendTriState(charBuf);
        act();
        sendcode = syscodebin + "FFFF0" + onoff;
        sendcode.toCharArray(charBuf, 13);
        mySwitch.sendTriState(charBuf);
        act();
      }
  }
  
  if (mode == 2) {
      String onoff = "F0";
      String syscodebin = int2bin(0);
      for (int x = 0; x <= 31; x++) {
        syscodebin = int2bin(x);
        syscodebin.replace("1", "F");
        char charBuf[13];
        String sendcode = syscodebin + "0FFFF" + onoff;
        sendcode.toCharArray(charBuf, 13);
        mySwitch.sendTriState(charBuf);
        act();
        sendcode = syscodebin + "F0FFF" + onoff;
        sendcode.toCharArray(charBuf, 13);
        mySwitch.sendTriState(charBuf);
        act();
        sendcode = syscodebin + "FF0FF" + onoff;
        sendcode.toCharArray(charBuf, 13);
        mySwitch.sendTriState(charBuf);
        act();
        sendcode = syscodebin + "FFF0F" + onoff;
        sendcode.toCharArray(charBuf, 13);
        mySwitch.sendTriState(charBuf);
        act();
        sendcode = syscodebin + "FFFF0" + onoff;
        sendcode.toCharArray(charBuf, 13);
        mySwitch.sendTriState(charBuf);
        act();
      }
  }
  
  if (mode == 3) {
    while (1) {
      if (mySwitch.available()) {
        binary = dec2binWzerofill(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength());
        mySwitch.resetAvailable();
        ping = (bin2tristate(binary));
        Serial.println(ping);
        pong = ping2pong(ping);
        Serial.println(pong);
        delay(5000);
        mySwitch.sendTriState(pong);
        break;
      }
    }
  }
  
  digitalWrite(act_led, LOW);
}

static char* ping2pong(char* p) {
  if (p[10] == '0' && p[11] == 'F') {
    p[10] = 'F';
    p[11] = '0';
    return p;
  }
  if (p[10] == 'F' && p[11] == 'O') {
    p[10] = '0';
    p[11] = 'F';
    return p;
  }
}

char * int2bin(int x)
{
  static char buffer[6];
  for (int i=0; i<5; i++) buffer[4-i] = '0' + ((x & (1 << i)) > 0);
  buffer[5] ='\0';
  return buffer;
}

int act() {
  digitalWrite(act_led, LOW);
  delay(100);
  digitalWrite(act_led, HIGH);
}

static char * dec2binWzerofill(unsigned long Dec, unsigned int bitLength){
  static char bin[64]; 
  unsigned int i=0;

  while (Dec > 0) {
    bin[32+i++] = (Dec & 1 > 0) ? '1' : '0';
    Dec = Dec >> 1;
  }

  for (unsigned int j = 0; j< bitLength; j++) {
    if (j >= bitLength - i) {
      bin[j] = bin[ 31 + i - (j - (bitLength - i)) ];
    }else {
      bin[j] = '0';
    }
  }
  bin[bitLength] = '\0';
  
  return bin;
}

static char* bin2tristate(char* bin) {
  char returnValue[50];
  int pos = 0;
  int pos2 = 0;
  while (bin[pos]!='\0' && bin[pos+1]!='\0') {
    if (bin[pos]=='0' && bin[pos+1]=='0') {
      returnValue[pos2] = '0';
    } else if (bin[pos]=='1' && bin[pos+1]=='1') {
      returnValue[pos2] = '1';
    } else if (bin[pos]=='0' && bin[pos+1]=='1') {
      returnValue[pos2] = 'F';
    } else {
      return "not applicable";
    }
    pos = pos+2;
    pos2++;
  }
  returnValue[pos2] = '\0';
  return returnValue;
}