#include<math.h>    // logaritmik islemler icin matematik kutuphanesi tanitilmasi
#define led 2       // 2.pinin led olarak tanimlanmasi

float R1=10000;     // sabit direnc degeri tanımlama
float A=31.03434454e-03, B= -52.97989246e-04, C=270.2612064e-07;    // Stein hart denklemindeki A-B-C degerleri


void setup() {
  Serial.begin(9600);       //haberlesme hizi
  pinMode(led,OUTPUT);      // ledin cikis olarak tanimlanmasi
}


double PTC(int okunan){     // matematiksel islemler icin fonk yazma

 double sicaklik;
 sicaklik = log(((10240000 / okunan) - R1));   // (8 bit (1024) / okunan deger) - direnc logaritma alinmasi 
 sicaklik = 1 / (A + (B + (C * sicaklik * sicaklik)) * sicaklik);   // formulunden sicaklik degeri bulunmasi
 sicaklik = sicaklik - 273.15;          // sicakligi santigrat dereceye cevirme
 return sicaklik;
}


void loop() {

  
  int deger = analogRead(A0);       // A0 pinindeki analog degeri okuma
  double sicaklik = PTC(deger);     // PTC fonksiyonuna gitme      


  if(sicaklik > 30){        // sicaklik 30 dereceden buyukse led yansin
    digitalWrite(led,HIGH);
  }
  else{                     // degilse led sonsun
    digitalWrite(led,LOW);
  }


  delay(250);               //0.25 sn bekleme
  

}
