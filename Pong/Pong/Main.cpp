#include <c2d2\chien2d2.h>
#include <c2d2\chienaudio2.h>

int main(int args, char **argc){
	//inicializações essenciais
	C2D2_Inicia(400, 400, C2D2_JANELA, C2D2_DESENHO_OPENGL, "Pong");	//cria a janela
	CA2_Inicia();
	C2D2_Botao* teclado = C2D2_PegaTeclas();		//possibilita poder reconhecer as teclas do teclado
	C2D2_Mouse* mouse = C2D2_PegaMouse();			//possibilita poder reconhecer os botoes do mouse

	unsigned int bola = C2D2_CarregaSpriteSet("Assets/ball.png", 63, 63);		//carrega a imagem da bola
	int xBola = 100;			//inicializa a posicao de x
	int yBola = 200;			//inicializa a posicao de y
	int speed = 1;				//define a velocidade incial da bola
	int direcaoX = 1;			//define a direcao inicial em x
	int direcaoY = 1;			//define a direcao inicial em y

	int totalBolaFrames = 8;	  //responsavel por verificar o numero de frames
	int atualBolaFrame = 0;		  //responsavel por verificar qual o frame atual
	int tempoBolaTransicao = 10;  //responsavel pelo tempo de troca de cada frame
								  //quando mais próximo de 0, mais rápido
	//carrega a imagem para a mira
	unsigned int mira = C2D2_CarregaSpriteSet("Assets/mira.png", 21, 21);

	//define os dados da barra1 (sprite, posicao em x, e em y e a velocidade
	unsigned int barra1 = C2D2_CarregaSpriteSet("Assets/barra.png", 61, 11);	
	int xBarra1 = 200;			
	int yBarra1 = 0;
	int speedBarra1 = 1;
	//define os dados da barra2 (sprite, posicao em x, e em y e a velocidade
	unsigned int barra2 = C2D2_CarregaSpriteSet("Assets/barra.png", 61, 11);
	int xBarra2 = 200;
	int yBarra2 = 390;
	int speedBarra2 = 1;

	//Carregar Texto
	unsigned int fonte = C2D2_CarregaFonte("Assets/fonte.png", 32);

	//pontuação primeiro jogador
	int scoreJogador1 = 0;
	char scoreJogador1Text[4];
	//pontuação segundo jogador
	int scoreJogador2 = 0;
	char scoreJogador2Text[4];

	//carregar som de batida
	unsigned int batida = CA2_CarregaEfeito("Assets/bate.ogg");

	//permite que o fundo seja de cor azul
	C2D2_TrocaCorLimpezaTela(0,100,255);
	//Update do jogo
	do{
		C2D2_LimpaTela();				//a cada vez que o programa roda, ele limpa a tela, evitando lixo de imagem

		//controle do jogador 1 com a barra 1
		if(teclado[C2D2_D].ativo && xBarra1 <= 339){
			xBarra1 = xBarra1+speedBarra1;
		}else if(teclado[C2D2_A].ativo && xBarra1 >= 0){
			xBarra1 = xBarra1-speedBarra1;
		}
		
		//controle jogador 2 com a barra 2
		if(teclado[C2D2_DIREITA].ativo && xBarra2 <= 339){
			xBarra2 = xBarra2+speedBarra2;
		}else if(teclado[C2D2_ESQUERDA].ativo && xBarra2 >= 0){
			xBarra2 = xBarra2-speedBarra2;
		}
		
		//detectar se houve colisao
		if(C2D2_ColidiuSprites(bola, atualBolaFrame, xBola, yBola, mira, 0, mouse->x, mouse->y)){
				//detectar os botoes do mouse
			if(mouse->botoes[C2D2_MESQUERDO].pressionado){
				C2D2_TrocaCorLimpezaTela(0,255,0);
			}else if(mouse->botoes[C2D2_MDIREITO].pressionado){
				C2D2_TrocaCorLimpezaTela(255, 0, 0);
			}else if(mouse->botoes[C2D2_MMEIO].pressionado){
				C2D2_TrocaCorLimpezaTela(0, 0, 255);
			}
		}


		//verifica se houve colisao da bola com a barra 1
		if(C2D2_ColidiuSprites(bola, 0, xBola, yBola, barra1, 0, xBarra1, yBarra1)){
			direcaoY *= -1;
			speed++;
			CA2_TocaEfeito(batida, CA2_CENTRO);
		}
		//verifica se houve colisao da bola com a barra 2
		if(C2D2_ColidiuSprites(bola, 0, xBola, yBola, barra2, 0, xBarra2, yBarra2)){
			direcaoY *= -1;
			speed++;
			CA2_TocaEfeito(batida, CA2_CENTRO);
		}
		
		//verifica se a bola bateu nas laterais do jogo
		if(xBola <= 0 || xBola >= 347){			//lateral direita esquerda
			direcaoX *=-1;
			if(xBola <=0){
				CA2_TocaEfeito(batida, CA2_ESQUERDA);
			}else if(xBola >= 347){
				CA2_TocaEfeito(batida, CA2_DIREITA);
			}
		}else if(yBola <= -10 || yBola >= 356){	//lateral inferior superior
			if(yBola <= -10){
				scoreJogador2++;
			}else if(yBola >= 356){
				scoreJogador1++;
			}
			xBola = 200;
			yBola = 200;
			speed = 1;
		}

		//a bola sempre esta em movimento
		xBola = xBola+(speed*direcaoX);			
		yBola = yBola+(speed*direcaoY);

		//animação da bola
		tempoBolaTransicao++;
		if(tempoBolaTransicao >= 2){
			atualBolaFrame++;
			if(atualBolaFrame>=totalBolaFrames){
				atualBolaFrame = 0;
			}
			tempoBolaTransicao = 0;
		}

		//Area de desenho. A ordem de desenho influencia
		C2D2_DesenhaSprite(mira, 0, mouse->x, mouse->y);
		C2D2_DesenhaTexto(fonte, 10, 50, "PONG__:D", C2D2_TEXTO_ESQUERDA);
		itoa(scoreJogador1, scoreJogador1Text, 10);
		C2D2_DesenhaTexto(fonte, 10, 85, scoreJogador1Text, C2D2_TEXTO_ESQUERDA);
		itoa(scoreJogador2, scoreJogador2Text, 10);
		C2D2_DesenhaTexto(fonte, 400, 85, scoreJogador2Text, C2D2_TEXTO_DIREITA);
		C2D2_DesenhaSprite(bola, atualBolaFrame, xBola, yBola);
		C2D2_DesenhaSprite(barra1, 0, xBarra1, yBarra1);
		C2D2_DesenhaSprite(barra2, 0, xBarra2, yBarra2);

		//sincroniza o jogo
		C2D2_Sincroniza(C2D2_FPS_PADRAO);
	}while(!teclado[C2D2_ESC].pressionado && !teclado[C2D2_ENCERRA].pressionado);

	CA2_Encerra();
	C2D2_Encerra();
	
	//finaliza o jogo
	return 0;
}