#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//无终卡农，上方五度版，给定节奏型
//以0代表休止，1代表C3，37代表C6，38代表延长

//然而这些参数不能修改，遗传代数和个体数可变
#define M 4//四小节
#define N 16//十六分音符为最小单位
#define T 50000//遗传代数
#define P 30//个体数

//给定节奏型，此为低声部，高声部按卡农规则后移一小节
const char *p="00111-1-1-111-1-111---11111111111-1-1-1-1-1-1---00111-1-1-111-1-";//0为休止1为音-为延长

struct music{
   int ml[M*N];//低声部旋律
   int mh[M*N];//高声部旋律
   int f;//适应度得分
};

struct music create(void);//产生一段旋律
int judge(struct music m);//对旋律打分（越低越好）
struct music change(struct music m);//简化的变异

int main(void){//完全与单旋律相同
   srand((unsigned)time(NULL));
   struct music m[2*P];//前三十个为亲本，后三十个为变异体
   int i,j,k,l;
   for(i=0;i<P;i++){//创建初始旋律
      m[i]=create();
      m[i].f=judge(m[i]);
   }
   for(i=1;i<P;i++){//初始旋律排序
      for(j=0;j<i;j++){
         if(m[i].f>m[j].f);
         else break;
      }
      struct music t;
      t=m[i];
      for(k=i;k>j;k--)
         m[k]=m[k-1];
      m[j]=t;
   }
   for(i=0;i<T;i++){//遗传过程的简化实现
      for(j=0;j<P;j++){//变异与打分
         m[j+P]=change(m[j]);
         m[j+P].f=judge(m[j+P]);
      }
      for(j=P;j<2*P;j++){
         for(k=0;k<j;k++)
            if(m[j].f>m[k].f);
            else break;
         struct music t;
         t=m[j];
         for(l=j;l>k;l--)
            m[l]=m[l-1];
         m[k]=t;
      }
   }
   for(i=0;i<P;i++){//输出结果（前P个）
      printf("%d\n",m[i].f);
      for(j=0;j<M*N;j++)
         printf(" %2d",m[i].mh[j]);
      printf("\n");
      for(j=0;j<M*N;j++)
         printf(" %2d",m[i].ml[j]);
      printf("\n");
   }
   system("pause");
   return 0;
}

struct music create(void){//按照给定节奏型生成
   struct music m;
   int i;
   for(i=0;i<M*N-N;i++){
      if(p[i]=='0') m.ml[i]=0;
      else if(p[i]=='-') m.ml[i]=38;
      else m.ml[i]=rand()%30+1;//这是由于要求高声部在音域范围内
   }
   for(i=0;i<N;i++) m.ml[M*N-N+i]=m.ml[i];
   for(i=0;i<N;i++) m.mh[i]=0;
   for(i=0;i<3*N;i++)
      if(m.ml[i]==12||m.ml[i]==24) m.mh[N+i]=m.ml[i]+6;
      else if(m.ml[i]==0) m.mh[N+i]=0;
      else if(m.ml[i]==38) m.mh[N+i]=38;
      else m.mh[N+i]=m.ml[i]+7;
   return m;
}

int judge(struct music m){
   int x=0,p1,p2,i;
   //音在调上
   for(i=0;i<M*N;i++)
      if(m.ml[i]==2||m.ml[i]==4||m.ml[i]==7||m.ml[i]==9||m.ml[i]==11||m.ml[i]==14||m.ml[i]==16||m.ml[i]==19||m.ml[i]==21||m.ml[i]==23||m.ml[i]==26||m.ml[i]==28||m.ml[i]==31||m.ml[i]==33||m.ml[i]==35) x+=10;
   //低声部不要太高，高声部不太低，酌情按喜好选择
   for(i=0;i<M*N;i++){
      if(m.ml[i]<38&&m.ml[i]>25) x+=0;
      if(m.mh[i]>0&&m.mh[i]<13) x+=0;
   }
   //单旋律音程
   p1=p2=2;
   p2++;
   while(p1<M*N){
      while(p2<M*N&&(p[p2]=='0'||p[p2]=='-')) p2++;
      if(p2==M*N) break;
      if(p2-p1==1){//快节奏时音程行进平稳但不反复
         if(m.ml[p2]-m.ml[p1]>2||m.ml[p2]-m.ml[p1]<-2) x+=1;
         else if(m.ml[p2]-m.ml[p1]==0) x+=1;
      }else{//否则在纯五度内，不产生增四
         if(m.ml[p2]-m.ml[p1]>7||m.ml[p2]-m.ml[p1]<-7) x+=1;
         else if(m.ml[p2]-m.ml[p1]==6||m.ml[p2]-m.ml[p1]==-6) x+=1;
      }
      p1=p2;
      p2++;
   }
   //旋律间不产生交叉
   for(i=N;i<4*N;i++){
      if(m.mh[i]!=0&&m.ml[i]!=0){
         p1=p2=i;
         while(m.mh[p1]==38) p1--;
         while(m.ml[p2]==38) p2--;
         if(m.mh[p1]>m.mh[p2]);
         else x+=1;
      }
   }
   //旋律间配和，为和谐音程，处理为同时敲击的两个音必须和谐
   for(i=N;i<M*N;i++){
      if(m.ml[i]==0||m.ml[i]==38||m.mh[i]==0||m.mh[i]==38);
      else if(m.mh[i]-m.ml[i]==3||m.mh[i]-m.ml[i]==4||m.mh[i]-m.ml[i]==7||m.mh[i]-m.ml[i]==8||m.mh[i]-m.ml[i]==9||m.mh[i]-m.ml[i]==12||m.mh[i]-m.ml[i]==15||m.mh[i]-m.ml[i]==16);
      else x+=10;
   }
   return x;
}

struct music change(struct music m){
   int x;
   x=rand()%(M*N);
   if(p[x]=='1'){
      m.ml[x]=rand()%30+1;
      if(x<N) m.ml[x+M*N-N]=m.ml[x];
      if(x>=M*N-N&&x<M*N){
         x=x-3*N;
         m.ml[x]=m.ml[x+3*N];
      }
      if(x<3*N){
         if(m.ml[x]==12||m.ml[x]==24) m.mh[x+N]=m.ml[x]+6;
         else m.mh[x+N]=m.ml[x]+7;
      }
   }
   return m;
}
