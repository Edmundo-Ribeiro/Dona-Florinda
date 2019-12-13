#ifndef gascarbonico_h
#define gascarbonico_h

#include "teclado.h"

NexPage CO2_PAG = NexPage(PAGINA_CO2,0,"CO2");

NexButton valor_CO2 = NexButton(PAGINA_CO2, 5, "co2");
NexButton intervalo_CO2 = NexButton(PAGINA_CO2, 6, "intervalo_co2");

#define INTERVALO_RAPIDO 200
#define INTERVALO_LENTO 60000
#define BTNCO2 1
#define BTNINTCO2 2

class gascarbonico{

private:
  
  int Pino_sensor;
  int Valor;
  int valor_setup = 0;
  int valor_intervalo = 0;
  int end_valor_setup = 0;
  int end_valor_intervalo = 0;
  int tempo_intervalo_sensor = INTERVALO_LENTO;
  unsigned long tempo_anterior = 0;
  //variáveis da função de de trocar a leitura do sensor para ppm -> https://github.com/ihormelnyk/mh-z19_co2_meter/blob/master/mh-z19_co2_meter.ino
  int prevVal = LOW;
  long th, tl, h, l, ppm = 0;

public:
  int Pino_rele;
  int co2_ligado = 0; // 1 quando ligado e 0 quando desligado
  
  gascarbonico(){
    this->valor_intervalo = EEPROM.read(end_valor_intervalo);
    this->valor_setup = EEPROM.read(end_valor_setup);
  }

  long PWM_ISR() {
    long tt = millis();
    int val = digitalRead(Pino_sensor);
    
    if (val == HIGH) {    
      if (val != prevVal) {
        h = tt;
        tl = h - l;
        prevVal = val;
      }
    }  else {    
      if (val != prevVal) {
        l = tt;
        th = l - h;
        prevVal = val;
        ppm = 2000 * (th - 2) / (th + tl - 4);      
      }
    }
    return ppm;
  }
  
  void setCO2(uint16_t valor){
    this->valor_setup = valor;
    EEPROM.update(end_valor_setup,valor);
  }

  void setIntCO2(uint16_t valor){
    this->valor_intervalo = valor;
    EEPROM.update(end_valor_intervalo,valor);
  }

  void SetupSR_CO2(int valor1, int valor2){

    this->Pino_rele = 18;
    this->Pino_sensor = 19;

    Serial.begin(9600);

    pinMode(Pino_rele, OUTPUT);
    pinMode(Pino_sensor, INPUT);
  } 

  void run(){
    
    unsigned long tempo_atual = millis();

    if(I.estado_atual == I.LIGADO){
    
      if((tempo_atual - this->tempo_anterior) >= this->tempo_intervalo_sensor){
        
        this->tempo_anterior = tempo_atual;

        Valor = PWM_ISR();
        //Valor = analogRead(this->Pino_sensor); 
       
        if(Valor <= (valor_setup - valor_intervalo)){
          digitalWrite(Pino_rele, HIGH);  //Liga rele
          this->co2_ligado = 1;    
          this->tempo_intervalo_sensor = INTERVALO_RAPIDO;
        }

        if(Valor >= (valor_setup + valor_intervalo)){

          digitalWrite(Pino_rele, LOW); //Desliga rele
          this->co2_ligado = 0;
          this->tempo_intervalo_sensor = INTERVALO_LENTO;
        }
        
      }
    }
  } 
};
  gascarbonico D_CO2; //Desclaração do objeto

 void CO2ValorPopCallback(void *ptr){
    PAGINA = PAGINA_CO2;//fala daonde voce veio
    botaoAbertado = BTNCO2; //fala qual eh botao
    teclado.show();
  }

  void CO2IntervaloPopCallback(void *ptr){
    PAGINA = PAGINA_CO2;
    botaoAbertado = BTNINTCO2;
    teclado.show();
  
  }
  
#endif 
