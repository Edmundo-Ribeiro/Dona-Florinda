#ifndef vaso_h
#define vaso_h
#include "filtro.h"

#define INTERVALO_MEDIR_IR 2000
#define INTERVALO_ATUAR_IR 30000
#define INTERVALO_MOLHAR_IR 4545//+-100ml // bomba faz 22ml por segundo
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
			 molhar,
			 calibrando; //flag para saber quando o "pulso" de irrigação deve acabar

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
			this->calibrando = false;
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

			//se o tempo de lavagem foi setado como diferente de zero, 
			//manter a bomba ligado por esse tempo
			if(this->tempo_lavagem != 0){
				(atual - this->timerLavarTerra >= this->tempo_lavagem) ? this->pararLavagem()
																	   :this->ligar();
			}
			//caso o tempo de lavar seja 0, fazer apenas a verificacao se a umidade do solo
			//precisa de correcao e atuar caso necessário
			else{
				//A cada determindado periodo maior de tempo, verificar se, com o valor da media, deve iniciar irrigacao 
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
					//resetar timer de atuar
					this->timerAtuar = atual;
				}

				// se a flag de molhar foi setada como true
				if(this->molhar){
					//molhar por um pequeno periodo de tempo e parar
					//isso é feito assim para dar tempo da agua espalhar no vaso
					(atual - this->timerMolhar >= INTERVALO_MOLHAR_IR) ? this->desligar()
																	   : this->ligar();
				}
			}
			//caso esse "pulso" de agua não tenha sido suiciente, quando o sistema verificar novamente se deve agir 
			//ele ira pedir mais um pulso
			//isso se repete até que o vaso esteja devidadmente irrigado
		}
		
		//recebe por qunato tempo o savo deve lavar a terra
		void lavarTerra(uint32_t tempo){
			this->tempo_lavagem = tempo * 1000;//passar pra milisegundos
			//resetar o timer de lavar a terra
			this->timerLavarTerra = millis();
		}

		//Interromper lavagem da terra
		void pararLavagem(){
			this->desligar();
			this->tempo_lavagem = 0;
		}

		//definir qual valor de umidade o vaso deve manter
		void setar(uint8_t ref){
			this->referencia = ref;
			EEPROM.put(this->end_base,this->referencia);
		}

		//ler o novo valor de max e salva-lo na memoria
		void calibraMax(){
			this->max = analogRead(this->sensor);
			EEPROM.put(this->end_base + end_max,this->max);
		}

		//ler o novo valor de min e salva-lo na memoria
		void calibraMin(){
			this->min = analogRead(this->sensor);
			EEPROM.put(this->end_base + end_min,this->min);
		}
		
};

#endif