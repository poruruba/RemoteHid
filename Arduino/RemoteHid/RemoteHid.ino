#include <Arduino.h>

#define M5STICKC

#ifdef M5STICKC
#include <M5StickC.h>
#endif
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <BLEHIDDevice.h>
#include <HIDTypes.h>
#include <WiFi.h>
#include <WiFiServer.h>
#include <ArduinoJson.h>
#include <aREST.h>
#include <base64.hpp>
#include "asciimap.h"

// BLEのパスキー
#define BLE_PASSKEY 123456

const char* wifi_ssid = "【WiFiアクセスポイントのSSID】";
const char* wifi_password = "【WiFiアクセスポイントのパスワード】";

const char* deviceName = "RemoteHid";
const char* manufacturerName = "RemoteHid";

// GET接続を待ち受けるポート番号
#define REST_PORT     80

#define MAX_MACROS 4

const int message_capacity = JSON_OBJECT_SIZE(1) + JSON_ARRAY_SIZE(MAX_MACROS) + MAX_MACROS * JSON_OBJECT_SIZE(3);
StaticJsonDocument<message_capacity> json_message;
char message_buffer[1024];
char base64_buffer[1024];

WiFiServer server(REST_PORT);
aREST rest = aREST();

void sendKeys(uint8_t mod, const uint8_t *keys, uint8_t num_key);
void sendKeyString(const char* ptr);

void debug_dump(const uint8_t *p_bin, uint16_t len){
  for( uint16_t i = 0 ; i < len ; i++ ){
    Serial.print((p_bin[i] >> 4) & 0x0f, HEX);
    Serial.print(p_bin[i] & 0x0f, HEX);
  }
  Serial.println("");
}

unsigned char tohex(char c){
  if( c >= '0' && c <= '9')
    return c - '0';
  if( c >= 'a' && c <= 'f' )
    return c - 'a' + 10;
  if( c >= 'F' && c <= 'F' )
    return c - 'A' + 10;

  return 0;
}

long parse_hex(const char* p_hex, unsigned char *p_bin){
  int index = 0;
  while( p_hex[index * 2] != '\0'){
    p_bin[index] = tohex(p_hex[index * 2]) << 4;
    p_bin[index] |= tohex(p_hex[index * 2 + 1]);
    index++;
  }

  return index;
}

String procPutKey(String command) {
  Serial.println("procPutKey called");
  Serial.println(command);
  
  decode_base64((unsigned char*)command.c_str(), (unsigned char*)base64_buffer);
  DeserializationError err = deserializeJson(json_message, base64_buffer);
  if( err ){
    Serial.print("Deserialize error: ");
    Serial.println(err.c_str());
    return "Deserialize Error";
  }

  JsonArray arry = json_message["keys"];
  for( int i = 0 ; i < arry.size(); i++ ){
    const char* type = arry[i]["type"];
    if( strcmp(type, "code") == 0 ){
      const char* code = arry[i]["code"];
      int mod = arry[i]["mod"];
      Serial.print("code=");
      Serial.println(code);
      Serial.print("mod=");
      Serial.println(mod);

      unsigned char code_bin[6]; 
      if( strlen(code) > sizeof(code_bin) * 2 ){
        Serial.println("Code size Error");
        return "Code size Error";
      }

      int code_len = parse_hex(code, code_bin);
      sendKeys(mod, code_bin, code_len);
    }else if( strcmp(type, "text") == 0 ){
      const char* text = arry[i]["text"];
      Serial.print("text=");
      Serial.println(text);

      sendKeyString(text);
    }
  }

  return "OK";
}

/*
 * BLEデバイス処理
 */
BLEHIDDevice* hid;
BLECharacteristic* input;
BLECharacteristic* output;

bool connected = false;

class MyCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer){
    connected = true;
    BLE2902* desc = (BLE2902*)input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    desc->setNotifications(true);
  }

  void onDisconnect(BLEServer* pServer){
    connected = false;
    BLE2902* desc = (BLE2902*)input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    desc->setNotifications(false);
  }
};

