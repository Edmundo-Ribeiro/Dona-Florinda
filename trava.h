#ifndef trava_h
#define trava_h

#include "datahora.h"

#define INTERVALO 5000

void mostraDadosTrava();

//alterar nome do objeto na pagina
NexButton icone_config = NexButton(PAGINA_MENU, 4, "defDataHora");//mudar esse nome

NexButton btnTravaSuperior = NexButton(PAGINA_TRAVA, 1, "travaSuperior");
NexButton btnTravaInferior = NexButton(PAGINA_TRAVA, 2, "travaInferior");
NexButton relogio = NexButton(PAGINA_TRAVA, 3, "relogio");
NexButton voltar_trava = NexButton(PAGINA_TRAVA, 4, "voltar");

class Trava{
  private:
    unsigned long timerSuperior = millis();
    unsigned long timerInferior = millis();
 
  public:
    bool estado_porta_superior,
         estado_porta_inferior;
    
    Trava(){
      pinMode(PINO_FIM_DE_CURSO_SUPERIOR, INPUT);
      pinMode(PINO_FIM_DE_CURSO_INFERIOR, INPUT);
      pinMode(PINO_TRANSISTOR_INFERIOR, OUTPUT);
      pinMode(PINO_TRANSISTOR_SUPERIOR, OUTPUT);
    }
    
    void Trava::abrirInferior(){
      digitalWrite(PINO_TRANSISTOR_INFERIOR, HIGH); //Liga relé
      timerInferior = millis();//resetar o timer

    }

    void Trava::abrirSuperior(){
      digitalWrite(PINO_TRANSISTOR_SUPERIOR, HIGH); //Liga relé
      timerSuperior = millis();//resetar o timer

    }

    void Trava::run(){
      this->estado_porta_superior = digitalRead(PINO_FIM_DE_CURSO_SUPERIOR);
      this->estado_porta_inferior = digitalRead(PINO_FIM_DE_CURSO_INFERIOR);//colocar pino correto aqui

      if( (millis() - this->timerSuperior) >= INTERVALO ){
        digitalWrite(PINO_TRANSISTOR_SUPERIOR, LOW); // Desliga relé
        this->timerSuperior = millis();
        mostraDadosTrava();
      }


      if( (millis() - this->timerInferior) >= INTERVALO ){
        Serial.println((millis() - this->timerInferior));
        digitalWrite(PINO_TRANSISTOR_INFERIOR, LOW); // Desliga relé
        this->timerInferior = millis();
        mostraDadosTrava();
      }
    }

};




Trava T; //declaração do objeto

void TravaSuperiorPopCallBack(){
  //mudar imagem para imagem de cadeado aberto
  //abrir trava superior
  T.abrirSuperior();
}

void TravaInferiorPopCallBack(){
  //mesma coisa da trava superior
  T.abrirInferior();
}

void RelogioPopCallBack(){
   PAGINA = PAGINA_MENU;
   configData.show();
   mostraHoraData();
}

void voltarTravaCallBack(void *ptr){
    PAGINA = PAGINA_MENU;
    menu.show();
}

void iconeCongigPopCallBack(void *ptr){
  PAGINA= PAGINA_TRAVA;
  trava.show();
  mostraDadosTrava();
}

void mostraDadosTrava(){
    if(T.estado_porta_superior == ABERTA){
      //mudar imagem para cadeado aberto
    }
    else{
      // mudar imagem para cadeado fechado
    }

    if(T.estado_porta_inferior == ABERTA){
      //mudar imagem para cadeado aberto
    }
    else{
      // mudar imagem para cadeado fechado
    }
}


#endif
