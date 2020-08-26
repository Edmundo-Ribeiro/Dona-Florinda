#include <Nextion.h>
#include <string.h>
#include <EEPROM.h>
#include <RTClib.h>
//--------------------------------------------------------------------
#include "definicoes.h"
#include "datahora.h"
#include "iluminacao.h"
#include "teclado.h"
#include "agendamento.h"
#include "gascarbonico.h"
#include "trava.h"
#include "tempumi.h"
#include "exaustao.h"
#include "novo_irrigacao.h"

void caseIlimunacao(uint16_t valor){
	if(valor > 24 || valor < 0)
		mensagem.setText("Valor invalido");
	else{
		I.setar(botaoApertado, valor);				
		PAGINA = PAGINA_ILUMINACAO;
		iluminacao.show();
		mostraDadosIluminacao();
	}
}

void caseDataHora(uint16_t valor){
	//remover texto falando que relogio desconfigurou
	
	aviso_relogio.setVisible(0);
	switch(botaoApertado){
		case BTNHORA:
			if(valor > 24 || valor < 0){
				mensagem.setText("Valor invalido");
			}
			else{
				setHora(valor);
				PAGINA = PAGINA_DATA_HORA;
				configData.show();
				mostraHoraData();
			}

			break;

		case BTNMINUTO:
			if(valor > 60 || valor < 0){
				mensagem.setText("Valor invalido");
			}
			else{

				setMinuto(valor);
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
				setDia(valor);
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
				setMes(valor);	
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
				setAno(valor);
				PAGINA = PAGINA_DATA_HORA;
				configData.show();
				mostraHoraData();
			}
			break;
	}
}

void caseAgendamento(uint16_t valor){
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
			if(valor > 60 || valor < 0){
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
				A.setDia(valor);
				PAGINA = PAGINA_AGENDAMENTO;
				agendamento.show();
				mostraDadosAgendamento();
			}
			break;

		case BTNMES:
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
			if(valor > 2099 || valor < 1900){ // verificar qual o ano maximo
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
}

void caseCO2(uint16_t valor){
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
			if(valor < 100 || valor > 500){
				mensagem.setText("Valor inválido");
			} 
			else{
				CO2.setIntCO2(valor);
				PAGINA = PAGINA_CO2;
				CO2_PAG.show();
				mostraDadosCO2();
			}
			break;
	} 
}

void caseTempUmi(uint16_t valor){
	switch(botaoApertado){

		case BTNTEMP:
			if(valor < 0 || valor > 100){
				mensagem.setText("Valor inválido");
			} 
			else{
				TU.setar_temperatura(valor);
				PAGINA = PAGINA_TEMP_E_UMI;
				temp_e_umi.show();
				mostraDadosTempUmi();
			}
			break;

		case BTNTEMPVAR:
			if(valor > 10){
				mensagem.setText("Valor inválido");
			} 
			else{
				TU.setar_variacao_temperatura(valor);
				PAGINA = PAGINA_TEMP_E_UMI;
				temp_e_umi.show();
				mostraDadosTempUmi();
			}
			break;

		case BTNUMI:
			if(valor < 0 || valor > 100){
				mensagem.setText("Valor inválido");
			}
			else{
				TU.setar_umidade(valor);
				PAGINA = PAGINA_TEMP_E_UMI;
				temp_e_umi.show();
				mostraDadosTempUmi();
			}
			break;

		case BTNUMIVAR:
			if(valor > 10){
				mensagem.setText("Valor inválido");
			}
			else{
				TU.setar_variacao_umidade(valor);
				PAGINA = PAGINA_TEMP_E_UMI;
				temp_e_umi.show();
				mostraDadosTempUmi();
			}
      		break;
	}
}

