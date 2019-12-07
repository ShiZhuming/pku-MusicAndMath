#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//二音对一音基本对位的写作，这相当于给定了节奏型
//以0代表休止，1代表C3，37代表C6，38代表延长

#define M 4//四小节
#define N 8//八分音符为最小单位
#define T 50000//遗传代数
#define P 30//个体数

struct music{
   int ml[M*N];//低声部旋律，取为固定调
   int mh[M*N];//高声部旋律，取为对位旋律
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

struct music create(void){//高声部高于低声部由生成函数和变异函数保证，但适应度函数中仍将给出限制
   struct music m;
   int i;
   for(i=0;i<M*N/2;i++){
      m.ml[2*i]=rand()%37+1;
      m.ml[2*i+1]=38;
   }
   for(i=0;i<M*N-1;i++){
      m.mh[i]=rand()%(38-m.ml[(i/2)*2])+m.ml[(i/2)*2];
   }
   m.mh[M*N-1]=38;
   m.mh[0]=0;
   return m;
}

int judge(struct music m){
   int x=0,i;
   //音在调上
   for(i=0;i<M*N/2;i++)
      if(m.ml[2*i]==2||m.ml[2*i]==4||m.ml[2*i]==7||m.ml[2*i]==9||m.ml[2*i]==11||m.ml[2*i]==14||m.ml[2*i]==16||m.ml[2*i]==19||m.ml[2*i]==21||m.ml[2*i]==23||m.ml[2*i]==26||m.ml[2*i]==28||m.ml[2*i]==31||m.ml[2*i]==33||m.ml[2*i]==35) x+=1;
   for(i=1;i<M*N-1;i++)
      if(m.mh[i]==2||m.mh[i]==4||m.mh[i]==7||m.mh[i]==9||m.mh[i]==11||m.mh[i]==14||m.mh[i]==16||m.mh[i]==19||m.mh[i]==21||m.mh[i]==23||m.mh[i]==26||m.mh[i]==28||m.mh[i]==31||m.mh[i]==33||m.mh[i]==35) x+=1;
   //单个旋律的旋律音程
   for(i=0;i<M*N/2-1;i++){
      if(m.ml[2*i]-m.ml[2*i+2]>9||m.ml[2*i]-m.ml[2*i+2]<-9) x+=1;
   }
   for(i=1;i<M*N-1;i++){//高声部更平稳
      if(m.mh[i]-m.mh[i+1]>4||m.mh[i]-m.mh[i+1]<-4) x+=1;
   }
   //双声部间的配和，强音和谐
   for(i=1;i<M*N/2-1;i++){
      if(m.mh[2*i]-m.ml[2*i]==3||m.mh[2*i]-m.ml[2*i]==4||m.mh[2*i]-m.ml[2*i]==7||m.mh[2*i]-m.ml[2*i]==8||m.mh[2*i]-m.ml[2*i]==9||m.mh[2*i]-m.ml[2*i]==15||m.mh[2*i]-m.ml[2*i]==16||m.mh[2*i]-m.ml[2*i]==19||m.mh[2*i]-m.ml[2*i]==20||m.mh[2*i]-m.ml[2*i]==21||m.mh[2*i]-m.ml[2*i]==27||m.mh[2*i]-m.ml[2*i]==28||m.mh[2*i]-m.ml[2*i]==31||m.mh[2*i]-m.ml[2*i]==32||m.mh[2*i]-m.ml[2*i]==33);
      else x+=1;
   }
   //高声部不要太低以及低声部不要太高
   for(i=1;i<M*N-1;i++)
      if(m.mh[i]<13) x+=1;
   for(i=0;i<M*N/2;i++)
      if(m.ml[2*i]>25) x+=1;
   //以下部分来自某基础乐理书。不知理由。
   //不能使用平行五度行进（避免产生空虚感）
   for(i=1;i<M*N/2-2;i++){
      if((m.mh[2*i]-m.ml[2*i]==7||m.mh[2*i]-m.ml[2*i]==19||m.mh[2*i]-m.ml[2*i]==31)&&(m.mh[2*i+2]-m.ml[2*i+2]==7||m.mh[2*i+2]-m.ml[2*i+2]==19||m.mh[2*i+2]-m.ml[2*i+2]==31)) x+=1;
   }
   //对位声部避免出现同音反复和连续模进（旋律单调）
   //同音反复
   for(i=1;i<M*N-2;i++){
      if(m.mh[i]==m.mh[i+1]) x+=1;
   }
   //连续模进
   for(i=1;i<M*N/2-2;i++){
      if(m.mh[2*i]-m.ml[2*i]==m.mh[2*i+2]-m.ml[2*i+2]&&m.mh[2*i]-m.ml[2*i]==m.mh[2*i+4]-m.ml[2*i+4]) x+=1;
   }
   //开头要求
   if(m.mh[1]-m.ml[0]==0||m.mh[1]-m.ml[0]==7||m.mh[1]-m.ml[0]==12);
   else x+=1;
   //结尾要求
   //以和谐音程进入中止式
   if(m.mh[M*N-2]-m.ml[M*N-2]==0||m.mh[M*N-2]-m.ml[M*N-2]==12||m.mh[M*N-2]-m.ml[M*N-2]==24||m.mh[M*N-2]-m.ml[M*N-2]==36);
   else x+=1;
   if(m.mh[M*N-3]-m.ml[M*N-4]==3||m.mh[M*N-3]-m.ml[M*N-4]==4||m.mh[M*N-3]-m.ml[M*N-4]==7||m.mh[M*N-3]-m.ml[M*N-4]==8||m.mh[M*N-3]-m.ml[M*N-4]==9||m.mh[M*N-3]-m.ml[M*N-4]==15||m.mh[M*N-3]-m.ml[M*N-4]==16||m.mh[M*N-3]-m.ml[M*N-4]==19||m.mh[M*N-3]-m.ml[M*N-4]==20||m.mh[M*N-3]-m.ml[M*N-4]==21||m.mh[M*N-3]-m.ml[M*N-4]==27||m.mh[M*N-3]-m.ml[M*N-4]==28||m.mh[M*N-3]-m.ml[M*N-4]==31||m.mh[M*N-3]-m.ml[M*N-4]==32||m.mh[M*N-3]-m.ml[M*N-4]==33);
   else x+=1;
   //中止在主音上
   if(m.mh[M*N-2]!=13&&m.mh[M*N-2]!=25) x+=1;
   //以下是一些骚操作
   //反向行进
   for(i=1;i<M*N/2-1;i++){
      if((m.mh[2*i]-m.mh[2*i+2])*(m.ml[2*i]-m.ml[2*i+2])>=0) x+=1;
   }
   //模仿
   for(i=N/2;i<M*N/2-1;i++){
      if((m.mh[2*i]-m.mh[2*i+2])*(m.ml[2*i-N]-m.ml[2*i+2-N])<0) x+=1;
   }
   return x;
}

struct music change(struct music m){
   int x;
   x=rand()%2;
   if(x==0){
      x=rand()%(M*N/2);
      if(m.mh[2*x]<m.mh[2*x+1]&&m.mh[2*x]!=0)
         m.ml[2*x]=rand()%m.mh[2*x]+1;
      else m.ml[2*x]=rand()%m.mh[2*x+1];
   }else{
      x=rand()%(M*N-2)+1;
      m.mh[x]=rand()%(38-m.ml[(x/2)*2])+m.ml[(x/2)*2];
   }
   return m;
}
