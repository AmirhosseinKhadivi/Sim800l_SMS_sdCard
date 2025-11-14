#include <SoftwareSerial.h>
#include <SD.h>

SoftwareSerial sim(7, 8); 
String PhoneNumber = "+98911xxxxx75";

// -------------------- STRUCT --------------------
struct SMS {
  String sender;
  String message;
};

// -------------------- CLASS --------------------
class SMSDatabase {
public:
  void begin(int csPin) {
    SD.begin(csPin);
  }

  void addSMS(SMS msg) {
    File f = SD.open("sms.csv", FILE_WRITE);
    if (f) {
      f.print(msg.sender);
      f.print(",");
      f.println(msg.message);
      f.close();
    }
  }
};

SMSDatabase db;

// -------------------- FUNCTIONS --------------------

// Read SMS
SMS readSMS(int index) {
  SMS smsData;
  sim.print("AT+CMGR=");
  sim.println(index);
  delay(1000);

  String sms = "";
  while (sim.available()) {
    sms += (char)sim.read();
  }

  int startQuote = sms.indexOf('"');
  int endQuote = sms.indexOf('"', startQuote + 1);
  if (startQuote == -1 || endQuote == -1) return smsData;

  smsData.sender = sms.substring(startQuote + 1, endQuote);

  int msgStart = sms.indexOf('\n', endQuote) + 1;
  smsData.message = sms.substring(msgStart);
  smsData.message.trim();

  return smsData;
}

// Send SMS (base function)
void sendSMS(String phoneNumber, String message) {
  sim.print("AT+CMGF=1\r");
  delay(100);
  sim.print("AT+CMGS=\"");
  sim.print(phoneNumber);
  sim.println("\"");
  delay(200);
  sim.print(message);
  delay(100);
  sim.write(26);  
  delay(5000);

  SMS msgData;
  msgData.sender = phoneNumber;
  msgData.message = message;
  db.addSMS(msgData);
}

// Get signal strength
int checkSignal() {
  sim.println("AT+CSQ");
  delay(500);

  String csq = "";
  while (sim.available()) {
    csq += (char)sim.read();
  }

  int colon = csq.indexOf(':');
  int comma = csq.indexOf(',');
  if (colon == -1 || comma == -1) return 0;
  String rssiStr = csq.substring(colon + 2, comma);
  return rssiStr.toInt();
}

// Reset SIM800
void resetModule() {
  sim.println("AT+CFUN=1,1");
  delay(8000);
}

// Smart SMS sending with retries
bool sendSMS_withSignalRetry(String number, String message, int minSignal) {

  int rssi = checkSignal();
  Serial.print("Initial Signal: ");
  Serial.println(rssi);

  if (rssi >= minSignal) {
    Serial.println("Signal OK → sending SMS");
    sendSMS(number, message);
    return true;
  }

  Serial.println("Weak signal → searching 1 minute...");
  unsigned long start = millis();

  while (millis() - start < 60000) {
    rssi = checkSignal();
    if (rssi >= minSignal) {
      Serial.println("Signal found → send");
      sendSMS(number, message);
      return true;
    }
    delay(5000);
  }

  Serial.println("Still weak → searching 3 minutes...");
  start = millis();

  while (millis() - start < 180000) {
    rssi = checkSignal();
    if (rssi >= minSignal) {
      Serial.println("Signal found → send");
      sendSMS(number, message);
      return true;
    }
    delay(20000);
  }

  Serial.println("No signal → rebooting module...");
  resetModule();
  delay(8000);

  rssi = checkSignal();
  if (rssi >= minSignal) {
    Serial.println("Signal OK after reboot → sending...");
    sendSMS(number, message);
    return true;
  }

  Serial.println("Still no signal. Message NOT sent.");
  return false;
}

// -------------------- SETUP --------------------
void setup() {
  Serial.begin(9600);
  sim.begin(9600);

  sim.println("AT");
  delay(1000);

  db.begin(10);

  sim.println("AT+CMGF=1");
  delay(500);

  Serial.println("System Ready.");
}

// -------------------- LOOP --------------------
void loop() {

  SMS m = readSMS(1);
  if (m.sender == PhoneNumber) {
    Serial.print("SMS from: ");
    Serial.println(m.sender);
    Serial.print("Message: ");
    Serial.println(m.message);
    db.addSMS(m);
  }

  sendSMS_withSignalRetry(PhoneNumber, "Test Data", 12);

  delay(60000);
}
