/*
 * Project Beta
 * Description: The Sensor Node 2 of the project 
 * Author: Emil Tan
 * Date: 18/6/20
 */

#define CASE_1 '1'
#define CASE_2 '2'

uint8_t test; 

/********************************************************
 *                 BUFFER VARS                          *
 *******************************************************/
const size_t UART_TX_BUF_SIZE = 20;
uint8_t txBuf[UART_TX_BUF_SIZE]; 
uint8_t rxBuf[UART_TX_BUF_SIZE];
char status[20]; 

/********************************************************
 *                  SLEEP VARS                          *
 *******************************************************/
SystemSleepConfiguration sleepConfig; 
SystemSleepResult sleepResult;
pin_t wakeUpPin;

/********************************************************
 *                    BLE VARS                          *
 *******************************************************/
const BleUuid serviceUuid("7E400001-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid rxUuid("7E400002-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid txUuid("7E400003-B5A3-F393-E0A9-E50E24DCCA9E");

BleCharacteristic txCharacteristic("tx", BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, onDataReceived, NULL);

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {
    test = data[0];
    Serial.write(data[0]);
}

void setup() {
  //SERIAL CONNECTION CONFIG
  Serial.begin();

  // BLE CONFIG
  BLE.on();
  BLE.addCharacteristic(txCharacteristic);
  BLE.addCharacteristic(rxCharacteristic);
  BleAdvertisingData bleData;
  bleData.appendServiceUUID(serviceUuid);
  BLE.advertise(&bleData);

  //SLEEP CONFIG
  sleepConfig.mode(SystemSleepMode::STOP)
    .duration(1200s) // sleep for 20 minutes 
    .ble(); 
}

void loop() {
  if (BLE.connected()) {
    delay(5000);
    sleepResult = System.sleep(sleepConfig);

    if (sleepResult.wakeupReason() == SystemSleepWakeupReason::BY_BLE) {
      Serial.write("WOKEN UP BY BT");
      
      Serial.write(test);
      
      switch (test) {
        case CASE_1 : {
            Serial.write("CASE 1");
            strcpy(status, "1");
            memcpy(txBuf, status, sizeof(txBuf));
            txCharacteristic.setValue(txBuf, sizeof(txBuf));
            break; 
        }
        case CASE_2 : {
            Serial.write("CASE 2");
            strcpy(status, "1");
            memcpy(txBuf, status, sizeof(txBuf));
            txCharacteristic.setValue(txBuf, sizeof(txBuf));            
            break; 
        } 
      }  
    }
  }
} 