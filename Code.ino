/***********************************************************************************************************************
  Project: Smart alarm with esp32 and telegram bot
  Components: esp32, X2 red led, PIR HC-SR501 motion sensor, breadboard, DHT11 temperature and humidity sensor, buzzer
***********************************************************************************************************************/

//libraries
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <DHT.h>

// wifi network station credentials
#define WIFI_SSID "wifi_name"
#define WIFI_PASSWORD "wifi_password"
// telegram BOT TOKEN (Get from Botfather)
#define BOT_TOKEN "xxxx:yyyyy"
// telegram CHAT ID (Get from IDBot)
#define CHAT_ID "111111111"

#define DHTPIN 5 //DHT11 pin
#define DHTTYPE DHT11

const unsigned long BOT_MTBS = 1000;  // mean time between scan messages

DHT dht(DHTPIN, DHTTYPE);

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime;  // last time messages' scan has been done

const int m_sensor = 19;       // motion sensor pin
const int buzzerPin = 23;      // buzzer pin
const int led1 = 18;           // led1 pin
const int led2 = 21;           // led2 pin
int pinStateCurrent = LOW;     // current state of motion sensor
bool alarmz = false;           // current state of alarm
bool trigger = false;

void setup() {
  delay(4000);
  Serial.begin(9600);  // initialize serial

  // attempt to connect to Wifi network:
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(m_sensor, INPUT);       // set ESP32 pin to input mode to read value from OUTPUT pin of sensor
  pinMode(buzzerPin, OUTPUT);     // set the buzzer pin as an output
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  //create a task that will be executed in the loop2() function, with priority 0 and executed on core 0
  xTaskCreatePinnedToCore(
    loop2,   /* Task function. */
    "loop2", /* name of task. */
    1000,    /* Stack size of task */
    NULL,    /* parameter of the task */
    0,       /* priority of the task */
    NULL,    /* Task handle to keep track of created task */
    0);      /* pin task to core 0 */
}

void loop2(void* pvParameters) {
  while (1) {
    pinStateCurrent = digitalRead(m_sensor);  // read state of the motion sensor

    if (pinStateCurrent == HIGH && alarmz == true) {  // pin state change: LOW -> HIGH
      trigger=true;
      // TODO: turn on alarm, light or activate a device ... here
      while (alarmz == true) {
        digitalWrite(buzzerPin, HIGH);
        digitalWrite(led1, HIGH);
        digitalWrite(led2, HIGH);
        delay(200);
        digitalWrite(buzzerPin, LOW);
        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        delay(200);
      }
    }
    delay(1000);
  }
}

void handleNewMessages(int numNewMessages) {
  //Serial.print("handleNewMessages");
  //Serial.println(numNewMessages);
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
    //--------------------------------------------------------------------------------------------------------------------
    //commands
    if (text == "/Temperature") {
      int t = dht.readTemperature();
      int h = dht.readHumidity();
      bot.sendMessage(chat_id, "Temperature: " + String(t) + " °C\n");
      bot.sendMessage(chat_id, "Umidity: " + String(h) + "%\n");
    }

    if (text == "/On") {
      alarmz=true;
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      bot.sendMessage(chat_id, "Allarm enabled");
    }

    if (text == "/Off") {
      alarmz=false;
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      bot.sendMessage(chat_id, "Allarm disabled");
    }
    //menù
    if (text == "/start") {
      String welcome = "Welcome, I'm Tails' assistant.\n\n";
      welcome += "Commands:\n";
      welcome += "/Temperature\n";
      welcome += "/On\n";
      welcome += "/Off\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
    //--------------------------------------------------------------------------------------------------------------------
  }
}

void loop() {
  if (millis() - bot_lasttime > BOT_MTBS) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    if(trigger==true){
      trigger=false;
      bot.sendMessage(CHAT_ID, "There’s someone in the house!!!");
    }
    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    bot_lasttime = millis();
  }
}
