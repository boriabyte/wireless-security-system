# wireless-security-system
An ESP-32 based security system which functions on the basis of NFC card and NFC card readers.

## Table of contents
### 1. Motivation and origins
### 2. How it works
### 3. Technologies used
### 4. Challenges & potential uses

## 1. Motivation and origins
Developed at a Summer school organized by Google and [Digital Stack](https://digitalstack.ro/en/home/) - Unlocking the possibilites of IoT - this little project has been done by myself & two other fellow students: Hoang Pham Nhat and Mihai Tatomir. This is something original we came up with, and it was, more or less, a requirement to pass the school. Besides the origin, this idea also interested us because it outlined the importance of such technology and the way our current world functions.

## 2. How it works
By including a multitude of libraries which incorporate the functionality of all the components used (NFC card, NFC reader, ESP-32 board, LEDs, a buzzer and a proximity sensor), this ensamble has its components communicate wirelessly, using WiFi technology. Each ESP-32 board (each member of the team having one - though the number may be virtually unlimited) has an NFC reader and two LEDs so that each one of us can use an NFC card and transmit the signal and information to the other two. Additionally, Pham also had a proximity sensor which would transmit a package containing the distance of the closest object to the sensor itself, while Mihai had a buzzer which would emit a sound when a certain condition was met (in our case, and for test purposes, one of the three NFC cards had the number 5555 written on it - this was the condition needed to be met); besides this, the green LED of each one of us would light up, meaning confirmation. When the condition would not be met, the buzzer would not sound anything, and a red LED would light up for all three of us.

A very useful feature is that, even though it uses WiFi, this system does not need to be connected to the internet to function; this was tested during development.

## 3. Technologies used
### a) software
      - Arduino IDE
      - C++ variation for Arduino IDE

### b) hardware
      - ESP32 board x3
      - Piezo buzzer x1
      - Ultrasonic proxmity sensor HC-SR04 x1
      - NFC cards x3
      - NFC reader - RFID module RC522 x3
      - LEDs x6

## 4. Challenges & potential uses
We met some challenges when we needed to understand how to transmit the data and in what format: we did not want to make things too complicated using JSON formats or some other ones, so we went ahead with just some simple data structures, whose data would be sent as parameters to some built-in functions. Another challenge was the occasional "lag" of the NFC reader which would not pick up the NFC cards, thus not transmitting the data to the other boards.

Regarding uses, there are multiple: a card-based security system which is able to communicate wirelessly, even without WiFi, data transmission over long distances of different things contained in packages such as distance, temperature and multiple other parameters which can also be used as security conditions.
