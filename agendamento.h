#ifndef agendar_h
#define agendar_h
#include "teclado.h"
#include "iluminacao.h"



	NexPage agendamento = NexPage(PAGINA_AGENDAMENTO,0,"Agendamento");
	NexNumber agendarHora = NexNumber(PAGINA_AGENDAMENTO, 1, "hora");
	NexNumber agendarMinuto = NexNumber(PAGINA_AGENDAMENTO, 2, "minuto");
	NexNumber agendarDia = NexNumber(PAGINA_AGENDAMENTO, 3, "dia");
	NexNumber agendarMes = NexNumber(PAGINA_AGENDAMENTO, 4, "mes");
	NexNumber agendarAno = NexNumber(PAGINA_AGENDAMENTO, 5, "ano");
	NexButton voltar_agendar = NexButton(PAGINA_AGENDAMENTO, 6, "voltar");
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
					uint16_t year = rtc.getTime().year;
					degubEstadoVariveis();
					if(this->ciclo_agendado != I.CICLO_NENHUM){

						uint8_t date,mon,hour,min;
						

							date = rtc.getTime().date; 
							mon = rtc.getTime().mon; 	 						   
	    					hour = rtc.getTime().hour; 
						    min = rtc.getTime().min; 


						if(date == this->dia_agendado && 
							mon == this->mes_agendado &&
	    					year == this->ano_agendado &&
						    hour == this->hora_agendada &&
						    min == this->minuto_agendado){

												
							I.troca_ciclo(I.CICLO_NENHUM);//só pra ter certeza que estava desligado e desconsiderar oq foi feito antes
							I.troca_ciclo(this->ciclo_agendado);
							this->ciclo_agendado = I.CICLO_NENHUM;

							EEPROM.update(end_ciclo_agendado, this->ciclo_agendado);
						}

						if(date > this->dia_agendado || 
							mon > this->mes_agendado ||
	    					year > this->ano_agendado){
							this->ciclo_agendado = I.CICLO_NENHUM;

						}
					}
				


					if(PAGINA == PAGINA_AGENDAMENTO){
						mostraDadosAgendamento();
					}

					//reset o contador do intervalo
					this->agendamentoMillis = atual;


					if(year == BASEYEAR + 2){
						EEPROM.put(end_endereco_minutos_passados, 1002);
					}
					else if(year == BASEYEAR + 4){
						EEPROM.put(end_endereco_minutos_passados, 1004);

					}
					else if(year == BASEYEAR + 6){
						EEPROM.put(end_endereco_minutos_passados, 1006);

					}
					else if(year == BASEYEAR + 8){
						EEPROM.put(end_endereco_minutos_passados, 1008);
					}
				}	
				
			}
};

SAI A;



void degubEstadoVariveis(){

		Serial.print("ano_agendado");
		Serial.println(A.ano_agendado);
		Serial.print("dia_agendado");
		Serial.println(A.dia_agendado);
		Serial.print("mes_agendado");
		Serial.println(A.mes_agendado);
		Serial.print("hora_agendado");
		Serial.println(A.hora_agendada);
		Serial.print("minuto_agendado");
		Serial.println(A.minuto_agendado);
		Serial.print("ciclo_agendado");
		Serial.println(A.ciclo_agendado);
		
}

void mostraDadosAgendamento(){	
		dia.setValue(A.dia_agendado);
		mes.setValue(A.mes_agendado);
		ano.setValue(A.ano_agendado);
		hora.setValue(A.hora_agendada);
		minuto.setValue(A.minuto_agendado);

		if(A.ciclo_agendado == I.CICLO_1){
			ciclo1_agendar.setValue(1);
			ciclo2_agendar.setValue(0);
		}
		else if(A.ciclo_agendado == I.CICLO_2){
			ciclo1_agendar.setValue(0);
			ciclo2_agendar.setValue(1);
		}
		else{
			ciclo1_agendar.setValue(0);
			ciclo2_agendar.setValue(0);
		}
}
void AgendaHoraPopCallback(void *ptr){
	uint32_t bufferValor;
	botaoAbertado = BTNHORA;
	hora.getValue(&bufferValor);
	PassaBotaoParaTela(bufferValor);
}
void AgendaMinutoPopCallback(void *ptr){
	uint32_t bufferValor;
	botaoAbertado = BTNMINUTO;
	minuto.getValue(&bufferValor);
	PassaBotaoParaTela(bufferValor);
}
void AgendaDiaPopCallback(void *ptr){
	uint32_t bufferValor;
	botaoAbertado = BTNDIA;
	dia.getValue(&bufferValor);
	PassaBotaoParaTela(bufferValor);
}
void AgendaMesPopCallback(void *ptr){
	uint32_t bufferValor;
	botaoAbertado = BTNMES;
	mes.getValue(&bufferValor);
	PassaBotaoParaTela(bufferValor);
}
void AgendaAnoPopCallback(void *ptr){
	uint32_t bufferValor;
	botaoAbertado = BTNANO;
	ano.getValue(&bufferValor);
	PassaBotaoParaTela(bufferValor);
}
void VoltarAgendar(void *ptr){
	PAGINA = PAGINA_ILUMINACAO;
	iluminacao.show();
	mostraDadosIluminacao();
}
void SelecionaCiclo1PopCallback(void *ptr){
	uint32_t estado_botao_c1;

	ciclo1_agendar.getValue(&estado_botao_c1);

	Serial.println("IF ESTADO1");
	if(estado_botao_c1 == I.LIGADO){

		ciclo2_agendar.setValue(I.DESLIGADO);
		A.seleciona_ciclo(I.CICLO_1);
		
	}
	else{
		A.seleciona_ciclo(I.CICLO_NENHUM);	
	}
}
void SelecionaCiclo2PopCallback(void *ptr){
	uint32_t estado_botao_c2;

	ciclo2_agendar.getValue(&estado_botao_c2);

	if(estado_botao_c2 == I.LIGADO){	
		ciclo1_agendar.setValue(I.DESLIGADO);
		A.seleciona_ciclo(I.CICLO_2);
		
	}
	else{
		A.seleciona_ciclo(I.CICLO_NENHUM);	
	}
}
void AgendarPopCallBack(void *ptr){
		PAGINA = PAGINA_AGENDAMENTO;
		agendamento.show();
		mostraDadosAgendamento();
		Serial.println("OIII");
}

#endif