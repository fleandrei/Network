/* TEMPLATE
Basic file to adapt to make your own program.

Allows you to get the board data and play one turn.
*/

#include <stdio.h>
#include <stdlib.h>
#include <networksAPI.h>
#include <unistd.h>


extern int debug;	/* hack to enable debug messages */

typedef enum etat{LIBRE,ADVERSE,NOTRE,ENCOUR}etat;
typedef enum Type{VIDE,HORIZONTAL,VERTICAL,TYP1,TYP2,TYP3}Type;
typedef struct caze{
  int x;
  int y;
  Type typ;
  etat statu;
  struct caze*d;
  struct caze*h;
  struct caze*g;
  struct caze*b;
  int ntc; //Nombre de tour nécéssaire avant d'être capturé.
}caze;


void jouer_soi_meme(char*boardData, t_return_code ret, int player, int x, int y);
//node* init(char*boardData,int x, int y); 


int captur(caze joueur[],int nbrnode,int x, int y,char BoardData[]){
  joueur[nbrnode].x=x;
  joueur[nbrnode].y=y;
  joueur[nbrnode].typ=(int)BoardData[x*y+x];
  if(joueur[nbrnode].typ==5){
    joueur[nbrnode].ntc=2;
    joueur[nbrnode].statu=ENCOUR;
  }else if(joueur[nbrnode].typ==4){
    joueur[nbrnode].ntc=1;
    joueur[nbrnode].statu=ENCOUR;
  }else{
    joueur[nbrnode].ntc=0;
    joueur[nbrnode].statu=NOTRE;
  }

  nbrnode=nbrnode+1;
  return nbrnode;

      }
void affichejoueur(caze joueur[],int nbrnode);

caze* initmap(char BoardData[],int X,int Y);

int main()
{
	debug = 1;
	char boardName[50];					/* name of the board */
	char* boardData;					/* data of the board */
	t_return_code ret = NORMAL_MOVE;	/* indicates the status of the previous move */
	t_move move;						/* a move */
	int player;
	int sizeX, sizeY;
	int nbrnode_j = 0;
	int nbrnode_a = 0;
	int E_j=0;
	int E_a=0;
	/* connection to the server */
	connectToServer("pc4200.polytech.upmc.fr", 1234, "Guillaume_Andrei");
	
	/* wait for a game, and retrieve informations about it */
	waitForBoard("NEO timeout=10 ", boardName, &sizeX, &sizeY);
	boardData = (char*) malloc(sizeX * sizeY);
	player = getBoardData(boardData);
	
	/* display the board */
	printBoard();
	//	jouer_soi_meme( boardData, ret, player, sizeX, sizeY);
	 
	caze* map=initmap(boardData,sizeX,sizeY);
	
	
	caze*nous=(caze*)malloc(sizeof(caze)*(sizeX*sizeY/2));
	caze*advers=(caze*)malloc(sizeof(caze)*(sizeX*sizeY/2));
	if(player==1){ //si l'adverssaire commence
	  E_j=2;
	  nbrnode_a=captur(advers,nbrnode_a,0,0,boardData);
	  nbrnode_j=captur(nous,nbrnode_j,sizeX-1,sizeY-1,boardData);
	}else{
	  E_a=2;
	  nbrnode_a=captur(advers,nbrnode_a,sizeX-1,sizeY-1,boardData);
	  nbrnode_j=captur(nous,nbrnode_j,0,0,boardData);
	}
	affichejoueur(nous,nbrnode_j);
	affichejoueur(advers,nbrnode_a);
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


void affichejoueur(caze joueur[],int nbrnode){
  int i=0;
  if(nbrnode==0){
    printf("Il y a une erreure le joueur ne possède aucun noeud\n");
  }
  for(i=0;i<nbrnode;i++){
    printf("N%d.X=%d\n",i+1,joueur[i].x);  
    printf("N%d.Y=%d\n",i+1,joueur[i].y);
    printf("N%d.statu=%d\n",i+1,joueur[i].statu);
    printf("N%d.type=%d\n",i+1,joueur[i].typ);
  }
}

caze* initmap(char BoardData[],int X,int Y){
  int i=0;
  int N=X*Y;
  caze *map=(caze*)malloc(sizeof(caze)*N);
  for(i=0;i<N;i++){
    map[i].x=i%X;
    map[i].y=i/X;
    map[i].typ=(int)BoardData[i];
    map[i].statu=LIBRE;
  }
  return map;
}
