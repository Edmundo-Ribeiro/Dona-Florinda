#ifndef tempumi_h
#define tempumi_h
#include "teclado.h"
#include "filtro.h"
#include <DHT.h>

DHT dht(PINO_SENSOR_DHT, DHT22); 

void mostraDadosTempUmi();
void debugEstado();
// ###########################################################################################################
// ########################## VARIAVEIS PARA PAGINA DE TEMPERATURA E UMIDADE  ################################
// ###########################################################################################################

  NexButton btn_setar_temp = NexButton(PAGINA_TEMP_E_UMI, 1, "setar_temp"); //atualizar ID e obj_name
  NexButton btn_setar_umi = NexButton(PAGINA_TEMP_E_UMI, 2, "setar_umi");
  NexButton btn_setar_variacao_temp = NexButton(PAGINA_TEMP_E_UMI, 5, "setar_var_temp");
  NexButton btn_setar_variacao_umi = NexButton(PAGINA_TEMP_E_UMI, 6, "setar_var_umi");
  NexButton voltar_tempumi = NexButton(PAGINA_TEMP_E_UMI, 9, "voltar");
  
  NexProgressBar termometro = NexProgressBar(PAGINA_TEMP_E_UMI, 7, "j0");
  NexProgressBar nuvem = NexProgressBar(PAGINA_TEMP_E_UMI, 8, "j1");
  
  NexText temperatura = NexText(PAGINA_TEMP_E_UMI, 3, "temperatura");
  NexText umidade = NexText(PAGINA_TEMP_E_UMI, 4, "umidasde");
  
  NexPage temp_e_umi = NexPage(PAGINA_TEMP_E_UMI, 0, "UmidadeTemp");
  NexButton icone_temp_umi = NexButton(PAGINA_MENU, 2, "UmiTemp");


  class STU{
    
    public:
      uint8_t temp_ideal,
              umi_ideal,
              temp_variacao,
              umi_variacao,
              media_temp,
              media_umi;

      unsigned long Temp_Millis,
                    Temp_Intervalo;

      filtro mediaST, //media da temperatura
             mediaSU; //media da umidade

    STU(){
      this->temp_ideal = EEPROM.read(end_temperatura_ideal);
      this->umi_ideal = EEPROM.read(end_umidade_ideal);
      this->temp_variacao = EEPROM.read(end_variacao_temperatura); 
      this->umi_variacao = EEPROM.read(end_variacao_umidade);
      this->Temp_Intervalo = 100000;  
      this->mediaST.tamanho(10);//quantidade de variáveis consideradas na média, pode ser alterada
      this->mediaSU.tamanho(10);//quantidade de variáveis consideradas na média, pode ser alterada
      pinMode(PINO_RELE_AR_CONDICIONADO, OUTPUT);
      pinMode(PINO_RELE_AQUECEDOR, OUTPUT);
      pinMode(PINO_RELE_UMIDIFICADOR, OUTPUT);
      pinMode(PINO_RELE_DESUMIDIFICADOR, OUTPUT);
      }

    void setar_temperatura(uint8_t valor){
          this->temp_ideal = valor;
          EEPROM.update(end_temperatura_ideal, valor);
    }

    void setar_umidade(uint8_t valor){
          this->umi_ideal = valor;
          EEPROM.update(end_umidade_ideal, valor);
    }

    void setar_variacao_temperatura(uint8_t valor){
          this->temp_variacao = valor;
          EEPROM.update(end_variacao_temperatura, valor);
    }

    void setar_variacao_umidade(uint8_t valor){
          this->umi_variacao = valor;
          EEPROM.update(end_variacao_umidade, valor);
    }

    void run(bool estado_porta){
          if (estado_porta == FECHADA){
            unsigned long atual = millis();
              if(atual - this->Temp_Millis >= this->Temp_Intervalo){
                mediaST.adiciona(dht.readTemperature());
                mediaSU.adiciona(dht.readHumidity());
                //temperatura.setText(mediaST.calcula());
                //umidade.setText(mediaSU.calcula());
                termometro.setValue(map(mediaST.media_nova, 0, this->temp_ideal+this->temp_variacao, 0, 100));
                nuvem.setValue(map(mediaSU.media_nova, 0, this->umi_ideal+this->umi_variacao, 0, 100));

                this->media_temp = mediaST.calcula();
                this->media_umi = mediaSU.calcula();
        
                  if(this->media_temp > this->temp_ideal + this->temp_variacao){
                    digitalWrite(PINO_RELE_AR_CONDICIONADO, HIGH);
                    digitalWrite(PINO_RELE_AQUECEDOR, LOW);  
                  }
                  if(this->media_temp < this->temp_ideal - this->temp_variacao){
                    digitalWrite(PINO_RELE_AQUECEDOR, HIGH);
                    digitalWrite(PINO_RELE_AR_CONDICIONADO, LOW); 
                  }
                  if(this->media_umi > this->umi_ideal + this->umi_variacao){
                    digitalWrite(PINO_RELE_DESUMIDIFICADOR, HIGH);
                    digitalWrite(PINO_RELE_UMIDIFICADOR, LOW);
                  }
                  if(this->media_umi < this->umi_ideal - this->umi_variacao){
                    digitalWrite(PINO_RELE_UMIDIFICADOR, HIGH);
                    digitalWrite(PINO_RELE_DESUMIDIFICADOR, LOW);
                  }
                  else{
                    digitalWrite(PINO_RELE_AR_CONDICIONADO, LOW);
                    digitalWrite(PINO_RELE_UMIDIFICADOR, LOW); 
                    digitalWrite(PINO_RELE_DESUMIDIFICADOR, LOW); 
                    digitalWrite(PINO_RELE_AQUECEDOR, LOW); 
                  }
              this->Temp_Millis = atual;
                if(PAGINA == PAGINA_TEMP_E_UMI){
                  mostraDadosTempUmi();
                }
              }

          }
    }
    };
  STU TU; //Desclaração do objeto sistema de temperatura e umidade
