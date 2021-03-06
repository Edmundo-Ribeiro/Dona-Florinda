#ifndef iluminacao_h
#define iluminacao_h
#include "teclado.h"


#define INTERVALO_ILUMINACAO 10000

//não presisaria fazer isso se tivesse um .h e .cpp separados para parte grafica e a classe SI
void mostraDadosIluminacao();
uint16_t minutoAtual();
void debugEstadoVariaveis();

// ###########################################################################################################
// ######################## DECLARAÇÃO DO OBJETO E LÓGICA DE FUNCIONAMENTO ###################################
// ###########################################################################################################

class SI{			
	public:
		bool estado_atual;

		uint8_t q_horas_ligado_c1, // onde fica armazenado quantas horas o ciclo 1 tem que ficar ligado (mudar pra minutos?)
			    q_horas_ligado_c2, // onde fica armazenado quantas horas o ciclo 2 tem que ficar ligado
			    ciclo_atual;   // onde fica armazenado qual do ciclo está sendo executado (ciclo1, ciclo 2, ciclo_nenhum)
		
		uint16_t minuto_de_desligar, // onde fica armazenado o minuto do dia em que deve desligar e ligar
			     minuto_de_ligar;

		unsigned long iluminacaoMillis = 0;

		bool estado_porta;
		

		SI(){

			pinMode(PINO_PAINEL, OUTPUT);
			pinMode(PINO_LED, OUTPUT);

			this->desligarLED();
			this->desligarPainel();

			uint16_t q_minutos_ligado;

			this->estado_atual = EEPROM.read(end_estado_atual);
			this->ciclo_atual = EEPROM.read(end_ciclo_atual);
			
			this->q_horas_ligado_c1 = EEPROM.read(end_q_horas_ligado_c1);
			this->q_horas_ligado_c2 = EEPROM.read(end_q_horas_ligado_c2);

			EEPROM.get(end_minuto_de_ligar, this->minuto_de_ligar);
			/*
			como não estou salvando o minuto de desligar na memoria
			sempre que inicia ele é calculado a partir do minuto de ligar 
			e o intervalo de tempo 
			*/
			if(this->ciclo_atual == CICLO_1){
				q_minutos_ligado = this->q_horas_ligado_c1*60;
			}
			else if(this->ciclo_atual == CICLO_2){
				q_minutos_ligado = this->q_horas_ligado_c2*60;
			}
			
			this->minuto_de_desligar = calcularMinuto(this->minuto_de_ligar, q_minutos_ligado);	

			if(this->ciclo_atual == CICLO_NENHUM){
				this->minuto_de_ligar = MINUTOS_DIA; 
				this->minuto_de_desligar = 0;
			}

			this->estado_porta = FECHADA;

		}

		//metodo para calcular o minuto de ligar ou desligar considerando a passagem do dia
		uint16_t calcularMinuto(uint16_t inicio, uint16_t intervalo){
			uint16_t minuto = inicio + intervalo;
			/*se o resultado da soma for maior que a quantidade de minutos em um dia
			  deve-se pegar o resto da divisão inteira*/
			minuto = minuto <= MINUTOS_DIA ? 
									   minuto : 
									   minuto % MINUTOS_DIA;

			return minuto;
		}

		void desligarPainel(){
			//Serial.println("\nDesligando Painel...\n");//só na fase de teste, dps tirar isso
			digitalWrite(PINO_PAINEL, LOW);
		}

		void ligarPainel(){
			//Serial.println("\nLigando Painel...\n");//só na fase de teste, dps tirar isso
			digitalWrite(PINO_PAINEL, HIGH);
		}

		void desligarLED(){
			//Serial.println("\nDesligando LED...\n");//só na fase de teste, dps tirar isso
			digitalWrite(PINO_LED, LOW);
		}

		void ligarLED(){
			//Serial.println("\nLigando LED...\n");//só na fase de teste, dps tirar isso
			digitalWrite(PINO_LED, HIGH);// no relé isso é LOW
		}

		void desligarSistema(){
			this->desligarPainel();
			this->desligarLED();
			this->estado_atual = DESLIGADO;
			EEPROM.update(end_estado_atual,this->estado_atual);
		}
		void ligarSistema(){
			this->ligarPainel();
			this->desligarLED();
			this->estado_atual = LIGADO;
			EEPROM.update(end_estado_atual,this->estado_atual);
		}

		void pausa(){
			this->desligarPainel();
			this->ligarLED();
		}

