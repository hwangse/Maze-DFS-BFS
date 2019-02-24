#include <stdlib.h>
#include "..\ExternDoc.h"
#include "..\UI\Zoom.h"
#include "..\UI\MsgView.h"
#include "..\Graphics\DrawFunc.h"
#include "..\Example\Example.h"


#define ERROR_NUMBER -1

int height=0,width=0;
int findRouteDFS=0;
int findRouteBFS=0;
int **parent; //BFS에서 사용되는 parent 배열

typedef struct _pos{
	int x;
	int y;
}Pos;

typedef struct _vertex{	
	bool right;
	bool bottom;
	struct _vertex* next;
	Pos p;
}Vertex;

typedef struct _stack{
	Vertex* head;
}Stack;

Stack s,all,allForQ;

typedef struct _queue{
	Vertex* front;
	Vertex* rear;
}Queue;

Vertex **maze=NULL;		//전역으로 선언된 maze정보
//function prototype

static void drawDirect(CDC *pDC);
static void drawBuffered();

void enqueue(Queue* q,int x,int y);
Pos dequeue(Queue *q);
int IsEmptyQueue(Queue *q);
void Spush(Stack* s,int x,int y);
Pos Spop(Stack *s);
int IsEmptyStack(Stack* s);

void DFS();
void BFS();	//전역으로 선언된 maze배열을 탐색한다.
//Start of user code
#include <float.h>

/*****************************************************************
* function	: bool readFile(const char* filename)
* argument	: cons char* filename - filename to be opened
* return	: true if success, otherwise flase
* remark	: After read data file, phisycal view must be set;
*			  otherwise drawing will not be executed correctly.
*			  The window will be invalidated after readFile()
*			  returns true.
******************************************************************/

bool readFile(const char* filename){
	
	height=0;
	width=0;
	//start of the user code
	FILE* fp=fopen(filename,"r");
	int i,j;
	char temp;

	if(!fp) return false;
	else{				//파일이 정상적으로 열렸다면
		//미로의 height와 width 구하기
		while(fscanf(fp,"%c",&temp)!=EOF){
			if(height==0){
				if(temp=='-')
					width++;
				else if(temp=='\n')
					height++;
			}
			else{
				if(temp=='\n')
					height++;
				else
					continue;
			}
		}
		height /= 2;
		//전역으로 선언된 maze를 동적할당 해주기
		maze=(Vertex**)malloc(sizeof(Vertex*)*height);

		for(i=0;i<height;i++)
			maze[i] = (Vertex*)malloc(sizeof(Vertex) * width);

		//////////////////////////////////////////
		fclose(fp);
		fp=fopen(filename,"r");

		for(i=0;i<height*2;i++){
			for(j=0;j<width*2+2;j++){
				fscanf(fp,"%c",&temp);
			if(temp=='\n')
				continue;

			if(!(i%2)){
				if(i==0)//첫번째 가로줄은 무시(미로의 벽)
					continue;
				else{	//bottom check
						if(!(j%2))
							continue;
						else{
							if(temp=='-')
								maze[(i-1)/2][(j-1)/2].bottom=1;
							else 
								maze[(i-1)/2][(j-1)/2].bottom=0;
						}
				}
			}
			else{	//right check
					if(j==0)//첫번째 세로벽은 무시(미로의 벽임)
						continue;
					else if(j%2)//홀수번째 열은 무시
						continue;
					else{
						if(temp=='|')
							maze[i/2][(j-1)/2].right=1;
						else
							maze[i/2][(j-1)/2].right=0;
					}
			}
			}

		}
		for(j=0;j<width;j++)
			maze[height-1][j].bottom=1;
		fclose(fp);
		//////////////////////////////////////////
		setWindow(0,0,width,height,1);
	}
	return true; //edit after finish this function
	//end of usercode
}

