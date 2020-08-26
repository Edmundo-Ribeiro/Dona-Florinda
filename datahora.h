#ifndef datahora_h
#define datahora_h
#include "teclado.h"
// ###########################################################################################################
// ########################## VARIAVEIS PARA PAGINA DE DEFINIÇÂO DA DATA E HORA ##############################
// ###########################################################################################################
	#define INTERVALO_DADOS_DATAHORA 10000
	unsigned long datahoraMillis = 0;


	
	NexButton hora = NexButton(PAGINA_DATA_HORA, 5, "hora");
	NexButton minuto = NexButton(PAGINA_DATA_HORA, 6, "minuto");
	NexButton dia = NexButton(PAGINA_DATA_HORA, 2, "dia");
	NexButton mes = NexButton(PAGINA_DATA_HORA, 3, "mes");
	NexButton ano = NexButton(PAGINA_DATA_HORA, 4, "ano");
	NexButton voltar_datahora = NexButton(PAGINA_DATA_HORA, 1, "voltar");
	NexText aviso_relogio = NexText(PAGINA_DATA_HORA, 7, "aviso");
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
		DateTime t = rtc.now();

		char DD[]="DD",
		 MM[]="MM",
		 YYYY[]="YYYY",
		 hh[]="hh",
		 mm[]="mm";


		dia.setText(t.toString(DD));

		mes.setText(t.toString(MM));

		ano.setText(t.toString(YYYY));
	
		hora.setText(t.toString(hh));

		minuto.setText(t.toString(mm));

	}

	void voltarDataHoraCallBack(void *ptr){		
		PAGINA = PAGINA_TRAVA;
		trava.show();
		//talvez tenha que chamar mostrardadostrava
	}

	void runDataHora(){
		if((millis() - datahoraMillis) >= INTERVALO_DADOS_DATAHORA)
			if(PAGINA == PAGINA_DATA_HORA){

				mostraHoraData();
				datahoraMillis = millis();
			}
	}
	void setHora( uint8_t aux_hora){
		DateTime  now = rtc.now();
		DateTime adjust = DateTime(now.year(), now.month(), now.day(), aux_hora, now.minute(), now.second());
		rtc.adjust(adjust);
	}
	void setMinuto( uint8_t aux_minuto){
		DateTime  now = rtc.now();
		DateTime adjust = DateTime(now.year(), now.month(), now.day(), now.hour(), aux_minuto, now.second());
		rtc.adjust(adjust);
	}
	void setDia( uint8_t aux_dia){
		DateTime  now = rtc.now();
		DateTime adjust = DateTime(now.year(), now.month(), aux_dia, now.hour(), now.minute(), now.second());
		rtc.adjust(adjust);
	}
	void setMes( uint8_t aux_mes){
		DateTime  now = rtc.now();
		DateTime adjust = DateTime(now.year(), aux_mes, now.day(), now.hour(), now.minute(), now.second());
		rtc.adjust(adjust);
	}
	void setAno( uint16_t aux_ano){
			DateTime  now = rtc.now();
		DateTime adjust = DateTime(aux_ano, now.month(), now.day(), now.hour(), now.minute(), now.second());
		rtc.adjust(adjust);
	}
// ###########################################################################################################

#endif