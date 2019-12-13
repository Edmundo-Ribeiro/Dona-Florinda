#include <Nextion.h>




/*
 * Primeiro codigo que faz rotina limpa sem possibilidade de alterar nada
 * Segundo codigo que recebe variaveis para o setting
 * Terceiro codigo que tem as variaveis na memoria
 * Quarto codigo que tem a memoria alterada pela tecla
 * 
 */


#define PAGINA_IR 1

#define botaoIR0 0
#define botaoIR1 1
#define botaoIR1 1
#define botaoIR1 1


NexDSButton Gota1 = NexDSButton(PAGINA_IR, 14, "Gota1");//botao da pagina 1 numero 6 nome numeroGota1
NexDSButton Gota2 = NexDSButton(PAGINA_IR, 11, "setPoint2");//botao da pagina 1 numero 7 nome numeroGota2
NexDSButton Gota3 = NexDSButton(PAGINA_IR, 12, "setPoint3");//botao da pagina 1 numero 8 nome numeroGota3
NexDSButton Gota4 = NexDSButton(PAGINA_IR, 13, "setPoint4");//botao da pagina 1 numero 9 nome numeroGota4


 //Primeiro
#ifndef Irrigacao_h
#define Irrigacao_h
class Irrigacao {
private:
  //variavel que armazena qual pino ligar
  int PinRele0;
  int PinRele1;
  int PinRele2;
  int PinRele3;
  //variavel que armazena qual pino se le
  int PinSensor0;
  int PinSensor1;
  int PinSensor2;
  int PinSensor3;
  //medida media da umidade
  int mediA0;
  int mediA1;
  int mediA2;
  int mediA3;
  //estatus de o que esta ligado
  bool Ligado0;
  bool Ligado1;
  bool Ligado2;
  bool Ligado3;
  int TerraUmida;
  int TerraSECA;
  float Tempo;
  float TempoAnterior0;
  float TempoAnterior1;
  float TempoAnterior2;
  float TempoAnterior3;
  //intervalo para cada medida
  float IntervaloMedida0;
  float IntervaloMedida1;
  float IntervaloMedida2;
  float IntervaloMedida3;
  //intervalo que fica ligado 
  float IntervaloLigado0;
  float IntervaloLigado1;
  float IntervaloLigado2;
  float IntervaloLigado3;

public:

	//nome dos botoes
	const uint8_t Botao0 = 0;
	const uint8_t Botao1 = 1;
	const uint8_t Botao2 = 2;
	const uint8_t Botao3 = 3;


