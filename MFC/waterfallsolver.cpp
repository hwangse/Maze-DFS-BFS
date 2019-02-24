#include "stdafx.h"
#include "mylist.h"
#include "waterfallsolver.h"

#include <stdio.h>
#include <stdlib.h>

Line *mLine = NULL;
Point *mPoint = NULL;
int mLine_num;
int mPoint_num;


mylist* m_flow_point = NULL;

bool init_state = false;
bool draw_state = false;
bool sele_state = false;
//////////////////////////
int  curr_point = 0;

void init_data(void){
	mLine=NULL;
	mPoint=NULL;
	mLine_num=0;
	mPoint_num=0;

	m_flow_point=NULL;
////////////////////////////////////
	init_state=true;
	draw_state=false;
	sele_state=false;
	curr_point=0;

}

void data_read(LPCTSTR fname){
	FILE* file;
	int i, tempx, tempy;

	file=fopen(fname,"r");
	fscanf(file,"%d",&mLine_num);

	////////////에러발생가능//////////
	mLine=(Line *)malloc(sizeof(Line)*mLine_num);

	for(i=0;i<mLine_num;i++)
		fscanf(file,"%d %d %d %d",&mLine[i].xl,&mLine[i].yl,&mLine[i].xr,&mLine[i].yr);

	fscanf(file,"%d",&mPoint_num);
	mPoint=(Point *)malloc(sizeof(Point)*mPoint_num);

	for(i=0;i<mPoint_num;i++)
		fscanf(file,"%f %f",&mPoint[i].x,&mPoint[i].y);

	fclose(file);


}

/* 문제는 물 그릴때 푼다. 아래의 drawWaterflow() 내에서 호출한다. */
void waterfall_Solver(void){
	Point S; 
	Line *minP=NULL, *P;
	int i, j;
	int flag=0;
	float tslew, ty, cy, minlv=MAX, templv,m,n,smallerY,level=0;
	
	if(init_state==false) return;
	
	if(m_flow_point!=NULL) delete m_flow_point;
	m_flow_point = new mylist();

	S.x = mPoint[curr_point].x; 
	S.y = mPoint[curr_point].y;
	m_flow_point->add(&S);

	for(i=0; i<mLine_num; i++){ 
		//P=&mLine[i];
		minP=NULL;
		minlv=MAX;
		/*차가 가장 작은 선분 구하기*/
		for(j=0;j<mLine_num;j++){
			P=&mLine[j];
			smallerY=P->yl<P->yr?P->yl:P->yr;
			if(S.x>P->xl && S.x<P->xr && S.y>smallerY )
			{
				tslew=(float)(P->yl-P->yr)/(float)(P->xl-P->xr); //선분의 기울기구하기
				n=((float)P->yl)-tslew*((float)P->xl); //y=mx+n에서 n 값 구하기
				level=S.y-(tslew*(float)S.x+n); //선분과 점 사이의 거리

				if(level<minlv)
				{
					minlv=level;
					minP=P;
					flag=1;
				}
			}
	
		
			else
				continue;
		}
	
		if(minP!=NULL) //차가 가장 작은 선분이 존재할 떄
		{
			P=minP;
			S.y=S.y-minlv;
			m_flow_point->add(&S); //만나는 좌표 배열에 넣어주기

			if(tslew>0) //선분의 기울기가 양수라면
			{
				S.x=(float)P->xl;
				S.y=(float)P->yl;
			}
			else //선분의 기울기가 음수라면
			{
				S.x=(float)P->xr;
				S.y=(float)P->yr;
			} 
			m_flow_point->add(&S);
		}
		else
			continue;
		

	}
	S.y = 0; m_flow_point->add(&S);
}

void drawBackground(CDC* pDC){
	int i; CPen MyPen; 

	if(init_state==false) return;

	MyPen.CreatePen(PS_SOLID,10,RGB(51,0,180));
	pDC->SelectObject(MyPen);
	pDC->MoveTo(50,100);pDC->LineTo(550,100);
	pDC->MoveTo(50,500);pDC->LineTo(550,500);
	MyPen.DeleteObject();

	MyPen.CreatePen(PS_SOLID,5,RGB(140,51,0));
	pDC->SelectObject(MyPen);
	for(i=0;i<mLine_num;i++)
	{
		pDC->MoveTo(mLine[i].xl*20+50,500-mLine[i].yl*20);pDC->LineTo(mLine[i].xr*20+50,500-mLine[i].yr*20);
	}
	MyPen.DeleteObject();

	MyPen.CreatePen(PS_SOLID,5,RGB(0,0,0));
	pDC->SelectObject(MyPen);
	for(i=0;i<mPoint_num;i++)
		pDC->Ellipse(mPoint[i].x*20+50,500-mPoint[i].y*20,mPoint[i].x*20+55,500-mPoint[i].y*20+5);
	MyPen.DeleteObject();

	
}

void drawStartPoint(CDC* pDC){
	CPen MyPen;
	int i;
	if(init_state==false) return;
	if(sele_state==false) return;

	MyPen.CreatePen(PS_SOLID,5,RGB(255,0,0));
	pDC->SelectObject(MyPen);
	pDC->Ellipse(mPoint[curr_point].x*20+50,500-mPoint[curr_point].y*20,mPoint[curr_point].x*20+55,500-mPoint[curr_point].y*20+5);
	MyPen.DeleteObject();

}

void drawWaterflow(CDC* pDC){
	node* temp;
	CPen MyPen;
	if(init_state==false) return;
	if(sele_state==false) return;
	if(draw_state==false) return;

	if(m_flow_point==NULL) return; //list가 비었다면
	else
	{
		temp=m_flow_point->move_first();
		MyPen.CreatePen(PS_SOLID,5,RGB(0,0,255));
		pDC->SelectObject(MyPen);
	
		while(temp->next!=NULL){
			pDC->MoveTo(temp->point->x*20+50,500-temp->point->y*20);
			pDC->LineTo(temp->next->point->x*20+50,500-temp->next->point->y*20);
			temp=m_flow_point->move_next();
		}
		MyPen.DeleteObject();
	}
	
}
//start Point를 빨간색으로 변경
void change_start_point(void){


	if(sele_state==false)
		{
			sele_state=true;
			curr_point=0;
	}
	else
	{
		curr_point++;
		if(curr_point==mPoint_num)
		{
			curr_point=0;		
		}

	}

}

void enable_drawWaterflow(bool mEnable){
	if(init_state==false) return;
	draw_state = mEnable;
}