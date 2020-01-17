#ifndef novorrigacao_h
#define novoirrigacao_h
#include "vaso.h"

#define NUM_VASOS 4

NexPage umidade_solo = NexPage(PAGINA_IRRIGACAO, 0, "UmidadeSolo");

NexButton sets[NUM_VASOS] = {NexButton(PAGINA_IRRIGACAO, 14, "setPoint0"),
							NexButton(PAGINA_IRRIGACAO, 11, "setPoint1"),
							NexButton(PAGINA_IRRIGACAO, 12, "setPoint2"),
							NexButton(PAGINA_IRRIGACAO, 13, "setPoint3")};

NexProgressBar gotas[NUM_VASOS] = {NexProgressBar(PAGINA_IRRIGACAO, 7, "gota0"),
								  NexProgressBar(PAGINA_IRRIGACAO, 8, "gota1"),
								  NexProgressBar(PAGINA_IRRIGACAO, 9, "gota2"),
								  NexProgressBar(PAGINA_IRRIGACAO, 10, "gota3")};

NexText medidas[NUM_VASOS] = {NexText(PAGINA_IRRIGACAO, 4, "numeroGota0"),
							  NexText(PAGINA_IRRIGACAO, 5, "numeroGota1"),
							  NexText(PAGINA_IRRIGACAO, 6, "numeroGota2"),
							  NexText(PAGINA_IRRIGACAO, 7, "numeroGota3")};

NexButton voltar_irr = NexButton(PAGINA_IRRIGACAO, 14, "voltar");//botao da pagina 1 numero 9 nome numeroGota4
NexButton lavar = NexButton(PAGINA_IRRIGACAO, 15, "lavar");//botao da pagina 1 numero 9 nome numeroGota4
NexButton icone_irr = NexButton(PAGINA_MENU, 1, "UmidadeSolo");

// uint8_t PINOSV[NUM_VASOS] = {PINO_VASO_0, PINO_VASO_1, PINO_VASO_2, PINO_VASO_3};
// uint8_t PINOSS[NUM_VASOS] = {PINO_SIRR_0, PINO_SIRR_1, PINO_SIRR_2, PINO_SIRR_3};
// uint8_t ENDS[NUM_VASOS] = {end_vaso_0, end_vaso_1, end_vaso_2, end_vaso_3};

class SIRR{
	public:
		vaso vasos[NUM_VASOS] = {
								vaso(0, PINO_VASO_0, PINO_SIRR_0, end_vaso_0),
								vaso(1, PINO_VASO_1, PINO_SIRR_1, end_vaso_1),
								vaso(2, PINO_VASO_2, PINO_SIRR_2, end_vaso_2),
								vaso(3, PINO_VASO_3, PINO_SIRR_3, end_vaso_3)
								};

		void run(){
			for(uint8_t id = 0; id < NUM_VASOS; id++ ){
				this->vasos[id].run();
			}
		}

		void lavarTerra(uint8_t id, uint32_t tempo){
			this->vasos[id].lavarTerra(tempo);
		}

		void calibraMax(uint8_t id, uint16_t max){
			this->vasos[id].calibraMax(max);
		}

		void calibraMin(uint8_t id, uint16_t min){
			this->vasos[id].calibraMin(min);
		}

		void setar(uint8_t id, uint8_t ref){
			this->vasos[id].setar(ref);
		}

};

SIRR IR;

void Set0Callback(void *ptr){
	botaoApertado = botaoIR0;
	teclado.show();
	PassaBotaoParaTela(IR.vasos[0].referencia);
}
void Set1Callback(void *ptr){
	botaoApertado = botaoIR1;
	teclado.show();
	PassaBotaoParaTela(IR.vasos[1].referencia);
}
void Set2Callback(void *ptr){
	botaoApertado = botaoIR2;
	teclado.show();
	PassaBotaoParaTela(IR.vasos[2].referencia);
}
void Set3Callback(void *ptr){
	botaoApertado = botaoIR3;
	teclado.show();
	PassaBotaoParaTela(IR.vasos[3].referencia);
}


void voltarIRR(void *ptr){
	PAGINA = PAGINA_MENU;
	menu.show();
}

// void lavarCallback(void *ptr){
// 	PAGINA = PAGINA_LAVAGEM;
// 	lavagem.show();
// }

void iconeIrrCallback(void *ptr){
	PAGINA = PAGINA_IRRIGACAO;
	umidade_solo.show();
}

void mostraDadosIrr(){
	char conteudo_botao[7];

	for(uint8_t id = 0; id < NUM_VASOS; id++ ){
		
		sprintf(conteudo_botao, "%02d%", IR.vasos[id].referencia);
		sets[id].setText(conteudo_botao);
		
		gotas[id].setValue(round(IR.vasos[id].media));

		sprintf(conteudo_botao, "%02d%", round(IR.vasos[id].media) );
		medidas[id].setText(conteudo_botao);
	}

}

#endif