/******************************************************************
* function	: bool FreeMemory()
*
* remark	: Save user data to a file
*******************************************************************/
void freeMemory(){
	//start of the user code
	for(int i=0;i<height;i++)
		free(maze[i]);
	free(maze);
	//end of usercode
}

/**************************************************************
* function	: bool writeFile(const char* filename)
*
* argument	: const char* filename - filename to be written
* return	: true if success, otherwise false
* remark	: Save user data to a file
****************************************************************/
bool writeFile(const char* filename){
	//start of the user code
	bool flag;
	flag = 0;

	return flag;
	//end of usercode
}

/************************************************************************
* fucntion	: void drawMain(CDC* pDC)
*
* argument	: CDC* pDC - device context object pointer
* remark	: Main drawing function. Called by CMFC_MainView::OnDraw()
*************************************************************************/
void drawMain(CDC *pDC){
	//if direct drawing is defined
#if defined(GRAPHICS_DIRECT)
	drawDirect(pDC);
	//if buffered drawing is defined
#elif defined(GRAPHICS_BUFFERED)
	drawBuffered();
#endif
}

/************************************************************************
* function	: static void drawDirect(CDC *pDC
*
* argument	: CDC* pDC - device context object pointer
* remark	: Direct drawing routines here.
*************************************************************************/
static void drawDirect(CDC *pDC){
	//begin of user code

	//Nothing to write currently.
	//end of user code
}

