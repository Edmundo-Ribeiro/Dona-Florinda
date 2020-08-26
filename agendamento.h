#ifndef agendar_h
#define agendar_h
#include "teclado.h"
#include "iluminacao.h"



void degubEstadoVariveis();
void mostraDadosAgendamento();

// ###########################################################################################################
// ######################## DECLARAÇÃO DO OBJETO E LÓGICA DE FUNCIONAMENTO ###################################
// ###########################################################################################################

class SAI{			
		public:		
			uint8_t ciclo_agendado;
			DateTime data_agendada;
				    
			unsigned long agendamentoMillis = 0;
			unsigned long agendamentoIntervalo = 40000;

			SAI(){
				uint16_t aux_ano;
				EEPROM.get(end_ano_agendado, aux_ano);

				this->data_agendada = DateTime(
					aux_ano,
					EEPROM.read(end_mes_agendado),
					EEPROM.read(end_dia_agendado),
					EEPROM.read(end_hora_agendado),
					EEPROM.read(end_minuto_agendado)
				);

				this->ciclo_agendado = EEPROM.read(end_ciclo_agendado);
			}

			void seleciona_ciclo(uint8_t ciclo){
				this->ciclo_agendado = ciclo;
				EEPROM.update(end_ciclo_agendado, ciclo);
				mostraDadosAgendamento();
			}
			void setDia(uint8_t valor){
				this->data_agendada = DateTime(
					this->data_agendada.year(),
					this->data_agendada.month(),
					valor,
					this->data_agendada.hour(),
					this->data_agendada.minute()
				);
				EEPROM.update(end_dia_agendado,valor);
			}
			void setMes(uint8_t valor){
				this->data_agendada = DateTime(
					this->data_agendada.year(),
					valor,
					this->data_agendada.day(),
					this->data_agendada.hour(),
					this->data_agendada.minute()
				);
				EEPROM.update(end_mes_agendado,valor);
			}
			void setAno(uint16_t valor){
				this->data_agendada = DateTime(
					valor,
					this->data_agendada.month(),
					this->data_agendada.day(),
					this->data_agendada.hour(),
					this->data_agendada.minute()
				);
				EEPROM.put(end_ano_agendado,valor);
			}
			void setHora(uint8_t valor){
				this->data_agendada = DateTime(
					this->data_agendada.year(),
					this->data_agendada.month(),
					this->data_agendada.day(),
					valor,
					this->data_agendada.minute()
				);
				EEPROM.update(end_hora_agendado,valor);
			}
			void setMinuto(uint8_t valor){
				this->data_agendada = DateTime(
					this->data_agendada.year(),
					this->data_agendada.month(),
					this->data_agendada.day(),
					this->data_agendada.hour(),
					valor
				);
				EEPROM.update(end_minuto_agendado,valor);
			}
			void run(SI& iluminacao){


				//pega o millisegundo atual
				// unsigned long atual = millis();
				
				//olha se passou o intervalo 
				if(millis() - this->agendamentoMillis >= this->agendamentoIntervalo){
					
					if(this->ciclo_agendado != CICLO_NENHUM){
						DateTime now = rtc.now();
		

						if (now >= this->data_agendada + TimeSpan(0,0,30,0)){
							this->ciclo_agendado = CICLO_NENHUM;
						}


						else if(now >= this->data_agendada){
							iluminacao.troca_ciclo(CICLO_NENHUM);//só pra ter certeza que estava desligado e desconsiderar oq foi feito antes
							iluminacao.troca_ciclo(this->ciclo_agendado);
							this->ciclo_agendado = CICLO_NENHUM;

						}

						EEPROM.update(end_ciclo_agendado, this->ciclo_agendado);

					}

					if(PAGINA == PAGINA_AGENDAMENTO){
						mostraDadosAgendamento();
						degubEstadoVariveis();
					}	
				

					//reset o contador do intervalo
					this->agendamentoMillis = millis();

				}

				
			}
};

// ###########################################################################################################

SAI A; //Intanciando o objeto

// ###########################################################################################################
// ################### VARIAVEIS e FUNCIONALIDADES PARA PAGINA DE AGENDAMENTO ################################
// ###########################################################################################################


