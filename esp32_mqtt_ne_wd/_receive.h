#include <Arduino.h>
#include <MqttConnector.h>

extern MqttConnector* mqtt;

extern String MQTT_CLIENT_ID;
extern String MQTT_PREFIX;

extern int relayPin;
extern int relayPinState;
extern char myName[];

extern int LED_PIN;

void register_receive_hooks() {
  mqtt->on_subscribe([&](MQTT::Subscribe *sub) -> void {
    Serial.printf("myName = %s \r\n", myName);
    sub->add_topic(MQTT_PREFIX + myName + "/$/+");
    sub->add_topic(MQTT_PREFIX + MQTT_CLIENT_ID + "/$/+");
  });

  mqtt->on_before_message_arrived_once([&](void) { });

  mqtt->on_message([&](const MQTT::Publish & pub) { });

  mqtt->on_after_message_arrived([&](String topic, String cmd, String payload) {
    Serial.printf("topic: %s\r\n", topic.c_str());
    Serial.printf("cmd: %s\r\n", cmd.c_str());
    Serial.printf("payload: %s\r\n", payload.c_str());
    if (cmd == "$/command") {
      if (payload == "ON") {
        digitalWrite(relayPin, HIGH);
        digitalWrite(LED_PIN, LOW);
        relayPinState = HIGH;
      }
      else if (payload == "OFF") {
        digitalWrite(relayPin, LOW);
        digitalWrite(LED_PIN, HIGH);
        relayPinState = LOW;
      }
      String inString = payload;
      if (inString.toFloat() == 0.0f) {
        Serial.println(inString);
        if (inString == "A")
        {
          colorWipe(strip.Color(0, 0, 255), 1);
//          Serial.println("Command completed LED turned ON");
        }
        if (inString == "B")
        {
          colorWipe(strip.Color(0, 255, 0), 1);
//          Serial.println("Command completed LED turned OFF");
        }
        if (inString == "E")
        {
          digitalWrite(16, LOW);
          colorWipe(strip.Color(255, 0, 0), 1);
//          Serial.println("Command completed LED turned OFF");
        }
        if (inString == "D")
        {
          colorWipe(strip.Color(255, 255, 255), 1);
//          Serial.println("Command completed LED turned OFF");
        }
        if (inString == "C")
        {
          rainbow(50);
//          Serial.println("Command completed LED turned OFF");
        }

      } else {
        Serial.println(inString.toFloat());
          myservo.write(inString.toFloat()*180);

      }
    }
    else if (cmd == "$/reboot") {
      ESP.restart();
    }
    else {
      // another message.
    }
  });
}
