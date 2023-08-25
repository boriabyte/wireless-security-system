#include <esp_now.h>
#include <WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include "NfcAdapter.h"

#define CS_PIN 5
#define LED_PIN1 15
#define LED_PIN2 21

MFRC522 mfrc522(CS_PIN, UINT8_MAX); // Create MFRC522 instance

NfcAdapter nfc = NfcAdapter(&mfrc522);

// REPLACE WITH YOUR ESP RECEIVER'S MAC ADDRESS - run example code in ARDUINO IDE to find MAC ADDRESS 
uint8_t broadcastAddress1[] = {0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX};
uint8_t broadcastAddress2[] = {0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX};

//test structs - packages used for transmitting info - replace with what you want
typedef struct test_struct {

  int x;
  float y;

} test_struct;

typedef struct test_struct2 {

  int a;
  float b;

} test_struct2;

//*******************************************************************************

test_struct test;
test_struct2 test2;

esp_now_peer_info_t peerInfo;

//**********TRANSMITTER********** - code that TRANSMITS THE DATA TO THE OTHER BOARDS
//can be omitted - WHOLLY DEPENDS ON THE FUNCTIONALITY YOU WANT TO IMPLEMENT IN WHICH BOARD
//example: if you ONLY WANT TO TRANSMIT, ONLY INCLUDE THIS PIECE OF CODE

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {

  char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
 snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");

}

//******************************

//***********RECEIVER*********** - code that RECEIVES THE DATA FROM THE OTHER BOARDS
//can be omitted - WHOLLY DEPENDS ON THE FUNCTIONALITY YOU WANT TO IMPLEMENT IN WHICH BOARD
//example: if you ONLY WANT TO RECEIVE, ONLY INCLUDE THIS PIECE OF CODE

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {

  memcpy(&test2, incomingData, sizeof(test2));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("a: ");
  Serial.println(test2.a);
  Serial.print("b: ");
  Serial.println(test2.b);
  Serial.println();

}
 
//******************************
 
void setup() {

  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  Serial.begin(115200);
 
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  //OMIT THE RESPECTIVE PIECE OF CODE IN CASE OF ONLY TRANSMITTER OR RECEIVER
  esp_now_register_send_cb(OnDataSent); //omit if only receiver
  esp_now_register_recv_cb(OnDataRecv); //omit if only transmitter
   
  // register peer
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // register first peer  
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  
  // register second peer  
  memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  
    SPI.begin();        // Init SPI bus
    mfrc522.PCD_Init(); // Init MFRC522
    nfc.begin();

}
 
void loop() {

      //TEST CONDITION - REPLACE WITH THAT YOU WISH - MODIFY CODE AT OWN WILL
      //THIS CODE IS USING THE SECOND STRUCTURE FOR TESTING PURPOSES - CAN BE DELETED
      /*
      if(test2.a == 5555){

            //LIGHT UP LED IF CONDITION MET 
            digitalWrite(LED_PIN1, HIGH);
            delay(1500);
            digitalWrite(LED_PIN1, LOW);
            test2.a = 0;
        
      }
      else if (test2.a != 5555 && test2.a != 0){

            //LIGHT UP LED IF CONDITION NOT MET
            digitalWrite(LED_PIN2, HIGH);
            delay(1500);
            digitalWrite(LED_PIN2, LOW);
            test2.a = 0;
      }
      */

      //RFID MODULE DETECTION OF NFC CARD
      if (nfc.tagPresent())
      {
          Serial.println("Reading NFC tag");
          NfcTag tag = nfc.read();
          Serial.println(tag.getTagType());
          Serial.print("UID: ");Serial.println(tag.getUidString());
      
          if (tag.hasNdefMessage()) // every tag won't have a message
          {
                NdefMessage message = tag.getNdefMessage();
                Serial.print("\nThis NFC Tag contains an NDEF Message with ");
                Serial.print(message.getRecordCount());
                Serial.print(" NDEF Record");
                if (message.getRecordCount() != 1) {
                      Serial.print("s");
                }
                Serial.println(".");
          
                // cycle through the records, printing some info from each
                int recordCount = message.getRecordCount();
                for (int i = 0; i < recordCount; i++)
                {
                      Serial.print("\nNDEF Record ");Serial.println(i+1);
                      NdefRecord record = message.getRecord(i);
              
                      Serial.print("  TNF: ");Serial.println(record.getTnf());
                      Serial.print("  Type: ");
              
                      // The TNF and Type should be used to determine how your application processes the payload
                      // There's no generic processing for the payload, it's returned as a byte[]
                      int payloadLength = record.getPayloadLength();
                      const byte *payload = record.getPayload();
              
                      // Force the data into a String (might work depending on the content)
                      // Real code should use smarter processing
                      String payloadAsString = "";
                      for (int c = 0; c < payloadLength; c++) {
                            payloadAsString += (char)payload[c];
                      }

                      payloadAsString = payloadAsString.substring(3);

                      test.x = payloadAsString.toInt();
                      test.y = 0;

                      Serial.print("  Payload (as String): ");
                      Serial.println(payloadAsString);
                      Serial.println("\n");

                      // id is probably blank and will return ""
                      if (record.getIdLength() > 0) {
                            Serial.print("  ID: ");
                      }
               }
          }
        
          if(test.x == 5555){
            
            digitalWrite(LED_PIN1, HIGH);
            delay(1500);
            digitalWrite(LED_PIN1, LOW);     

            esp_err_t result = esp_now_send(0, (uint8_t *) &test, sizeof(test_struct));

          }
          else{

            digitalWrite(LED_PIN2, HIGH);
            delay(1500);
            digitalWrite(LED_PIN2, LOW);
            
            esp_err_t result = esp_now_send(0, (uint8_t *) &test, sizeof(test_struct));
          }
      
      }
  

 delay(2000);

}
