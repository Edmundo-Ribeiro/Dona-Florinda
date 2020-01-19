#ifndef novorrigacao_h
#define novoirrigacao_h
#include "vaso.h"

//colocar isso em definiçoes.h
#define NUM_VASOS 4
#define INTERVALO_MOSTRA_DADOS_IR 5000


void mostraDadosIrr();
void mostraDadosLavagem();

//Falta colocar os ids e nomes corretos!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!<---------
//declarando todos os botoes e objetos que aparecem na tela
// ////////////////////////////////////////////////////////////////////////////////////////////////////////

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

	NexButton Maxs[NUM_VASOS] = {
								NexButton(PAGINA_IRRIGACAO, 14, "max0"),
								NexButton(PAGINA_IRRIGACAO, 11, "max1"),
								NexButton(PAGINA_IRRIGACAO, 12, "max2"),
								NexButton(PAGINA_IRRIGACAO, 13, "max3")
								};

	NexButton Mins[NUM_VASOS] = {
								NexButton(PAGINA_IRRIGACAO, 14, "min0"),
								NexButton(PAGINA_IRRIGACAO, 11, "min1"),
								NexButton(PAGINA_IRRIGACAO, 12, "min2"),
								NexButton(PAGINA_IRRIGACAO, 13, "min3")
								};

	NexDSButton btn_calibrar = NexDSButton(PAGINA_IRRIGACAO, 14, "calibrar");
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
// ////////////////////////////////////////////////////////////////////////////////////////////////////////

//Classe para o sistema 
class SIRR{
	public:
		//inicializando os vasos das plantas
		vaso vasos[NUM_VASOS] = {
								vaso(0, PINO_VASO_0, PINO_SIRR_0, end_vaso_0),
								vaso(1, PINO_VASO_1, PINO_SIRR_1, end_vaso_1),
								vaso(2, PINO_VASO_2, PINO_SIRR_2, end_vaso_2),
								vaso(3, PINO_VASO_3, PINO_SIRR_3, end_vaso_3)
								};

		unsigned long timerMostrardados;
		bool calibrando = false;
		bool lavagemGeral = false;

		void run(){
			uint8_t id;

			//se não está calibrando execultar tudo normamente
			if(!this->calibrando){	
				for(id = 0; this->calibrando && (id < NUM_VASOS); id++ ){
					this->vasos[id].run();
				}
			}
			//se o botao de calibrar foi acionado o sistema não deve reagir
			//as variacoes nas medidas, então ele é desligado e caso estivesse lavando
			//a terra isso também é interrompido
			else{
				for(id = 0; id < NUM_VASOS; id++ ){
					this->vasos[id].pararLavagem();
				}
			}

			//de tempo em tempos atualizar as informacoes na tela
			if(millis() - timerMostrardados >= INTERVALO_MOSTRA_DADOS_IR){
				if(PAGINA == PAGINA_IRRIGACAO)
					mostraDadosIrr();
				else if(PAGINA == PAGINA_LAVAGEM)
					mostraDadosLavagem();
			}
		}

		//mandar o vaso com tal id lavar a terra por certo tempo(em segundos)
		void lavarTerra(uint8_t id, uint32_t tempo){
			this->vasos[id].lavarTerra(tempo);
		}

		//mandar o vaso com tal id parar de lavar a terra
		void pararLavagem(uint8_t id){
			this->vasos[id].pararLavagem();
		}
		//mandar o vaso com tal id ler novo valor de max
		void calibraMax(uint8_t id){
			this->vasos[id].calibraMax();
		}
		//mandar o vaso com tal id ler novo valor de min
		void calibraMin(uint8_t id){
			this->vasos[id].calibraMin();
		}

		//informar para o vaso com tal id, qual o valor de umidade do solo desejado
		void setar(uint8_t id, uint8_t ref){
			this->vasos[id].setar(ref);
		}

		//iniciar modo de calibracao do sistema
		void calibrar(bool flag){
			//quando o botao de calibrar for acionado
			//a flag muda pra true e a fila é zerada
			//para que as novas medicoes feitas sejam
			//com os novos valores de min e max
			this->calibrando = flag;
			for(uint8_t id = 0; flag && (id < NUM_VASOS); id++ ){
				this->vasos[id].F.resetar();
			}
		}
};

SIRR IR;

// com base no estado do botao ativar ou desativar modo de calibragem
void calibrarCallback(void *ptr){
	uint32_t estado_botao;

	btn_calibrar.getValue(&estado_botao);

	IR.calibrar(estado_botao);
}