		//mudar esse nome
		void verificacoes(){
			//pegar qual o minuto atual 
			uint16_t minuto_atual = minutoAtual();

			

			/*Veririficações no caso do sistema ter sido reiniciado após desligamento ou queda de energia
			caso o sistema seja desligado no meio de um ciclo e ligado novamente algum tempo depois, 
			devemos verificar em qual estado ele deveria estar (painel desligado ou painel ligado).
			
			Nesse primeiro if verificamos qual dos tipos de ciclos estamos executando ou seja,
			se é um ciclo em que o minuto de ligar é antes ou depois do minuto de desligar*/
			bool esta_no_lado_ciclo_ligado;

			if(this->minuto_de_ligar < minuto_de_desligar){
				//verificando em qual lado do ciclo está (lado desligado ou lado ligado)
				esta_no_lado_ciclo_ligado = (minuto_atual >= this->minuto_de_ligar 
											 && 
											 minuto_atual <= this->minuto_de_desligar);
			}
			else{
				esta_no_lado_ciclo_ligado = !(minuto_atual >= this->minuto_de_desligar 
						  					  &&
						  					  minuto_atual <= this->minuto_de_ligar);
			}

			//se está ligado mas não está do lado do ciclo ligado
			if( !esta_no_lado_ciclo_ligado){
				//desligue
				this->desligarSistema();
			}
			else if( esta_no_lado_ciclo_ligado){
				this->ligarSistema();
				//não tem problema chamar essa função repetidamente pois 
				//com EEPROM.update não é feita nenhuma escrita na memoria 
				//se o valor for o mesmo
			}
		}
		void run(bool estado_porta){
			/*
			se a porta está aberta pausar o sistema
			ou seja, ligar fita de led, deligar painel, 
			parar de contar minutos passados, 
			verificar se deve ligar ou não o painel
			*/
			this->estado_porta = estado_porta;

			unsigned long atual = millis();

			if(estado_porta == ABERTA){

				this->pausa();
				//resetar o contador do intervalo para garantir que vai esperar 5
				//segundos antes de ligar o painel

				if(atual - this->iluminacaoMillis >= INTERVALO_ILUMINACAO){
					if(PAGINA == PAGINA_ILUMINACAO){
						mostraDadosIluminacao();
						this->iluminacaoMillis = atual;
					}
				}
			}
			/*
			Caso a porta esteja fechada 
			deligar a fita de ledl
			retomar ao estado que deveira estar(painel ligado ou desligao)
			e etc...
			*/
			else{

				
				//desliga fita de led e retoma o estado do painel
				//this->retomar();
				//pegar milisegundo atual "inicio to timer"
				
				//olha se passou o intervalo 
				if(atual - this->iluminacaoMillis >= INTERVALO_ILUMINACAO){
					// Serial.print("estado da porta:");
					// Serial.println(estado_porta);
					this->verificacoes();

					//resetar o contador do intervalo
					this->iluminacaoMillis = atual;

					//se estiver na pagina de iluminação atualiza os dados
					if(PAGINA == PAGINA_ILUMINACAO){
						mostraDadosIluminacao();
						debugEstadoVariaveis();
					}
				}
			}
		}

		void setar(uint8_t ciclo, uint8_t valor){
			if(ciclo == CICLO_1){
				this->q_horas_ligado_c1 = valor;
				EEPROM.update(end_q_horas_ligado_c1,valor);
			}
			else if(ciclo == CICLO_2){
				this->q_horas_ligado_c2 = valor;
				EEPROM.update(end_q_horas_ligado_c2,valor);
			}
			this->troca_ciclo(this->ciclo_atual);
		}

		void troca_ciclo(uint8_t ciclo_destino){
			uint8_t q_horas_ligado, 
					q_horas_desligado,
					ciclo_origem;

			uint16_t minuto_atual,
					 minutos_restantes;
			
			//definir de onde está saindo
			ciclo_origem = this->ciclo_atual;

			//atribuir o valor da quantidade de horas ligado conforme o ciclo de destino
			if(ciclo_destino == CICLO_1){
				q_horas_ligado = this->q_horas_ligado_c1;
				this->ciclo_atual = CICLO_1;
			}
			else if(ciclo_destino == CICLO_2){
				q_horas_ligado = this->q_horas_ligado_c2;
				this->ciclo_atual = CICLO_2;
					
			}
			else{
				q_horas_ligado = 0;
				this->ciclo_atual = CICLO_NENHUM;
			}
			
			
			q_horas_desligado = 24 - q_horas_ligado;

			minuto_atual = minutoAtual();
			
			//situação em que o cliente setou um novo valor para algum dos ciclos
			//nesse caso só atualizar que horas deve desligar e o metodo run cuida de ver se deve ligar ou desligar
			if(ciclo_origem == ciclo_destino){
				this->minuto_de_desligar = calcularMinuto(this->minuto_de_ligar, q_horas_ligado*60);
			}

			//se está saindo de ciclo nenhum para algum ciclo 
			//entaõ o sistema esta sendo iniciado naquele momento
			//dessa forma, o minuto de ligar é o estante atual e basta calcular quando desligar
			else if(ciclo_destino == CICLO_NENHUM){
				this->minuto_de_ligar = MINUTOS_DIA; 
				this->minuto_de_desligar = 0;
			}

			else{
				this->minuto_de_ligar = minuto_atual;
				this->minuto_de_desligar = calcularMinuto(minuto_atual, q_horas_ligado*60);
			}

			EEPROM.put(end_minuto_de_ligar, this->minuto_de_ligar);

			EEPROM.update(end_ciclo_atual, this->ciclo_atual);
			
			if(this->estado_porta == FECHADA){
				this->verificacoes();
			}
			mostraDadosIluminacao();
		}
};

