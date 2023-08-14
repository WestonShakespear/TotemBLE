

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>



#define NAME "festifaerie.com"



BLEServer* pServer = NULL;




BLEService *pService = NULL;

// Brightness Service
BLECharacteristic *brightnessService = NULL;
// Flashlight Service
BLECharacteristic *flashlightService = NULL;
// Mode Service 
BLECharacteristic *modeService = NULL;
// Change Animation Service
BLECharacteristic *remoteService = NULL;


BLECharacteristic* pCharacteristic = NULL;

bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;




#define SERVICE_UUID "69929676-371d-4abb-ae50-8f14f2541656"

#define CHAR_ID_A "00947533-3143-470c-9c12-0816383efdd8"
#define CHAR_ID_B "017a77ec-5a68-45c8-972e-8442cac37928"
#define CHAR_ID_C "024ea33f-43da-4e62-9333-baf75af57c12"
#define CHAR_ID_D "03097d35-7f88-4bbb-9573-3b5f60f104b6"


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

    BLEDevice::init(NAME);
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());


    pService = pServer->createService(SERVICE_UUID);
    // pCharacteristic = pService->createCharacteristic(
    //                             CHAR_ID_A,
    //                             BLECharacteristic::PROPERTY_READ |
    //                             BLECharacteristic::PROPERTY_WRITE
    //                             );

    brightnessService = pService->createCharacteristic(
                                CHAR_ID_A,
                                BLECharacteristic::PROPERTY_READ |
                                BLECharacteristic::PROPERTY_WRITE
                                );
    
    flashlightService = pService->createCharacteristic(
                                CHAR_ID_B,
                                BLECharacteristic::PROPERTY_READ |
                                BLECharacteristic::PROPERTY_WRITE
                                );

    modeService = pService->createCharacteristic(
                                CHAR_ID_C,
                                BLECharacteristic::PROPERTY_READ |
                                BLECharacteristic::PROPERTY_WRITE
                                );

    remoteService = pService->createCharacteristic(
                                CHAR_ID_D,
                                BLECharacteristic::PROPERTY_READ |
                                BLECharacteristic::PROPERTY_WRITE
                                );

    brightnessService->setValue("brightness");
    flashlightService->setValue("flash");
    modeService->setValue("mode");
    remoteService->setValue("remote");


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





std::string modes[] = {"first", "second", "third"};
int modeCounter = 0;
#define NUM_MODES 3


void loop()
{
checkToReconnect();

  
    if (deviceConnected)
    {
        Serial.println("Device connected");

        std::string brightness = brightnessService->getValue();
        std::string flashlight = flashlightService->getValue();
        std::string mode = modeService->getValue();
        std::string remote = remoteService->getValue();

        Serial.print("Brightness:  ");
        Serial.print(brightness.c_str());

        Serial.print("  |Flashlight:  ");
        Serial.print(flashlight.c_str());

        Serial.print("  |Mode:  ");
        Serial.print(mode.c_str());

        Serial.print("  |Remote:  ");
        Serial.println(remote.c_str());

        if (mode.compare("next") == 0)
        {
            Serial.print("Change to next mode: ");
            modeCounter++;
            if (modeCounter >= NUM_MODES) modeCounter = 0;

            modeService->setValue(modes[modeCounter]);
            modeService->notify();

        }

        if (remote.compare("next") == 0)
        {
            Serial.println("Changed to next animation");
            remoteService->setValue("normal");
            remoteService->notify();

        }





    //        delay(10); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
    }
    else
    {
        Serial.println("Device Disconnected");
    }






    // put your main code here, to run repeatedly:
    delay(2000);





}











