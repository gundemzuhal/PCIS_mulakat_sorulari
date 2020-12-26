#include <16f877.h>              // denetleyicinin baslik dosyasi tanimlama
#fuses XT, NOWDT, NOPROTECT, NOBROwNOUT,NOLVP,NOPUT,NOWRT,NODEBUG,NOCPD       // Denetleyici konfig�rasyon ayarlari

#device ADC=10
#include <math.h>             // logaritmik islemler icin matematik kutuphanesi tanitilmasi
#fuses HS
#use delay(clock=4000000)     // osilator frekansi 4Mhz ayarlanmasi

#use fast_io(a)               // A ve B portu icin port yonlendirme komutlari
#use fast_io(b)

   
   unsigned long int okunan;     // analog deger okumak icin degisken tnaimlama
   float sicaklik;               // asicaklik degeri okumak icin degisken tnaimlama
   float R1=10000;               // sabit direnc degeri tanımlama
   float A=31.03434454e-03, B= -52.97989246e-04, C=270.2612064e-07;        // Stein hart denklemindeki A-B-C degerleri
   
void main()
{
   set_tris_a(0b00000001);       // a portu 0. pini giris olarak tanimlama
   set_tris_b(0b00000000);       // b portu pinleri cikis olarak tanimlama
   output_b(0x00);               // b portu pinleri sifir yapma
   setup_adc(adc_clock_div_32);        // ADC clock frekansi fosc/32
   setup_adc_ports(ALL_ANALOG);        // Tum ADC girisleri analog  

   while(TRUE)
   {
      set_adc_channel(0);         // CEvrim icin AN0 secildi
      delay_us(20);              // bekleme
      okunan=read_adc();         // okunan analog degeri degiskene atama
     
      sicaklik = log((10240000 / okunan) - R1);             // (8 bit (1024) / okunan deger) - direnc logaritma alinmasi 
      sicaklik = 1 / (A + (B + (C * sicaklik * sicaklik)) * sicaklik);           // formulunden sicaklik degeri bulunmasi
      sicaklik = sicaklik - 273.15;                   // sicakligi santigrat dereceye cevirme
    
      if(sicaklik > 30){               // sicaklik 30 dereceden buyukse led yansin
         output_high(pin_b0);
      }
      else{                            // degilse led sonsun
         output_low(pin_b0);
      }
   
      delay_ms(100);                   // 10 ms bekleme
   }
}
