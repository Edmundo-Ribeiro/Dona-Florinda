#include <Nextion.h>
#include <string.h>
#include <EEPROM.h>
#include <DS3231.h>
#include "definicoes.h"
#include "datahora.h"
#include "iluminacao.h"
#include "teclado.h"
#include "agendamento.h"
#include "gascarbonico.h"
#include "trava.h"
#include "tempumi.h"
#include "Irrigacao.h"
#include "exaustao.h"



void ConfirmaPopCallback(void *ptr){
	uint16_t valor;
	uint8_t pagina_anterior = PAGINA;
	char buffer[5];
	entrada.getText(buffer,sizeof(buffer));
	valor = (uint16_t)atoi(buffer);	
	// PAGINA = PAGINA_TECLADO;
	switch(pagina_anterior){

		case PAGINA_ILUMINACAO:
			if(valor > 24 || valor < 0)
				mensagem.setText("Valor invalido");
			else{
				I.setar(botaoApertado, valor);
				
				PAGINA = PAGINA_ILUMINACAO;
				iluminacao.show();
				mostraDadosIluminacao();
			}
			break;
			
		case PAGINA_DATA_HORA:
			switch(botaoApertado){
				case BTNHORA:
					if(valor > 24 || valor < 0){
						mensagem.setText("Valor invalido");
						//teclado.show();
					}
					else{
						setHora(rtc,valor);
						PAGINA = PAGINA_DATA_HORA;
						configData.show();
						mostraHoraData();
					}

					break;

				case BTNMINUTO:
					if(valor > 60 || valor < 1){
						mensagem.setText("Valor invalido");
					}
					else{

						setMinuto(rtc,valor);
						PAGINA = PAGINA_DATA_HORA;
						configData.show();
						mostraHoraData();					
					}
					break;

				case BTNDIA:
					if(valor > 31 || valor < 1){
						mensagem.setText("Valor invalido");
					}
					else{
						// Clock.setDate(valor);
						setDia(rtc,valor);
						PAGINA = PAGINA_DATA_HORA;
						configData.show();
						mostraHoraData();
					}
					break;

				case BTNMES:
					if(valor > 12 || valor < 1){
						mensagem.setText("Valor invalido");
					}
					else{
						setMes(rtc,valor);	
						PAGINA = PAGINA_DATA_HORA;
						configData.show();
						mostraHoraData();
					}
					break;

				case BTNANO:
					if(valor > 2999 || valor < 1900){ // verificar qual o ano maximo
						mensagem.setText("Valor invalido");
						
					}
					else{
						setAno(rtc,valor);
						PAGINA = PAGINA_DATA_HORA;
						configData.show();
						mostraHoraData();
					}
					break;
			}
			break;
	  
		case PAGINA_AGENDAMENTO:
			switch(botaoApertado){
				case BTNHORA:
					if(valor > 24 || valor < 0){
						mensagem.setText("Valor invalido");
						//teclado.show();
					}
					else{
						A.setHora(valor);
						PAGINA = PAGINA_AGENDAMENTO;
						agendamento.show();
						mostraDadosAgendamento();
					}

					break;

				case BTNMINUTO:
					if(valor > 60 || valor < 1){
						mensagem.setText("Valor invalido");
					}
					else{

						A.setMinuto(valor);
						PAGINA = PAGINA_AGENDAMENTO;
						agendamento.show();
						mostraDadosAgendamento();					
					}
					break;

				case BTNDIA:
					if(valor > 31 || valor < 1){
						mensagem.setText("Valor invalido");
					}
					else{
						// Clock.setDate(valor);
						A.setDia(valor);
						PAGINA = PAGINA_AGENDAMENTO;
						agendamento.show();
						mostraDadosAgendamento();
					}
					break;

				case BTNMES:
					Serial.println("Case..BTNMES");
					if(valor > 12 || valor < 1){
						mensagem.setText("Valor invalido");
					}
					else{
						A.setMes(valor);	
						PAGINA = PAGINA_AGENDAMENTO;
						agendamento.show();
						mostraDadosAgendamento();
					}
					break;

				case BTNANO:
					if(valor > 2999 || valor < 1900){ // verificar qual o ano maximo
						mensagem.setText("Valor invalido");
						
					}
					else{
						A.setAno(valor);
						PAGINA = PAGINA_AGENDAMENTO;
						agendamento.show();
						mostraDadosAgendamento();
					}
					break;
			}
			break;

        case PAGINA_CO2:
        	switch(botaoApertado){
				case BTNCO2:
					if(valor < 500 || valor > 5000){
						mensagem.setText("Valor inválido");
					} 
					else{
						CO2.setCO2(valor);
						PAGINA = PAGINA_CO2;
						CO2_PAG.show();
						mostraDadosCO2();
					}
          		break;
          
        		case BTNINTCO2:
					if(valor < 200 || valor > 500){
						mensagem.setText("Valor inválido");
					} 
					else{
						CO2.setIntCO2(valor);
						PAGINA = PAGINA_CO2;
						CO2_PAG.show();
						mostraDadosCO2();
					}
          		break;
  
   
        	break;
        	}

		case PAGINA_TEMP_E_UMI:
			switch(botaoApertado){
	          case BTNTEMP:
	            if(valor < 0 || valor > 100){
	              mensagem.setText("Valor inválido");
	            } else{
	              TU.setar_temperatura(valor);
	              temp_e_umi.show();
	              PAGINA = PAGINA_TEMP_E_UMI;
	            }
	          break;
        
	          case BTNTEMPVAR:
	            if(valor > 10){
	              mensagem.setText("Valor inválido");
	            } else{
	              TU.setar_variacao_temperatura(valor);
	              temp_e_umi.show();
	              PAGINA = PAGINA_TEMP_E_UMI;
	            }
	          break;

	          case BTNUMI:
	            if(valor < 0 || valor > 100){
	              mensagem.setText("Valor inválido");
	            } else{
	              TU.setar_umidade(valor);
	              temp_e_umi.show();
	              PAGINA = PAGINA_TEMP_E_UMI;
	            }
	          break;

	          case BTNUMIVAR:
	            if(valor > 10){
	              mensagem.setText("Valor inválido");
	            } else{
	              TU.setar_variacao_umidade(valor);
	              temp_e_umi.show();
	              PAGINA = PAGINA_TEMP_E_UMI;
	            }
	          break;
     		break;
	    	}

		case PAGINA_IR:
			switch(botaoApertado){
				case botaoIR0://botao irriga0ligado
			    	IR.IrrigaVaso0(valor);  //pega o valor que recebe
			    	//necessario corrigir o valor da pagina em que Irrig esta declarado
			    	Irrig.show();//volta para a tela anterior
			 	break;
			}
			break;
     
		case PAGINA_EXAUSTAO:
			switch(botaoApertado){
			 	case BTNCICLOLIG:
			 		if (valor == 0 || valor > 99){
			 			mensagem.setText("Valor invalido");
			 		}
			 		else{
			 			E.SetCiclo(valor, LIGADO);
			 			PAGINA = PAGINA_EXAUSTAO;
			 			exaustao.show();
			 			mostraDadosExaustao();
			 		}
			 	break;

			 	case BTNCICLODESLIG:
			 		if (valor == 0){
			 			mensagem.setText("Valor invalido");
			 		}
			 		else{
			 			E.SetCiclo(valor, DESLIGADO);
			 			PAGINA = PAGINA_EXAUSTAO;
			 			exaustao.show();
			 			mostraDadosExaustao();
			 		}
			 	break;
			}
	}
}
	


