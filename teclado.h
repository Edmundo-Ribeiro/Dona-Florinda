#ifndef teclado_h
#define teclado_h


NexPage teclado = NexPage(2,0,"Teclado");
NexButton confirma = NexButton(2, 4, "confirma");
NexText entrada = NexText(2,13,"numeroTeclado");
NexText mensagem = NexText(2,15,"mensagem");

void PassaBotaoParaTela(uint32_t bufferValor){
	char buffer[5];
	teclado.show();
	sprintf(buffer, "%d",bufferValor);//passar para string
	entrada.setText(buffer);
}

void PassaTextoParaTela(char* buffer){
	teclado.show();
	entrada.setText(buffer);
}
#endif