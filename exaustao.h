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

class Exaustao{
    public:
        bool estado_atual = 0; // estado_atual é 0 tá desligado, e 1 se está ligado
        // Ciclos da exaustão
        uint32_t Ciclo_ligado = Ciclo_padrao,
                 Ciclo_desligado = Ciclo_padrao,
                 Tempo_restante = Ciclo_desligado;
        uint8_t minutopassou = 0;

        unsigned long timer = millis();

        // "Seta" o relé
        void SetupRele(){
            pinMode(PINO_RELE_EXAUST, OUTPUT);
        }
        void Set_Ciclo_intervalo(uint16_t intervalo, bool ciclo){
            if(ciclo == LIGADO){
                this->Ciclo_ligado = intervalo;
            }
            else{
                this->Ciclo_desligado = intervalo;
            }
        }
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
                if(tempoPassado >= this->Ciclo_desligado){
                    this->timer = millis();
                    this->ligarExaustor();
                }
                this->Tempo_restante = (this->Ciclo_desligado - tempoPassado)/MINUTO;
            }
            else{
                 if(tempoPassado >= this->Ciclo_ligado){
                    this->timer = millis();
                    this->desligarExaustor();
                }

                this->Tempo_restante = (this->Ciclo_desligado - tempoPassado)/MINUTO;
            }
        }
        void SetCiclo(int valor, int tipo){
            if (tipo == LIGADO){
                this->Ciclo_ligado = valor*MINUTO;
            }
            else{
                this->Ciclo_desligado = valor*MINUTO;
            } 
        }
};
Exaustao E;

void debugExaustao(){
		Serial.print("############# Variaveis Exaustão #####################\n\n");
    
		Serial.print("Ciclo ligado: ");
		Serial.println(E.Ciclo_ligado);

		Serial.print("Ciclo desligado: ");
		Serial.println(E.Ciclo_desligado);

				
		Serial.print("Minuto passou: ");
		Serial.println(E.minutopassou);

		Serial.print("######################################################\n\n");
}

void mostraDadosExaustao(){ 
    char conteudo_botao[4], 
         texto_tempo_restante[4];
    E.Ciclo_ligado /= MINUTO;
    E.Ciclo_desligado /= MINUTO;
    sprintf(conteudo_botao,"%02d", E.Ciclo_ligado);
    min_ligado.setText(conteudo_botao);

    sprintf(conteudo_botao,"%02d", E.Ciclo_desligado);
    min_desligado.setText(conteudo_botao);    

    sprintf(texto_tempo_restante, "%02d", E.Tempo_restante);

    texto_restante.setText(texto_tempo_restante);
    if (E.estado_atual){
        exaustor.setPic(EXAUSTON);
        texto_exaustao.setPic(EXAUSTON);
    }
    else{
        exaustor.setPic(EXAUSTOFF);
        texto_exaustao.setPic(EXAUSTOFF);
    }
    E.Ciclo_ligado *= MINUTO;
    E.Ciclo_desligado *= MINUTO;
}

void setarCicloLigado(void *ptr){
    botaoApertado = BTNCICLOLIG;
    teclado.show();
    PassaBotaoParaTela((E.Ciclo_ligado)/MINUTO);
}

void setarCicloDesligado(void *ptr){
    botaoApertado = BTNCICLODESLIG;
    teclado.show();
    PassaBotaoParaTela((E.Ciclo_desligado)/MINUTO);
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