void caseExaustao(uint16_t valor){
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

void caseIrrigacao(uint16_t valor){

	if (valor <= 0 || valor > 100)
	 	mensagem.setText("Valor invalido");

	else {
		switch(botaoApertado){
			case botaoIR0:
				IR.setar(0, valor);
			 	break;

		 	case botaoIR1:
				IR.setar(1, valor);
			 	break;

		 	case botaoIR2:
				IR.setar(2, valor);
			 	break;

		 	case botaoIR3:
				IR.setar(3, valor);
			 	break;

			case botaoMax0:
				IR.calibraMax(0);
			 	break;

			case botaoMax1:
				IR.calibraMax(1);
			 	break;
			case botaoMax2:
				IR.calibraMax(2);
			 	break;

			case botaoMax3:
				IR.calibraMax(3);
			 	break;

			case botaoMin0:
				IR.calibraMin(0);
			 	break;

			case botaoMin1:
				IR.calibraMin(1);
			 	break;
			case botaoMin2:
				IR.calibraMin(2);
			 	break;

			case botaoMin3:
				IR.calibraMin(3);
			 	break;
	
		}

		PAGINA = PAGINA_IRRIGACAO;
		umidade_solo.show();//volta para a tela anterior
		mostraDadosIrr();
	}
}

void caseLavagem(uint16_t valor){

	if (valor <= 0 || valor > TEMP_MAX_LAVAGEM)
	 	mensagem.setText("Valor invalido");

	else{
		switch(botaoApertado){
			case botaoIR0:
				IR.lavarTerra(0,valor);
			 	break;

		 	case botaoIR1:
				IR.lavarTerra(1,valor);
			 	break;

		 	case botaoIR2:
				IR.lavarTerra(2,valor);
			 	break;

		 	case botaoIR3:
				IR.lavarTerra(3,valor);
			 	break;

			case botaoLVG:
				for(uint8_t id = 0; id < NUM_VASOS; id++ ){
					IR.lavarTerra(id,valor);
				}
			 	break;
		}
		
		PAGINA = PAGINA_LAVAGEM;
		lavagem_terra.show();//volta para a tela anterior
		mostraDadosLavagem();
	}
}

void ConfirmaPopCallback(void *ptr){
	uint16_t valor;
	uint8_t pagina_anterior = PAGINA;
	char buffer[5];
	entrada.getText(buffer,sizeof(buffer));
	valor = (uint16_t)atoi(buffer);	
	// PAGINA = PAGINA_TECLADO;
	switch(pagina_anterior){

		case PAGINA_ILUMINACAO:
			caseIlimunacao(valor);
			break;
			
		case PAGINA_DATA_HORA:
			caseDataHora(valor);
			break;
	  
		case PAGINA_AGENDAMENTO:
			caseAgendamento(valor);
			break;

        case PAGINA_CO2:
        	caseCO2(valor);
        	break;

		case PAGINA_TEMP_E_UMI:
			caseTempUmi(valor);
     		break;
     
		case PAGINA_EXAUSTAO:
			caseExaustao(valor);
			break;

		case PAGINA_IRRIGACAO:
			caseIrrigacao(valor);
			break;

		case PAGINA_LAVAGEM:
			caseLavagem(valor);
			break;
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
	&icone_temp_umi,
	//Irrigacao
	&sets[0],
	&sets[1],
	&sets[2],
	&sets[3],
	&Maxs[0],
	&Maxs[1],
	&Maxs[2],
	&Maxs[3],
	&Mins[0],
	&Mins[1],
	&Mins[2],
	&Mins[3],
	&btn_calibrar,
	&voltar_irr,
	&icone_lavar,
	&icone_irr,
	&lavar_gotas[0],
	&lavar_gotas[1],
	&lavar_gotas[2],
	&lavar_gotas[3],
	&lavar_geral,
	&voltar_lavagem,
	
    NULL
};
 

bool avisoVisivelFlag = 0;

void setup() {
	Serial.begin(9600);
	Serial2.begin(9600);
	nexInit();
  	dht.begin(); //Sensor de umidade e temperatura

	if( rtc.begin() ) {

		if(rtc.lostPower()){
			dbSerialPrintln("Relogio desconfigurado");
			PAGINA = PAGINA_DATA_HORA;
			configData.show();
			aviso_relogio.setVisible(1);
			//criar campo de texto informando que relogio desconfigurou
		}
		avisoSR.setVisible(0);
		avisoVisivelFlag = 0;
	}
	else{
		dbSerialPrintln("Setup");
		PAGINA = PAGINA_MENU;
		menu.show();
		avisoSR.setVisible(1);
		avisoVisivelFlag = 1;
	}

	
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

//pagina iluminacao
// ####################################################################################################
	btn_setar_c1.attachPop(setarCiclo1Callback);
	btn_setar_c2.attachPop(setarCiclo2Callback);
	icone_ilumincao.attachPop(iconeIluminacaoCallback);
	btn_c1.attachPop(selecionaC1CallBack);
	btn_c2.attachPop(selecionaC2CallBack);
	voltar_iluminacao.attachPop(voltarIluminacaoCallBack);
// ####################################################################################################

//pagina Agendamento
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
	sets[0].attachPop(Set0Callback);
	sets[1].attachPop(Set1Callback);
	sets[2].attachPop(Set2Callback);
	sets[3].attachPop(Set3Callback);
	Maxs[0].attachPop(setMax0);
	Maxs[1].attachPop(setMax1);
	Maxs[2].attachPop(setMax2);
	Maxs[3].attachPop(setMax3);
	Mins[0].attachPop(setMin0);
	Mins[1].attachPop(setMin1);
	Mins[2].attachPop(setMin2);
	Mins[3].attachPop(setMin3);
	voltar_irr.attachPop(voltarIRR);
	icone_irr.attachPop(iconeIrrCallback);
// ####################################################################################################

//pagina Lavagem
// ####################################################################################################
	lavar_gotas[0].attachPop(Lavar0Callback);
	lavar_gotas[1].attachPop(Lavar1Callback);
	lavar_gotas[2].attachPop(Lavar2Callback);
	lavar_gotas[3].attachPop(Lavar3Callback);
	lavar_geral.attachPop(LavarGeralCallback);
	voltar_lavagem.attachPop(voltarLavagemCallback);
	icone_lavar.attachPop(IconeLavarCallback);
	btn_calibrar.attachPop(calibrarCallback);
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
	
	T.run();
	// E.run();
	// TU.run(T.estado_porta_inferior);
	IR.run();	    
	
	if( !rtc.begin() ) {
		if(!avisoVisivelFlag){
			PAGINA = PAGINA_MENU;
			menu.show();
			avisoVisivelFlag = 1;
		 	avisoSR.setVisible(1);
		}
	}
	else{
		avisoVisivelFlag = 0;
		if(rtc.lostPower()){
			dbSerialPrintln("Relogio desconfigurado");
			if( PAGINA != PAGINA_DATA_HORA){
				PAGINA = PAGINA_DATA_HORA;
				configData.show();
				aviso_relogio.setVisible(1);
				//criar campo de texto informando que relogio desconfigurou
			}
		}
		else {
			runDataHora();
			I.run(0); //I.run(T.estado_porta_inferior);
			A.run(I);
			// CO2.run(I.estado_atual, E.estado_atual);
		}
	}
}
		
