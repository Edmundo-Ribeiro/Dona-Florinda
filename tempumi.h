#ifndef tempumi_h
#define tempumi_h
#include "teclado.h"
#include "filtro.h"
#include <DHT.h>
DHT dht(SENSORDHTPINO, DHT22); 

// ###########################################################################################################
// ########################## VARIAVEIS PARA PAGINA DE TEMPERATURA E UMIDADE  ################################
// ###########################################################################################################

  NexButton btn_setar_temp = NexButton(PAGINA_TEMP_E_UMI, 1, "setar_temp"); //atualizar ID e obj_name
  NexButton btn_setar_umi = NexButton(PAGINA_TEMP_E_UMI, 2, "setar_umi");
  NexButton btn_setar_variacao_temp = NexButton(PAGINA_TEMP_E_UMI, 6, "setar_var_temp");
  NexButton btn_setar_variacao_umi = NexButton(PAGINA_TEMP_E_UMI, 7, "setar_var_umi");
  NexButton voltar_tempumi = NexButton(PAGINA_TEMP_E_UMI, 6, "voltar");
  
  NexProgressBar termometro = NexProgressBar(PAGINA_TEMP_E_UMI, 8, "termometro");
  NexProgressBar nuvem = NexProgressBar(PAGINA_TEMP_E_UMI, 9, "nuvem");
  
  NexText temperatura = NexText(PAGINA_TEMP_E_UMI, 3, "temperatura");
  NexText umidade = NexText(PAGINA_TEMP_E_UMI, 4, "umidade");
  
  NexPage temp_e_umi = NexPage(PAGINA_ILUMINACAO, 0, "UmidadeTemp");
  NexButton icone_temp_umi = NexButton(PAGINA_MENU, 4, "UmidadeTemp");


  class STU{
    
    public:
      uint8_t temp_ideal,
          umi_ideal,
          temp_variacao,
          umi_variacao,
          media_temp,
          media_umi,
          rele_arcondicionado,
          rele_desumidificador,
          rele_umidificador,
          Temp_Millis,
          Temp_Intervalo;

    STU(){
      this->temp_ideal = EEPROM.read(temperatura_ideal); //inicializar 
      this->umi_ideal = EEPROM.read(umidade_ideal);
      this->temp_variacao = EEPROM.read(variacao_temperatura); //inicializar 
      this->umi_variacao = EEPROM.read(variacao_umidade);
      this->rele_arcondicionado;
      this->rele_umidificador;
      this->rele_desumidificador;
      this->Temp_Millis;
      this->Temp_Intervalo;    
    }

    void define_intervalo(uint8_t tempo){
      this->Temp_Intervalo = tempo;
    }
    
    void define_pino_arcondicionado(uint8_t pino){
        this->rele_arcondicionado = pino;
        pinMode(this->rele_arcondicionado, OUTPUT);
    }
    void define_pino_umidificador(uint8_t pino){
        this->rele_umidificador = pino;
        pinMode(this->rele_umidificador, OUTPUT);
    }
    void define_pino_desumidificador(uint8_t pino){
        this->rele_desumidificador = pino;
        pinMode(this->rele_desumidificador, OUTPUT);
    }
    void setar_temperatura(uint8_t valor){
          this->temp_ideal = valor;
          EEPROM.update(temperatura_ideal, valor);
    }
    void setar_umidade(uint8_t valor){
          this->umi_ideal = valor;
          EEPROM.update(umidade_ideal, valor);
    }
    void setar_variacao_temperatura(uint8_t valor){
          this->temp_variacao = valor;
          EEPROM.update(variacao_temperatura, valor);
    }
    void setar_variacao_umidade(uint8_t valor){
          this->umi_variacao = valor;
          EEPROM.update(variacao_umidade, valor);
    }


    void run(){
      long atual = millis();
      filtro mediaST; //media da temperatura
      filtro mediaSU; //media da umidade
      mediaST.tamanho(10);//quantidade de variáveis consideradas na média, pode ser alterada
      mediaSU.tamanho(10);//quantidade de variáveis consideradas na média, pode ser alterada
   
      if(atual-this->Temp_Millis >= this->Temp_Intervalo){
        mediaST.adiciona(dht.readTemperature());
        mediaSU.adiciona(dht.readHumidity());
       // temperatura.setText(mediaST.calcula());
       // umidade.setText(mediaSU.calcula());
        termometro.setValue(map(mediaST.media_nova, 0, this->temp_ideal+this->temp_variacao, 0, 100));
        nuvem.setValue(map(mediaSU.media_nova, 0, this->umi_ideal+this->umi_variacao, 0, 100));

        this->media_temp = mediaST.calcula();
        this->media_umi = mediaSU.calcula();
        if(this->media_temp > this->temp_ideal + this->temp_variacao){
          digitalWrite(this->rele_arcondicionado, HIGH);//verificar se o CO2 tá ativado
        }
        if(this->media_temp < this->temp_ideal - this->temp_variacao){
          digitalWrite(this->rele_arcondicionado, LOW);
        }
        if(this->media_umi > this->umi_ideal + this->umi_variacao){
          digitalWrite(this->rele_desumidificador, HIGH);
        }
        if(this->media_umi < this->umi_ideal - this->umi_variacao){
          digitalWrite(this->rele_umidificador, HIGH);
        }
        else{
          digitalWrite(this->rele_arcondicionado, LOW);
          digitalWrite(this->rele_umidificador, LOW); 
          digitalWrite(this->rele_desumidificador, LOW); 
        }
      }
    this->Temp_Millis = atual;
    }
  };
  STU TU; //Desclaração do objeto sistema de temperatura e umidade
