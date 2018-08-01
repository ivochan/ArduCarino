//variabili ultrasuoni
int triggerPort = 5;
int echoPort = 4;
long duration=0;
//Motore sx nero
int in1 = 9;
int in2 = 8;
int enA = 11;
//Motore dx grigio
int in3 = 13;
int in4 = 12;
int enB = 10;
//Motore Generale
int command = 0;
int pre_command = 0;
int speed = 150;//velocita' iniziale
#define STOP 0
#define AVANTI 1
#define DESTRA 2
#define SINISTRA 3
#define INDIETRO 4
enum Motore { DX, SX };

void setup() {
  //inizializzo seriale
  Serial.begin( 9600 );
  //inizializzo ultrasuoni
  pinMode( triggerPort, OUTPUT );
  pinMode( echoPort, INPUT );
  //inizializzo motore sx
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enA, OUTPUT);
  //inizializzo motore dx
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);
  //velocita' iniziale
  analogWrite(enA, speed);
  analogWrite(enB, speed);
  
}

void loop() {
  ultrasuoni();

}

//########### implementazione dei metodi ##########

void serialEvent() {
  while (Serial.available() > 0) {
    //disabilitare il fine linea dal monitor seriale
    command = Serial.parseInt();
    switch (command) {

      case STOP: //0
        setMotorCfg(SX, LOW, LOW);
        setMotorCfg(DX, LOW, LOW);
        break;

      case AVANTI: //1
        setMotorCfg(SX, HIGH, LOW);
        setMotorCfg(DX, HIGH, LOW);
        break;

      case DESTRA: //2
        setMotorCfg(SX, HIGH, LOW);
        setMotorCfg(DX, LOW, HIGH); //rotazione antioraria
        break;

      case SINISTRA: //3
        setMotorCfg(SX, LOW, HIGH); //rotazione antioraria
        setMotorCfg(DX, HIGH, LOW);
        break;

      case INDIETRO: //4
        setMotorCfg(SX, LOW, HIGH);
        setMotorCfg(DX, LOW, HIGH);
        break;

       default:
         if(command >4 && command <256){
            setSpeed(command, pre_command);
          }
          
    }//switch
    if(command>=0 && command <=4)
       pre_command = command;
   
  }
}

void ultrasuoni() {
  //porta bassa l'uscita del trigger
  digitalWrite( triggerPort, LOW );
  //invia un impulso di 10microsec su trigger
  digitalWrite( triggerPort, HIGH );
  delayMicroseconds( 10 );
  digitalWrite( triggerPort, LOW );

  duration = pulseIn( echoPort, HIGH );

  if( duration < 38000 ){ 
    Serial.println( duration );
  }
  delay(60);
}

void setMotorCfg(Motore m, int value1, int value2) {
  if (m == SX) {
    digitalWrite(in1, value1);
    digitalWrite(in2, value2);
  }
  else {
    digitalWrite(in3, value1);
    digitalWrite(in4, value2);
  }
}

void setSpeed(int speed, int pre_command){
  //Imposta la velocita' sul motore SX
  analogWrite(enA, speed);
  //Imposta la velocita' sul motore DX
  analogWrite(enB, speed);
  //esecuzione del comando
  switch (pre_command) {
    
     case STOP:
        setMotorCfg(SX,LOW,LOW);
        setMotorCfg(DX,LOW,LOW);
        break;
        
      case AVANTI:
       setMotorCfg(SX,HIGH,LOW);
       setMotorCfg(DX,HIGH,LOW);
       break;
        
      case DESTRA:
        setMotorCfg(SX,HIGH,LOW);
        setMotorCfg(DX,LOW,HIGH);
        break;

      case SINISTRA:
        setMotorCfg(SX,LOW,HIGH);
        setMotorCfg(DX,HIGH,LOW);
        break;

      case INDIETRO:
        setMotorCfg(SX,LOW,HIGH);
        setMotorCfg(DX,LOW,HIGH);
        break;
     
      default:
       ;
  }//switch
}//setSpeed
