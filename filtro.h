#ifndef filtro_h

#define filtro_h

#include <Queue.h>

 class filtro{

   public:
    Queue<float> fila;
    uint8_t tamanho_janela;
    float removido, media_antiga, media_nova, media_auxiliar;

   void tamanho(uint8_t tam){
     this->tamanho_janela = tam;
     this->media_antiga = 0;
     this->media_nova = 0;
     this->media_auxiliar=0;
    }

    void adiciona(float valor){
      this->media_antiga = this->media_nova;
      
      if(this->fila.count() >= this->tamanho_janela){
        this->removido = this->fila.pop();
      }

      this->fila.push(valor);
      this->media_auxiliar = (valor - this->removido)/this->tamanho_janela;
    }

    float calcula(){
      if(this->fila.count() >= this->tamanho_janela){
        this->media_nova = this->media_antiga + this->media_auxiliar;
        return this->media_nova;
      }
      else{
        return this->fila.peek();
      }
    }

 };
#endif
