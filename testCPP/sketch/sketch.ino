#include "testing.h"
#include "definitions.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>







BLEServer* pServer = NULL;

// Brightness Service
BLEService *brightnesSerice
// Flashlight Service

// Mode Service 

// Change Animation Service


BLEService *pService = NULL;
BLECharacteristic* pCharacteristic = NULL;

bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;


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

void setup()
{
    Serial.begin(115200);




    Serial.println("Starting BLE work!");

    BLEDevice::init("BASS CANYON TOTEM");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());


    pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
                                CHAR_ID_A,
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









void loop()
{
checkToReconnect();

  
  if (deviceConnected) {
//    Serial.println("Device connected");
//        pCharacteristic->setValue((uint8_t*)&value, 4);
//        pCharacteristic->notify();
        
//        value++;
//    std::string value = pCharacteristic->getValue();
//  Serial.print("The new characteristic value is: ");
//  Serial.println(value.c_str());
////        delay(10); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
//    }
//    else
//    {
//      Serial.println("Device Disconnected");
//    }
    
  // put your main code here, to run repeatedly:
  delay(2000);





}
