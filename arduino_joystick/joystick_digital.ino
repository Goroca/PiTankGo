
/*
 Código de ejemplo Arduino para el módulo Joystick
 Prototipo del taller www.web-robotica.com

 
 Este sketch es de dominio público.
 */
 
#define xPin A14
#define yPin A15
#define buttonPin 52

#define UP 2
#define DOWN 3
#define LEFT 4
#define RIGHT 5
#define PULSADOR 6

#define FLAG_PULSAR     0x01 //0 1
#define FLAG_UP         0x02 //1 2
#define FLAG_DOWN       0x04 //2 4
#define FLAG_LEFT       0x08 //3 8
#define FLAG_RIGHT      0x10 //4 16

int xPosition = 0;
int yPosition = 0;
int buttonState = 0;
int flag = 0;

void setup() {
  // inicializar las comunicaciones en serie a 9600 bps:
  Serial.begin(9600); 
  
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);

  //activar resistencia pull-up en el pin pulsador 
  pinMode(buttonPin, INPUT_PULLUP); 

  //salidas
  pinMode(UP, OUTPUT);
  pinMode(DOWN, OUTPUT);  
  pinMode(LEFT, OUTPUT);
  pinMode(RIGHT, OUTPUT);  
  pinMode(PULSADOR, OUTPUT);
  pinMode(yPin, OUTPUT);
}

void loop() {
  xPosition = analogRead(xPin);
  yPosition = analogRead(yPin);
  buttonState = digitalRead(buttonPin);

  
  if(xPosition>600){
    digitalWrite(RIGHT, HIGH);
    Serial.print("derecha");
    }else{
    digitalWrite(RIGHT, LOW);
    }
   if(xPosition<250){
      digitalWrite(LEFT, HIGH);
      Serial.print("izquierda");
    }else{
    digitalWrite(LEFT, LOW);
    }

  if(yPosition>100){
      digitalWrite(UP, HIGH);
      Serial.print("arriba");

  }else{
    digitalWrite(UP, LOW);
    }
  if(yPosition<2){
      digitalWrite(DOWN, HIGH);
      Serial.print("abajo");
  }else{digitalWrite(RIGHT, LOW);
    }

  if(buttonState==0){
     digitalWrite(PULSADOR, HIGH);
     Serial.print("Pulsado");
  }else{digitalWrite(PULSADOR, LOW);
  }

  
Serial.print(yPosition);
Serial.print("\n");
  delay(100); // añadir un poco de retraso entre lecturas
}
