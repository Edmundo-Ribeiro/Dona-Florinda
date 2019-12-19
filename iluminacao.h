#ifndef iluminacao_h
#define iluminacao_h
#include "teclado.h"

//TO DO
//na parte de mostrar os dados ainda não estão considerando quando está em ciclo nenhum

//não presisaria fazer isso se tivesse um .h e .cpp separados para parte grafica e a classe SI
void mostraDadosIluminacao();
uint16_t minutoAtual();
void degubEstadoVariaveis();

// ###########################################################################################################
// ########################## VARIAVEIS PARA PAGINA DE ILUMINAÇÃO ############################################
// ###########################################################################################################
	NexPage iluminacao = NexPage(PAGINA_ILUMINACAO,0,"Iluminacao");
	
	
	NexText voltar_iluminacao = NexText(PAGINA_ILUMINACAO, 1, "voltar");
	NexDSButton btn_c2 = NexDSButton(PAGINA_ILUMINACAO, 2, "btn_c2");
	NexDSButton btn_c1 = NexDSButton(PAGINA_ILUMINACAO, 3, "btn_c1");
	NexText estado_ciclo_texto = NexText(PAGINA_ILUMINACAO, 4, "estado_ciclo");
	NexButton btn_setar_c1 = NexButton(PAGINA_ILUMINACAO, 5, "setar_c1");
	NexButton btn_setar_c2 = NexButton(PAGINA_ILUMINACAO, 6, "setar_c2");
	
	// NexButton btn_soma = NexButton(PAGINA_ILUMINACAO, 9, "soma");
	// NexButton btn_subtrai = NexButton(PAGINA_ILUMINACAO, 10, "subtrai");
	NexProgressBar progresso = NexProgressBar(PAGINA_ILUMINACAO,8,"progresso");
	NexText tempo_restante = NexText(PAGINA_ILUMINACAO, 10, "tempo_restante");
	
	NexButton icone_ilumincao = NexButton(PAGINA_MENU,3,"iluminacao");	
// ###########################################################################################################

class SI{			
	public:
		bool estado_atual;

		uint8_t q_horas_ligado_c1, // onde fica armazenado quantas horas o ciclo 1 tem que ficar ligado (mudar pra minutos?)
			    q_horas_ligado_c2, // onde fica armazenado quantas horas o ciclo 2 tem que ficar ligado
			    ciclo_atual;   // onde fica armazenado qual do ciclo está sendo executado (ciclo1, ciclo 2, ciclo_nenhum)
		
		uint16_t minuto_de_desligar, // onde fica armazenado o minuto do dia em que deve desligar e ligar
			     minuto_de_ligar;


		const uint8_t CICLO_NENHUM = 0;
		const uint8_t CICLO_1 = 1;
		const uint8_t CICLO_2 = 2;

		unsigned long iluminacaoMillis = 0;
		unsigned long iluminacaoIntervalo = 10000;

		SI(){

			pinMode(PINO_PAINEL, OUTPUT);
			pinMode(PINO_LED, OUTPUT);

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
			if(this->ciclo_atual == this->CICLO_1){
				q_minutos_ligado = this->q_horas_ligado_c1*60;
			}
			else if(this->ciclo_atual == this->CICLO_2){
				q_minutos_ligado = this->q_horas_ligado_c2*60;
			}
			
			this->minuto_de_desligar = calcularMinuto(this->minuto_de_ligar, q_minutos_ligado);	

			if(this->ciclo_atual == this->CICLO_NENHUM){
				this->minuto_de_ligar = MINUTOS_DIA; 
				this->minuto_de_desligar = 0;
			}

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
			Serial.println("\nDesligando Painel...\n");
			digitalWrite(PINO_PAINEL, LOW);
		}

		void ligarPainel(){
			Serial.println("\nLigando Painel...\n");
			digitalWrite(PINO_PAINEL, HIGH);
		}

		void desligarLED(){
			Serial.println("\nDesligando LED...\n");
			digitalWrite(PINO_LED, LOW);
		}

		void ligarLED(){
			Serial.println("\nLigando LED...\n");
			digitalWrite(PINO_LED, HIGH);
		}

