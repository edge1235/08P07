#define PIN_LED  9
#define PIN_TRIG 12   
#define PIN_ECHO 13   

#define SND_VEL 346.0  
#define INTERVAL 25    
#define PULSE_DURATION 10 
#define _DIST_MIN 100.0 
#define _DIST_MAX 300.0  

#define TIMEOUT ((INTERVAL / 2) * 1000.0)
#define SCALE (0.001 * 0.5 * SND_VEL) 

unsigned long last_sampling_time;  

void setup() {
  // initialize GPIO pins
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);  
  pinMode(PIN_ECHO, INPUT);  
  digitalWrite(PIN_TRIG, LOW);  
  
  Serial.begin(57600);
  
  last_sampling_time = millis();
}

void loop() {
  float distance;
  
  unsigned long current_time = millis();
  
  if (current_time - last_sampling_time < INTERVAL)
    return;
  
  last_sampling_time = current_time;
  
  distance = USS_measure(PIN_TRIG, PIN_ECHO);
  
  int ledValue = 255; 
  if ((distance == 0.0) || (distance > _DIST_MAX)) {
      distance = _DIST_MAX + 10.0;   
      ledValue = 255;              
  } else if (distance < _DIST_MIN) {
      distance = _DIST_MIN - 10.0; 
      ledValue = 255;               
  } else {
      // 거리에 따라 LED 밝기 계산
      if (distance == 200.0) {
          ledValue = 0; // 최대 밝기
      }
      else if (distance == 100.0 || distance == 300.0) {
          ledValue = 255; // 최소 밝기 (꺼짐)
      }
      else if (distance == 150.0 || distance == 250.0) {
          ledValue = 128; // 50% 밝기
      }
      else {
          if (distance > _DIST_MIN && distance < 200.0) {
              ledValue = map(distance, 100, 200, 255, 0);
          }
          else if (distance > 200.0 && distance < _DIST_MAX) {
              ledValue = map(distance, 200, 300, 0, 255);
          }
      }
  }
  
  analogWrite(PIN_LED, ledValue);
  
  Serial.print("Min:");        Serial.print(_DIST_MIN);
  Serial.print(", distance:"); Serial.print(distance);
  Serial.print(", Max:");      Serial.print(_DIST_MAX);
  Serial.print(", LED Value:"); Serial.print(ledValue);
  Serial.println("");
  
}

float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  unsigned long duration = pulseIn(ECHO, HIGH, TIMEOUT);
  float distance = duration * SCALE; 
  
  if (isinf(distance) || isnan(distance)) {
      Serial.print("Invalid distance calculation. Duration: ");
      Serial.println(duration);
      return 0.0;
  }
  
  return distance;
}