// ###########################################################################################################

SI I; //Intanciando o objeto


// ###########################################################################################################
// ########################## VARIAVEIS PARA PAGINA DE ILUMINAÇÃO ############################################
// ###########################################################################################################
NexPage iluminacao = NexPage(PAGINA_ILUMINACAO,0,"Iluminacao");

NexButton voltar_iluminacao = NexButton(PAGINA_ILUMINACAO, 1, "voltar");
NexDSButton btn_c2 = NexDSButton(PAGINA_ILUMINACAO, 2, "btn_c2");
NexDSButton btn_c1 = NexDSButton(PAGINA_ILUMINACAO, 3, "btn_c1");
NexText estado_ciclo_texto = NexText(PAGINA_ILUMINACAO, 4, "estado_ciclo");
NexButton btn_setar_c1 = NexButton(PAGINA_ILUMINACAO, 5, "setar_c1");
NexButton btn_setar_c2 = NexButton(PAGINA_ILUMINACAO, 6, "setar_c2");
NexProgressBar progresso = NexProgressBar(PAGINA_ILUMINACAO,8,"progresso");
NexText tempo_restante = NexText(PAGINA_ILUMINACAO, 10, "tempo_restante");

NexButton icone_ilumincao = NexButton(PAGINA_MENU,3,"iluminacao");	
// ###########################################################################################################

// ###########################################################################################################
// #####################CALLBACKS E FUNÇÔES PARA PAGINA DE DEFINIÇÂO ILUMINAÇÃO ##############################
// ###########################################################################################################
	
void debugEstadoVariaveis(){
	dbSerialPrint("############# Variaveis Iluminação #####################\n\n");
	
	dbSerialPrint("Estado Atual: ");
	dbSerialPrintln(I.estado_atual);

	dbSerialPrint("Ciclo Atual: ");
	dbSerialPrintln(I.ciclo_atual);

			
	dbSerialPrint("q_horas_ligado_c1: ");
	dbSerialPrintln(I.q_horas_ligado_c1);
	dbSerialPrint("q_horas_ligado_c2: ");
	dbSerialPrintln(I.q_horas_ligado_c2);

	dbSerialPrint("minuto_de_desligar: ");
	dbSerialPrintln(I.minuto_de_desligar);
	dbSerialPrint("minuto_de_ligar: ");
	dbSerialPrintln(I.minuto_de_ligar);
	
	dbSerialPrint("minuto_atual: ");
	dbSerialPrintln(minutoAtual());

	dbSerialPrint("######################################################\n\n");

}

uint16_t minutoAtual(){
	return 60*rtc.now().hour() + rtc.now().minute();
}