		void desligar(){
			this->desligarPainel();
			this->estado_atual = DESLIGADO;
			EEPROM.update(end_estado_atual,this->estado_atual);
		}
		void ligar(){
			this->ligarPainel();
			this->estado_atual = LIGADO;
			EEPROM.update(end_estado_atual,this->estado_atual);
		}

		void pausa(){
			this->desligarPainel();
			this->ligarLED();
		}

		void retomar(){
			this->desligarLED();
			if(this->estado_atual == LIGADO)
				this->ligarPainel();
			else
				this->desligarPainel();
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
			if(this->estado_atual == LIGADO && !esta_no_lado_ciclo_ligado){
				//desligue
				this->desligar();
			}
			else if(this->estado_atual == DESLIGADO && esta_no_lado_ciclo_ligado){
				this->ligar();
			}
		}
		void run(bool estado_porta){
			/*
			se a porta está aberta pausar o sistema
			ou seja, ligar fita de led, deligar painel, 
			parar de contar minutos passados, 
			verificar se deve ligar ou não o painel
			*/
			if(estado_porta == ABERTA){
				this->pausa();
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
				unsigned long atual = millis();
				//olha se passou o intervalo 
				if(atual - this->iluminacaoMillis >= this->iluminacaoIntervalo){
					
					this->verificacoes();

					//resetar o contador do intervalo
					this->iluminacaoMillis = atual;

					//se estiver na pagina de iluminação atualiza os dados
					if(PAGINA == PAGINA_ILUMINACAO){
						mostraDadosIluminacao();
					}
					degubEstadoVariaveis();
				}
			}
		}