// ############################################################################################################

// ###########################################################################################################
// #############Função Callback = O que o programa vai fazer quando o botão for apertado/solto ###############
// ###########################################################################################################

  void TemperaturaCallback(void *ptr){
      botaoAbertado = BTNTEMP;
      teclado.show();
      PassaBotaoParaTela(TU.temp_ideal);
  }
  void UmidadeCallback(void *ptr){
      botaoAbertado = BTNUMI;
      teclado.show();
      PassaBotaoParaTela(TU.umi_ideal);
  }
  void VariacaoUmidadeCallback(void *ptr){
      botaoAbertado = BTNUMIVAR;
      teclado.show();
      PassaBotaoParaTela(TU.umi_variacao);
  }
  void VariacaoTemperaturaCallback(void *ptr){
      botaoAbertado = BTNTEMPVAR;
      teclado.show();
      PassaBotaoParaTela(TU.temp_variacao);
  }   

  void icone_temp_umiCallback(void *ptr){
    PAGINA = PAGINA_TEMP_E_UMI;
    temp_e_umi.show();
  }
  void voltarTempUmiCallback(void *ptr){
    PAGINA = PAGINA_MENU;
    menu.show();
  }

 void mostraDadosTempUmi(){  

    char conteudo_botao[8],
    texto_temperatura[8],
    texto_umidade[8];
    
    sprintf(conteudo_botao,"%02d º", EEPROM.read(temperatura_ideal));
    btn_setar_temp.setText(conteudo_botao);

    sprintf(conteudo_botao,"%02d %", EEPROM.read(umidade_ideal));
    btn_setar_umi.setText(conteudo_botao);

    sprintf(conteudo_botao,"%02d º", EEPROM.read(variacao_temperatura));
    btn_setar_variacao_temp.setText(conteudo_botao);

    sprintf(conteudo_botao,"%02d º", EEPROM.read(variacao_umidade));
    btn_setar_variacao_umi.setText(conteudo_botao);

    termometro.setValue(map(TU.media_temp, 0, TU.temp_ideal+TU.temp_variacao, 0, 100));
    nuvem.setValue(map( TU.media_umi, 0, TU.umi_ideal+TU.umi_variacao, 0, 100));
   
    sprintf(texto_temperatura, "%02d º",  TU.media_temp);
    sprintf(texto_umidade, "%02d º",  TU.media_umi);

    temperatura.setText(texto_temperatura);
    umidade.setText(texto_umidade);
    
  }
  
#endif
