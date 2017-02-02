#include <RFM69.h>
#include <SPI.h>

#define NETWORKID 100 //same on all boards
#define NODEID 1 //individual to this
#define SLAVEID 2 //who we're sending to

#define FREQUENCY RF69_915MHZ //"you can't take the sky.."
#define IS_RFM69HCW true //board type and ya the tutorial

#define RFM69_CS      10
#define RFM69_IRQ     2
#define RFM69_IRQN    0  // Pin 2 is IRQ 0!
#define RFM69_RST     9

Messages sendBuffer;

RFM69 radio = RFM69(RFM69_CS, RFM69_IRQ, IS_RFM69HCW, RFM69_IRQN);

void ini_rf(){
  pinMode(RFM69_RST, OUTPUT);

  digitalWrite(RFM69_RST, HIGH);
  delay(100);
  digitalWrite(RFM69_RST, LOW);
  delay(100);

  if (!radio.initialize(FREQUENCY, NODEID, NETWORKID))
    Serial.println("initialize failed");

  if (IS_RFM69HCW)
    radio.setHighPower();
    
  radio.setPowerLevel(31); //highest possible
  radio.encrypt(ENCRYPTKEY);

  Serial.print("\nTransmitting at ");
  Serial.print(radio.getFrequency());
  Serial.println(" Hz");
  
}

bool radioPoleUpdate(){
  sendBuffer.message.type = CMD_MESSAGE;
  sendBuffer.message.payload.cmd.valveTarget = targetAngle;

  
  if(radio.sendWithRetry(SLAVEID,sendBuffer.data,CMD_SIZE,100)){
    Messages* reply = (Messages*)(radio.DATA);

    switch(reply->message.type){
      case 's':
      angle = reply->message.payload.stat.valveAngle;
      valveBattery = reply->message.payload.stat.bateryPercent;
      #ifdef VALVE_MASTER_DEBUG
      Serial.print("Angle: ");
      Serial.println(reply->message.payload.stat.valveAngle);
      Serial.print("Target: ");
      Serial.println(reply->message.payload.stat.valveTarget);
      Serial.print("Battery: ");
      Serial.println(reply->message.payload.stat.bateryPercent);
      Serial.print("State: ");
      Serial.println(reply->message.payload.stat.state);
      #endif
      break;
    }

    return true;
  }
  return false;
  
}

