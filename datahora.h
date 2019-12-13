#ifndef datahora_h
#define datahora_h
#include "teclado.h"
// ###########################################################################################################
// ########################## VARIAVEIS PARA PAGINA DE DEFINIÇÂO DA DATA E HORA ##############################
// ###########################################################################################################
	#define CONFIGINTERVALO 2000
	unsigned long configMillis = 0;

	

	NexButton definirDataHora = NexButton(0, 5, "defDataHora");
	NexPage configData = NexPage(PAGINA_DATA_HORA,0,"configData");
	NexNumber hora = NexNumber(PAGINA_DATA_HORA, 1, "hora");
	NexNumber minuto = NexNumber(PAGINA_DATA_HORA, 2, "minuto");
	NexNumber dia = NexNumber(PAGINA_DATA_HORA, 3, "dia");
	NexNumber mes = NexNumber(PAGINA_DATA_HORA, 4, "mes");
	NexNumber ano = NexNumber(PAGINA_DATA_HORA, 5, "ano");
	NexButton voltar_datahora = NexButton(PAGINA_DATA_HORA, 6, "voltar");
// ###########################################################################################################


// ###########################################################################################################
// #####################CALLBACKS E FUNÇÔES PARA PAGINA DE DEFINIÇÂO DA DATA E HORA ##########################
// ###########################################################################################################

	void definirDataHoraPopCallBack(void *ptr){
		PAGINA = PAGINA_DATA_HORA;
		configData.show();
	}

	void HoraPopCallback(void *ptr){
		uint32_t bufferValor;
		botaoAbertado = BTNHORA;
		hora.getValue(&bufferValor);
		PassaBotaoParaTela(bufferValor);
	}
	void MinutoPopCallback(void *ptr){
		uint32_t bufferValor;
		botaoAbertado = BTNMINUTO;
		minuto.getValue(&bufferValor);
		PassaBotaoParaTela(bufferValor);
	}
	void DiaPopCallback(void *ptr){
		uint32_t bufferValor;
		botaoAbertado = BTNDIA;
		dia.getValue(&bufferValor);
		PassaBotaoParaTela(bufferValor);
	}
	void MesPopCallback(void *ptr){
		uint32_t bufferValor;
		botaoAbertado = BTNMES;
		mes.getValue(&bufferValor);
		PassaBotaoParaTela(bufferValor);
	}
	void AnoPopCallback(void *ptr){
		uint32_t bufferValor;
		botaoAbertado = BTNANO;
		ano.getValue(&bufferValor);
		PassaBotaoParaTela(bufferValor);
	}

	void mostraHoraData(){
		Time t = rtc.getTime();
		dia.setValue(t.date);
		mes.setValue(t.mon);
		ano.setValue(t.year);
		hora.setValue(t.hour);
		minuto.setValue(t.min);
	
	}

	void voltarDataHoraCallBack(void *ptr){
		PAGINA = PAGINA_MENU;
		menu.show();
	}

	void runConfig(){
		if((millis() - configMillis) >= CONFIGINTERVALO)
			if(PAGINA == PAGINA_DATA_HORA){
				//Time T = rtc.getTime();
				// uint32_t buffer;
				
				mostraHoraData();
				configMillis = millis();
			}
	}
	void setHora(DS3231 &rtc, uint8_t aux_hora){
		uint8_t aux_minuto = rtc.getTime().min;
		uint8_t aux_segundo = rtc.getTime().sec;
		rtc.setTime(aux_hora,aux_minuto,aux_segundo);
	}
	void setMinuto(DS3231 &rtc, uint8_t aux_minuto){
		uint8_t aux_hora = rtc.getTime().hour;
		uint8_t aux_segundo = rtc.getTime().sec;
		rtc.setTime(aux_hora,aux_minuto,aux_segundo);
	}
	void setDia(DS3231 &rtc, uint8_t aux_dia){
		uint8_t aux_mes = rtc.getTime().mon;		
		uint16_t aux_ano = rtc.getTime().year;
		rtc.setDate(aux_dia,aux_mes,aux_ano);
	}
	void setMes(DS3231 &rtc, uint8_t aux_mes){
		uint8_t aux_dia = rtc.getTime().date;
		uint16_t aux_ano = rtc.getTime().year;
		rtc.setDate(aux_dia,aux_mes,aux_ano);
	}
	void setAno(DS3231 &rtc, uint16_t aux_ano){
		uint8_t aux_dia = rtc.getTime().date;
		uint8_t aux_mes = rtc.getTime().mon;
		rtc.setDate(aux_dia,aux_mes,aux_ano);
	}
// ###########################################################################################################

#endif