NexTouch *nex_listen_list[] = {
    //data e hora
    &hora,
    &minuto,
    &dia,
    &mes,
    &ano,
	&voltar_datahora,
    //agendamento
    &agendarHora,
	&agendarMinuto,
	&agendarDia,
	&agendarMes, 
	&agendarAno,
	&voltar_agendar,
	&ciclo1_agendar,
	&ciclo2_agendar,
	//teclado
    &confirma,
    //iluminação
    &btn_c1,
	&btn_c2,
	&btn_setar_c1,
	&btn_setar_c2,
	&agendar,
	&icone_ilumincao,
	&voltar_iluminacao,
 	//Co2
 	&icone_CO2,
 	&voltar_CO2,
 	&valor_CO2,
 	&intervalo_CO2,
  	//Travas
  	&btnTravaSuperior,
  	&btnTravaInferior,
  	&relogio,
  	&icone_config,
  	&voltar_trava,
  	//Exaustao
  	&icone_exaustao,
  	&veloc_ventilador,
  	&min_ligado,
  	&min_desligado,
  	&voltar_exaustao,
  	//UmiTemp
	&btn_setar_temp,
	&btn_setar_umi,
	&btn_setar_variacao_temp,
	&btn_setar_variacao_umi,
	&voltar_tempumi,
	&Gota1, //escuta se o botao Gota1 foi abertado
    NULL
};
 

