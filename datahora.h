#ifndef datahora_h
#define datahora_h
#include "teclado.h"
// ###########################################################################################################
// ########################## VARIAVEIS PARA PAGINA DE DEFINIÇÂO DA DATA E HORA ##############################
// ###########################################################################################################
	#define CONFIGINTERVALO 2000
	unsigned long configMillis = 0;

	

	
	NexButton hora = NexButton(PAGINA_DATA_HORA, 5, "hora");
	NexButton minuto = NexButton(PAGINA_DATA_HORA, 6, "minuto");
	NexButton dia = NexButton(PAGINA_DATA_HORA, 2, "dia");
	NexButton mes = NexButton(PAGINA_DATA_HORA, 3, "mes");
	NexButton ano = NexButton(PAGINA_DATA_HORA, 4, "ano");
	NexButton voltar_datahora = NexButton(PAGINA_DATA_HORA, 1, "voltar");
// ###########################################################################################################


// ###########################################################################################################
// #####################CALLBACKS E FUNÇÔES PARA PAGINA DE DEFINIÇÂO DA DATA E HORA ##########################
// ###########################################################################################################


	void HoraPopCallback(void *ptr){
		char bufferValor[5];
		botaoApertado = BTNHORA;
		hora.getText(bufferValor,sizeof(bufferValor));
		PassaTextoParaTela(bufferValor);
	}
	void MinutoPopCallback(void *ptr){
		char bufferValor[5];
		botaoApertado = BTNMINUTO;
		minuto.getText(bufferValor,sizeof(bufferValor));
		PassaTextoParaTela(bufferValor);
	}
	void DiaPopCallback(void *ptr){
		char bufferValor[5];
		botaoApertado = BTNDIA;
		dia.getText(bufferValor,sizeof(bufferValor));
		PassaTextoParaTela(bufferValor);
	}
	void MesPopCallback(void *ptr){
		char bufferValor[5];
		botaoApertado = BTNMES;
		mes.getText(bufferValor,sizeof(bufferValor));
		PassaTextoParaTela(bufferValor);
	}
	void AnoPopCallback(void *ptr){
		char bufferValor[7];//pq 7? não sei, mas só funciona assim
		botaoApertado = BTNANO;
		ano.getText(bufferValor,sizeof(bufferValor));
		PassaTextoParaTela(bufferValor);
	}

	void mostraHoraData(){
		char buffer[5];
		Time t = rtc.getTime();
		itoa(t.date, buffer,10);
		dia.setText(buffer);
		itoa(t.mon, buffer,10);
		mes.setText(buffer);
		itoa(t.year, buffer,10);
		ano.setText(buffer);
		itoa(t.hour, buffer,10);
		Serial.println(buffer);
		hora.setText(buffer);
		itoa(t.min, buffer,10);
		minuto.setText(buffer);
	
	}

	void voltarDataHoraCallBack(void *ptr){
		PAGINA = PAGINA_TRAVA;
		trava.show();
		//talvez tenha que chamar mostrardadostrava
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