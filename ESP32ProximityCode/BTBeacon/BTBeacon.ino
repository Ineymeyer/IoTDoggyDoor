#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

String knownBLEAddresses[] = {"ff:ff:10:36:29:cd"}; // Array with known BLE addresses
int RSSI_THRESHOLD = -55,                           // If the RSSI of the device is lower, the device is near, if it is higher, the device is too far
    scanTime = 1,                                   // Scan time in seconds
    missCount = 0;                                  // Keeps track of consecutive scan misses
bool device_found = false;                          // Boolean that indicates if the device has been found during a scan
BLEScan* pBLEScan;                                  // Creates a new scan object

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
	// Callback function that runs after a device is found during the scan
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      for (int i = 0; i < (sizeof(knownBLEAddresses) / sizeof(knownBLEAddresses[0])); i++) {
        // Uncomment to Enable Debug Information
        /* 
		Serial.println("*************Start**************");
        Serial.println(sizeof(knownBLEAddresses));
        Serial.println(sizeof(knownBLEAddresses[0]));
        Serial.println(sizeof(knownBLEAddresses)/sizeof(knownBLEAddresses[0]));
        Serial.println(advertisedDevice.getAddress().toString().c_str());
        Serial.println(knownBLEAddresses[i].c_str());
        Serial.println("*************End**************");
		*/
        
        if (strcmp(advertisedDevice.getAddress().toString().c_str(), knownBLEAddresses[0].c_str()) == 0) {
          device_found = true;
          missCount = 0;
		  // Uncomment to Enable Debug Information
		  /*
		  Serial.print("\nIts a match!\n");
          Serial.print("Tag address: " + knownBLEAddresses[i] + "\n");
          Serial.print("Tag address found: " + String(advertisedDevice.getAddress().toString().c_str()) + "\n");
          Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
		  */
          break;
        }
      }
    }
};

void setup() {
  Serial.begin(115200); // Enable UART on ESP32
  BLEDevice::init("");  // Initialize the ESP32
  
  // Pin configuration
  pinMode(5, OUTPUT);    // Make pin 5 as output enabling the onboard LED
  pinMode(17, OUTPUT);   // Set output pin that indicates to the Pi if the tag is near
  pinMode(16, OUTPUT);   // Set output pin that indicates to the Arduino if the tag is near
  digitalWrite(5, HIGH); // Begin with the LED off (HIGH = LED off, LOW = LED on)
  
  // Scan configuration
  pBLEScan = BLEDevice::getScan(); // Create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks()); // Initialize Callback Function
  pBLEScan->setActiveScan(true); // Active scan uses more power, but get results faster
  pBLEScan->setInterval(1); // Set Scan interval
  pBLEScan->setWindow(1);  // Less or equal setInterval value
}

void loop() {
  // Start a new scan
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  
  // If the device is not found, increment the miss count by one
  if (device_found == false) {
    missCount++;
    // Serial.print(missCount);
	
	// If the device has missed 5 or more times, reset the pins to their initial states
    if (missCount >= 5) {
      digitalWrite(5, HIGH);
      digitalWrite(17, LOW);
      digitalWrite(16, LOW);
      // Serial.print("Device not found!\n");
    }
  }
  
  // Loops over all devices found during the scan
  for (int i = 0; i < foundDevices.getCount(); i++) {
      BLEAdvertisedDevice device = foundDevices.getDevice(i);
	  
	  // If the address of the current device is equal to the saved BLE address get and print the RSSI to the serial monitor
      if (String(device.getAddress().toString().c_str()) == knownBLEAddresses[0]) {
        int rssi = device.getRSSI();
        Serial.print("RSSI: ");
        Serial.println(rssi);
        
		// If the RSSI is within the threshold and it is found, turn on the LED and set the indicator pins high, then reset the boolean to false
		// If it is not found, turn off the LED and set the indicator pins low
        if (rssi > RSSI_THRESHOLD && device_found == true) {
          digitalWrite(5, LOW);
          digitalWrite(17, HIGH);
          digitalWrite(16, HIGH);
          device_found = false;
          break;
        } else {
          digitalWrite(5, HIGH);
          digitalWrite(17, LOW);
          digitalWrite(16, LOW);
          break;
        }
      }
   }
  
  // Clear the results of the scan
  pBLEScan->clearResults();
}