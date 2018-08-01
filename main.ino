//codice da caricare sulla scheda di controllo Arduino
//variabili sensore ultrasuoni
int triggerPort = 5;
int echoPort = 4;
long duration=0;

//Motore sx
int in1 = 9;
int in2 = 8;
int enA = 11;
//Motore dx
int in3 = 13;
int in4 = 12;
int enB = 10;

//variabili dei motori
enum Motore { DX, SX };
int command = 0;
int pre_command = 0;
//velocita' iniziale
int speed = 150;
//comandi direzionali
#define STOP 0
#define AVANTI 1
#define DESTRA 2
#define SINISTRA 3
#define INDIETRO 4

//inizializzazione dei componenti
void setup() {
  //inizializzazione seriale
  Serial.begin( 9600 );
  //inizializzazione sensore ultrasuoni
  pinMode( triggerPort, OUTPUT );
  pinMode( echoPort, INPUT );
  //inizializzazione motore sx
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enA, OUTPUT);
  //inizializzazione motore dx
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);
  //impostazione velocita' iniziale
  analogWrite(enA, speed);
  analogWrite(enB, speed); 
}

//esecuzione dei metodi
void loop() {
  //esecuzione delle misurazioni
  ultrasuoni();

}

//########### implementazione dei metodi ##########

//metodo che viene eseguito quando si verifica un interrupt
void serialEvent() {
  //si controlla la presenza di caratteri sulla porta seriale
  while (Serial.available() > 0) {
    //nota: disabilitare il fine linea dal monitor seriale
    //lettura dei caratteri e conversione in valore intero
    command = Serial.parseInt();
    //codifica del comando letto per effettuare l'azione 
    //corrispondente sui motori
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
        setMotorCfg(DX, LOW, HIGH);//rotazione antioraria
        break;

      case SINISTRA: //3
        setMotorCfg(SX, LOW, HIGH);//rotazione antioraria
        setMotorCfg(DX, HIGH, LOW);
        break;

      case INDIETRO: //4
        setMotorCfg(SX, LOW, HIGH);
        setMotorCfg(DX, LOW, HIGH);
        break;

       default:
         if(command >4 && command <256){
          //se command non e' un numero compreso tra
          //0 e 4 allora non e' un comand direzionale
          //ma la velocita' di rotazione dei motori
          //pre_command reimposta i motori con il comando
          //direzionale precedente (funzione di aggiornamento)
            setSpeed(command, pre_command);
          }
          
    }//switch
    if(command>=0 && command <=4)
    //command e' un comando direzionale
       pre_command = command;
       //la variabile pre_command contiene 
       //il comando appena codificato
  }//while
}//serialEvent()

//metodo che effettua una serie di misurazioni ripetute, 
//tramite il sensore ultrasuoni, per determinare la
//distanza tra il robot ed un eventuale ostacolo
void ultrasuoni() {
   //porta bassa l'uscita del trigger
  digitalWrite( triggerPort, LOW );
  //invia un impulso di 10microsec su trigger
  digitalWrite( triggerPort, HIGH );
  delayMicroseconds( 10 );
  digitalWrite( triggerPort, LOW );
  //misura della distanza
  duration = pulseIn( echoPort, HIGH );
  //verifica se il segnale e' fuori portata
  if( duration < 38000 ){ 
    //si stampa su seriale la misura della distanza
    ///non ancora convertita in metri) se corretta
    Serial.println( duration );
  }
  //tempo di attesa necessario a garantire il
  //corretto funzionamento del sensore
  delay(60);
}//ultrasuoni()

//metodo che imposta il verso di rotazione dei motori dx e sx
void setMotorCfg(Motore m, int value1, int value2) {
  //utilizzo dell'enumerazione che si riferisce al motore
  //per applicare il comando direzionale, agendo sui
  //rispettivi pin di interesse
  if (m == SX) {
    digitalWrite(in1, value1);
    digitalWrite(in2, value2);
  }
  else {
    digitalWrite(in3, value1);
    digitalWrite(in4, value2);
  }
}//setMotorCfg(Motore,int,int)

//metodo che imposta la velocita' di rotazione dei motori
void setSpeed(int speed, int pre_command){
  //imposta la velocita' sul motore SX
  analogWrite(enA, speed);
  //imposta la velocita' sul motore DX
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
}//setSpeed(int,int)