  void SetupRele(int , int , int , int);
  void SetupSensor(int , int, int , int );
  void SetupConst(int, int);
  void SetupCalibragemMAX(int, int);
  void SetupCalibragemMIN(int, int);
  void Medida();
  void Irriga();
  void DesIrriga();
  void LavaTerra(int);
  void IrrigaVaso0(int);
  void IrrigaVaso1(int);
  void IrrigaVaso2(int);
  void IrrigaVaso3(int);
  void MestreSetup();
  void MestreLoop();
};
  void Irrigacao::MestreSetup(){
  //setup das constantes de tempo
  this->TempoAnterior0 = millis();
  this->TempoAnterior1 = millis();
  this->TempoAnterior2 = millis();
  this->TempoAnterior3 = millis();
  this->IntervaloMedida0 = 1000*60*2;//dois minutos
  this->IntervaloMedida1 = 1000*60*2;//dois minutos
  this->IntervaloMedida2 = 1000*60*2;//dois minutos
  this->IntervaloMedida3 = 1000*60*2;//dois minutos
  //
  
  this->SetupRele(15, 16, 17, 18);
  this->SetupSensor(A0, A1, A2, A3);
  //carrega valor inicial a media
  this->mediA0 = 0;
  this->mediA1 = 0;
  this->mediA2 = 0;
  this->mediA3 = 0;
  //
  this->Ligado0 = false;
  this->Ligado1 = false;
  this->Ligado2 = false;
  this->Ligado3 = false;
  //
  //todo calibragem depois
  }
  void Irrigacao::MestreLoop(){
    this->IntervaloMedida0 = 1000*60*2;//dois minutos
    //realiza a medida de umidade
    //
    this->DesIrriga();
    
    this->Medida();
    
    this->Irriga();
    
    }
 
  
  void Irrigacao::Medida(){
  int i;
  
  int media0;
  int media1;
  int media2;
  int media3;
  //faz a medida da umidade
  
  media0 = analogRead(A0);
  media1 = analogRead(A1);
  media2 = analogRead(A2);
  media3 = analogRead(A3);
  //deixa a medida em porcento
  media0 = map(media0, 365, 585, 100, 10);
  
  media1 = map(media1, 365, 585, 100, 10);
  
  media2 = map(media2, 365, 585, 100, 10);
  
  media3 = map(media3, 365, 585, 100, 10);
  //aplica a media movel lidando com o caso que acabou de ligar
if(this->mediA0 == 0)
  {
  this->mediA0 = media0;
  }
  
else{
    this->mediA0 = (this->mediA0*8 + media0*2)/10;  
  }

  if(this->mediA1 == 0)
  {
  this->mediA1 = media1;
  }
  
else{
    this->mediA1 = (this->mediA1*8 + media1*2)/10;  
  }

  if(this->mediA2 == 0)
  {
  this->mediA2 = media2;
  }
  
else{
    this->mediA2 = (this->mediA2*8 + media2*2)/10;  
  }

  if(this->mediA0 == 3)
  {
  this->mediA3 = media3;
  }
  
else{
    this->mediA3 = (this->mediA3*8 + media3*2)/10;  
  }
 //confere se eh necessario irrigar
 //colocar variavel de valor de treshold
  if(this->mediA0 < 80)
  { 
   this->Ligado0 = true;
  }
//colocar variavel de valor de treshold
  if(this->mediA1 < 80)
  { 
   this->Ligado1 = true;
  }
//colocar variavel de valor de treshold  
  if(this->mediA2 < 80)
  { 
   this->Ligado2 = true;
  }
//colocar variavel de valor de treshold
  if(this->mediA3 < 80)
  { 
   this->Ligado3 = true;
  }
  
  }
  
  void Irrigacao::Irriga(){
    if(this->Ligado0 == true)
      {
      digitalWrite(this->PinRele0,HIGH);
      }
    if(this->Ligado1 == true)
      {
      digitalWrite(this->PinRele1,HIGH);
      }

    if(this->Ligado2 == true)
      {
      digitalWrite(this->PinRele2,HIGH);
      }

    if(this->Ligado3 == true)
      {
      digitalWrite(this->PinRele3,HIGH);
      }
    
    }
void Irrigacao::DesIrriga(){
    //if de current milis()
    this->Tempo = millis();
    if(this->Tempo - this->TempoAnterior0 > this->IntervaloMedida0){
      if(this->Ligado0 == true)
      {
      this->Ligado0 = false;
      digitalWrite(this->PinRele0,LOW);
      }
      
      this->TempoAnterior0 = millis();
    }


    
    if(this->Tempo - this->TempoAnterior1 > this->IntervaloMedida1){
      if(this->Ligado1 == true)
      {
      this->Ligado1 = false;
      digitalWrite(this->PinRele1,LOW);
      }
       
      this->TempoAnterior1 = millis();
    }


      
    if(this->Tempo - this->TempoAnterior2 > this->IntervaloMedida2){
      if(this->Ligado2 == true)
      {
      this->Ligado2 = false;
      digitalWrite(this->PinRele2,LOW);
      }
       
      this->TempoAnterior2 = millis();
    }


    
    if(this->Tempo - this->TempoAnterior3 > this->IntervaloMedida3){
      if(this->Ligado3 == true)
      {
      this->Ligado3 = false;
      digitalWrite(this->PinRele3,LOW);
      }
      
      this->TempoAnterior3 = millis();
    }

    
    
    }


void Irrigacao::LavaTerra(int valor){
  //seta a flag de ligado
  this->Ligado0 = true;
  this->Ligado1 = true;
  this->Ligado2 = true;
  this->Ligado3 = true;
  //liga os reles
 //
 //chamar a funcao teclado e pegar um valor
 //
  
  digitalWrite(this->PinRele0,HIGH);
  digitalWrite(this->PinRele1,HIGH);
  digitalWrite(this->PinRele2,HIGH);
  digitalWrite(this->PinRele3,HIGH);
  //receber valor digitado pelo teclado seja por funcao ou por chamar a funcao
  this->IntervaloMedida0 = 2*60*1000; //2 minutos
  this->IntervaloMedida1 = 2*60*1000; //2 minutos
  this->IntervaloMedida2 = 2*60*1000; //2 minutos
  this->IntervaloMedida3 = 2*60*1000; //2 minutos
  }

  
