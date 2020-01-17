#ifndef filtro_h
#define filtro_h

#include <Queue.h>

 class filtro{

	 public:
		Queue<float> fila;
		uint8_t tamanho_janela;
		float removido, media_antiga, media_nova;

	 void tamanho(uint8_t tam){
		 this->tamanho_janela = tam;
		 this->media_antiga = 0;
		 this->media_nova = 0;
		 this->removido = 0;
		}

		void adiciona(float valor){
			this->media_antiga = this->media_nova;//0
			
			this->fila.push(valor);//push(2)

			if(this->fila.count() >= this->tamanho_janela){
				this->removido = this->fila.pop();
			}

			this->media_nova = this->media_antiga + valor - this->removido;
		}

		float calcula(){
			if(this->fila.count() < this->tamanho_janela)
				return fila.peek();
			else
				return this->media_nova/ this->tamanho_janela;
		}

 };
#endif
