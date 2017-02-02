
#define VALVE_MASTER_DEBUG
#define AC_LED_PIN 7
#define PR_LED_PIN 4
#define CN_LEN_PIN 8
byte targetAngle;
byte angle;
byte valveBattery;

void setup() {
  #ifdef VALVE_MASTER_DEBUG
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Loaded with MASTER in DEBUG");
  #endif

  pinMode(AC_LED_PIN,OUTPUT);
  pinMode(PR_LED_PIN,OUTPUT);
  pinMode(CN_LEN_PIN,OUTPUT);

  ini_rf();

}

void loop() {

  float analog = 0;

  for(int i = 0; i < 50; ++i){
    analog += analogRead(A5);
    delay(5);
  }

  analog /= 50;

  targetAngle = map(analog,0,1024,0,180);
  
  if(radioPoleUpdate()){
    digitalWrite(AC_LED_PIN,abs(targetAngle - angle)<5);
    digitalWrite(PR_LED_PIN,valveBattery < 20);
    digitalWrite(CN_LEN_PIN,HIGH);
  }else{
    digitalWrite(AC_LED_PIN,LOW);
    digitalWrite(PR_LED_PIN,LOW);
    digitalWrite(CN_LEN_PIN,LOW);
  }
  
  

}
