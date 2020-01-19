#ifndef gascarbonico_h
#define gascarbonico_h

#include "teclado.h"
#include "filtro.h"

void mostraDadosCO2();
void debugEstadoVariaveisCO2();

// comentario teste 
NexPage CO2_PAG = NexPage(PAGINA_CO2,0,"CO2");

NexButton valor_CO2 = NexButton(PAGINA_CO2, 1, "concentracao");
NexButton intervalo_CO2 = NexButton(PAGINA_CO2, 4, "intervalo");
NexButton voltar_CO2 = NexButton(PAGINA_CO2, 3, "voltar");
NexCrop cilindro = NexCrop(PAGINA_CO2, 2, "cilindro");
NexText valor_lido = NexText(PAGINA_CO2, 5, "valorLido");

NexButton icone_CO2 = NexButton(PAGINA_MENU,6,"CO2");

#define INTERVALO_RAPIDO 1000
#define INTERVALO_LENTO 30000
#define IMG_CILINDRO_LIGADO 19
#define IMG_CILINDRO_DESLIGADO 18

//Sistema de Injeção de Gas
class SIG{

public:

  //ED: ver se não da pra trocar esses inteiros por tipos que ocupam menos espaço (uint8 ...) ou definições
  int Valor;
  uint16_t referencia; //valor da concentração de CO2 que vai ser definida pelo usuario
  uint16_t intervalo;// intervalo que da a margem de operação para ativação e desativação da injeção de co2

  unsigned long tempo_intervalo_sensor = INTERVALO_LENTO;
  unsigned long tempo_anterior = 0;
  //variáveis da função de de trocar a leitura do sensor para ppm -> https://github.com/ihormelnyk/mh-z19_co2_meter/blob/master/mh-z19_co2_meter.ino
  int prevVal = LOW;
  long th, tl, h, l, ppm = 0;

  bool estado_atual = DESLIGADO; // 1 quando ligado e 0 quando desligado
  
  SIG(){
    this->referencia =  EEPROM.get(end_co2_referencia, this->referencia);
    this->intervalo = EEPROM.get(end_co2_intervalo, this->intervalo);
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
    this->referencia = valor;
    EEPROM.put(end_co2_referencia, valor);
  }

  void setIntCO2(uint16_t valor){
    this->intervalo = valor;
    EEPROM.put(end_co2_intervalo, valor);
  }

  void run(bool estado_iluminacao, bool estado_exaustor){
    filtro co2;
    unsigned long tempo_atual = millis();
    co2.tamanho(10);

    this->tempo_anterior = tempo_atual;

    Valor = PWM_ISR();

    co2.adiciona(Valor);

    if(estado_iluminacao == LIGADO && estado_exaustor == DESLIGADO){
    
      if((tempo_atual - this->tempo_anterior) >= this->tempo_intervalo_sensor){

        if(Valor <= (referencia - intervalo)){
          digitalWrite(PINO_RELE_CO2, HIGH);  //Liga rele
          this->estado_atual = LIGADO;    
          this->tempo_intervalo_sensor = INTERVALO_RAPIDO;
        }

        if(Valor >= (referencia + intervalo)){

          digitalWrite(PINO_RELE_CO2, LOW); //Desliga rele
          this->estado_atual = DESLIGADO;
          this->tempo_intervalo_sensor = INTERVALO_LENTO;
        }
        
        if(PAGINA == PAGINA_CO2){
          mostraDadosCO2();
          debugEstadoVariaveisCO2();
        }

      }
    }
    else{
      //tem que desligar as paradas para garantir
    }
  } 
};

  SIG CO2; //Desclaração do objeto

 void CO2ValorPopCallback(void *ptr){
    PAGINA = PAGINA_CO2;//fala daonde voce veio
    botaoApertado = BTNCO2; //fala qual eh botao
    teclado.show();
    PassaBotaoParaTela(CO2.referencia);
  }

  void CO2IntervaloPopCallback(void *ptr){
    PAGINA = PAGINA_CO2;
    botaoApertado = BTNINTCO2;
    teclado.show();
    PassaBotaoParaTela(CO2.intervalo);
  }

void IconeCO2Callback(void *ptr){
    PAGINA = PAGINA_CO2;
    CO2_PAG.show();
    mostraDadosCO2();
  }

  void VoltarCO2CallBack(void *ptr){
    PAGINA = PAGINA_MENU;
    menu.show();
  }

  void mostraDadosCO2(){
    char buffer[10];

    sprintf( buffer,"%d ppm", EEPROM.get(end_co2_referencia, CO2.referencia) );
    valor_CO2.setText(buffer);

    sprintf( buffer,"%d ppm", EEPROM.get(end_co2_intervalo, CO2.intervalo) );
    intervalo_CO2.setText(buffer);

    //onde está armazendando o valor lido?
    //sprintf( buffer,"%d ppm", <colocar aqui> );
    //valor_lido.setText(buffer);

    if(CO2.estado_atual == LIGADO){
      cilindro.setPic(IMG_CILINDRO_LIGADO);
    }
    else{
      cilindro.setPic(IMG_CILINDRO_DESLIGADO);
    }


  }

  void debugEstadoVariaveisCO2(){
    dbSerialPrint("############# Variaveis CO2 #####################\n\n");

    dbSerialPrint("Estado Atual: ");
    dbSerialPrintln(CO2.estado_atual);
        
    dbSerialPrint("Valor de referencia: ");
    dbSerialPrintln(CO2.referencia);
    dbSerialPrint("Valor do intervalo: ");
    dbSerialPrintln(CO2.intervalo);


    dbSerialPrint("#################################################\n\n");

    // Serial.print("Valor lido: ");
    // Serial.println(CO2.);
  }



  
#endif 