// ペアリング処理用
class MySecurity : public BLESecurityCallbacks {
  bool onConfirmPIN(uint32_t pin){
    return false;
  }

  uint32_t onPassKeyRequest(){
    Serial.println("ONPassKeyRequest");
    return BLE_PASSKEY;
  }

  void onPassKeyNotify(uint32_t pass_key){
    // ペアリング時のPINの表示
    Serial.println("onPassKeyNotify number");
    Serial.println(pass_key);
  }

  bool onSecurityRequest(){
    Serial.println("onSecurityRequest");
    return true;
  }

  void onAuthenticationComplete(esp_ble_auth_cmpl_t cmpl){
    Serial.println("onAuthenticationComplete");
    if(cmpl.success){
      // ペアリング完了
      Serial.println("auth success");
    }else{
      // ペアリング失敗
      Serial.println("auth failed");
    }
  }
};

// BLEデバイスの起動
void taskServer(void*){
  BLEDevice::init(deviceName);

  BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT_MITM);
  BLEDevice::setSecurityCallbacks(new MySecurity());  

  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyCallbacks());

  hid = new BLEHIDDevice(pServer);
  input = hid->inputReport(1); // <-- input REPORTID from report map
  output = hid->outputReport(1); // <-- output REPORTID from report map

  hid->manufacturer()->setValue(manufacturerName);

  hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
  hid->hidInfo(0x00,0x02);

  BLESecurity *pSecurity = new BLESecurity();
  pSecurity->setKeySize(16);

  pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);
  pSecurity->setCapability(ESP_IO_CAP_OUT);
  pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);

  uint32_t passkey = BLE_PASSKEY;
  esp_ble_gap_set_security_param(ESP_BLE_SM_SET_STATIC_PASSKEY, &passkey, sizeof(uint32_t));

    const uint8_t report[] = {
      USAGE_PAGE(1),      0x01,       // Generic Desktop Ctrls
      USAGE(1),           0x06,       // Keyboard
      COLLECTION(1),      0x01,       // Application
      REPORT_ID(1),       0x01,        //   Report ID (1)
      USAGE_PAGE(1),      0x07,       //   Kbrd/Keypad
      USAGE_MINIMUM(1),   0xE0,
      USAGE_MAXIMUM(1),   0xE7,
      LOGICAL_MINIMUM(1), 0x00,
      LOGICAL_MAXIMUM(1), 0x01,
      REPORT_SIZE(1),     0x01,       //   1 byte (Modifier)
      REPORT_COUNT(1),    0x08,
      HIDINPUT(1),           0x02,       //   Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position
      REPORT_COUNT(1),    0x01,       //   1 byte (Reserved)
      REPORT_SIZE(1),     0x08,
      HIDINPUT(1),           0x01,       //   Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position
      REPORT_COUNT(1),    0x06,       //   6 bytes (Keys)
      REPORT_SIZE(1),     0x08,
      LOGICAL_MINIMUM(1), 0x00,
      LOGICAL_MAXIMUM(1), 0x65,       //   101 keys
      USAGE_MINIMUM(1),   0x00,
      USAGE_MAXIMUM(1),   0x65,
      HIDINPUT(1),           0x00,       //   Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position
      REPORT_COUNT(1),    0x05,       //   5 bits (Num lock, Caps lock, Scroll lock, Compose, Kana)
      REPORT_SIZE(1),     0x01,
      USAGE_PAGE(1),      0x08,       //   LEDs
      USAGE_MINIMUM(1),   0x01,       //   Num Lock
      USAGE_MAXIMUM(1),   0x05,       //   Kana
      HIDOUTPUT(1),          0x02,       //   Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile
      REPORT_COUNT(1),    0x01,       //   3 bits (Padding)
      REPORT_SIZE(1),     0x03,
      HIDOUTPUT(1),          0x01,       //   Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile
      END_COLLECTION(0)
    };

  hid->reportMap((uint8_t*)report, sizeof(report));
  hid->startServices();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->setAppearance(HID_KEYBOARD);
  pAdvertising->addServiceUUID(hid->hidService()->getUUID());
  pAdvertising->start();
  hid->setBatteryLevel(7);

