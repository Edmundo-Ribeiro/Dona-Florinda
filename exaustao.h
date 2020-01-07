#ifndef exaustao_h
#define exaustao_h
#include "teclado.h"
//ED: Lembrando que o código deve ver se o gas está ativo para não ligar a extaustao enquanto estiver injetando co2

NexPage exaustao = NexPage(PAGINA_EXAUSTAO, 0,"Exaustao");
NexButton voltar_exaustao = NexButton(PAGINA_EXAUSTAO, 1, "voltar");
NexButton min_ligado = NexButton(PAGINA_EXAUSTAO, 2, "minlig");
NexButton min_desligado = NexButton(PAGINA_EXAUSTAO, 3, "mindes");
NexButton veloc_ventilador = NexButton(PAGINA_EXAUSTAO, 4, "veloc");
NexCrop exaustor = NexCrop(PAGINA_EXAUSTAO, 5, "exaustor");
NexText texto_restante = NexText(PAGINA_EXAUSTAO, 6, "tempo_rest");
NexCrop texto_exaustao = NexCrop(PAGINA_EXAUSTAO, 7, "texto_exaus");

NexButton icone_exaustao = NexButton(PAGINA_MENU, 8, "botaoExaustao");
#define Ciclo_padrao 420000 // ciclo padrão de 7 minutos (7*60*1000)
#define EXAUSTOFF 24 // Id da imagem no Nextion da exaustao off
#define EXAUSTON 25 // Id da imagem no Nextion da exaustao on

class Exaustao{
    private:
        // Como só vai utilizar 1 relé para os 2 exaustores
        int pinRele;
        // Só vai precisar de saber se o tubo de CO2 vai estar ligado


        unsigned long exaustaoMillis = 0,
                      minutoMillis = 0;
        const bool LIGADO = 1;
        const bool DESLIGADO = 0;

        const uint8_t minuto = 60000;

    public:
        uint8_t  Tempo_restante = Ciclo_desligado,
                 minutopassou = 0;
        bool tuboCO2, //verifica se está ligado
             estado_atual = 0; // estado_atual é 0 tá desligado, e 1 se está ligado
        // Ciclos da exaustão
        uint16_t Ciclo_ligado = Ciclo_padrao,
                 Ciclo_desligado = Ciclo_padrao;
        const uint8_t CICLOLIG = 1;
        const uint8_t CICLODESLIG = 2;
        // "Seta" o relé
        void SetupRele(uint16_t pinRele){
            pinMode(this->pinRele, OUTPUT);
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
        //Olha se o tubo de CO2 tá ligado. Se estiver, é pra reinicar o ciclo no desligado.
        if(tuboCO2 && this->estado_atual){
            digitalWrite(this->pinRele, HIGH);
            this->estado_atual = 0;
        }
        //olha se passou o intervalo
        if((atual - this->exaustaoMillis >= this->Ciclo_ligado) && !tuboCO2 && !this->estado_atual){ //Fica desligado
            digitalWrite(this->pinRele, HIGH);
            this->estado_atual = 1;
            this->exaustaoMillis = atual;
            this->minutopassou = 0;
        }
        if((atual - this->exaustaoMillis >= this->Ciclo_desligado) && !tuboCO2 && this->estado_atual){ //Fica ligado
            digitalWrite(this->pinRele, LOW);
            this->estado_atual = 0;
            this->exaustaoMillis = atual;
            this->minutopassou = 0;
        }
        if(atual - this->minutoMillis >= this->minuto){
            this->minutoMillis = atual;
            this->minutopassou += 60000; //Soma minutos em milissegundos.
            if(this->estado_atual){
                Tempo_restante = Ciclo_ligado - this->minutopassou;
            }
            else{
                Tempo_restante = Ciclo_desligado - this->minutopassou;
            }
            Tempo_restante /= 60000; //Passa me milissegundos pra minutos.
        }
    }
};
Exaustao E;
void mostraDadosExaustao(){ 
        char conteudo_botao[2], 
       texto_tempo_restante[2];
    uint16_t minutos_ligado,minutos_desligado;


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
    botaoApertado = E.CICLOLIG;
    teclado.show();
    PassaBotaoParaTela((E.Ciclo_ligado)/60000);
}
void setarCicloDesligado(void *ptr){
    botaoApertado = E.CICLODESLIG;
    teclado.show();
    PassaBotaoParaTela((E.Ciclo_desligado)/60000);
}
void iconeExaustaoCallback(void *ptr){
    PAGINA = PAGINA_EXAUSTAO;
    exaustao.show();
    mostraDadosExaustao();
}

#endif