

//******************************TODO ***************************//
// create failsafe mode by shorting some pins

//******************************TODO ***************************//
#include <FastLED.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"


//test
#define NUM_LEDS 1

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 3
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

BLEServer* pServer = NULL;
BLEService *pService = NULL;
BLECharacteristic* pCharacteristic = NULL;

bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;


#define SERVICE_UUID        "69d6506b-e6f4-4328-a251-0e0066583c15"


#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"



#define CHAR_ID_A "17947533-3143-470c-9c12-0816383efdd8"
#define CHAR_ID_B "da7a77ec-5a68-45c8-972e-8442cac37928"
#define CHAR_ID_C "594ea33f-43da-4e62-9333-baf75af57c12"
#define CHAR_ID_D "84097d35-7f88-4bbb-9573-3b5f60f104b6"
#define CHAR_ID_E "5b7883e9-f159-4f98-938e-254691847ef8"
#define CHAR_ID_F "d50fd81c-38b8-4251-9ad9-67ad3363791e"
#define CHAR_ID_G "9bb19edf-25eb-4b41-8195-25bb1e6a41b9"
#define CHAR_ID_H "8069e5df-633d-4943-9b1a-f49a72992d6c"
#define CHAR_ID_I "eb65962a-0f0a-4aa5-a38f-5c9ce8852f11"
#define CHAR_ID_J "19929676-371d-4abb-ae50-8f14f2541656"

int uuidToInt(std::string uuid)
{
    std::string first(1, uuid[0]);
    std::string second(1, uuid[1]);

    return (std::stoi(first) * 10) + std::stoi(second);
}



class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("Connected");
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println("Disconnected");
    }

    void onRead(BLECharacteristic* pCharacteristic, esp_ble_gatts_cb_param_t *param)
    {
      Serial.println("Read");
    }
    void onWrite(BLECharacteristic* pCharacteristic, esp_ble_gatts_cb_param_t *param)
    {
      Serial.println("Write");
    }
};


class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      std::string uuid = pCharacteristic->getUUID().toString();

      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("UUID (");

        for (int i = 0; i < uuid.length(); i++) {
          Serial.print(uuid[i]);
        }

        Serial.print(") changed to: ");

        for (int i = 0; i < rxValue.length(); i++) {
          Serial.print(rxValue[i]);
        }

        Serial.println();

        // Do stuff based on the command received from the app
//        if (rxValue.find("1") != -1) { 
//          Serial.println("Turning ON!");
//          digitalWrite(LED, HIGH);
//        }
//        else if (rxValue.find("0") != -1) {
//          Serial.println("Turning OFF!");
//          digitalWrite(LED, LOW);
//        }

        Serial.println();
        Serial.println("*********");
      }
    }
};

void checkToReconnect() //added
{
  // disconnected so advertise
  if (!deviceConnected && oldDeviceConnected) {
    delay(500); // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("Disconnected: start advertising");
    oldDeviceConnected = deviceConnected;
  }
  // connected so reset boolean control
  if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
    Serial.println("Reconnected");
    oldDeviceConnected = deviceConnected;
  }
}



void setup() {
  uuidToInt(CHAR_ID_A);
  
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("BASS CANYON TOTEM");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  
  pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                               CHARACTERISTIC_UUID,
                               BLECharacteristic::PROPERTY_READ |
                               BLECharacteristic::PROPERTY_WRITE
                             );

  pCharacteristic->setValue("Hello World says Neil");
  pCharacteristic->setCallbacks(new MyCallbacks());
  
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
  checkToReconnect();

  
  if (deviceConnected) {
    Serial.println("Device connected");
//        pCharacteristic->setValue((uint8_t*)&value, 4);
//        pCharacteristic->notify();
        
//        value++;
//    std::string value = pCharacteristic->getValue();
//  Serial.print("The new characteristic value is: ");
//  Serial.println(value.c_str());
//        delay(10); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
    }
    else
    {
      Serial.println("Device Disconnected");
    }
    
  // put your main code here, to run repeatedly:
  delay(2000);
}