//  Serial.println("Advertising started!");
  delay(portMAX_DELAY);
};

void sendKeys(uint8_t mod, const uint8_t *keys, uint8_t num_key = 1){
  BLE2902* desc = (BLE2902*)input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
  if( !desc->getNotifications() )
    return;

  uint8_t msg[] = {mod, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  for( int i = 0 ; i < num_key && i < 6 ; i++ ){
    msg[2 + i] = keys[i];
  }

  input->setValue(msg, sizeof(msg));
  debug_dump(msg, sizeof(msg));
  input->notify();

  uint8_t msg1[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  input->setValue(msg1, sizeof(msg1));
  debug_dump(msg1, sizeof(msg1));
  input->notify();
    
  delay(20);
}

void sendKeyString(const char* ptr){
  BLE2902* desc = (BLE2902*)input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
  if( !desc->getNotifications() )
    return;
      
  // 1文字ずつHID(BLE)で送信
  while(*ptr){
    if( *ptr >= ASCIIMAP_SIZE_JP ){
      ptr++;
      continue;
    }
    KEYMAP map = asciimap_jp[(uint8_t)*ptr];
    uint8_t msg[] = {map.modifier, 0x00, map.usage, 0x00, 0x00, 0x00, 0x00, 0x00};
    input->setValue(msg, sizeof(msg));
    debug_dump(msg, sizeof(msg));
    input->notify();
    ptr++;
    
    uint8_t msg1[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    input->setValue(msg1, sizeof(msg1));
    debug_dump(msg1, sizeof(msg1));
    input->notify();
    
    delay(20);
  }
}

void wifi_connect(void){
  Serial.print("WiFi Connenting");
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected : ");
  Serial.println(WiFi.localIP());
#ifdef M5STICKC
  M5.Lcd.println(WiFi.localIP());
#endif
}

void setup() {
#ifdef M5STICKC
  M5.begin();
  M5.IMU.Init();

  M5.Axp.ScreenBreath(9);
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);

  M5.Lcd.printf("[%s]\n", deviceName);
  M5.Lcd.println(" A:Switch Desktop");
  M5.Lcd.println(" B:Save Desktop");
  delay(500);
#endif
    
  Serial.begin(9600);
  Serial.printf("Starting %s!\n", deviceName);

  // BLEデバイスの起動処理の開始
  xTaskCreate(taskServer, "server", 20000, NULL, 5, NULL);

  // GETエンドポイントの定義
  rest.function("putkey", procPutKey);
  
  // Give name & ID to the device (ID should be 6 characters long)
  rest.set_id("0001");
  rest.set_name("esp32");
  
  // Webサーバ起動
  wifi_connect();
  server.begin();
  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    // GET呼び出しを検知
    for( int i = 0 ; i < 10000; i += 10 ){
      if(client.available()){
        // GET呼び出しのコールバック呼び出し
        rest.handle(client);
        return;
      }
      delay(10);
    }
    // まれにGET呼び出し受付に失敗するようです。
    Serial.println("timeout");
  }

#ifdef M5STICKC
  M5.update();

  if( M5.BtnA.wasReleased() ){
    if(connected){
        Serial.println("BtnA released");
        // Switch Desktop
        uint8_t keys[] = { 0x07 /* D */};
        sendKeys(KEY_MASK_WIN, keys, 1);

        delay(10);
    }
  }
  if( M5.BtnB.wasReleased() ){
    if(connected){
        Serial.println("BtnB released");
        // Save Desktop
        uint8_t keys[] = { 0x46 /* PrintScreen */};
        sendKeys(KEY_MASK_WIN, keys, 1);

        delay(10);
    }
  }
#endif
}