//funcao aux pra reduzir codigo repetido.
//abrir o teclado para que o novo valor de referencia
//seja setado
void auxSet(uint8_t id, uint8_t botao){
	botaoApertado = botao;
	teclado.show();
	PassaBotaoParaTela(IR.vasos[id].referencia);
}
void Set0Callback(void *ptr) { auxSet(0,botaoIR0);}
void Set1Callback(void *ptr) { auxSet(1,botaoIR1);}
void Set2Callback(void *ptr) { auxSet(2,botaoIR2);}
void Set3Callback(void *ptr) { auxSet(3,botaoIR3);}

//mudar isso para definiçoes.h----
#define MAX true
#define MIN false

//para dar um feedback instantaneo para o cliente
//quando ele colocar o sensor na agua e apertar
//o botao de calibrar maximo, a gota vai mudar para
//totalmente cheia e o valor que vai aparecer é 100
//o contrario disso para quando o botao de minimo
//for apertado

//funcao auxCalibragem é uma fncao auxiliar 
//para não ficar repetindo tanto codigo.
//minOUmax recebe true para setar o maximo
//e false para setar o minimo
void auxCalibragem(uint8_t id, bool minOUmax){
	char conteudo_botao[7];

	if(minOUmax == MAX){
		IR.vasos[id].calibraMax();
		gotas[id].setValue(100);
		sprintf(conteudo_botao, "%02d%",100 );
	}
	else{
		IR.vasos[id].calibraMin();
		gotas[id].setValue(0);
		sprintf(conteudo_botao, "%02d%",0 );
	}
	
	medidas[id].setText(conteudo_botao);
}
void setMax0(void *ptr) { auxCalibragem(0,MAX);}
void setMax1(void *ptr) { auxCalibragem(1,MAX);}
void setMax2(void *ptr) { auxCalibragem(2,MAX);}
void setMax3(void *ptr) { auxCalibragem(3,MAX);}
void setMin0(void *ptr) { auxCalibragem(0,MIN);}
void setMin1(void *ptr) { auxCalibragem(1,MIN);}
void setMin2(void *ptr) { auxCalibragem(2,MIN);}
void setMin3(void *ptr) { auxCalibragem(3,MIN);}

void voltarIRR(void *ptr){
	PAGINA = PAGINA_MENU;
	menu.show();
}
void iconeIrrCallback(void *ptr){
	PAGINA = PAGINA_IRRIGACAO;
	umidade_solo.show();
}

//funcao aux para reduzir a quantidade de codigo repetido.
//se o botao de lavar do vaso de tal id for acionado
//ele abre o teclado para inserir quanto tempo deve
//manter a bomba ligada. Se ele foi apertado para 
//desligar entao a lavagem é interrompida
void auxLavar(uint8_t id, uint8_t botao){
	uint32_t estado_botao;

	lavar_gotas[id].getValue(&estado_botao);

	if(estado_botao == LIGADO){
		botaoApertado = botao;
		teclado.show();
		PassaBotaoParaTela(IR.vasos[id].tempo_lavagem);
	}
	else{
		IR.pararLavagem(id);
	}
}
void Lavar0Callback(void *ptr) { auxLavar(0,botaoIR0);}
void Lavar1Callback(void *ptr) { auxLavar(1,botaoIR1);}
void Lavar2Callback(void *ptr) { auxLavar(2,botaoIR2);}
void Lavar3Callback(void *ptr) { auxLavar(3,botaoIR3);}

void LavarGeralCallback(void *ptr){
	uint32_t estado_botao;

	lavar_geral.getValue(&estado_botao);

	IR.lavagemGeral = estado_botao;
	
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
void IconeLavarCallback(void *ptr){
	PAGINA = PAGINA_LAVAGEM;
	lavagem_terra.show();
}

//Ideia: ter um DSbutton chamado calibrar,
//quando ele fosse apertado os botoes de setar somem
//e botes de setar max e min aparecem para cada gota
//dps é só apertar o DSbutton e voltar com os botoes de setar
void mostraDadosIrr(){
	char conteudo_botao[7];

	
	btn_calibrar.setValue(IR.calibrando);

	for(uint8_t id = 0; id < NUM_VASOS; id++ ){
		
		sprintf(conteudo_botao, "%02d%", IR.vasos[id].referencia);
		sets[id].setText(conteudo_botao);
		
		gotas[id].setValue(round(IR.vasos[id].media));

		sprintf(conteudo_botao, "%02d%", round(IR.vasos[id].media) );
		medidas[id].setText(conteudo_botao);
	}
}
void mostraDadosLavagem(){
	uint8_t count = 0;

	lavar_geral.setValue(IR.lavagemGeral);

	for(uint8_t id = 0; id < NUM_VASOS; id++ ){
		if(IR.vasos[id].estado_atual == LIGADO){
			lavar_gotas[id].setValue(LIGADO);
		}
		else
			lavar_gotas[id].setValue(DESLIGADO);
	}
}

#endif