// ###########################################################################################################

// ###########################################################################################################
// #############Função Callback = O que o programa vai fazer quando o botão for apertado/solto ###############
// ###########################################################################################################

  void TemperaturaCallback(void *ptr){
      botaoApertado = BTNTEMP;
      teclado.show();
      PassaBotaoParaTela(TU.temp_ideal);
  }

  void UmidadeCallback(void *ptr){
      botaoApertado = BTNUMI;
      teclado.show();
      PassaBotaoParaTela(TU.umi_ideal);
  }

  void VariacaoUmidadeCallback(void *ptr){
      botaoApertado = BTNUMIVAR;
      teclado.show();
      PassaBotaoParaTela(TU.umi_variacao);
  }

  void VariacaoTemperaturaCallback(void *ptr){
      botaoApertado = BTNTEMPVAR;
      teclado.show();
      PassaBotaoParaTela(TU.temp_variacao);
  }   

  void icone_temp_umiCallback(void *ptr){
      PAGINA = PAGINA_TEMP_E_UMI;
      temp_e_umi.show();
      //mostraDadosTempUmi();
  }

  void voltarTempUmiCallback(void *ptr){
      PAGINA = PAGINA_MENU;
      menu.show();
  }

    void debugEstado(){
    Serial.print("############# Variaveis Temperatura e Umidade #####################\n\n");
    
    Serial.print("Temperatura Ideal: ");
    Serial.println(TU.temp_ideal);

    Serial.print("Umidade Variacao Ideal: ");
    Serial.println(TU.umi_variacao);

    Serial.print("Página Atual: ");
    Serial.println(PAGINA);
    
    Serial.print("######################################################\n\n");

  }

void mostraDadosTempUmi(){  

    char conteudo_botao[4],
         texto_temperatura[4],
         texto_umidade[4];
    
    sprintf(conteudo_botao,"%02dº", EEPROM.read(end_temperatura_ideal));
    btn_setar_temp.setText(conteudo_botao);

    sprintf(conteudo_botao,"%02d%", EEPROM.read(end_umidade_ideal));
    btn_setar_umi.setText(conteudo_botao);

    sprintf(conteudo_botao,"%02dº", EEPROM.read(end_variacao_temperatura));
    btn_setar_variacao_temp.setText(conteudo_botao);

    sprintf(conteudo_botao,"%02dº", EEPROM.read(end_variacao_umidade));
    btn_setar_variacao_umi.setText(conteudo_botao);

    termometro.setValue(map(TU.media_temp, 0, TU.temp_ideal+TU.temp_variacao, 0, 100));
    nuvem.setValue(map( TU.media_umi, 0, TU.umi_ideal+TU.umi_variacao, 0, 100));
   
    sprintf(texto_temperatura, "%02dº",  TU.media_temp);
    sprintf(texto_umidade, "%02dº",  TU.media_umi);

    temperatura.setText(texto_temperatura);
    umidade.setText(texto_umidade);
    
  }
  
#endif
