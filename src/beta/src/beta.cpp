/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/Acer/Desktop/ELEC4740_IoT/project/src/beta/src/beta.ino"
/*
 * Project Beta
 * Description: The Sensor Node 2 of the project 
 * Author: Emil Tan
 * Date: 18/6/20
 */

/********************************************************
 *                 BUFFER VARS                          *
 *******************************************************/
void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);
void setup();
void loop();
#line 11 "c:/Users/Acer/Desktop/ELEC4740_IoT/project/src/beta/src/beta.ino"
const size_t UART_TX_BUF_SIZE = 20;
uint8_t txBuf[UART_TX_BUF_SIZE]; 
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
const BleUuid serviceUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid rxUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid txUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");

BleCharacteristic txCharacteristic("tx", BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, onDataReceived, NULL);

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {
  switch(data[0]) {
    case 0 : {
      break; 
    }
    case 1 : {
      //turn on 
      break; 
    }
    case 2 : {
      //turn off 
      break; 
    }
  }
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
    .gpio(D2, RISING)
    .gpio(D3, RISING) 
    .ble(); 
}

void loop() {
  if (BLE.connected()) {
    sleepResult = System.sleep(sleepConfig);
    if (sleepResult.wakeupReason() == SystemSleepWakeupReason::BY_GPIO) {
      wakeUpPin = sleepResult.wakeupPin();
      switch (wakeUpPin) {
        case 2 : {
          strcpy(status, "1");
          break; 
        }
        case 3 : {
          strcpy(status, "2");
          break; 
        }
      }
    memcpy(txBuf, status, sizeof(txBuf));
    txCharacteristic.setValue(txBuf, sizeof(txBuf));
    } 
  }
}