void mostraDadosIluminacao(){	

	char conteudo_botao[8], 
			texto_tempo_restante[8];
	

	sprintf(conteudo_botao,"%02d | %02d", EEPROM.read(end_q_horas_ligado_c1), 24-EEPROM.read(end_q_horas_ligado_c1));
	btn_c1.setText(conteudo_botao);

	sprintf(conteudo_botao,"%02d | %02d", EEPROM.read(end_q_horas_ligado_c2), 24-EEPROM.read(end_q_horas_ligado_c2));
	btn_c2.setText(conteudo_botao);		


	
	if(I.ciclo_atual == CICLO_1){
		btn_c1.setValue(1);
		btn_c2.setValue(0);
	}
	else if(I.ciclo_atual == CICLO_2){
		btn_c1.setValue(0);
		btn_c2.setValue(1);
	}
	else{
		btn_c1.setValue(0);
		btn_c2.setValue(0);
	}

	uint16_t minuto_atual = minutoAtual();


	if(I.ciclo_atual == CICLO_NENHUM){
		estado_ciclo_texto.setText("ESCURO");
		progresso.setValue(0);
		sprintf(texto_tempo_restante, "--:--");
	}
	
	else if(I.minuto_de_ligar < I.minuto_de_desligar){

		if(I.estado_atual == LIGADO){
			estado_ciclo_texto.setText("LUZ");
			progresso.setValue( map(minuto_atual, I.minuto_de_ligar, I.minuto_de_desligar, 0, 100) );
			sprintf(texto_tempo_restante, "%02d:%02d",
					(I.minuto_de_desligar - minuto_atual)/60, (I.minuto_de_desligar - minuto_atual)%60 );
		}

		else{
			estado_ciclo_texto.setText("ESCURO");
			if(minuto_atual <= I.minuto_de_ligar){
				progresso.setValue( map(minuto_atual, 0 , I.minuto_de_ligar,  100*(MINUTOS_DIA - I.minuto_de_desligar)/MINUTOS_DIA , 100) );
				sprintf(texto_tempo_restante, "%02d:%02d",
					(I.minuto_de_ligar - minuto_atual)/60, (I.minuto_de_ligar - minuto_atual)%60 );
			}
			else{
				progresso.setValue( map(minuto_atual, I.minuto_de_desligar , MINUTOS_DIA , 0 , 100*(MINUTOS_DIA - I.minuto_de_desligar)/MINUTOS_DIA ) );
				// Serial.println( map(minuto_atual, I.minuto_de_desligar , MINUTOS_DIA , 0 , 100*(MINUTOS_DIA - I.minuto_de_desligar)/MINUTOS_DIA ) );
				sprintf(texto_tempo_restante, "%02d:%02d",
						(I.minuto_de_ligar + MINUTOS_DIA - minuto_atual)/60, (I.minuto_de_ligar + MINUTOS_DIA - minuto_atual)%60 );
			
			}
		}
	}
	else{
		
		if(I.estado_atual == DESLIGADO){
			estado_ciclo_texto.setText("ESCURO");
			progresso.setValue( map(minuto_atual, I.minuto_de_desligar, I.minuto_de_ligar, 0, 100) );
			sprintf(texto_tempo_restante, "%02d:%02d",
					(I.minuto_de_ligar - minuto_atual)/60, (I.minuto_de_ligar - minuto_atual)%60 );
		}

		else{
			estado_ciclo_texto.setText("LUZ");
			if(minuto_atual <= I.minuto_de_ligar){
				progresso.setValue( map(minuto_atual, 0 , I.minuto_de_desligar,  (MINUTOS_DIA - I.minuto_de_ligar)/MINUTOS_DIA , 100) );
				sprintf(texto_tempo_restante, "%02d:%02d",
					(I.minuto_de_desligar - minuto_atual)/60, (I.minuto_de_desligar - minuto_atual)%60 );

			}
			else{
				progresso.setValue( map(minuto_atual, I.minuto_de_ligar , MINUTOS_DIA , 0 , (MINUTOS_DIA - I.minuto_de_ligar)/MINUTOS_DIA ) );
				sprintf(texto_tempo_restante, "%02d:%02d",
						(I.minuto_de_desligar + MINUTOS_DIA - minuto_atual)/60, (I.minuto_de_desligar + MINUTOS_DIA - minuto_atual)%60 );
			}
		}
	}

	if(I.estado_porta == ABERTA){
		estado_ciclo_texto.setText("PAUSA");
	}

	tempo_restante.setText(texto_tempo_restante);
}

void selecionaC1CallBack(void *ptr){
	uint32_t estado_botao_c1;

	btn_c1.getValue(&estado_botao_c1);

	if(estado_botao_c1 == LIGADO){

		if(I.ciclo_atual != CICLO_1){
			btn_c2.setValue(DESLIGADO);
			I.troca_ciclo(CICLO_1);
		}
	}
	else{
		I.troca_ciclo(CICLO_NENHUM);
		
	}
	mostraDadosIluminacao();
}

void selecionaC2CallBack(void *ptr){
	uint32_t estado_botao_c2;

	btn_c2.getValue(&estado_botao_c2);

	if(estado_botao_c2 == LIGADO){

		if(I.ciclo_atual != CICLO_2){
			btn_c1.setValue(DESLIGADO);
			I.troca_ciclo(CICLO_2);
		}
	}
	else{
		I.troca_ciclo(CICLO_NENHUM);
		
	}
	mostraDadosIluminacao();
}

void setarCiclo1Callback(void *ptr){
	botaoApertado = CICLO_1;
	teclado.show();
	//colocar pra levar o numero do botão para a tela do teclado
	PassaBotaoParaTela(I.q_horas_ligado_c1);
}
void setarCiclo2Callback(void *ptr){
	botaoApertado = CICLO_2;
	teclado.show();
	PassaBotaoParaTela(I.q_horas_ligado_c2);
}

void iconeIluminacaoCallback(void *ptr){
	

	PAGINA = PAGINA_ILUMINACAO;
	iluminacao.show();
	mostraDadosIluminacao();
}

void voltarIluminacaoCallBack(void *ptr){
	PAGINA = PAGINA_MENU;
	menu.show();
}

// ###########################################################################################################

#endif