/*
 * Project gamma
 * Description: Cluster head in the project
 * Author: Emil Tan
 * Date: 20/6/20
 */

int hasReceivedData; 
int ctr = 1; // increment counter every 5 minutes 

/********************************************************
 *                 BUFFER VARS                          *
 *******************************************************/
const size_t UART_TX_BUF_SIZE = 20;
uint8_t txBuf[UART_TX_BUF_SIZE]; 
char command[20]; 

/********************************************************
 *                  SLEEP VARS                          *
 *******************************************************/
SystemSleepConfiguration sleepConfig; 
SystemSleepResult sleepResult;
pin_t wakeUpPin;

/********************************************************
 *                    BLE VARS                          *
 *******************************************************/
const size_t SCAN_RESULT_COUNT = 20;
const unsigned long SCAN_PERIOD_MS = 2000;
unsigned long lastScan = 0;
BleScanResult scanResults[SCAN_RESULT_COUNT];
// UUIDs
const BleUuid betaUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid alphaUuid("7E400001-B5A3-F393-E0A9-E50E24DCCA9E"); 
const BleUuid betaRxUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid betaTxUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid alphaRxUuid("7E400002-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid alphaTxUuid("7E400003-B5A3-F393-E0A9-E50E24DCCA9E"); 
// Characteristcis, peer devices and addresses
BleCharacteristic betaTxCharacteristic;
BleCharacteristic betaRxCharacteristic;
BleCharacteristic alphaTxCharacteristic;
BleCharacteristic alphaRxCharacteristic;
BlePeerDevice beta;
BlePeerDevice alpha; 
BleAddress betaAddress;
BleAddress alphaAddress;

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {
 
    // if soil moisture > z || cloudy || air moistrure > P
    //    strcpy(command, "2");
    //    memcpy(txBuf, command, sizeof(txBuf));
    //    betaRxCharacteristic.setValue(txBuf, sizeof(txBuf));
    // else if soil moisture < X && sunny && air temperature > Y 
    //    strcpy(command, "0");
    //    memcpy(txBuf, command, sizeof(txBuf));
    //    betaRxCharacteristic.setValue(txBuf, sizeof(txBuf));

    //   hasReceivedData = HIGH;
}

void setup() {
  Serial.begin();
  BLE.on();
  betaTxCharacteristic.onDataReceived(onDataReceived, &betaTxCharacteristic);
  alphaTxCharacteristic.onDataReceived(onDataReceived, &alphaTxCharacteristic);

  sleepConfig.mode(SystemSleepMode::STOP)
    .duration(10s)
    .ble();
}

void loop() {
  /*******************************************
  *      WHEN CONNECTED TO SENSOR NODES     *
  *******************************************/
//   if (alpha.connected()) {
  if (beta.connected()) {
//   if (alpha.connected() && beta.connected()) {
    hasReceivedData = LOW;
    System.sleep(sleepConfig);
    
    /*******************************************
    *             BETA CODE TESTING            *
    *******************************************/
    
    strcpy(command, "WAKE THE FUCK UP");
    memcpy(txBuf, command, sizeof(txBuf));
    betaRxCharacteristic.setValue(txBuf, sizeof(txBuf));
    
    // // poll alpha node to measure only the sunlight and the air temperature (5 minutes)
    // if (ctr < 2) {
    //     strcpy(command, "1");
    //     memcpy(txBuf, command, sizeof(txBuf));
    //     alphaRxCharacteristic.setValue(txBuf, sizeof(txBuf));
    //     ctr++;
    // } 
    // // poll alpha node to measure every sensors (10 minutes)
    // else {
    //     strcpy(command, "2");
    //     memcpy(txBuf, command, sizeof(txBuf));
    //     alphaRxCharacteristic.setValue(txBuf, sizeof(txBuf));
    //     ctr = 1;
    // }
    // while (!hasReceivedData){
    //     delay(100);
    // };
  }
  /*******************************************
  *         ATTEMPTING TO CONNECT           *
  *******************************************/
  else {
    // preventing from scanning too often
    if (millis() - lastScan >= SCAN_PERIOD_MS) {
      lastScan = millis();
      size_t count = BLE.scan(scanResults, SCAN_RESULT_COUNT);
      if (count > 0) {
        // scanning surrounding ble devices 
        for (uint8_t ii = 0; ii < count; ii++) {
          BleUuid foundServiceUuid;
          size_t svcCount = scanResults[ii].advertisingData.serviceUUID(&foundServiceUuid, 1);
          // connecting to beta 
          if (svcCount > 0 && foundServiceUuid == betaUuid) {
            beta = BLE.connect(scanResults[ii].address);
            if (beta.connected()) {
              betaAddress = scanResults[ii].address;
              beta.getCharacteristicByUUID(betaTxCharacteristic, betaTxUuid);
              beta.getCharacteristicByUUID(betaRxCharacteristic, betaRxUuid);
            }
          break;
          }
          // connecting to alpha
          else if (svcCount > 0 && foundServiceUuid == alphaUuid) {
            alpha = BLE.connect(scanResults[ii].address); 
            if (alpha.connected()) {
              alphaAddress = scanResults[ii].address;
              alpha.getCharacteristicByUUID(alphaTxCharacteristic, alphaTxUuid);
              alpha.getCharacteristicByUUID(alphaRxCharacteristic, alphaRxUuid);
            }
          break;
          }
        }
      }
    }
  }   
}