// Medidor de Temperatura/Humidade/Ponto de Orvalho
/*******************************************************
Este código usa o sensor DH11 e LCD Keypad Shield DF Robot Screen para
criar uma estao de medicao de temperatura, humidade e ponto de
orvalho. 

DHT11 Shield Pins no Arduino Uno:

Vcc - +5v
Data - A1 (pode definir outro pino modificando o "#define DHTPIN A5")
GND - GND

Licença:
Livre para uso mas mantenham o nome!

Versão 1.0

Anderson Cursino 03/02/2017
********************************************************/

#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN A1     
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int t=0;
int h=0;
int Timer= 0;
int Animacao = 0;
int x;
int poor=0;

/* Configuracoes do Keypad: 
 
#define btnDIR    0
#define btnCIM    1
#define btnBAI   2
#define btnESQ   3
#define btnSELECT 4
#define btnNENHUM   5
int read_buttons() {
int adc_key_in = analogRead(0);
 if (adc_key_in > 1000) return btnNENHUM;
 if (adc_key_in < 50)   return btnDIR;  
 if (adc_key_in < 195)  return btnCIM; 
 if (adc_key_in < 380)  return btnBAI; 
 if (adc_key_in < 555)  return btnESQ; 
 if (adc_key_in < 790)  return btnSELECT;   
}
*/

void setup () {
 lcd.begin(16,2);
 delay(250);
 lcd.setCursor(0,0);
 lcd.setCursor(2,0);
 lcd.write("Temperatura"); 
 lcd.setCursor(3,2);
 lcd.write("by Dersao"); 
 lcd.blink(); 
 delay(1000);
 lcd.noBlink(); 
 for (int i = 0; i < 16; i++) {
    delay(100);
    lcd.scrollDisplayLeft();
 }
 delay(900);
 lcd.clear();
 Serial.begin(9600);
 dht.begin();
}

static void ReadDHT11()
{
  h = dht.readHumidity();
  t = dht.readTemperature();
}

void loop () {
  Timer  = Timer+1;
  if(Timer==1)
  {
    ReadDHT11();
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(t);
    lcd.print("\337C");
    lcd.setCursor(8,0);
    lcd.print("H:");
    lcd.print(h);
    lcd.print("%");
    lcd.setCursor(0,1);   
    lcd.write("Condicao: ");
    //lcd.print(pontoOrvalho(t, h),0); 
    poor=pontoOrvalho(t, h);
    if (poor > 24)
  {
    lcd.print("Ruim  ");
  }
  else if (poor < 10 )
  {
    lcd.print("Seco  ");
  }
  else
  {
    lcd.print("Normal");
}
    lcd.setCursor(13,0);
  if(Animacao==0) {
    lcd.print("  -");
    }
    if(Animacao==1)
    {
      lcd.print("  *");
    }
    Animacao = Animacao + 1;
    if(Animacao==2)
    {
      Animacao=0;
    }
  }
  if(Timer==1100) {
    Timer=0;
  }
}

double pontoOrvalho(double celsius, double humidade)
{
        double a = 17.271;
        double b = 237.7;
        double temp = (a * celsius) / (b + celsius) + log(humidade/100);
        double Td = (b * temp) / (a - temp);
        return Td;
}

