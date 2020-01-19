#ifndef trava_h
#define trava_h

#include "datahora.h"

#define INTERVALO 5000
#define IMG_TRAVA_ABERTA 29
#define IMG_TRAVA_FECHADA 28

void mostraDadosTrava();

//alterar nome do objeto na pagina
NexButton icone_config = NexButton(PAGINA_MENU, 4, "defDataHora");//mudar esse nome

NexCrop btnTravaSuperior = NexCrop(PAGINA_TRAVA, 4, "travaSuperior");
NexCrop btnTravaInferior = NexCrop(PAGINA_TRAVA, 3, "travaInferior");
NexButton relogio = NexButton(PAGINA_TRAVA, 1, "relogio");
NexButton voltar_trava = NexButton(PAGINA_TRAVA, 2, "voltar");

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
      this->estado_porta_superior = digitalRead(PINO_FIM_DE_CURSO_SUPERIOR);
      this->estado_porta_inferior = digitalRead(PINO_FIM_DE_CURSO_INFERIOR);

    }
    
    void Trava::abrirInferior(){
      digitalWrite(PINO_TRANSISTOR_INFERIOR, HIGH); // trava desativada
      timerInferior = millis();//resetar o timer

    }

    void Trava::abrirSuperior(){
      digitalWrite(PINO_TRANSISTOR_SUPERIOR, HIGH);// trava desativada
      timerSuperior = millis();//resetar o timer

    }

    void Trava::run(){
      this->estado_porta_superior = digitalRead(PINO_FIM_DE_CURSO_SUPERIOR);
      this->estado_porta_inferior = digitalRead(PINO_FIM_DE_CURSO_INFERIOR);

      if( (millis() - this->timerSuperior) >= INTERVALO ){
        digitalWrite(PINO_TRANSISTOR_SUPERIOR, LOW); // trava ativada
        this->timerSuperior = millis();
        if(PAGINA==PAGINA_TRAVA)
          mostraDadosTrava();
      }


      if( (millis() - this->timerInferior) >= INTERVALO ){
        digitalWrite(PINO_TRANSISTOR_INFERIOR, LOW); // trava ativada
        this->timerInferior = millis();
        if(PAGINA==PAGINA_TRAVA)
          mostraDadosTrava();
      }
    }

};




Trava T; //declaração do objeto

void TravaSuperiorPopCallBack(){
  //mudar imagem para imagem de cadeado aberto
  btnTravaSuperior.setPic(IMG_TRAVA_ABERTA);
  //abrir trava superior
  T.abrirSuperior();
}

void TravaInferiorPopCallBack(){
  //mesma coisa da trava superior
  btnTravaInferior.setPic(IMG_TRAVA_ABERTA);
  T.abrirInferior();
}

void RelogioPopCallBack(){
   PAGINA = PAGINA_DATA_HORA;
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
      btnTravaSuperior.setPic(IMG_TRAVA_ABERTA);
    }
    else{
      // mudar imagem para cadeado fechado
      btnTravaSuperior.setPic(IMG_TRAVA_FECHADA);
    }

    if(T.estado_porta_inferior == ABERTA){
      //mudar imagem para cadeado aberto
      btnTravaInferior.setPic(IMG_TRAVA_ABERTA);
    }
    else{
      btnTravaInferior.setPic(IMG_TRAVA_FECHADA);
    }
}


#endif
