#ifndef exaustao_h
#define exaustao_h
#include "teclado.h"

NexPage exaustao = NexPage(PAGINA_EXAUSTAO, 0,"Exaustao");
NexButton voltar_exaustao = NexButton(PAGINA_EXAUSTAO, 1, "voltar");
NexButton min_ligado = NexButton(PAGINA_EXAUSTAO, 2, "minlig");
NexButton min_desligado = NexButton(PAGINA_EXAUSTAO, 3, "mindes");
NexButton veloc_ventilador = NexButton(PAGINA_EXAUSTAO, 4, "veloc");
NexCrop exaustor = NexCrop(PAGINA_EXAUSTAO, 5, "exaustor");
NexText texto_restante = NexText(PAGINA_EXAUSTAO, 6, "tempo_rest");
NexCrop texto_exaustao = NexCrop(PAGINA_EXAUSTAO, 7, "texto_exaus");

NexButton icone_exaustao = NexButton(PAGINA_MENU, 7, "Exaustao");

#define Ciclo_padrao 420000 // ciclo padrão de 7 minutos (7*MINUTO)
#define EXAUSTOFF 20 // Id da imagem no Nextion da exaustao off
#define EXAUSTON 21 // Id da imagem no Nextion da exaustao on

#define MINUTO 60000
#define INTERVALO_MOSTRAR 5000

void mostraDadosExaustao();
void debugExaustao();

class Exaustao{
    public:
        bool estado_atual = 0; // estado_atual é 0 tá desligado, e 1 se está ligado
        // Ciclos da exaustão

        uint16_t Ciclo_ligado, Ciclo_desligado;

        unsigned long Tempo_restante;

        unsigned long timer, exaustaomillis;

        Exaustao(){
            pinMode(PINO_RELE_EXAUST, OUTPUT);
            this->Tempo_restante = 0;
            this->timer = millis();
            this->exaustaomillis = millis();
            EEPROM.get(end_exautao_ligado, this->Ciclo_ligado); 
            EEPROM.get(end_exautao_desligado, this->Ciclo_desligado); 
        }

        // void Set_Ciclo_intervalo(uint16_t intervalo, bool ciclo){
        //     if(ciclo == LIGADO){
        //         this->Ciclo_ligado = intervalo;
        //     }
        //     else{
        //         this->Ciclo_desligado = intervalo;
        //     }
        // }

        void ligarExaustor(){
            digitalWrite(PINO_RELE_EXAUST, HIGH);//ver se é HIGH ou LOW
            this->estado_atual = LIGADO;
        }

        void desligarExaustor(){
            digitalWrite(PINO_RELE_EXAUST, LOW);//ver se é HIGH ou LOW
            this->estado_atual = DESLIGADO;
        }

        void run(){
            //pega o millisegundo atual
            unsigned long atual = millis();
            unsigned long tempoPassado = atual - this->timer;

            if(this->estado_atual == DESLIGADO){
                if(tempoPassado >= (this->Ciclo_desligado * MINUTO)){
                    this->timer = millis();
                    this->ligarExaustor();
                }
                this->Tempo_restante = (this->Ciclo_desligado * MINUTO) - tempoPassado;
            }
            else{
                 if(tempoPassado >= (this->Ciclo_ligado * MINUTO)){
                    this->timer = millis();
                    this->desligarExaustor();
                }
                this->Tempo_restante = (this->Ciclo_ligado * MINUTO) - tempoPassado;
                      }
            if (PAGINA == PAGINA_EXAUSTAO && (atual - this->exaustaomillis) >= INTERVALO_MOSTRAR){
                mostraDadosExaustao();
                this->exaustaomillis = atual;
                debugExaustao();
            }
        }

        void SetCiclo(uint16_t valor, bool tipo){
            if (tipo == LIGADO){
                this->Ciclo_ligado = valor;
                EEPROM.put(end_exautao_ligado, this->Ciclo_ligado); 
            }
            else{
                this->Ciclo_desligado = valor;
                EEPROM.put(end_exautao_desligado, this->Ciclo_desligado); 
            } 
        }
};

Exaustao E;

void debugExaustao(){
		Serial.print("############# Variaveis Exaustão #####################\n\n");
    
		Serial.print("Ciclo ligado: ");
		Serial.println(E.Ciclo_ligado);

        Serial.print("Estado atual: ");
        Serial.println(E.estado_atual);

		Serial.print("Ciclo desligado: ");
		Serial.println(E.Ciclo_desligado);

				
		Serial.print("Minuto restante: ");
		Serial.println(E.Tempo_restante);

		Serial.print("######################################################\n\n");
}

void mostraDadosExaustao(){ 
    char conteudo_botao[4], 
         texto_tempo_restante[7];
   
    sprintf(conteudo_botao,"%02d", E.Ciclo_ligado);
    min_ligado.setText(conteudo_botao);

    sprintf(conteudo_botao,"%02d", E.Ciclo_desligado);
    min_desligado.setText(conteudo_botao);    

   
    if(E.Tempo_restante > 60000){
        sprintf(texto_tempo_restante, "%02d min", (E.Tempo_restante / MINUTO) + 1);// + 1 pois a a parte fracionaria é ignorada
    }
    else{
        sprintf(texto_tempo_restante, "%02d s", (E.Tempo_restante / 1000));
    }

    texto_restante.setText(texto_tempo_restante);
    if (E.estado_atual == LIGADO){
        exaustor.setPic(EXAUSTON);
        texto_exaustao.setPic(EXAUSTON);
    }
    else{
        exaustor.setPic(EXAUSTOFF);
        texto_exaustao.setPic(EXAUSTOFF);
    }
    
}

void setarCicloLigado(void *ptr){
    botaoApertado = BTNCICLOLIG;
    teclado.show();
    PassaBotaoParaTela(E.Ciclo_ligado);
}

void setarCicloDesligado(void *ptr){
    botaoApertado = BTNCICLODESLIG;
    teclado.show();
    PassaBotaoParaTela(E.Ciclo_desligado);
}

void iconeExaustaoCallback(void *ptr){
    PAGINA = PAGINA_EXAUSTAO;
    exaustao.show();
    mostraDadosExaustao();
}

void voltarExaustaoCallback(void *ptr){
    PAGINA = PAGINA_MENU;
    menu.show();
}

#endif