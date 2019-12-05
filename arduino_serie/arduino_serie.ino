
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
 
//PINES SALIDA 
#define xPin A14
#define yPin A15
#define buttonPin 52

#define BUTTON     01 //0 1
#define UP         02 //1 2
#define DOWN       03 //2 4
#define LEFT       04 //3 8
#define RIGHT      05 //4 16

int xPosition = 0;
int yPosition = 0;
int buttonState = 0;
int var = 0;
int last_var =0;
int marcador = 0;
 String mensaje;
String lastmensaje;
void setup() {
  // inicializar las comunicaciones en serie a 9600 bps:
  Serial.begin(115200); 
  lcd.begin(16, 2);
  
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  //activar resistencia pull-up en el pin pulsador 
  pinMode(buttonPin, INPUT_PULLUP); 

  lcd.setCursor(15,1);
  lcd.print(marcador);

}

void loop() {
  xPosition = analogRead(xPin);
  yPosition = analogRead(yPin);
  buttonState = digitalRead(buttonPin);

  
  //RIGHT
  if(xPosition>600){
    var=RIGHT;
    }
   //LEFT
   if(xPosition<250){
    var=LEFT;   }
  //UP
   if(yPosition>750){
    var=UP;
    }

  //DOWN
  if(yPosition<250){
    var=DOWN;
    }

  //BUTTON
  if(buttonState==0){ 
    var=BUTTON;
    }
    
  
  if(var!=last_var && var!=0){
  Serial.println(var);
  delay(300);
  var=0;
  }
  if(Serial.available()>0){
   
    lcd.clear();
    mensaje= Serial.readString();

    if (mensaje=="x"){
       marcador++;
    }else{
      lastmensaje=mensaje;
    }
       lcd.setCursor(0,0);
       lcd.print(lastmensaje);
       lcd.setCursor(15,1);
       lcd.print(marcador);
     
    delay(300);
  }
  
}