NexPage agendamento = NexPage(PAGINA_AGENDAMENTO,0,"Agendamento");
NexButton agendarHora = NexButton(PAGINA_AGENDAMENTO, 5, "hora");
NexButton agendarMinuto = NexButton(PAGINA_AGENDAMENTO, 6, "minuto");
NexButton agendarDia = NexButton(PAGINA_AGENDAMENTO, 2, "dia");
NexButton agendarMes = NexButton(PAGINA_AGENDAMENTO, 3, "mes");
NexButton agendarAno = NexButton(PAGINA_AGENDAMENTO, 4, "ano");
NexButton voltar_agendar = NexButton(PAGINA_AGENDAMENTO, 1, "voltar");
NexDSButton ciclo1_agendar = NexDSButton(PAGINA_AGENDAMENTO, 7, "ciclo1");
NexDSButton ciclo2_agendar = NexDSButton(PAGINA_AGENDAMENTO, 8, "ciclo2");

NexButton agendar = NexButton(PAGINA_ILUMINACAO, 7, "agendar");


void degubEstadoVariveis(){

		dbSerialPrint("ano_agendado");
		dbSerialPrintln(A.ano_agendado);
		dbSerialPrint("dia_agendado");
		dbSerialPrintln(A.dia_agendado);
		dbSerialPrint("mes_agendado");
		dbSerialPrintln(A.mes_agendado);
		dbSerialPrint("hora_agendado");
		dbSerialPrintln(A.hora_agendada);
		dbSerialPrint("minuto_agendado");
		dbSerialPrintln(A.minuto_agendado);
		dbSerialPrint("ciclo_agendado");
		dbSerialPrintln(A.ciclo_agendado);
		
}

void mostraDadosAgendamento(){	
	char DD[]="DD",
		 MM[]="MM",
		 YYYY[]="YYYY",
		 hh[]="hh",
		 mm[]="mm";


		agendarDia.setText(A.data_agendada.toString(DD));

		agendarMes.setText(A.data_agendada.toString(MM));

		agendarAno.setText(A.data_agendada.toString(YYYY));
	
		agendarHora.setText(A.data_agendada.toString(hh));

		agendarMinuto.setText(A.data_agendada.toString(mm));

		if(A.ciclo_agendado == CICLO_1){
			ciclo1_agendar.setValue(1);
			ciclo2_agendar.setValue(0);
		}
		else if(A.ciclo_agendado == CICLO_2){
			ciclo1_agendar.setValue(0);
			ciclo2_agendar.setValue(1);
		}
		else{
			ciclo1_agendar.setValue(0);
			ciclo2_agendar.setValue(0);
		}
}
void AgendaHoraPopCallback(void *ptr){
	char bufferValor[5];
	botaoApertado = BTNHORA;
	agendarHora.getText(bufferValor,sizeof(bufferValor));
	PassaTextoParaTela(bufferValor);
}
void AgendaMinutoPopCallback(void *ptr){
	char bufferValor[5];
	botaoApertado = BTNMINUTO;
	agendarMinuto.getText(bufferValor,sizeof(bufferValor));
	PassaTextoParaTela(bufferValor);
}
void AgendaDiaPopCallback(void *ptr){
	char bufferValor[5];
	botaoApertado = BTNDIA;
	agendarDia.getText(bufferValor,sizeof(bufferValor));
	PassaTextoParaTela(bufferValor);
}
void AgendaMesPopCallback(void *ptr){
	char bufferValor[5];
	botaoApertado = BTNMES;
	agendarMes.getText(bufferValor,sizeof(bufferValor));
	PassaTextoParaTela(bufferValor);
}
void AgendaAnoPopCallback(void *ptr){
	char bufferValor[7];
	botaoApertado = BTNANO;
	agendarAno.getText(bufferValor,sizeof(bufferValor));
	PassaTextoParaTela(bufferValor);
}

void VoltarAgendar(void *ptr){
	PAGINA = PAGINA_ILUMINACAO;
	iluminacao.show();
	mostraDadosIluminacao();
}
void SelecionaCiclo1PopCallback(void *ptr){
	uint32_t estado_botao_c1;

	ciclo1_agendar.getValue(&estado_botao_c1);

	if(estado_botao_c1 == LIGADO){
		ciclo2_agendar.setValue(DESLIGADO);
		A.seleciona_ciclo(CICLO_1);
		
	}
	else{
		A.seleciona_ciclo(CICLO_NENHUM);	
	}
}
void SelecionaCiclo2PopCallback(void *ptr){
	uint32_t estado_botao_c2;

	ciclo2_agendar.getValue(&estado_botao_c2);

	if(estado_botao_c2 == LIGADO){	
		ciclo1_agendar.setValue(DESLIGADO);
		A.seleciona_ciclo(CICLO_2);
		
	}
	else{
		A.seleciona_ciclo(CICLO_NENHUM);	
	}
}
void AgendarPopCallBack(void *ptr){
		PAGINA = PAGINA_AGENDAMENTO;
		agendamento.show();
		mostraDadosAgendamento();
}


// ###########################################################################################################

#endif