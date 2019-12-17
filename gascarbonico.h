#ifndef gascarbonico_h
#define gascarbonico_h

#include "teclado.h"

NexPage CO2_PAG = NexPage(PAGINA_CO2,0,"CO2");

NexButton valor_CO2 = NexButton(PAGINA_CO2, 5, "co2");
NexButton intervalo_CO2 = NexButton(PAGINA_CO2, 6, "intervalo_co2");

#define INTERVALO_RAPIDO 500
#define INTERVALO_LENTO 60000
#define BTNCO2 1
#define BTNINTCO2 2

class gascarbonico{

private:
  //ED: ver se não da pra trocar esses inteiros por tipos que ocupam menos espaço (uint8 ...) ou definições
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
  int co2_ligado = DESLIGADO; // 1 quando ligado e 0 quando desligado
  
  gascarbonico(){
    this->valor_intervalo = EEPROM.read(end_valor_intervalo);
    this->valor_setup = EEPROM.read(end_valor_setup);
    pinMode(PINO_RELE_CO2, OUTPUT);
    pinMode(PINO_SENSOR_CO2, INPUT);
  }

  long PWM_ISR() {
    long tt = millis();
    int val = digitalRead(PINO_SENSOR_CO2);
    
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

  void run(bool estado_atual){
    
    unsigned long tempo_atual = millis();

    if(estado_atual == LIGADO){
    
      if((tempo_atual - this->tempo_anterior) >= this->tempo_intervalo_sensor){
        
        this->tempo_anterior = tempo_atual;

        Valor = PWM_ISR();
        //Valor = analogRead(this->Pino_sensor); 
       
        if(Valor <= (valor_setup - valor_intervalo)){
          digitalWrite(PINO_RELE_CO2, HIGH);  //Liga rele
          this->co2_ligado = 1;    
          this->tempo_intervalo_sensor = INTERVALO_RAPIDO;
        }

        if(Valor >= (valor_setup + valor_intervalo)){

          digitalWrite(PINO_RELE_CO2, LOW); //Desliga rele
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
