#include <SPI.h>

#define SLAVE 10
#define RelayPin 2
#define HydrolysisTemp 60
#define HydrolysisTime 3600
#define RoastTemp 210
#define RoastTime 4800

int i =0;                       //time count
int phase = 1;
int hydrolysis_time_elapsed = 0;
int roast_time_elapsed = 0;


void setup() {

#ifdef GND
  pinMode(GND, OUTPUT);
  digitalWrite(GND, LOW); 
#endif
#ifdef VCC
  pinMode(VCC, OUTPUT);
  digitalWrite(VCC, HIGH);
#endif
  pinMode(SLAVE, OUTPUT);
  pinMode(RelayPin, OUTPUT);
  digitalWrite(SLAVE, HIGH);

  Serial.begin(9600);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setDataMode(SPI_MODE0);

}


void loop() {

  //Thermocouple

  unsigned int thermocouple; // 14-Bit Thermocouple Temperature Data + 2-Bit
  unsigned int internal; // 12-Bit Internal Temperature Data + 4-Bit
  float current_temp;
  delay(1000);
  digitalWrite(SLAVE, LOW);                             //  Enable the chip
  thermocouple = (unsigned int)SPI.transfer(0x00) << 8;   //  Read high byte thermocouple
  thermocouple |= (unsigned int)SPI.transfer(0x00);       //  Read low byte thermocouple 
  internal = (unsigned int)SPI.transfer(0x00) << 8;       //  Read high byte internal
  internal |= (unsigned int)SPI.transfer(0x00);           //  Read low byte internal 
  digitalWrite(SLAVE, HIGH);                            //  Disable the chip


  // Display current temperature

  if((thermocouple & 0x0001) != 0) {
    Serial.print(i);
    Serial.print(" ");
    Serial.print("ERROR: ");
    if ((internal & 0x0004) !=0) {
      Serial.print("Short to Vcc, ");
    }
    if ((internal & 0x0002) !=0) {
      Serial.print("Short to GND, ");
    }
    if ((internal & 0x0001) !=0) {
      Serial.print("Open Circuit, ");
    }    

    i++;
  } 
  else {
    if((thermocouple & 0x8000) == 0){ // above 0 Degrees Celsius 
      current_temp = (thermocouple >> 2) * 0.25;
    } 
    else {                          // below zero
      current_temp = (0x3fff - (thermocouple >> 2) + 1)  * -0.25;
    }
    Serial.print(i);
    Serial.print(" ");
    Serial.print(current_temp);
    Serial.print(", "); 

    i++;
  }


  //Detect Phase

  if(roast_time_elapsed > RoastTime){
    phase = 5;
  } 
  else if ((hydrolysis_time_elapsed > HydrolysisTime && current_temp > RoastTemp) || phase==4){
    phase = 4;
  } 
  else if(hydrolysis_time_elapsed > HydrolysisTime){       
    phase = 3;
  } 
  else if(current_temp > HydrolysisTemp){    
    phase = 2;
  }


  //Control Relay

  if(phase != 5){
    int sv = RoastTemp;
    if(phase == 4){
      roast_time_elapsed++;
    }
    if(phase == 2){
      hydrolysis_time_elapsed++; 
      sv = HydrolysisTemp;
    }
    if(current_temp < sv){
      digitalWrite(RelayPin, HIGH);
    } 
    else {
      digitalWrite(RelayPin, LOW);
    }
  } 
  else {
    digitalWrite(RelayPin, LOW);
  }


  // Display Phase

  Serial.print(phase); 
  Serial.println();

}
