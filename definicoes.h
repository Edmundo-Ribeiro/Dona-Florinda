#ifndef definicoes_h
#define definicoes_h

DS3231  rtc(SDA, SCL);
/*Pinos do CO2:
    Pino rele do Cilindro de CO2 = 18
    Pino sensor do CO2 = 19
  Pino da trava:
    Pino do fim de curso = 1
    Pino do transistor da trava = 2
*/




uint8_t botaoAbertado = 0;
uint8_t PAGINA = 0;
#define PAGINA_MENU 0
#define PAGINA_DATA_HORA 5
#define PAGINA_TECLADO 3
#define PAGINA_ILUMINACAO 4
#define PAGINA_AGENDAMENTO 6
#define PAGINA_CO2 7
#define PAGINA_TRAVA 0
#define PAGINA_EXAUSTAO 8
#define PAGINA_TEMP_E_UMI 9
//#define LIGADO 1
//#define DESLIGADO 0

#define SENSORDHTPINO 18
#define PINO_LED 8
#define PINO_PAINEL 7
//	Definir aqui os edereços ondes as coisas serão salvas
// 0 - quantidade de horas ligado ciclo 1 - q_horas_ligado_c1
#define end_q_horas_ligado_c1 40

//1 - quantidade de horas ligado ciclo 2 - q_horas_ligado_c2
#define end_q_horas_ligado_c2 41

// //2 e 3 - quantidade de horas ligado ciclo 2 - q_horas_ligado_c2
// #define end_minutos_passados 2

//4 e 5 - minuto de ligar
#define end_minuto_de_ligar 4

//6 - estado atual
#define end_estado_atual 6

//7 - ciclo atual
#define end_ciclo_atual 42

#define end_dia_agendado 8
#define end_mes_agendado 9
#define end_hora_agendado 10
#define end_minuto_agendado 11
#define end_ano_agendado 12// e 13
#define end_ciclo_agendado 14

#define temperatura_ideal 15
#define umidade_ideal 16
#define variacao_temperatura 17
#define variacao_umidade 18


#define OPEN 1
#define CLOSED 0
#define RESUMED 1
#define NOTRESUMED 0

#define BASEYEAR 2019
#define end_endereco_minutos_passados 1000// 1000 e 1001


#define BTNHORA 3
#define BTNMINUTO 4
#define BTNDIA 5
#define BTNMES 6
#define BTNANO 7

#define BTNTEMP 8
#define BTNUMI 9
#define BTNTEMPVAR 10
#define BTNUMIVAR 11

#define MINUTOS_DIA 1440
// #####################################################################

NexPage menu = NexPage(PAGINA_MENU,0,"Menu"); 



#endif