void setup() {
	Serial.begin(9600);
	Serial2.begin(9600);
  	dht.begin(); //Sensor de umidade e temperatura
	rtc.begin(); //Relogio
	dbSerialPrintln("SETUP");

	nexInit;
	PAGINA = PAGINA_MENU;
	menu.show();
	
// ####################################################################################################
	hora.attachPop(HoraPopCallback);
	minuto.attachPop(MinutoPopCallback);
	dia.attachPop(DiaPopCallback);
	mes.attachPop(MesPopCallback);
	ano.attachPop(AnoPopCallback);
	voltar_datahora.attachPop(voltarDataHoraCallBack);
// ####################################################################################################
	

// ####################################################################################################
	confirma.attachPop(ConfirmaPopCallback);
// ####################################################################################################


// ####################################################################################################
	btn_setar_c1.attachPop(setarCiclo1Callback);
	btn_setar_c2.attachPop(setarCiclo2Callback);
	icone_ilumincao.attachPop(iconeIluminacaoCallback);
	btn_c1.attachPop(selecionaC1CallBack);
	btn_c2.attachPop(selecionaC2CallBack);
	voltar_iluminacao.attachPop(voltarIluminacaoCallBack);
// ####################################################################################################


// ####################################################################################################
	agendarHora.attachPop(AgendaHoraPopCallback);
	agendarMinuto.attachPop(AgendaMinutoPopCallback);
	agendarDia.attachPop(AgendaDiaPopCallback);
	agendarMes.attachPop(AgendaMesPopCallback);
	agendarAno.attachPop(AgendaAnoPopCallback);
	voltar_agendar.attachPop(VoltarAgendar);
	ciclo1_agendar.attachPop(SelecionaCiclo1PopCallback);
	ciclo2_agendar.attachPop(SelecionaCiclo2PopCallback);
	agendar.attachPop(AgendarPopCallBack);
// ####################################################################################################

//pagina CO2
// ####################################################################################################
	valor_CO2.attachPop(CO2ValorPopCallback);
	intervalo_CO2.attachPop(CO2IntervaloPopCallback); 
	icone_CO2.attachPop(IconeCO2Callback);
	voltar_CO2.attachPop(VoltarCO2CallBack);

// ####################################################################################################

  
//pagina trava
// ####################################################################################################
btnTravaSuperior.attachPop(TravaSuperiorPopCallBack);
btnTravaInferior.attachPop(TravaInferiorPopCallBack);
icone_config.attachPop(iconeCongigPopCallBack);
relogio.attachPop(RelogioPopCallBack);
voltar_trava.attachPop(voltarTravaCallBack);
// ####################################################################################################

//pagina Irrigacao
// ####################################################################################################
Gota1.attachPop(Irriga0Callback);
// ####################################################################################################


//pagina temperatura
//#####################################################################################################
btn_setar_temp.attachPop(TemperaturaCallback);
btn_setar_umi.attachPop(UmidadeCallback);
btn_setar_variacao_temp.attachPop(VariacaoTemperaturaCallback);
btn_setar_variacao_umi.attachPop(VariacaoUmidadeCallback);
voltar_tempumi.attachPop(voltarTempUmiCallback);
icone_temp_umi.attachPop(icone_temp_umiCallback);
//#####################################################################################################

//pagina exaustao
//#####################################################################################################
min_ligado.attachPop(setarCicloLigado);
min_desligado.attachPop(setarCicloDesligado);
icone_exaustao.attachPop(iconeExaustaoCallback);
voltar_exaustao.attachPop(voltarExaustaoCallback);
//#####################################################################################################
}

void loop() {

	
	nexLoop(nex_listen_list);
	//T.run();
	runConfig();

	//if(T.estado_porta_inferior == FECHADA){
		I.run(FECHADA);
		A.run();
		CO2.run(I.estado_atual);
		//D_CO2.run(I.estado_atual);
		//TU.run();	    
	//}
	// else{
	//     if(D_CO2.co2_ligado == 1)
	//       digitalWrite(D_CO2.Pino_rele, LOW);
	// }
	//}
}
		
// }
