#ifndef novorrigacao_h
#define novoirrigacao_h
#include "vaso.h"

#define NUM_VASOS 4
#define INTERVALO_MOSTRA_DADOS 5000
//Falta colocar os ids e nomes corretos!!!

NexPage umidade_solo = NexPage(PAGINA_IRRIGACAO, 0, "UmidadeSolo");

NexButton sets[NUM_VASOS] = {
							NexButton(PAGINA_IRRIGACAO, 14, "setPoint0"),
							NexButton(PAGINA_IRRIGACAO, 11, "setPoint1"),
							NexButton(PAGINA_IRRIGACAO, 12, "setPoint2"),
							NexButton(PAGINA_IRRIGACAO, 13, "setPoint3")
							};

NexProgressBar gotas[NUM_VASOS] =	{
									NexProgressBar(PAGINA_IRRIGACAO, 7, "gota0"),
									NexProgressBar(PAGINA_IRRIGACAO, 8, "gota1"),
									NexProgressBar(PAGINA_IRRIGACAO, 9, "gota2"),
									NexProgressBar(PAGINA_IRRIGACAO, 10, "gota3")
									};
	
NexText medidas[NUM_VASOS] = 	{
								NexText(PAGINA_IRRIGACAO, 4, "numeroGota0"),
								NexText(PAGINA_IRRIGACAO, 5, "numeroGota1"),
								NexText(PAGINA_IRRIGACAO, 6, "numeroGota2"),
								NexText(PAGINA_IRRIGACAO, 7, "numeroGota3")
								};

NexButton voltar_irr = NexButton(PAGINA_IRRIGACAO, 14, "voltar");
NexButton icone_lavar = NexButton(PAGINA_IRRIGACAO, 15, "lavar");
NexButton icone_irr = NexButton(PAGINA_MENU, 1, "UmidadeSolo");



NexPage lavagem_terra = NexPage(PAGINA_LAVAGEM, 0, "LavagemTerra");

NexDSButton lavar_gotas[NUM_VASOS] = 	{
										NexDSButton(PAGINA_LAVAGEM, 14, "setPoint0"),
										NexDSButton(PAGINA_LAVAGEM, 11, "setPoint1"),
										NexDSButton(PAGINA_LAVAGEM, 12, "setPoint2"),
										NexDSButton(PAGINA_LAVAGEM, 13, "setPoint3")
										};

NexDSButton lavar_geral = NexDSButton(PAGINA_LAVAGEM, 14, "geral");
NexButton voltar_lavagem = NexButton(PAGINA_LAVAGEM, 14, "voltar");

class SIRR{
	public:
		vaso vasos[NUM_VASOS] = {
								vaso(0, PINO_VASO_0, PINO_SIRR_0, end_vaso_0),
								vaso(1, PINO_VASO_1, PINO_SIRR_1, end_vaso_1),
								vaso(2, PINO_VASO_2, PINO_SIRR_2, end_vaso_2),
								vaso(3, PINO_VASO_3, PINO_SIRR_3, end_vaso_3)
								};

		unsigned long timerMostrardados;

		void run(){
			for(uint8_t id = 0; id < NUM_VASOS; id++ ){
				this->vasos[id].run();
			}
			if(millis() - timerMostrardados >= INTERVALO_MOSTRA_DADOS){
				if(PAGINA == PAGINA_IRRIGACAO)
					mostraDadosIrr();
				else if(PAGINA == PAGINA_LAVAGEM)
					mostraDadosLavagem();
			}
		}

		void lavarTerra(uint8_t id, uint32_t tempo){
			this->vasos[id].lavarTerra(tempo);
		}

		void pararLavagem(uint8_t id){
			this->vasos[id].pararLavagem();
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
void iconeIrrCallback(void *ptr){
	PAGINA = PAGINA_IRRIGACAO;
	umidade_solo.show();
}
void IconeLavarCallback(void *ptr){
	PAGINA = PAGINA_LAVAGEM;
	lavagem_terra.show();
}

void Lavar0Callback(void *ptr){
	uint32_t estado_botao;

	lavar_gotas[0].getValue(&estado_botao);

	if(estado_botao == LIGADO){
		botaoApertado = botaoIR0;
		teclado.show();
		PassaBotaoParaTela(IR.vasos[0].tempo_lavagem);
	}
	else{
		IR.pararLavagem(0);
	}
}
void Lavar1Callback(void *ptr){
	uint32_t estado_botao;

	lavar_gotas[1].getValue(&estado_botao);

	if(estado_botao == LIGADO){
		botaoApertado = botaoIR1;
		teclado.show();
		PassaBotaoParaTela(IR.vasos[1].tempo_lavagem);
	}
	else{
		IR.pararLavagem(1);
	}
}
void Lavar2Callback(void *ptr){
	uint32_t estado_botao;

	lavar_gotas[2].getValue(&estado_botao);

	if(estado_botao == LIGADO){
		botaoApertado = botaoIR2;
		teclado.show();
		PassaBotaoParaTela(IR.vasos[2].tempo_lavagem);
	}
	else{
		IR.pararLavagem(2);
	}
}
void Lavar3Callback(void *ptr){
	uint32_t estado_botao;

	lavar_gotas[3].getValue(&estado_botao);

	if(estado_botao == LIGADO){
		botaoApertado = botaoIR3;
		teclado.show();
		PassaBotaoParaTela(IR.vasos[3].tempo_lavagem);
	}
	else{
		IR.pararLavagem(3);
	}
}
void LavarGeralCallback(void *ptr){
	uint32_t estado_botao;

	lavar_geral.getValue(&estado_botao);

	if(estado_botao == LIGADO){
		botaoApertado = botaoLVG;
		teclado.show();
		PassaBotaoParaTela(IR.vasos[0].tempo_lavagem);
	}

	else for(uint8_t id = 0; id < NUM_VASOS; id++){
		IR.pararLavagem(id);
	}
}
void voltarLavagem(void *ptr){
	PAGINA = PAGINA_IRRIGACAO;
	umidade_solo.show();
	mostraDadosIrr();
}
//Ideia: ter um DSbutton chamado calibrar,
//quando ele fosse apertado os botoes de setar somem
//e botes de setar max e min aparecem para cada gota
//dps é só apertar o DSbutton e voltar com os botoes de setar
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

void mostraDadosLavagem(){
	for(uint8_t id = 0; id < NUM_VASOS; id++ ){
		
	}
}

#endif