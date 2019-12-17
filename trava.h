#ifndef trava_h
#define trava_h

#define INTERVALO 5000

//alterar nome do objeto na pagina
NexButton btnTravaSuperior = NexButton(PAGINA_TRAVA, 0, "BTrava");
NexButton btnTravaInferior = NexButton(PAGINA_TRAVA, 0, "BTrava");


class Trava{
  private:
    unsigned long timerSuperior = millis();
    unsigned long timerInferior = millis();
 
  public:
    bool estado_porta_superior,
         estado_porta_inferior;
    
    Trava(){
      pinMode(PINO_FIM_DE_CURSO, INPUT);
      pinMode(PINO_TRANSISTOR_INFERIOR, OUTPUT);
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
      this->estado_porta_superior = digitalRead(PINO_FIM_DE_CURSO);
      this->estado_porta_inferior = digitalRead(PINO_FIM_DE_CURSO);//colocar pino correto aqui

      if( (millis() - this->timerSuperior) >= INTERVALO ){
        digitalWrite(PINO_TRANSISTOR_SUPERIOR, LOW); // Desliga relé
        this->timerSuperior = millis();
      }

      if( (millis() - this->timerInferior) >= INTERVALO ){
        Serial.println((millis() - this->timerInferior));
        digitalWrite(PINO_TRANSISTOR_INFERIOR, LOW); // Desliga relé
        this->timerInferior = millis();
      }
    }

};




Trava T; //declaração do objeto
void TravaSuperiorPopCallBack(){
  T.abrirSuperior();
}

void TravaInferiorPopCallBack(){
  T.abrirInferior();
}

#endif
