#ifndef definicoes_h
#define definicoes_h

//Inicialização do relogio
DS3231  rtc(SDA, SCL);
/*Pinos do CO2:
    Pino rele do Cilindro de CO2 = 18
    Pino sensor do CO2 = 19
  Pino da trava:
    Pino do fim de curso = 1
    Pino do transistor da trava = 2
*/


//IDs das Paginas  
uint8_t botaoApertado = 0;
uint8_t PAGINA = 0;
#define PAGINA_MENU 0
#define PAGINA_DATA_HORA 5
#define PAGINA_TECLADO 2
#define PAGINA_ILUMINACAO 4
#define PAGINA_AGENDAMENTO 6
#define PAGINA_CO2 7
#define PAGINA_EXAUSTAO 8
#define PAGINA_TEMP_E_UMI 3
#define PAGINA_TRAVA 10
#define PAGINA_IRRIGACAO 1
#define PAGINA_LAVAGEM 9


//Pinos utilizados

//pinos 17 e 16 são pribidos!!!

//Trava
#define PINO_FIM_DE_CURSO_SUPERIOR 2
#define PINO_FIM_DE_CURSO_INFERIOR 3
#define PINO_TRANSISTOR_INFERIOR 4
#define PINO_TRANSISTOR_SUPERIOR 5

//Iluminacao
#define PINO_LED 8
#define PINO_PAINEL 7

//CO2
#define PINO_SENSOR_CO2 19
#define PINO_RELE_CO2 20

//Exaustor
#define PINO_RELE_EXAUST 21 // Não tenho certeza se neste pino o relé vai funcionar

//Temp Umi
#define PINO_RELE_AR_CONDICIONADO 22
#define PINO_RELE_AQUECEDOR 23
#define PINO_RELE_UMIDIFICADOR 24
#define PINO_RELE_DESUMIDIFICADOR 25
#define PINO_SENSOR_DHT 26

#define PINO_VASO_0 30
#define PINO_VASO_1 31
#define PINO_VASO_2 32
#define PINO_VASO_3 33
#define PINO_SIRR_0 A0
#define PINO_SIRR_1 A1
#define PINO_SIRR_2 A2
#define PINO_SIRR_3 A3
//Definir aqui os edereços ondes as coisas serão salvas

#define end_q_horas_ligado_c1 50
#define end_q_horas_ligado_c2 51
#define end_ciclo_atual 52
#define end_minuto_de_ligar 4// e 5
#define end_estado_atual 6

#define end_dia_agendado 8
#define end_mes_agendado 9
#define end_hora_agendado 10
#define end_minuto_agendado 11
#define end_ano_agendado 12// e 13
#define end_ciclo_agendado 14

#define end_temperatura_ideal 15
#define end_umidade_ideal 16
#define end_variacao_temperatura 17
#define end_variacao_umidade 18

//precisam de 2 bytes!
#define end_co2_referencia 19//e 20
#define end_co2_intervalo 21// e 22

//precisam de 2 bytes!
#define end_exautao_ligado 23// e 24
#define end_exautao_desligado 25// e 26

#define end_vaso_0 25//ate 29
#define end_vaso_1 30//ate 34 
#define end_vaso_2 35//ate 39
#define end_vaso_3 40//ate 44


#define LIGADO 1
#define DESLIGADO 0
#define ABERTA 1
#define FECHADA 0


//Botoes
#define BTNHORA 16
#define BTNMINUTO 17
#define BTNDIA 18
#define BTNMES 19
#define BTNANO 20

#define BTNTEMP 8
#define BTNUMI 9
#define BTNTEMPVAR 10
#define BTNUMIVAR 11

#define BTNCO2 12
#define BTNINTCO2 13

#define BTNCICLOLIG 14
#define BTNCICLODESLIG 15


#define botaoIR0 0
#define botaoIR1 1
#define botaoIR2 2
#define botaoIR3 3
#define botaoLVG 30

//Definicoes especificas do codigo de iluminaçao
#define MINUTOS_DIA 1440
#define CICLO_NENHUM 0
#define CICLO_1 1
#define CICLO_2 2

// #####################################################################

NexPage menu = NexPage(PAGINA_MENU,0,"Menu");

NexPage configData = NexPage(PAGINA_DATA_HORA,0,"configData");

NexPage trava = NexPage(PAGINA_TRAVA,0,"Trava");



#endif
