#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
String knownBLEAddresses[] = {"ff:ff:10:36:29:cd"};
int RSSI_THRESHOLD = -55;
bool device_found = false;
int scanTime = 1; //In seconds
BLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      for (int i = 0; i < (sizeof(knownBLEAddresses) / sizeof(knownBLEAddresses[0])); i++)
      {
        // Uncomment to Enable Debug Information
        // Serial.println("*************Start**************");
        // Serial.println(sizeof(knownBLEAddresses));
        // Serial.println(sizeof(knownBLEAddresses[0]));
        // Serial.println(sizeof(knownBLEAddresses)/sizeof(knownBLEAddresses[0]));
        // Serial.println(advertisedDevice.getAddress().toString().c_str());
        // Serial.println(knownBLEAddresses[i].c_str());
        // Serial.println("*************End**************");
        
        if (strcmp(advertisedDevice.getAddress().toString().c_str(), knownBLEAddresses[i].c_str()) == 0) {
          device_found = true;
        //  Serial.print("\nIts a match!\n");
         // Serial.print("Tag address: " + knownBLEAddresses[i] + "\n");
         // Serial.print("Tag address found: " + String(advertisedDevice.getAddress().toString().c_str()) + "\n");
         // Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
          break;
        }
      }
    }
};

void setup() {
  Serial.begin(115200); //Enable UART on ESP32
  Serial.println("Scanning..."); // Print Scanning
  pinMode(5, OUTPUT); //make BUILTIN_LED pin as output
  pinMode(17,OUTPUT);
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks()); //Init Callback Function
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(1); // set Scan interval
  pBLEScan->setWindow(1);  // less or equal setInterval value
}
void loop() {
BLEScanResults foundDevices = pBLEScan->start(scanTime, false);

  for (int i = 0; i < foundDevices.getCount(); i++) {
      BLEAdvertisedDevice device = foundDevices.getDevice(i);
  
      if (String(device.getAddress().toString().c_str()) == knownBLEAddresses[0]) {
        int rssi = device.getRSSI();
        Serial.print("RSSI: ");
        Serial.println(rssi);
        
        if (rssi > RSSI_THRESHOLD && device_found == true) {
          digitalWrite(5, LOW);
          digitalWrite(17, HIGH);
          device_found = false;
          break;
        } else {
          digitalWrite(5, HIGH);
          digitalWrite(17, LOW);
          break;
        }
      }
   }
  
  pBLEScan->clearResults();
}
