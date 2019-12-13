#ifndef trava_h
#define trava_h

#include "gascarbonico.h"

#define INTERVALO 10000
//alterar nome do objeto na pagina
NexButton TravaBotao = NexButton(PAGINA_TRAVA, 0, "BTrava");



class trava{
  private:
    int Pino_fimcurso = 1;
    int Pino_trans = 2;
    unsigned long tempo_anterior = 0;
 
  public:
    int porta;
    void trava::setup(){
      Serial.begin(9600);
      pinMode(Pino_fimcurso, INPUT);
      pinMode(Pino_trans, OUTPUT);
    }
    
    void trava::testar_porta(){
      this->porta = digitalRead(Pino_fimcurso);
    }
    
    void trava::abrir(){
      unsigned long tempo_atual = millis();
      digitalWrite(Pino_trans, HIGH); //Liga relé
      
      if((tempo_atual - this->tempo_anterior) >= INTERVALO){
           digitalWrite(Pino_trans, LOW); // Desliga relé
           this->tempo_anterior = tempo_atual;
      }
    }
};




trava T; //declaração do objeto
void TravaPopCallBack(){
  T.abrir();
  }

#endif
