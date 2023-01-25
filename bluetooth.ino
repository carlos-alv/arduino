#include <Servo.h>
#include <SoftwareSerial.h>
#include <DHT.h>

#define LED 13
#define SERVO 9
#define DHTTYPE DHT11
#define DHTPin 2
#define ROJO 4
#define VERDE 5
#define LED_R 6
#define LED_G 7
#define LED_B 8
#define LDR A0


const long A = 1000;     //Resistencia en oscuridad en KΩ
const int B = 15;        //Resistencia a la luz (10 Lux) en KΩ
const int Rc = 10;       //Resistencia calibracion en KΩ

//----CONEXIONADO----
// Pin 11 a RXD
// Pin 10 a TXD
// Pin Vcc a 5v
// Pin Gnd a tierra


Servo servoMotor;
SoftwareSerial miBT(10, 11);
DHT dht(DHTPin, DHTTYPE);
float maxTemp = 25;
float minTemp = 20;
float humLim = 60;
float luzLim = 90;

unsigned long nextTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(ROJO, OUTPUT);
  Serial.println("Listo");
  miBT.begin(38400);
  servoMotor.attach(SERVO);
  dht.begin();

  nextTime = millis();
}

void loop() {
  char dato = 0;
  String dato_completo = "";
  
  
  // COMPROBAR DATOS DE ENTRADA
  if(miBT.available()){
    dato = miBT.read();
    if(dato == 'M'){
      dato_completo = "";
      while(miBT.available()){
        dato = miBT.read();
        dato_completo += dato;
      }
      maxTemp = dato_completo.toFloat();
      Serial.print("Max temp: ");
      Serial.print(dato_completo);
    }

    if(dato == 'm'){
      dato_completo = "";
      while(miBT.available()){
        dato = miBT.read();
        dato_completo += dato;
      }
      minTemp = dato_completo.toFloat();
      Serial.print("Min temp: ");
      Serial.print(dato_completo);
    }

    if(dato == 'h'){
      dato_completo = "";
      while(miBT.available()){
        dato = miBT.read();
        dato_completo += dato;
      }
      humLim = dato_completo.toFloat();
      Serial.print("Hum lim: ");
      Serial.print(dato_completo);
    }

    if(dato == 'l'){
      dato_completo = "";
      while(miBT.available()){
        dato = miBT.read();
        dato_completo += dato;
      }
      luzLim = dato_completo.toFloat();
      Serial.print("Luz lim: ");
      Serial.print(dato_completo);
    }
  }


  // ENVIO DE DATOS

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
     Serial.println("Failed to read from DHT sensor!");
     return;
  }

  int input = analogRead(LDR);
  float ilum = ((long)input*A*10)/((long)B*Rc*(1024-input));
  
  String enviar = "v|";
  enviar = enviar + t;
  enviar = enviar + "|";
  enviar = enviar + h;
  enviar = enviar + "|";
  enviar = enviar + ilum;
  enviar = enviar + "|";
  enviar = enviar + millis();
  
  enviar = enviar + "|";
  enviar = enviar + maxTemp;
  enviar = enviar + "|";
  enviar = enviar + minTemp;
  enviar = enviar + "|";
  enviar = enviar + humLim;
  enviar = enviar + "|";
  enviar = enviar + luzLim;
  enviar = enviar + "\n\n";
  miBT.write(enviar.c_str());

  

  // LÓGICA DEL PROGRAMA

  if (t < minTemp){
    Serial.println("Encendiendo calefaccion");
    digitalWrite(ROJO, HIGH);
  } else if (t > maxTemp){
    digitalWrite(ROJO, LOW);
    servoMotor.write(0);
    Serial.println("Abriendo ventana");
  } else {
    Serial.println("Temperatura normal");
    servoMotor.write(90);
    digitalWrite(ROJO, LOW);
  }


  if (h > humLim){
    Serial.println("Encendiendo deshumidificador");
    digitalWrite(VERDE, HIGH);
  } else {
    Serial.println("Apagando deshumidificador");
    digitalWrite(VERDE, LOW);
  }

  if (ilum < luzLim){
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_B, HIGH);
  } else {
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, LOW);
  }

  delay(1000);
}
