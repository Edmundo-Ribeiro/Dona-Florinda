#ifndef filtro_h
#define filtro_h

#include <Queue.h>

 class filtro{

	 public:
		DataQueue<float> fila;
		uint8_t tamanho_janela;
		float removido, media_antiga, media_nova;

	 void tamanho(uint8_t tam){
	 	 this->fila =  DataQueue<float>(tam);
		 this->tamanho_janela = tam;
		 this->media_antiga = 0;
		 this->media_nova = 0;
		 this->removido = 0;
		}

		void adiciona(float valor){
			this->media_antiga = this->media_nova;

			
			if(this->fila.item_count() >= this->tamanho_janela){
				this->removido = this->fila.dequeue();
			}
			this->fila.enqueue(valor);
			this->media_nova = this->media_antiga + (valor - this->removido);
			
			// dbSerialPrint(this->media_nova);
			// dbSerialPrint(" = ");

			// dbSerialPrint(this->media_antiga);
			// dbSerialPrint(" + ");
			// dbSerialPrint(valor);
			// dbSerialPrint(" - ");
			// dbSerialPrintln(this->removido);
			// dbSerialPrintln("\n\n");
		}

		float calcula(){
			return (this->fila.item_count() < this->tamanho_janela)
					? this->fila.front()
					: this->media_nova/this->tamanho_janela;
		}

		void resetar(){	
			for(uint8_t i;i < this->tamanho_janela ;i++)
				this->fila.dequeue();
		}

 };
#endif
