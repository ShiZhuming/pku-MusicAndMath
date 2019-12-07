#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//单声部旋律的写作
//以0代表休止，1代表C3，37代表C6，38代表延长

#define M 4//四小节
#define N 8//八分音符为最小单位
#define T 50000//遗传代数
#define P 30//个体数
#define X 400//延音符占比，然而果然不人工给定节奏型就会炸

struct music{
   int m[M*N];//旋律
   int f;//适应度得分
};

struct music create(void);//产生一段旋律
int judge(struct music m);//对旋律打分（越低越好）
struct music change(struct music m);//简化的变异

int main(void){
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
         printf(" %2d",m[i].m[j]);
      printf("\n");
   }
   system("pause");
   return 0;
}

struct music create(void){//随机生成
   struct music m;
   int i,j;
   m.m[0]=rand()%37+1;
   for(i=1;i<M*N;i++){
      j=rand()%X;
      if(j==0) m.m[i]=38;
      else m.m[i]=rand()%37+1;
   }
   m.m[N*M-1]=38;
   return m;
}

int judge(struct music m){//适应度函数
   int x=0,i,j,k,p1,p2,p3,p4;
   //首先是音在调上，此处采用C大调（相当于限制只能出现C大调音）
   for(i=0;i<M*N;i++)
      if(m.m[i]==2||m.m[i]==4||m.m[i]==7||m.m[i]==9||m.m[i]==11||m.m[i]==14||m.m[i]==16||m.m[i]==19||m.m[i]==21||m.m[i]==23||m.m[i]==26||m.m[i]==28||m.m[i]==31||m.m[i]==33||m.m[i]==35) x+=1;
   //这里可以采用五声音阶版（使用时注释掉上面的大调版，五声音阶版有民乐特色。。。）
   //for(i=0;i<M*N;i++)
   //   if(m.m[i]==38||m.m[i]==1||m.m[i]==3||m.m[i]==5||m.m[i]==8||m.m[i]==10||m.m[i]==13||m.m[i]==15||m.m[i]==17||m.m[i]==20||m.m[i]==22||m.m[i]==25||m.m[i]==27||m.m[i]==29||m.m[i]==32||m.m[i]==34||m.m[i]==37);
   //   else x+=1;
   //旋律音程（此处只做很少的要求，即跨度不宜过大，以及不出现增四减五）
   p1=p2=0;
   p2++;
   while(p1<M*N){
      while(m.m[p2]==38&&p2<M*N) p2++;
      if(p2==M*N) break;
      if(m.m[p2]-m.m[p1]>9||m.m[p2]-m.m[p1]<-9) x+=2;
      else if(m.m[p2]-m.m[p1]>7||m.m[p2]-m.m[p1]<-7) x+=1;
      else if(m.m[p2]-m.m[p1]==6||m.m[p2]-m.m[p1]==-6) x+=2;
      else if(m.m[p2]-m.m[p1]==0||m.m[p2]-m.m[p1]==0) x+=0;
      else if(m.m[p2]-m.m[p1]>4||m.m[p2]-m.m[p1]<-4) x+=0;
      p1=p2;
      p2++;
   }
   //对于旋律的控制，暂时没有太好的办法，将就如下，即每小节有按占比个延音符
   for(i=0;i<M;i++){
      k=0;
      for(j=N*i;j<N*i+N;j++)
         if(m.m[j]==38) k++;
      if(k>N/X) x+=k-N/X;
      else x+=N/X-k;
   }
   //结尾的中止式，终止于主音（这是一个暴力限制。。。）方便起见把它加到生成函数里去
   if(m.m[M*N-1]==38){
      p1=M*N-2;
      while(m.m[p1]==38) p1--;
      if(m.m[p1]==13||m.m[p1]==25);
      else x+=5;
   }else x+=5;
   //结尾中止式，希望协和进入或者倾向进入（这并不必要。。。）
   p1=M*N-1;
   while(p1>0&&m.m[p1]==38) p1--;
   p2=p1;
   p1--;
   while(p1>0&&m.m[p1]==38) p1--;
   if(p1>=0){
      if(m.m[p1]-m.m[p2]==1||m.m[p1]-m.m[p2]==-1||m.m[p1]-m.m[p2]==4||m.m[p1]-m.m[p2]==-4||m.m[p1]-m.m[p2]==3||m.m[p1]-m.m[p2]==-3);
      else x+=1;
   }
   //延长音的调性，长音在大小调的123级音上（这也是一个暴力限制，甚至可以不要）
   p1=0;
   while(p1<M*N-2){
      while(p1<M*N-2&&(m.m[p1]==38||(m.m[p1+1]!=38||m.m[p1+2]!=38))) p1++;
      if(p1==M*N-2) break;
      if(m.m[p1]==8||m.m[p1]==10||m.m[p1]==13||m.m[p1]==17||m.m[p1]==20||m.m[p1]==22||m.m[p1]==25||m.m[p1]==29||m.m[p1]==32||m.m[p1]==34);
      else x+=5;
      p1++;
   }
   //旋律的相似性，马尔可夫矩阵的尝试失败了。所以用这个比较强行的方法，即对旋律行进做出一定要求
   //第二小节与第一小节反向行进（后面的。。不要了，需要时可调整其加分分数，它们被用在复调中）
   p1=p2=0;
   p3=p4=N;
   while(p4<(M/2)*N&&m.m[p4]==38) p4++;
   p3=p4;
   p4++;
   p2++;
   while(p3<(M/2)*N){
      while(p2<(M/4)*N&&m.m[p2]==38) p2++;
      if(p2==(M/4)*N) break;
      while(p4<(M/2)*N&&m.m[p4]==38) p4++;
      if(p4==(M/2)*N) break;
      if((m.m[p1]-m.m[p2])*(m.m[p3]-m.m[p4])>0) x+=0;
      p3=p4;
      p1=p2;
      p4++;
      p2++;
   }
   //后两小节与前两小节基本形状一致
   p1=p2=0;
   p2++;
   p3=p4=(M/2)*N;
   while(p4<M*N&&m.m[p4]==38) p4++;
   p3=p4;
   p4++;
   while(p3<M*N){
      while(p2<(M/2)*N&&m.m[p2]==38) p2++;
      if(p2==(M/2)*N) break;
      while(p4<M*N&&m.m[p4]==38) p4++;
      if(p4==M*N) break;
      if((m.m[p4]-m.m[p3])*(m.m[p2]-m.m[p1])<0) x+=0;
      p3=p4;
      p4++;
      p1=p2;
      p2++;
   }
   //后两小节与前两小节节奏相似
   for(i=0;i<(M/2)*N;i++){
      if(m.m[i]==38&&m.m[i+(M/2)*N]!=38) x+=0;
   }
   return x;
}

struct music change(struct music m){//单音突变
   int i,j;
   i=rand()%(M*N-1);
   if(i==0)
      m.m[0]=rand()%37+1;
   else{
      j=rand()%X;
      if(j==0) m.m[i]=38;
      else m.m[i]=rand()%37+1;
   }
   return m;
}

