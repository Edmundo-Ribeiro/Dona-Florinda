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
    private:
        // Só vai precisar de saber se o tubo de CO2 vai estar ligado
        unsigned long exaustaoMillis = 0,
                      minutoMillis = 0;

    public:
        int tuboCO2 = 0; //verifica se está ligado
        uint8_t  Tempo_restante = Ciclo_desligado,
                 minutopassou = 0;
        bool estado_atual = 0; // estado_atual é 0 tá desligado, e 1 se está ligado
        // Ciclos da exaustão
        uint32_t Ciclo_ligado = Ciclo_padrao,
                 Ciclo_desligado = Ciclo_padrao;
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
        void run(){
            //pega o millisegundo atual
            unsigned long atual = millis();
            tuboCO2 = digitalRead(PINO_RELE_CO2);
            //Olha se o tubo de CO2 tá ligado. Se estiver, é pra reinicar o ciclo no desligado.
            if(tuboCO2 && this->estado_atual){
                digitalWrite(PINO_RELE_EXAUST, HIGH);
                this->estado_atual = 0;
            }
            //olha se passou o intervalo
            if((atual - this->exaustaoMillis >= this->Ciclo_ligado) && !tuboCO2 && !this->estado_atual){ //Fica desligado
                digitalWrite(PINO_RELE_EXAUST, HIGH);
                this->estado_atual = 1;
                this->exaustaoMillis = atual;
                this->minutopassou = 0;
            }
            if((atual - this->exaustaoMillis >= this->Ciclo_desligado) && !tuboCO2 && this->estado_atual){ //Fica ligado
                digitalWrite(PINO_RELE_EXAUST, LOW);
                this->estado_atual = 0;
                this->exaustaoMillis = atual;
                this->minutopassou = 0;
            }
            if(atual - this->minutoMillis >= MINUTO){
                this->minutoMillis = atual;
                this->minutopassou += MINUTO; //Soma minutos em milissegundos.
                if(this->estado_atual){
                    this->Tempo_restante = this->Ciclo_ligado - this->minutopassou;
                }
                else{
                    this->Tempo_restante = this->Ciclo_desligado - this->minutopassou;
                }
                this->Tempo_restante /= MINUTO; //Passa me milissegundos pra minutos.
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
void mostraDadosExaustao(){ 
    char conteudo_botao[3], 
    texto_tempo_restante[2];

    sprintf(conteudo_botao,"%02d", E.Ciclo_ligado);
    min_ligado.setText(conteudo_botao);

    sprintf(conteudo_botao,"%02d", E.Ciclo_desligado);
    min_desligado.setText(conteudo_botao);    

    texto_restante.setText(E.Tempo_restante);
    if (E.estado_atual){
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