void Irrigacao::SetupRele(int Pino0, int Pino1, int Pino2, int Pino3) {
  this->PinRele0 = Pino0;
  this->PinRele1 = Pino1;
  this->PinRele2 = Pino2;
  this->PinRele3 = Pino3;
  
  pinMode(this->PinRele0,OUTPUT);
  pinMode(this->PinRele1,OUTPUT);
  pinMode(this->PinRele2,OUTPUT);
  pinMode(this->PinRele3,OUTPUT);
  
}


void Irrigacao::SetupSensor( int Pino0 , int Pino1, int Pino2, int Pino3) {
  this->PinSensor0 = Pino0;
  this->PinSensor1 = Pino1;
  this->PinSensor2 = Pino2;
  this->PinSensor3 = Pino3;
  
}
void Irrigacao::IrrigaVaso0(int valor){
  if(this->Ligado0 == true){
    this->Ligado0 = false;
    digitalWrite(this->PinRele0,LOW);
    }
  
  if(this->Ligado0 == false){
    //chama funcao para digitar o valor de tempo ligador
    this->Ligado0 = true;
    digitalWrite(this->PinRele0,HIGH);
    //receber valor por funcao ou chamar teclado
    this->IntervaloMedida0 = valor*60*1000; //2 minutos
    }
}

void Irrigacao::IrrigaVaso1(int valor){
  if(this->Ligado1 == true){
    this->Ligado1 = false;
    digitalWrite(this->PinRele1,LOW);
    //
    }
  
  else if(this->Ligado1 == false){
    //chama funcao para digitar o valor de tempo ligador
    this->Ligado1 = true;
    digitalWrite(this->PinRele1,HIGH);
    //receber valor por funcao ou chamar teclado
    this->IntervaloMedida1 = valor*60*1000; //2 minutos
    }
}

void Irrigacao::IrrigaVaso2(int valor){
  if(this->Ligado2 == true){
    this->Ligado2 = false;
    digitalWrite(this->PinRele2,LOW);
    }
  
  if(this->Ligado2 == false){
    //chama funcao para digitar o valor de tempo ligador
    this->Ligado2 = true;
    digitalWrite(this->PinRele2,HIGH);
    //receber valor por funcao ou chamar teclado
    this->IntervaloMedida2 = valor*60*1000; //2 minutos
    }
}

//dividir em duas funcoes e receber valor
void Irrigacao::IrrigaVaso3(int valor){
  if(this->Ligado3 == true){
    this->Ligado3 = false;
    digitalWrite(this->PinRele3,LOW);
    }
  
  if(this->Ligado3 == false){
    //chama funcao para digitar o valor de tempo ligador
    this->Ligado3 = true;
    digitalWrite(this->PinRele3,HIGH);
    //receber valor por funcao ou chamar teclado
    this->IntervaloMedida3 = valor*60*1000; //2 minutos
    }
}





//provavelmente funcoes Absoletas
void Irrigacao::SetupConst(int endereco1, int endereco2) {
  
  
  /*
  this->TerraUmida = EEPROM.read(endereco1);
  this->TerraSECA = EEPROM.read(endereco2);
  */
}


void Irrigacao::SetupCalibragemMIN(int endereco1, int sensor) {
  /*
  int valor = analogRead(this->PinSensor1);
  EEPROM.write(endereco1,valor);
  this->TerraSECA = valor;
  */
}

void Irrigacao::SetupCalibragemMAX(int endereco2, int sensor) {
  /*
  int valor = analogRead(this->PinSensor1);
  EEPROM.write(endereco2,valor);
  this->TerraUmida = valor;

  */
}


void Irriga0Callback(void* ptr) {
	//botaoAbertado = IR.Botao0;
	//PAGINA = PAGINA_IR;
	//teclado.show();
	
	//colocar pra levar o numero do bot�o para a tela do teclado
}





Irrigacao IR;

#endif 