/***********************************************************************
* function	: static void drawBuffered()
*
* argument	: CDC* pDC -0 device object pointer
* remark	: Buffered drawing routines here.
************************************************************************/
static void drawBuffered(){
	//start of the user code
	Pos temp,temp2;
	int tempX,tempY,i=0,j=0,k=0,index,m,n;
	bool** flag=(bool**)malloc(sizeof(bool*)*height);
	for(i=0;i<height;i++)
		flag[i]=(bool*)calloc(width,sizeof(bool));


	DrawLine_I(0,0,0,height,5,RGB(100,100,250));
	DrawLine_I(0,0,width,0,5,RGB(100,100,250));
	for(int i=0;i<height;i++)
		for(int j=0;j<width;j++)
		{
			if( maze[i][j].right )
				DrawLine_I(j+1,i,j+1,i+1,5,RGB(100,100,250));
			if( maze[i][j].bottom )
				DrawLine_I(j,i+1,j+1,i+1,5,RGB(100,100,250));
		}

	if(findRouteDFS){ //DFS를 통해 route를 찾았다면
		temp=Spop(&all);

		while(!IsEmptyStack(&all)){
			temp2=Spop(&all);
			DrawLine_I(temp2.x+0.5,temp2.y+0.5,temp.x+0.5,temp.y+0.5,10,RGB(28,32,145));
			temp=temp2;
		}
		
		temp=Spop(&s);
		while(!IsEmptyStack(&s)){
			temp2=Spop(&s);
			DrawLine_I(temp2.x+0.5,temp2.y+0.5,temp.x+0.5,temp.y+0.5,5,RGB(207,62,151));
			temp=temp2;
		}
	}
	findRouteDFS=0;

	if(findRouteBFS){ //BFS를 통해 route를 찾았다면

		k=height-1;
		j=width-1;
		while(1){
			index=parent[k][j];
			tempX=index%width; tempY=index/width;
			DrawLine_I(j+0.5,k+0.5,tempX+0.5,tempY+0.5,10,RGB(28,32,145));
			if(tempX==0 && tempY==0)
				break;
			k=tempY;
			j=tempX;

		}


		for(m=height-1;m>=0;m--){
			for(n=width-1;n>=0;n--){
				k=m;
				j=n;
				while(parent[k][j]!=0){//parent[k][j]!=0
					if(flag[k][j])
						break;
					index=parent[k][j];
					flag[k][j]=true;
					tempX=index%width; tempY=index/width;

					DrawLine_I(j+0.5,k+0.5,tempX+0.5,tempY+0.5,10,RGB(28,32,145));
					k=tempY;
					j=tempX;

					}
				}
		}
		
		k=height-1;
		j=width-1;
		while(1){
			index=parent[k][j];
			tempX=index%width; tempY=index/width;
			DrawLine_I(j+0.5,k+0.5,tempX+0.5,tempY+0.5,5,RGB(207,62,151));
			if(tempX==0 && tempY==0)
				break;
			k=tempY;
			j=tempX;

		}

		for(i=0;i<height;i++)
			free(parent[i]);
		free(parent);
		
	}
	findRouteBFS=0;
	for(i=0;i<height;i++)
		free(flag[i]);
	free(flag);

	//end of the user code
}
void DFS(){
	//Stack s; 
	s.head=NULL;
	//Stack all; 
	all.head=NULL;
	Pos cur,next,temp;
	int i,move=0;
	int **VisitInfo=(int **)calloc(height,sizeof(int *));
	for(i=0;i<height;i++)
		VisitInfo[i]=(int *)calloc(width,sizeof(int));
	findRouteDFS=0;

	cur.x=0; cur.y=0;

	while(1){ //stack 이 비었을 때 break 해주기
		//next Vertex 정보 찾기
		move=0;

		if(cur.x==width-1 && cur.y==height-1){ //끝까지 도달했다면
			findRouteDFS=1;
			Spush(&s,cur.x,cur.y);
			Spush(&all,cur.x,cur.y);
			break;
		}
		//mark 안된 노드 있을 때는 전진, 주변 노드가 모두 방문 노드라면 후진, 
		if(!maze[cur.y][cur.x].right) //오른쪽 벽이 없다면
		{
			next.x=cur.x+1;
			next.y=cur.y;
			if(!VisitInfo[next.y][next.x])
				move=1;
		}
		if(!maze[cur.y][cur.x].bottom && !move){//아래쪽 벽이 없다면
			next.x=cur.x;
			next.y=cur.y+1;
			if(!VisitInfo[next.y][next.x])
				move=1;
		}
		if(cur.x!=0 && !maze[cur.y][cur.x-1].right && !move){//왼쪽 벽이 없다면
			next.x=cur.x-1;
			next.y=cur.y;
			if(!VisitInfo[next.y][next.x])
				move=1;
		}
		if(cur.y!=0 && !maze[cur.y-1][cur.x].bottom  &&!move){//위쪽 벽이 없다면
			next.x=cur.x;
			next.y=cur.y-1;
			if(!VisitInfo[next.y][next.x])
				move=1;
		}	

		if(move){ //움직일 수 있는 상태라면
			Spush(&s,cur.x,cur.y);
			Spush(&all,cur.x,cur.y);
			cur.x=next.x; cur.y=next.y;
			VisitInfo[cur.y][cur.x]=1;
		}

		else{	//움직일 수 없는 상태일때

			Spush(&all,cur.x,cur.y);
			temp=Spop(&s);
			cur.x=temp.x; cur.y=temp.y;

		}
	}

	for(i=0;i<height;i++)
		free(VisitInfo[i]);
	free(VisitInfo);
}
void BFS(){
	Queue q; //linked Queue
	Pos cur,next,temp;
	int i,move=0;
	int **VisitInfo=(int **)calloc(height,sizeof(int *));
	for(i=0;i<height;i++)
		VisitInfo[i]=(int *)calloc(width,sizeof(int));

	parent=(int **)malloc(sizeof(int *)*height);
	for(i=0;i<height;i++)
		parent[i]=(int*)calloc(width,sizeof(int));

	q.front=NULL; q.rear=NULL; //queue initialization

	cur.x=0; cur.y=0;
	VisitInfo[cur.x][cur.y]=1;
	enqueue(&q,cur.x,cur.y);
	findRouteBFS=0;
	
	while(!IsEmptyQueue(&q)) //queue 가 비어있지 않을 동안에
	{
		cur=dequeue(&q);
		move=0;

		if(cur.x==width-1 && cur.y==height-1){
			findRouteBFS=1;
			break;
		}

		//////////움직일 수 있는 경우(move=1)///////////
		if(!maze[cur.y][cur.x].right) //오른쪽 벽이 없다면
		{
			next.x=cur.x+1;
			next.y=cur.y;
			if(!VisitInfo[next.y][next.x]){
					move=1;
					VisitInfo[next.y][next.x]=1;
					enqueue(&q,next.x,next.y);
					parent[next.y][next.x]=cur.y*width+cur.x;
				}
		}
		if(!maze[cur.y][cur.x].bottom){//아래쪽 벽이 없다면
			next.x=cur.x;
			next.y=cur.y+1;
			if(!VisitInfo[next.y][next.x]){
				move=1;
				VisitInfo[next.y][next.x]=1;
				enqueue(&q,next.x,next.y);
				parent[next.y][next.x]=cur.y*width+cur.x;
			}
		}
		if(cur.x!=0 && !maze[cur.y][cur.x-1].right){//왼쪽 벽이 없다면
			next.x=cur.x-1;
			next.y=cur.y;
			if(!VisitInfo[next.y][next.x]){
				move=1;
				VisitInfo[next.y][next.x]=1;
				enqueue(&q,next.x,next.y);
				parent[next.y][next.x]=cur.y*width+cur.x;
			}
		}
		if(cur.y!=0 && !maze[cur.y-1][cur.x].bottom){//위쪽 벽이 없다면
			next.x=cur.x;
			next.y=cur.y-1;
			if(!VisitInfo[next.y][next.x]){
				move=1;
				VisitInfo[next.y][next.x]=1;
				enqueue(&q,next.x,next.y);
				parent[next.y][next.x]=cur.y*width+cur.x;
			}
		}	
		///////움직일 수 없는 경우(move=0)/////////////

		//딱히 해줄거 없음
	}

	for(i=0;i<height;i++)
		free(VisitInfo[i]);
	free(VisitInfo);
}

