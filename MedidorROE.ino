#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define pinDIRanalog 0
#define pinREFanalog 1

LiquidCrystal_I2C lcd(0x27, 16, 2);

//Calcula la tensión equivalente de entrada en los pines anlógicos
float r1 = 10000; //Divisor de tensión R1
float r2 = 3200;  //Divisor de tensión V2
float vMAX = (5*(r1+r2))/r2; //Vout=Vin(r2/(r1+R2))

// prueba cambio git


//Funciones para mostrar los datos en el display
float CalcVoltios(float rd) {
  return (rd * vMAX) / 1023;
}

float CalcROE(float vd, float vr) {
  if (vr >= vd) {
    return 99.9;
  } else {
    float pf = sqrt(vr / vd);
    return ((1 + pf) / (1 - pf));
  }
}

void MostrarLectura(float miDIR, float miREF) {
  lcd.setCursor(0, 0);
  lcd.print("DR:");
  lcd.print(miDIR);
  
  lcd.setCursor(9, 0);
  lcd.print("RF:");
  lcd.print(miREF);
}

void MostrarROE(float miROE) {
  lcd.setCursor(0, 1);
  lcd.print("ROE=1:");
  lcd.print(miROE); 
}

void MostrarNoLectura() {
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("EA2HW");

  lcd.setCursor(0, 1);
  lcd.print("No hay transmmisión"); //Cualquier texto cuando está a la escucha
}

void setup() {
  Serial.begin(9600);
  delay(1000);

  //Inicia el monitor
  lcd.init();      
  lcd.backlight();       
  lcd.clear();
    
  //Mensaje inicial     
  lcd.setCursor (0, 0);  
  lcd.print(" Medidor de ROE ");           
  lcd.setCursor (0, 1);  
  lcd.print("**** EA2HW ****");  
  
  delay(4000);
  lcd.clear(); 
}

void loop() {
  float voltiosDirectaRaw = 9; // analogRead(pinDIRanalog);
  if (voltiosDirectaRaw == 0)
    return MostrarNoLectura();

  float voltiosReflejadoRaw = 15; // analogRead(pinREFanalog);

  float voltiosDirecta = CalcVoltios(voltiosDirectaRaw);   
  float voltiosReflejado = CalcVoltios(voltiosReflejadoRaw);
  float roe = CalcROE(voltiosDirecta, voltiosReflejado);  
  
  MostrarLectura(voltiosDirecta, voltiosReflejado);
  MostrarROE(roe);  
}
