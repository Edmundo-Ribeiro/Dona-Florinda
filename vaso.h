#ifndef vaso_h
#define vaso_h
#include "filtro.h"

#define INTERVALO_MEDIR_IR 100
#define INTERVALO_ATUAR_IR 2000
#define INTERVALO_MOLHAR_IR 500 // bomba faz 22ml por segundo
#define TOLERANCIA 5

//salvar assim na memoria
//[1byte referencia][2bytes max][2bytes min]
#define end_max 1
#define end_min 3 

class vaso{
	public:
		float media;

		uint8_t rele, //rele da bomba
				sensor,//Sensor de umidade do solo
				id, // qual o vaso
				referencia, // valor de umidade desejado
				end_base; // endereço para salvar na memoria

		uint16_t max, // valor maximo para calibragem
				 min; // valor minimo para calibragem

		uint32_t tempo_lavagem; //Tempo inserido para lavar a terra

		bool estado_atual, // Se está irrigando ou não
			 molhar; //flag para saber quando o "pulso" de irrigação deve acabar

		//timers 
		unsigned long timerMedir;
		unsigned long timerMolhar;
		unsigned long timerAtuar;
		unsigned long timerLavarTerra;

		//filtro para as medidas
		filtro 	F;

		vaso(uint8_t id, uint8_t rele, uint8_t sensor, uint8_t end_base){
			this->rele = rele;
			this->sensor = sensor;
			this->id = id;
			this->estado_atual = DESLIGADO;
			this->molhar = false;
			this->media = 0;
			this->tempo_lavagem = 0;
			this->timerMedir = this->timerMolhar = this->timerAtuar = millis();
			
			pinMode(rele, OUTPUT);
			pinMode(sensor, INPUT); 

			this->end_base = end_base;

			this->referencia = EEPROM.read(end_base);
			this->max = EEPROM.get(end_base + end_max, this->max);
			this->min = EEPROM.get(end_base + end_min, this->min);


		}

		float medir(){
			F.adiciona( map( analogRead(this->sensor) , min, max, 0, 100 ) );
			this->media = F.calcula();
			return this->media;
		}

		void ligar(){
			digitalWrite(this->rele,LOW);
			this->estado_atual = LIGADO;
		}
		void desligar(){
			digitalWrite(this->rele,HIGH);
			this->estado_atual = DESLIGADO;

		}

		void run(){

			unsigned long atual = millis();

			//A cada periudo definido, fazer uma medição 
			if(atual - this->timerMedir >= INTERVALO_MEDIR_IR){
				this->medir();
				this->timerMedir = atual;
			}

			//se o tempo de lavagem foi setado como diferente de zero, manter a bomba ligado por esse tempo
			if(this->tempo_lavagem != 0){
				if(atual - this->timerLavarTerra >= this->tempo_lavagem){
					this->desligar();
					this->tempo_lavagem = 0;
				}
				else{
					this->ligar();
				}
			}

			else{
				//Depois de um periodo maior de tempo, verificar se, com o valor da media, deve iniciar irrigacao 
				if(atual - this->timerMedir >= INTERVALO_ATUAR_IR){

					//se a media esta abaixo do limite inferior
					if(this->media < referencia - TOLERANCIA){
						//setar a flag para molhar
						this->molhar = true;
						//resetar o timmer de molhar
						this->timerMolhar = atual;
					}
					//do contrario só garantir que a flag é falsa
					else
						this->molhar = false;
					//resetar timer
					this->timerAtuar = atual;
				}

				// ver se deve molhar
				if(this->molhar){
					//molhar por um pequeno periodo de tempo e parar(isso para dar tempo da agua espalhar)
					if(atual - this->timerMolhar >= INTERVALO_MOLHAR_IR){
						this->desligar();
					}
					else{
						this->ligar();
					}
				}
			}
			//caso esse "pulso" de agua não tenha sido suiciente, quando o sistema verificar novamente se deve agir 
			//ele ira pedir mais um pulso
			//isso se repete até que o vaso esteja devidadmente irrigado
		}

		void lavarTerra(uint32_t tempo){
			this->tempo_lavagem = tempo;
			//resetar o timer de lavar a terra
			this->timerLavarTerra = millis();
		}

		void setar(uint8_t ref){
			this->referencia = ref;
			EEPROM.put(this->end_base,this->referencia);
		}

		void calibraMax(uint16_t max){
			this->max = max;
			EEPROM.put(this->end_base + end_max,this->max);
		}
		void calibraMin(uint16_t min){
			this->min = min;
			EEPROM.put(this->end_base + end_min,this->min);
		}
};

#endif