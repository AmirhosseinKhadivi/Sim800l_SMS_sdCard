# SIM800L SMS Manager with SD Logging

This Arduino project allows you to **read SMS messages**, **send SMS with signal checks**, and **store messages on an SD card** using the SIM800L GSM module. It also includes a smart feature to **retry sending messages** if the signal is weak and reset the module if necessary.

---

## Features

- Read incoming SMS from a specified number
- Send SMS and automatically save sent messages to an SD card
- Check GSM signal strength before sending
- Retry sending messages when signal is weak:
  - Initial 1-minute retry
  - Extended 3-minute retry every 20 seconds
  - Module reboot if still no signal, then attempt to send again
- Simple logging on SD card in `sms.csv`

---

## Hardware Required

- Arduino board (Uno, Nano, Mega, etc.)
- SIM800L GSM module
- SD Card module
- Micro SD card
- Wires and breadboard

---

## Libraries Required

- [SoftwareSerial](https://www.arduino.cc/en/Reference/SoftwareSerial)
- [SD](https://www.arduino.cc/en/Reference/SD)

---

## Setup

1. Connect SIM800L to Arduino:
   - TX → Arduino RX
   - RX → Arduino TX
   - Ensure proper power supply to SIM800L
2. Connect SD card module to Arduino (CS pin defined in code as 10)
3. Install required libraries in Arduino IDE

---

## Usage

1. Open the Arduino sketch
2. Set your target phone number in the `PhoneNumber` variable
3. Upload to Arduino
4. Monitor Serial output for:
   - Incoming messages
   - Signal strength
   - Sending retries
   - SD card logging

---

## Notes

- Make sure to replace the example phone number with your own.
- Ensure the SIM800L module has sufficient power and a valid SIM card.
- SD card should be formatted to FAT16/FAT32.

---

## License

This project is open-source. Feel free to use and modify for personal or educational purposes.
