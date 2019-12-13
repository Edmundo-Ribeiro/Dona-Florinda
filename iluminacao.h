#ifndef iluminacao_h
#define iluminacao_h
#include "teclado.h"


/*TO DO:
[X] mudar set e get minutos paddos para load e save minutos passados
[] ao mudar relogio interno ajustar valores das variaveis minuto de ligar e minuto de desligar
[X] agendar hórario
[X] alterar endereço de minutos passado a cada 2 anos
[X] modo pausa
[X] ligar fita de led
[] testar mudanca de ano
[] bug, se o sistema estava desligado quando ele liga ele não atualiz qual o minuto de ligar/desligar
// ou seja, o tempo que deveria ficar ligado passa mas ele não desliga pois o minuto de desligar salvo já havia passado
//quando estava desligado
[] classe datahora
*/ 


#define MINUTOS_DIA 1440


//não presisaria fazer isso se tivesse um .h e .cpp separados para parte grafica e a classe SI
void mostraDadosIluminacao();
uint16_t minutoAtual();
void degubEstadoVariaveis();

// ###########################################################################################################
// ########################## VARIAVEIS PARA PAGINA DE DEFINIÇÂO ILUMINAÇÃO ##################################
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
	
	NexButton icone_ilumincao = NexButton(PAGINA_MENU,4,"iluminacao");
	
	class SI{			
		public:
			bool estado_atual,
				 door = CLOSED,
				 flag_resumed = RESUMED;

			uint8_t q_horas_ligado_c1,
				    q_horas_ligado_c2,
				    ciclo_atual;
				    
			uint16_t minuto_de_desligar,
				    minuto_de_ligar,
				    minutos_passados,
				    ultimo_minuto_registrado;


			const bool DESLIGADO = 0; //Acho que pode mudar isso para um #define
			const bool LIGADO = 1;
			const uint8_t CICLO_1 = 1;
			const uint8_t CICLO_2 = 2;
			const uint8_t CICLO_NENHUM = 0;
			uint8_t pino_painel, pino_led;
			unsigned long iluminacaoMillis = 0;
			unsigned long iluminacaoIntervalo = 10000;

			
			SI(){
				uint16_t q_minutos_desligado;

				this->estado_atual = EEPROM.read(end_estado_atual);
				this->ciclo_atual = EEPROM.read(end_ciclo_atual);
				
				this->q_horas_ligado_c1 = EEPROM.read(end_q_horas_ligado_c1);
				this->q_horas_ligado_c2 = EEPROM.read(end_q_horas_ligado_c2);

				EEPROM.get(end_minuto_de_ligar, this->minuto_de_ligar);//ler da eeprom
				//this->minuto_de_desligar = MINUTOS_DIA - this->minuto_de_ligar; 
				if(this->ciclo_atual == this->CICLO_1){
					q_minutos_desligado = (24 - this->q_horas_ligado_c1)*60;
				}
				else if(this->ciclo_atual == this->CICLO_2){
					q_minutos_desligado = (24 - this->q_horas_ligado_c2)*60;
				}
				else{
					q_minutos_desligado = MINUTOS_DIA;
				}
				this->minuto_de_desligar = calcularMinutoDeLigarDesligar(
												this->minuto_de_ligar,
												q_minutos_desligado);	

				//da pra descobrir com isso quanto tempo ficou fora da energia???
				this->ultimo_minuto_registrado =0;// minutoAtual();
				this->loadMinutosPassados();


				this->definePinos(PINO_RELE_PAINEL,PINO_RELE_LED);
			}

			void desligarPainel(){
				Serial.println("\nDesligando...\n");
				digitalWrite(this->pino_painel, LOW);
			}
			void ligarPainel(){
				Serial.println("\nLigando...\n");
				digitalWrite(this->pino_painel, HIGH);
			}

			void desligarLED(){
				Serial.println("\nDesligando LED...\n");
				digitalWrite(this->pino_led, LOW);
			}

			void ligarLED(){
				Serial.println("\nLigando LED...\n");
				digitalWrite(this->pino_led, HIGH);
			}

			void desligar(){
				this->desligarPainel();
				this->minutos_passados = 0;
				this->estado_atual = this->DESLIGADO;
				EEPROM.update(end_estado_atual,this->estado_atual);
				this->saveMinutosPassados();
			}
			void ligar(){
				this->ligarPainel();
				this->minutos_passados = 0;
				this->estado_atual = this->LIGADO;
				EEPROM.update(end_estado_atual,this->estado_atual);
				this->saveMinutosPassados();
			}

			void pause(){
				this->desligarPainel();
				this->ligarLED();
				this->door = OPEN;
				this->flag_resumed = NOTRESUMED;
			}

			void resume(){
				if(this->flag_resumed == NOTRESUMED){
					this->troca_ciclo(this->ciclo_atual);
					this->desligarLED();
					this->door = CLOSED;
					this->flag_resumed = RESUMED;
				}
			}


			uint16_t calcularMinutoDeLigarDesligar(uint16_t inicio, uint16_t intervalo){
				uint16_t variavel = inicio + intervalo;
				/*se o resultado da soma for maior que a quantidade de minutos em um dia
				  deve-se pegar o resto da divisão inteira*/
				variavel = variavel <= MINUTOS_DIA ? 
										   variavel : 
										   variavel % MINUTOS_DIA;

				return variavel;
			}

			///////////////////////////////////////////////////////////////////////////////////
			//Minutos Passados é dessa forma pois tem que ser atualizado na memoria varias vezes
			//Então a cada dois anos seu endereço de leitura e escrita muda!
			//Código que realia essa mudança está em agendamento.h

			uint16_t loadMinutosPassados(){
				uint16_t end_minutos_passados;
				EEPROM.get(end_endereco_minutos_passados, end_minutos_passados);
				EEPROM.get(end_minutos_passados, this->minutos_passados);
				
				return this->minutos_passados;
			}

			void saveMinutosPassados(){
				uint16_t end_minutos_passados;
				EEPROM.get(end_endereco_minutos_passados, end_minutos_passados);
				EEPROM.put(end_minutos_passados,this->minutos_passados);
			}


			void definePinos(uint8_t painel, uint8_t led){
			 	this->pino_painel = painel;
			 	this->pino_led = led;
				pinMode(this->pino_painel, OUTPUT);
				pinMode(this->pino_led, OUTPUT);
			}

			void troca_ciclo(uint8_t ciclo_destino){
				uint8_t q_horas_ligado, 
						q_horas_desligado,
						ciclo_origem;

				uint16_t minuto_atual,
						 minutos_restantes;
					
				ciclo_origem = this->ciclo_atual;

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
				

				if(ciclo_origem != this->CICLO_NENHUM){
					if(ciclo_destino != this->CICLO_NENHUM) {
						if(this->estado_atual == this->LIGADO){

							minutos_restantes = q_horas_desligado*60 - this->minutos_passados;

							if(minutos_restantes > 0){
								//manter ligado então
								this->minuto_de_desligar = calcularMinutoDeLigarDesligar(
															minuto_atual,
															minutos_restantes);
								this->minuto_de_ligar = calcularMinutoDeLigarDesligar(
															this->minuto_de_desligar,
															q_horas_desligado*60);
							}
							else{
								//já era pra ter desligado!
								this->minuto_de_desligar = minuto_atual;
								this->minuto_de_ligar = calcularMinutoDeLigarDesligar(
														 minuto_atual,
														 q_horas_desligado*60);
								this->desligar();
							}
						}
						else{

							minutos_restantes = q_horas_ligado*60 - this->minutos_passados;

							if(minutos_restantes > 0){
								//manter desligado então
								this->minuto_de_ligar = calcularMinutoDeLigarDesligar(
															minuto_atual,
															minutos_restantes);

								this->minuto_de_desligar = calcularMinutoDeLigarDesligar(
															this->minuto_de_ligar,
															q_horas_ligado*60);
							}
							else{
								//já era pra ter ligado!
								this->minuto_de_ligar = minuto_atual;
								this->minuto_de_desligar = calcularMinutoDeLigarDesligar(
														 minuto_atual,
														 q_horas_ligado*60);
								this->ligar();
							}
						}

					}
					else{
						this->desligar();
						this->minuto_de_ligar = MINUTOS_DIA + 10; //(valor que não é possivel)
						this->minuto_de_desligar = 0;
						EEPROM.put(end_minuto_de_ligar, this->minuto_de_ligar);
					}
				}
				else{

					if(ciclo_destino != this->CICLO_NENHUM){
						this->minuto_de_ligar = minuto_atual;
						this->minuto_de_desligar = calcularMinutoDeLigarDesligar(
													 minuto_atual,
													 q_horas_ligado*60);
						this->ligar();

					}
				}	


				EEPROM.put(end_minuto_de_ligar, this->minuto_de_ligar);
				EEPROM.update(end_ciclo_atual, this->ciclo_atual);
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

			void run(bool door_state){

				//pega o millisegundo atual
				if(door_state == OPEN){
					this->pause();
				}
				else{
					this->resume();
				}

				unsigned long atual = millis();
				//olha se passou o intervalo 
				if(atual - this->iluminacaoMillis >= this->iluminacaoIntervalo){

					


					uint16_t minuto_atual;
					//se estiver na pagina de iluminação atualiza os dados
					minuto_atual = minutoAtual();
					//verifica se passou "1" minuto
					if(minuto_atual != this->ultimo_minuto_registrado){
						if(this->ciclo_atual != this->CICLO_NENHUM){
							this->minutos_passados += 1;
							//se passou uma hora salvar a quantidade de minutos passados
							if(this->minutos_passados % 15 == 0 ){
								this->saveMinutosPassados();
								// EEPROM.put(end_minutos_passados,this->minutos_passados);
								// // EEPROM.put(end_minuto_de_ligar, this->minuto_de_ligar);
							}

						}
						//registra qual foi o ultimo minuto
						this->ultimo_minuto_registrado = minuto_atual;
					}


					if(this->minuto_de_ligar < minuto_de_desligar){

						if(this->estado_atual == this->LIGADO &&
							!(minuto_atual >= this->minuto_de_ligar 
							  && minuto_atual <= this->minuto_de_desligar)
							){
							this->desligar();
						}
						else if(this->estado_atual == this->DESLIGADO &&
							 (minuto_atual >= this->minuto_de_ligar 
							  && minuto_atual <= this->minuto_de_desligar)){
							this->ligar();
						}
					}
					else{
						if(this->estado_atual == this->LIGADO &&
							(minuto_atual >= this->minuto_de_desligar 
							  && minuto_atual <= this->minuto_de_ligar)){

							this->desligar();
						}
						else if(this->estado_atual == this->DESLIGADO &&
							!(minuto_atual >= this->minuto_de_desligar 
							  && minuto_atual <= this->minuto_de_ligar)){

							this->ligar();
						}
					}
					
					//verifica se deve desligar
					if(this->estado_atual == this->LIGADO && 
					   minuto_atual == this->minuto_de_desligar &&
					   this>door == CLOSED){
						this->desligar();


					}
					//verifica se deve ligar
					if(this->estado_atual == this->DESLIGADO && 
						minuto_atual == this->minuto_de_ligar &&
						this->door == CLOSED){
						this->ligar();
					}

					if(PAGINA == PAGINA_ILUMINACAO){
						mostraDadosIluminacao();
					}
					//reset o contador do intervalo
					this->iluminacaoMillis = atual;
					
					degubEstadoVariaveis();
				
				}
			}
	};

	SI I; //Desclaração do objeto sistema de iluminação
// ###########################################################################################################


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
		Serial.print("minutos_passados: ");
		Serial.println(I.minutos_passados);
		
		Serial.print("ultimo_minuto_registrado: ");
		Serial.println(I.ultimo_minuto_registrado);
		
		Serial.print("minuto_atual: ");
		Serial.println(minutoAtual());

	}

	uint16_t minutoAtual(){
		return rtc.getTime().hour*60 + rtc.getTime().min;
	}

	void mostraDadosIluminacao(){	

		char conteudo_botao[8], 
			 texto_tempo_restante[8];
		uint16_t minutos_ligado,minutos_desligado;

		if(I.ciclo_atual == I.CICLO_1){
			minutos_ligado = I.q_horas_ligado_c1*60;
		} 

		else if (I.ciclo_atual == I.CICLO_2){
			minutos_ligado = I.q_horas_ligado_c2*60;
		}
		else{
			minutos_ligado = 0;
		}
		minutos_desligado = MINUTOS_DIA - minutos_ligado;

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

		if(I.estado_atual == I.LIGADO){
			estado_ciclo_texto.setText("LUZ");
			progresso.setValue(map(I.minutos_passados, 0, minutos_ligado, 0, 100));
			sprintf(texto_tempo_restante,
					"%02d:%02d",
					 (minutos_ligado - I.minutos_passados)/60, (minutos_ligado - I.minutos_passados)%60);
		}
		else{
			estado_ciclo_texto.setText("ESCURO");
			progresso.setValue(map(I.minutos_passados, 0, minutos_desligado ,0 , 100));
			sprintf(texto_tempo_restante,
					"%02d:%02d",
					 (minutos_desligado - I.minutos_passados)/60, (minutos_desligado - I.minutos_passados)%60);
		}
		
		tempo_restante.setText(texto_tempo_restante);

	}

	void selecionaC1CallBack(void *ptr){
		uint32_t estado_botao_c1;

		btn_c1.getValue(&estado_botao_c1);

		if(estado_botao_c1 == I.LIGADO){

			if(I.ciclo_atual != I.CICLO_1){
				btn_c2.setValue(I.DESLIGADO);
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

		if(estado_botao_c2 == I.LIGADO){

			if(I.ciclo_atual != I.CICLO_2){
				btn_c1.setValue(I.DESLIGADO);
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
		PassaBotaoParaTela(I.q_horas_ligado_c1);
		//colocar pra levar o numero do botão para a tela do teclado
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