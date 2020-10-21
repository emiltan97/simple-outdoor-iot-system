/*
 * Project Beta
 * Description: The Sensor Node 2 of the project 
 * Author: Emil Tan
 * Date: 18/6/20
 */

#define CASE_0 '0'
#define CASE_1 '1'
#define CASE_2 '2'

uint8_t test; 
int led = D7;
Timer timer(10000, turn_off); 
int led_status = LOW;

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
const BleUuid serviceUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid rxUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid txUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");

BleCharacteristic txCharacteristic("tx", BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, onDataReceived, NULL);

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {
    test = data[0];
}

void turn_off() {
    digitalWrite(D7, LOW);
    led_status = LOW;
    timer.stop();
}

void turn_off_until_clear() {
    noInterrupts(); 
    
    digitalWrite(D7, LOW); 
    timer.stop();
    
    Serial.write("LIGHT and TIMER has stopped."); 
    
    while (digitalRead(D2)){
        Serial.write("D2 IS STILL HIGH"); 
    };
    
    Serial.write("D2 IS NO LONGER HIGH");
    
    timer.start();
    digitalWrite(D7, HIGH);
    
    interrupts();
}

void setup() {
  pinMode(D7, OUTPUT);
    
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
    
  pinMode(D2, INPUT_PULLDOWN);
  pinMode(D3, INPUT_PULLDOWN);
    
  attachInterrupt(D2, turn_off_until_clear, RISING); 
  attachInterrupt(D3, turn_off, RISING); 
}

void loop() {
  if (BLE.connected()) {
    if (!led_status){
        delay(5000);
        
        sleepResult = System.sleep(sleepConfig);
        
        if (sleepResult.wakeupReason() == SystemSleepWakeupReason::BY_BLE) {
        digitalWrite(D7, HIGH);
        led_status = HIGH;
        timer.start();
    }
    // if (sleepResult.wakeupReason() == SystemSleepWakeupReason::BY_GPIO) {
    //   wakeUpPin = sleepResult.wakeupPin();
    //   switch (wakeUpPin) {
    //     case 2 : {
    //       Serial.write("D2 is HIGH");
          
          
          
    //       strcpy(status, "1");
    //       memcpy(txBuf, status, sizeof(txBuf));
    //       txCharacteristic.setValue(txBuf, sizeof(txBuf));
    //       break; 
    //     }
    //     case 3 : {
    //       Serial.write("D3 is HIGH");
    //       strcpy(status, "2");
    //       memcpy(txBuf, status, sizeof(txBuf));
    //       txCharacteristic.setValue(txBuf, sizeof(txBuf));
    //       break; 
    //     }
    //   }
    // } 
    }
  }
} 