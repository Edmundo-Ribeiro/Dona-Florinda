#ifndef agendar_h
#define agendar_h
#include "teclado.h"
#include "iluminacao.h"



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

void degubEstadoVariveis();
void mostraDadosAgendamento();

class SAI{			
		public:
			
			uint8_t dia_agendado,
					mes_agendado,
					hora_agendada,
					minuto_agendado,
					ciclo_agendado;
				    
			uint16_t ano_agendado;

			unsigned long agendamentoMillis = 0;
			unsigned long agendamentoIntervalo = 40000;

			SAI(){
				this->dia_agendado = EEPROM.read(end_dia_agendado);
				this->mes_agendado = EEPROM.read(end_mes_agendado);
				this->ano_agendado = EEPROM.get(end_ano_agendado, this->ano_agendado);
				this->hora_agendada = EEPROM.read(end_hora_agendado);
				this->minuto_agendado = EEPROM.read(end_minuto_agendado);
				this->ciclo_agendado = EEPROM.read(end_ciclo_agendado);
			}

			void seleciona_ciclo(uint8_t ciclo){
				this->ciclo_agendado = ciclo;
				EEPROM.update(end_ciclo_agendado, ciclo);
				mostraDadosAgendamento();
			}
			void setDia(uint8_t valor){
				this->dia_agendado = valor;
				EEPROM.update(end_dia_agendado,valor);
			}
			void setMes(uint8_t valor){
				this->mes_agendado = valor;
				EEPROM.update(end_mes_agendado,valor);
			}
			void setAno(uint16_t valor){
				this->ano_agendado = valor;
				EEPROM.put(end_ano_agendado,valor);
			}
			void setHora(uint8_t valor){
				this->hora_agendada = valor;
				EEPROM.update(end_hora_agendado,valor);
			}
			void setMinuto(uint8_t valor){
				this->minuto_agendado = valor;
				EEPROM.update(end_minuto_agendado,valor);
			}
			void run(){


				//pega o millisegundo atual
				unsigned long atual = millis();
				//olha se passou o intervalo 
				if(atual - this->agendamentoMillis >= this->agendamentoIntervalo){
					
					if(this->ciclo_agendado != CICLO_NENHUM){

						uint8_t date,mon,hour,min;
						uint16_t year;
						

							date = rtc.getTime().date; 
							mon = rtc.getTime().mon; 	 						   
	    					hour = rtc.getTime().hour; 
						    min = rtc.getTime().min; 
						    year = rtc.getTime().year;

						if(date == this->dia_agendado && 
							mon == this->mes_agendado &&
	    					year == this->ano_agendado &&
						    hour == this->hora_agendada &&
						    min == this->minuto_agendado){

												
							I.troca_ciclo(CICLO_NENHUM);//só pra ter certeza que estava desligado e desconsiderar oq foi feito antes
							I.troca_ciclo(this->ciclo_agendado);
							this->ciclo_agendado = CICLO_NENHUM;

							EEPROM.update(end_ciclo_agendado, this->ciclo_agendado);
						}

						if(date > this->dia_agendado || 
							mon > this->mes_agendado ||
	    					year > this->ano_agendado){
							this->ciclo_agendado = CICLO_NENHUM;

						}
					}
				


					if(PAGINA == PAGINA_AGENDAMENTO){
						mostraDadosAgendamento();
						degubEstadoVariveis();
					}

					//reset o contador do intervalo
					this->agendamentoMillis = atual;

				}	
				
			}
};

SAI A;



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
	char buffer[5];
		itoa(A.dia_agendado, buffer,10);
		agendarDia.setText(buffer);
		itoa(A.mes_agendado, buffer,10);
		agendarMes.setText(buffer);
		itoa(A.ano_agendado, buffer,10);
		agendarAno.setText(buffer);
		itoa(A.hora_agendada, buffer,10);
		agendarHora.setText(buffer);
		itoa(A.minuto_agendado, buffer,10);
		agendarMinuto.setText(buffer);

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

#endif