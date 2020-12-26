#include <16f877.h>              // denetleyicinin baslik dosyasi tanimlama
#fuses XT, NOWDT, NOPROTECT, NOBROwNOUT,NOLVP,NOPUT,NOWRT,NODEBUG,NOCPD       // Denetleyici konfig�rasyon ayarlari

#device ADC=10
#include <math.h>             // logaritmik islemler icin matematik kutuphanesi tanitilmasi
#fuses HS
#use delay(clock=4000000)     // osilator frekansi 4Mhz ayarlanmasi

#use fast_io(a)               // A ve B portu icin port yonlendirme komutlari
#use fast_io(b)

#define use_portb_lcd TRUE       // Pportuna lcd tanimlandi
#include <lcd.c>                 // lcd dosyas� tanitildi

   int i = 0, r1=250;
   unsigned long int vo_okunan = 0, io_okunan = 0 ;     // analog deger okumak icin degisken tnaimlama
   float vo = 0, io = 0;
   
  
#int_AD
void ADC_Kesmesi(){              // ADC kesmesi
}


void main()
{
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_CCP1(CCP_OFF);
   setup_CCP2(CCP_OFF);


   set_tris_a(0b00000011);       // a portu 0. pini giris olarak tanimlama
   set_tris_b(0b00000000);       // b portu pinleri cikis olarak tanimlama
   output_b(0x00);               // b portu pinleri sifir yapma
   setup_adc(adc_clock_div_32);        // ADC clock frekansi fosc/32
   setup_adc_ports(ALL_ANALOG);        // Tum ADC girisleri analog  

   enable_interrupts(INT_AD);
   enable_interrupts(GLOBAL);          // Tum kesmelere izin verme
   
   lcd_init();                   // lcd hazir
   
   while(TRUE)
   {
   
      set_adc_channel(0);         // Cevrim icin AN0 secildi
      delay_us(20);              // bekleme
     
         for(i=0;i<20;i++){               // filtrelemek i�in 20 ad�ml�k d�ng�
      
            vo_okunan = read_adc();         // okunan analog degeri degiskene atama
            vo += vo_okunan;              // tum okunan degerlerin bir degiskende toplanmasi
         }
      
      vo = vo / 20;                       // okunan degerlerin ortalamasinin alinmasi
      
      vo = vo * 0.00976562;        // 10 bit 1024 10V'a b�l�nerek okunan deger ile �arp�l�r ADC donusumu 
           
      printf(lcd_putc, "\f%f V",vo);         // okunan voltaj bilgisi lcd'ye yazdirilir
      delay_us(250); 
                     
                     
      set_adc_channel(1);         // Cevrim icin AN1 secildi
      delay_us(20);              // bekleme
      
         for(i=0;i<20;i++){               // filtrelemek i�in 20 ad�ml�k d�ng�
      
            io_okunan = read_adc();         // okunan analog degeri degiskene atama
            io += io_okunan;              // tum okunan degerlerin bir degiskende toplanmasi
         }
         
      io = io / 20;                 // okunan degerlerin ortalamasinin alinmasi
      
      io = io * 0.00488759;         // besleme 5V oldugu icin okunan deger (5/1024) sayisina bolundu
      io = io / r1;                 // okunan voltaj degeri r1= 250 direncine bolunerek akim degeri bulunuyor
      io = io * 1000;               //akim mA cinsine cevirmek icin 1000 ile carpilir
      printf(lcd_putc, "\n%f mA" io);         // hesaplanan amper bilgisi lcd'ye yazdirilir
      
      delay_ms(250);             //bekleme
   }
}