void enqueue(Queue* q,int x,int y)
{
	Vertex* newVertex=(Vertex*)malloc(sizeof(Vertex));
	Pos* newPos=(Pos*)malloc(sizeof(Pos));
	
	newPos->x=x; newPos->y=y;
	newVertex->p=*(newPos);
	newVertex->next=NULL;

	if(IsEmptyQueue(q)){
		q->front=newVertex;
		q->rear=newVertex;
	}
	else{
		q->rear->next=newVertex;
		q->rear=newVertex;
	}
}
Pos dequeue(Queue *q)
{
	Pos Delete;
	Vertex* Dnode;

	if(IsEmptyQueue(q)){
		printf("Queue is Empty!\n");
		exit(-1);
	}
	Dnode=q->front;
	q->front=q->front->next;
	Delete=Dnode->p;
	free(Dnode);
	
	return Delete;
}
int IsEmptyQueue(Queue *q)
{
	if(!(q->front))
		return 1;
	return 0;
}
void Spush(Stack* s,int x,int y)
{
	Vertex* newVertex=(Vertex*)malloc(sizeof(Vertex));
	Pos* newPos=(Pos*)malloc(sizeof(Pos));
	newPos->x=x; newPos->y=y;
	newVertex->p=*(newPos);
	newVertex->next=NULL;

	if(IsEmptyStack(s))
		s->head=newVertex;
	else{
		newVertex->next=s->head;
		s->head=newVertex;
	}
}
Pos Spop(Stack *s)
{
	Vertex* Delete;
	Pos Dpos;

	if(IsEmptyStack(s)){
		printf("Stack is Empty!\n");
		exit(-1);
	}
	Delete=s->head;
	Dpos=Delete->p;
	s->head=s->head->next;

	return Dpos;
}
int IsEmptyStack(Stack* s)
{
	if(!(s->head))	//if stack is empty
		return 1;
	return 0;
}