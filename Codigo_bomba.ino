#include <dht.h>
#include <DS3231.h>
dht DHT;
DS3231  rtc(SDA, SCL);
#define DHT11_PIN 10

int sensorDepPin = 7;
int sensorTankPin = 6;
int Led1= 2;
int Led2= 3;
int Led3= 4;
int pitidoPin = 12;
int nivelDep = 0;
int nivelTank = 0;
int motorPin = 8;
int old_time_i;
String old_time_s;

void setup() {
  Serial.begin(9600);
  Serial.println("Inicio de sketch - valores del depósto");
  pinMode(sensorDepPin, INPUT);
  pinMode(sensorTankPin, INPUT);
  pinMode(pitidoPin, OUTPUT);
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  pinMode(Led3, OUTPUT);
  pinMode(motorPin, OUTPUT);
  rtc.begin();
  // Inicializacuón de fecha (Se tendría que inicializar automáticamente)
  rtc.setDOW(FRIDAY);      
  rtc.setTime(13, 45, 0);
//  old_time= rtc.getTimeStr();     
  rtc.setDate(30, 5, 2019); 
}

void loop() {
  nivelDep=digitalRead(sensorDepPin);
  nivelTank=digitalRead(sensorTankPin);

  
  Serial.print("El valor depósito = "); //VALOR MONITOR SERIE
  Serial.println(nivelDep); //VALOR MONITOR SERIE
  Serial.print("El valor tanque = "); //VALOR MONITOR SERIE
  Serial.println(nivelTank); //VALOR MONITOR SERIE

  for (int i = 2; i <= 4; i++) { // Cron del barrido de los LED
    digitalWrite(i, HIGH);
    delay(50);
    digitalWrite(i, LOW);
    delay(50);
  }    

    if(nivelTank == 0){
        if(nivelDep==0){
     tone(pitidoPin,370,500);
     digitalWrite(motorPin, HIGH);
        }else{
        digitalWrite(motorPin, LOW);
      }
      }else{
        digitalWrite(motorPin, HIGH); 
        if(nivelDep==0){
        tone(pitidoPin,370,500);
        }
      }
    

   //La recolección de los datos se realiza cada minuto
   if(rtc.getTimeStr(2)>old_time_i){ //getTimeStr(2) Quiero comprobar el minuto
      old_time_i=rtc.getTimeStr(2); //Puede que de error si si ejecuta en el segundo 59 del minuto por lo que habría que ver como arreglarolo
      int chk = DHT.read11(DHT11_PIN);

      Serial.print(rtc.getDOWStr());// Enviar dia de la semana
      Serial.print(" "); 
      Serial.print(rtc.getDateStr());// Enciar fecha 
      Serial.print(" -- ");
      Serial.println(rtc.getTimeStr()); // Enviar hora
  
      Serial.print("  Temperature = "); 
      Serial.print(DHT.temperature); // Enviar Temperatura
      Serial.println(" ºC");
      Serial.print("  Humidity    = ");
      Serial.print(DHT.humidity); // Enviar Humedad
      Serial.println(" %");
   
   } 
   delay(500);
   // si se quiere que los sensores de profundidad se calculen cada menos tiempo habría que bajar el delay
   // Bajar el delay puede dar lugar a error ya que el sistema se vería más afectado por las oscilaciones
}
