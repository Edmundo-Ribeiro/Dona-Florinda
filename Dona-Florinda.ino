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
				I.setar(botaoAbertado, valor);
				
				PAGINA = PAGINA_ILUMINACAO;
				iluminacao.show();
				mostraDadosIluminacao();
			}
			break;
			
		case PAGINA_DATA_HORA:
			switch(botaoAbertado){
				case BTNHORA:
					if(valor > 24 || valor < 0){
						mensagem.setText("Valor invalido");
						//teclado.show();
					}
					else{
						setHora(rtc,valor);
						PAGINA = PAGINA_DATA_HORA;
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
			switch(botaoAbertado){
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
        switch(botaoAbertado){
          case BTNCO2:
            if(valor < 500 || valor > 5000){
              mensagem.setText("Valor inválido");
            } else{
              D_CO2.setCO2(valor);
              PAGINA = PAGINA_CO2;
            }
          break;
          
          case BTNINTCO2:
            if(valor < 200 || valor > 500){
              mensagem.setText("Valor inválido");
            } else{
              D_CO2.setIntCO2(valor);
              PAGINA = PAGINA_CO2;
            }
          break;
  
          default:
          break;
        }
      case PAGINA_TEMP_E_UMI:
        switch(botaoAbertado){
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

        default:
        break;
      }
      case PAGINA_IR:
        switch(botaoAbertado){
        case botaoIR0://botao irriga0ligado
        	IR.IrrigaVaso0(valor);  //pega o valor que recebe
        	//necessario corrigir o valor da pagina em que Irrig esta declarado
        	Irrig.show();//volta para a tela anterior
         break;

        default:
        break;
        }
        

		default:
		break;
	}		
}


NexTouch *nex_listen_list[] = {
    &hora,
    &minuto,
    &dia,
    &mes,
    &ano,
    &agendarHora,
	&agendarMinuto,
	&agendarDia,
	&agendarMes, 
	&agendarAno,
	&voltar_agendar,
	&ciclo1_agendar,
	&ciclo2_agendar,
    &confirma,
    &definirDataHora,
    &btn_c1,
	&btn_c2,
	&btn_setar_c1,
	&btn_setar_c2,
	&agendar,
	&icone_ilumincao,
	&voltar_iluminacao,
	&voltar_datahora,
 //Botões CO2
 //&valor_CO2,
 //&intervalo_CO2,
  &btn_setar_temp,
  &btn_setar_umi,
  &btn_setar_variacao_temp,
  &btn_setar_variacao_umi,
  &voltar_tempumi,
  &Gota1, //escuta se o botao Gota1 foi abertado
  &btnTravaSuperior,
  &btnTravaInferior,
    NULL
};
 

void setup() {
	Serial.begin(9600);
	Serial2.begin(9600);
  dht.begin(); //Sensor de umidade e temperatura
	rtc.begin();
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
	definirDataHora.attachPop(definirDataHoraPopCallBack);
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
// ####################################################################################################

  
//pagina trava
// ####################################################################################################
btnTravaSuperior.attachPop(TravaSuperiorPopCallBack);
btnTravaInferior.attachPop(TravaInferiorPopCallBack);
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
}


void loop() {

	// if(analogRead(PINO_CAPACITOR) == SEM_ENERGIA){
	// 	//I.salvar();
	// 	// Serial.println(analogRead(PINO_CAPACITOR));
	// }

	// else{
		nexLoop(nex_listen_list);
		T.run();
		runConfig();

		//if(T.estado_porta_inferior == FECHADA){
			I.run(FECHADA);
			A.run();
			//D_CO2.run();
			//TU.run();	    
		//}
		// else{
		//     if(D_CO2.co2_ligado == 1)
		//       digitalWrite(D_CO2.Pino_rele, LOW);
		// }
	}
		
// }