		void setar(uint8_t ciclo, uint8_t valor){
			if(ciclo == this->CICLO_1){
				this->q_horas_ligado_c1 = valor;
				EEPROM.update(end_q_horas_ligado_c1,valor);
			}
			else if(ciclo == this->CICLO_2){
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
			if(ciclo_destino == this->CICLO_1){
				q_horas_ligado = this->q_horas_ligado_c1;
				this->ciclo_atual = this->CICLO_1;
			}
			else if(ciclo_destino == this->CICLO_2){
				q_horas_ligado = this->q_horas_ligado_c2;
				this->ciclo_atual = this->CICLO_2;
					
			}
			else{
				q_horas_ligado = 0;
				this->ciclo_atual = this->CICLO_NENHUM;
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
			else if(ciclo_destino == this->CICLO_NENHUM){
				this->minuto_de_ligar = MINUTOS_DIA; 
				this->minuto_de_desligar = 0;
			}

			else{
				this->minuto_de_ligar = minuto_atual;
				this->minuto_de_desligar = calcularMinuto(minuto_atual, q_horas_ligado*60);
			}

			EEPROM.put(end_minuto_de_ligar, this->minuto_de_ligar);

			EEPROM.update(end_ciclo_atual, this->ciclo_atual);
			this->verificacoes();
			mostraDadosIluminacao();
		}

		//metodo que será chamado quando o sistema desligar
		//por enquanto só estou salvando o estado atual
		void salvar(){
			EEPROM.update(end_estado_atual,this->estado_atual);
		}


	
};

SI I; //Desclaração do objeto sistema de iluminação



// ###########################################################################################################
// #####################CALLBACKS E FUNÇÔES PARA PAGINA DE DEFINIÇÂO ILUMINAÇÃO ##############################
// ###########################################################################################################
	
	void degubEstadoVariaveis(){
		Serial.print("Estado Atual: ");
		Serial.println(I.estado_atual);

		Serial.print("Ciclo Atual: ");
		Serial.println(I.ciclo_atual);

				
		Serial.print("q_horas_ligado_c1: ");
		Serial.println(I.q_horas_ligado_c1);
		Serial.print("q_horas_ligado_c2: ");
		Serial.println(I.q_horas_ligado_c2);

		Serial.print("minuto_de_desligar: ");
		Serial.println(I.minuto_de_desligar);
		Serial.print("minuto_de_ligar: ");
		Serial.println(I.minuto_de_ligar);
		
		Serial.print("minuto_atual: ");
		Serial.println(minutoAtual());

	}

	uint16_t minutoAtual(){
		return rtc.getTime().hour*60 + rtc.getTime().min;
	}

	void mostraDadosIluminacao(){	

		char conteudo_botao[8], 
			 texto_tempo_restante[8];
		

		sprintf(conteudo_botao,"%02d | %02d", EEPROM.read(end_q_horas_ligado_c1), 24-EEPROM.read(end_q_horas_ligado_c1));
		btn_c1.setText(conteudo_botao);

		sprintf(conteudo_botao,"%02d | %02d", EEPROM.read(end_q_horas_ligado_c2), 24-EEPROM.read(end_q_horas_ligado_c2));
		btn_c2.setText(conteudo_botao);		


		
		if(I.ciclo_atual == I.CICLO_1){
			btn_c1.setValue(1);
			btn_c2.setValue(0);
		}
		else if(I.ciclo_atual == I.CICLO_2){
			btn_c1.setValue(0);
			btn_c2.setValue(1);
		}
		else{
			btn_c1.setValue(0);
			btn_c2.setValue(0);
		}


		uint16_t minuto_atual = minutoAtual();

		// if(I.estado_atual == LIGADO){
		// 	estado_ciclo_texto.setText("LUZ");
		// 	progresso.setValue(map(minuto_atual, I.minuto_de_ligar, I.minuto_de_desligar, 0, 100));

		// 	sprintf(texto_tempo_restante,
		// 			"%02d:%02d",
		// 			 (abs(I.minuto_de_desligar - minuto_atual))/60, (I.minuto_de_desligar - minuto_atual)%60);
		// }

		// else if(I.ciclo_atual == I.CICLO_NENHUM){
		// 	estado_ciclo_texto.setText("ESCURO");
		// 	progresso.setValue(0);
		// 	sprintf(texto_tempo_restante, "--:--");
		// }

		// else{
		// 	estado_ciclo_texto.setText("ESCURO");
		// 	progresso.setValue(map(minuto_atual, I.minuto_de_desligar, I.minuto_de_ligar, 0, 100));
		// 	sprintf(texto_tempo_restante,
		// 			"%02d:%02d",
		// 			 (abs((int)I.minuto_de_ligar - (int)minuto_atual))/60, (I.minuto_de_ligar - minuto_atual)%60);
		// }

		//tempo_restante.setText(texto_tempo_restante);

		//definição do valor da progress bar
		if(I.minuto_de_ligar < I.minuto_de_desligar){

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
					Serial.println( map(minuto_atual, I.minuto_de_desligar , MINUTOS_DIA , 0 , 100*(MINUTOS_DIA - I.minuto_de_desligar)/MINUTOS_DIA ) );
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


		tempo_restante.setText(texto_tempo_restante);
	}

	void selecionaC1CallBack(void *ptr){
		uint32_t estado_botao_c1;

		btn_c1.getValue(&estado_botao_c1);

		if(estado_botao_c1 == LIGADO){

			if(I.ciclo_atual != I.CICLO_1){
				btn_c2.setValue(DESLIGADO);
				I.troca_ciclo(I.CICLO_1);
			}
		}
		else{
			I.troca_ciclo(I.CICLO_NENHUM);
			
		}
		mostraDadosIluminacao();
	}

	void selecionaC2CallBack(void *ptr){
		uint32_t estado_botao_c2;

		btn_c2.getValue(&estado_botao_c2);

		if(estado_botao_c2 == LIGADO){

			if(I.ciclo_atual != I.CICLO_2){
				btn_c1.setValue(DESLIGADO);
				I.troca_ciclo(I.CICLO_2);
			}
		}
		else{
			I.troca_ciclo(I.CICLO_NENHUM);
			
		}
		mostraDadosIluminacao();
	}

	void setarCiclo1Callback(void *ptr){
		botaoAbertado = I.CICLO_1;
		teclado.show();
		//colocar pra levar o numero do botão para a tela do teclado
		PassaBotaoParaTela(I.q_horas_ligado_c1);
	}
	void setarCiclo2Callback(void *ptr){
		botaoAbertado = I.CICLO_2;
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