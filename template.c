/* TEMPLATE
Basic file to adapt to make your own program.

Allows you to get the board data and play one turn.
*/

#include <stdio.h>
#include <stdlib.h>
#include <networksAPI.h>
#include <unistd.h>


extern int debug;	/* hack to enable debug messages */

typedef enum etat{libre,adverse,notre,encour}etat;

typedef struct node{
  int x;
  int y;
  int type;
  etat statu;
  struct node*d;
  struct node*h;
  struct node*g;
  struct node*b;
  int ntc; //Nombre de tour nécéssaire avant d'être capturé.
}node;


void jouer_soi_meme(char*boardData, t_return_code ret, int player, int x, int y);
//node* init(char*boardData,int x, int y); 

int captur(node joueur[],int nbrnode,int x, int y,char BoardData[]); 


int main()
{
	debug = 1;
	char boardName[50];					/* name of the board */
	char* boardData;					/* data of the board */
	t_return_code ret = NORMAL_MOVE;	/* indicates the status of the previous move */
	t_move move;						/* a move */
	int player;
	int sizeX, sizeY;

	/* connection to the server */
	connectToServer("pc4200.polytech.upmc.fr", 1234, "Guillaume_Andrei");
	
	/* wait for a game, and retrieve informations about it */
	waitForBoard("NEO timeout=3600 ", boardName, &sizeX, &sizeY);
	boardData = (char*) malloc(sizeX * sizeY);
	player = getBoardData(boardData);
	
	/* display the board */
	printBoard();
	//	jouer_soi_meme( boardData, ret, player, sizeX, sizeY);
	 

	
	
	node*nous=(node*)malloc(sizeof(node)*(sizeX*sizeY/2));
	node*advers=(node*)malloc(sizeof(node)*(sizeX*sizeY/2));
	
	  while(ret == NORMAL_MOVE){
	/* opponent turn */

	if (player == 1)
	{
		ret = getMove(&move);
		player = 0;
	}
	/* your turn */
	else
	{
		move.type = DO_NOTHING;
		move.x = 0;
		move.y = 0;
		ret = sendMove(move);
		player = 1;
	}
	printBoard();
	}


	if (ret == NORMAL_MOVE) {
	    printf("The game should continue\n");
	    }
	else if ((player == 1 && ret == WINNING_MOVE) || (player == 0 && ret == LOOSING_MOVE))
		printf("I lose the game :(\n");
	else
	printf("I win the game :)\n");
	
	/* we do not forget to free the allocated array */
	free(boardData);
	
	/* end the connection, because we are polite */
	closeConnection();
	
	return EXIT_SUCCESS;
}


void jouer_soi_meme(char*boardData, t_return_code ret, int player, int x, int y)
{
  char m;
  t_move move;
  while(ret == NORMAL_MOVE){
    	/* opponent turn */
    printf(" l = %d, h = %d \n", x, y);
    if (player == 1)
	{
		ret = getMove(&move);
		player = 0;
	}
	/* your turn */
    else
	{
	  printf("A vous de jouer! Quelle action? \n");
	  scanf(" %c", &m);
	  printf("m = %c, \n",m);
	  if(m =='n'){
	  move.type = DO_NOTHING;
	  move.x = 0;
	  move.y = 0;
	  }else if(m=='c'){
	    move.type = CAPTURE;
	    printf("x = ?\n");
	    scanf("%d", &(move.x));
	    printf("y = ?\n");
	    scanf("%d", &(move.y));
	  }else if(m=='d'){
	    move.type = DESTROY;
	    printf("x = ?\n");
	    scanf("%d", &(move.x));
	    printf("y = ?\n");
	    scanf("%d", &(move.y));
	  }else if(m=='h'){
	    move.type = LINK_H;
	    printf("x = ?\n");
	    scanf("%d", &(move.x));
	    printf("y = ?\n");
	    scanf("%d", &(move.y));
	  }else if(m=='v'){
	    move.type = LINK_V;
	    printf("x = ?\n");
	    scanf("%d", &(move.x));
	    printf("y = ?\n");
	    scanf("%d", &(move.y));
	  }
	  ret = sendMove(move);
	  player = 1;
	}	
	printBoard();
	
  }
}

/*node* init(char*boardData,int x, int y){
  node *tete=(node*)malloc(sizeof(node));
  tete->x=x;
  tete->y=y;
  tete->type=(int)boardData[]
*/

int captur(node joueur[],int nbrnode,int x, int y,char BoardData){
  joueur[nbrnode].x=x;
  joueur[nbrnode].y=y;
  joueur[nbrnode].type=(int)BoardData[x*y+x];
  if(joueur[nbrnode].type==5){
    joueur[nbrnode].ntc=2;
    joueur[nbrnode].statu=encour;
  }else if(joueur[nbrnode].type==4){
    joueur[nbrnode].ntc=1;
    joueur[nbrnode].statu=encour;
  }else{
    joueur[nbrnode].ntc=0;
    joueur[nbrnode].statu=notre;
  }
  nbrnode=nbrnode+1;
  return nbrnode;

      }
  
  
