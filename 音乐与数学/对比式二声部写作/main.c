#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//对比式二声部写作

#define T 50000//遗传代数
#define P 30//个体数
#define M 4//小节数
#define N 16//十六分音符为最小单位
#define X 6//大致的延长音占比

struct music{
   int l[M*N];//低声部
   int h[M*N];//高声部
   int f;
};

struct music create(void);//创建旋律
int judge(struct music m);//评价旋律
struct music change(struct music m);//变异

int main(void){
   srand((unsigned)time(NULL));
   struct music m[2*P];
   for(int i=0;i<P;i++){//创建初始旋律
      m[i]=create();
      m[i].f=judge(m[i]);
   }
   for(int j=1;j<P;j++){//排序
      int k;
      for(k=0;k<j;k++)
         if(m[j].f>m[k].f);
         else break;
      struct music l=m[j];
      for(int r=j-1;r>=k;r--)
         m[r+1]=m[r];
      m[k]=l;
   }
   for(int i=0;i<T;i++){//遗传算法
      for(int j=0;j<P;j++){
         m[j+P]=change(m[j]);
         m[j+P].f=judge(m[j+P]);
      }
      for(int j=0;j<P;j++){
         int k;
         for(k=0;k<P+j;k++){
            if(m[j+P].f>m[k].f);
            else break;
         }
         struct music l=m[j+P];
         for(int r=j+P-1;r>=k;r--)
            m[r+1]=m[r];
         m[k]=l;
      }
   }

   for(int i=0;i<P;i++){//输出
      for(int j=0;j<M*N;j++) printf(" %2d",m[i].l[j]);
      printf("\n");
      for(int j=0;j<M*N;j++) printf(" %2d",m[i].h[j]);
      printf("\n%d\n",m[i].f);
   }

   return 0;
}

struct music create(void){
   struct music m;
   int i;
   m.l[0]=0;//弱起小节
   m.l[1]=rand()%37+1;
   for(i=2;i<M*N-3;i++){
      m.l[i]=rand()%38+1;
   }
   m.l[M*N-3]=m.l[M*N-2]=m.l[M*N-1]=38;//结尾
   for(i=0;i<=N;i++) m.h[i]=0;
   if(m.l[N+1]==38) m.h[N+1]=rand()%37+1;
   else m.h[N+1]=m.l[N+1]+rand()%(38-m.l[N+1]);
   for(i=N+2;i<M*N-3;i++){
         if(m.l[i]==38) m.h[i]=1+rand()%38;
         else m.h[i]=m.l[i]+rand()%(39-m.l[i]);
   }
   m.h[M*N-3]=m.h[M*N-2]=m.h[M*N-1]=38;
   return m;
}

int judge(struct music m){
   int x=0,p1,p2,i,j,k,p3,p4;
   p1=p2=1;
   p2++;
   while(p1<M*N-4){//低声部旋律分
      while(p2<=M*N-4){
         if(m.l[p2]==38) p2++;
         else break;
      }
      if(p2-p1==1){//连续十六分音符应当平稳行进
         if(m.l[p2]-m.l[p1]==0) x+=1;
         else if(m.l[p2]-m.l[p1]==1||m.l[p2]-m.l[p1]==2||m.l[p2]-m.l[p1]==-1||m.l[p2]-m.l[p1]==-2) x+=0;
         else if(m.l[p2]-m.l[p1]==3||m.l[p2]-m.l[p1]==4||m.l[p2]-m.l[p1]==-3||m.l[p2]-m.l[p1]==-4) x+=1;
         else if(m.l[p2]-m.l[p1]==5||m.l[p2]-m.l[p1]==7||m.l[p2]-m.l[p1]==-5||m.l[p2]-m.l[p1]==-7) x+=2;
         else if(m.l[p2]-m.l[p1]==8||m.l[p2]-m.l[p1]==9||m.l[p2]-m.l[p1]==-8||m.l[p2]-m.l[p1]==-9) x+=2;
         else x+=5;
      }else{//否则和声音程
         if(m.l[p2]-m.l[p1]==0||m.l[p2]-m.l[p1]==1||m.l[p2]-m.l[p1]==2||m.l[p2]-m.l[p1]==-1||m.l[p2]-m.l[p1]==-2||m.l[p2]-m.l[p1]==3||m.l[p2]-m.l[p1]==4||m.l[p2]-m.l[p1]==-3||m.l[p2]-m.l[p1]==-4) x+=0;
         else if(m.l[p2]-m.l[p1]==5||m.l[p2]-m.l[p1]==7||m.l[p2]-m.l[p1]==-5||m.l[p2]-m.l[p1]==-7||m.l[p2]-m.l[p1]==8||m.l[p2]-m.l[p1]==9||m.l[p2]-m.l[p1]==-8||m.l[p2]-m.l[p1]==-9) x+=1;
         else x+=3;
      }
      p1=p2;
      p2++;
   }
   p1=p2=N+1;
   p2++;
   while(p1<M*N-4){//高声部旋律分，基本同上
      while(p2<=M*N-4){
         if(m.h[p2]==38) p2++;
         else break;
      }
      if(p2-p1==1){
         if(m.h[p2]-m.h[p1]==0) x+=1;
         else if(m.h[p2]-m.h[p1]==1||m.h[p2]-m.h[p1]==2||m.h[p2]-m.h[p1]==-1||m.h[p2]-m.h[p1]==-2) x+=0;
         else if(m.h[p2]-m.h[p1]==3||m.h[p2]-m.h[p1]==4||m.h[p2]-m.h[p1]==-3||m.h[p2]-m.h[p1]==-4) x+=1;
         else if(m.h[p2]-m.h[p1]==5||m.h[p2]-m.h[p1]==7||m.h[p2]-m.h[p1]==-5||m.h[p2]-m.h[p1]==-7) x+=2;
         else if(m.h[p2]-m.h[p1]==8||m.h[p2]-m.h[p1]==9||m.h[p2]-m.h[p1]==-8||m.h[p2]-m.h[p1]==-9) x+=2;
         else x+=5;
      }else{
         if(m.h[p2]-m.h[p1]==0||m.h[p2]-m.h[p1]==1||m.h[p2]-m.h[p1]==2||m.h[p2]-m.h[p1]==-1||m.h[p2]-m.h[p1]==-2||m.h[p2]-m.h[p1]==3||m.h[p2]-m.h[p1]==4||m.h[p2]-m.h[p1]==-3||m.h[p2]-m.h[p1]==-4) x+=0;
         else if(m.h[p2]-m.h[p1]==5||m.h[p2]-m.h[p1]==7||m.h[p2]-m.h[p1]==-5||m.h[p2]-m.h[p1]==-7||m.h[p2]-m.h[p1]==8||m.h[p2]-m.h[p1]==9||m.h[p2]-m.h[p1]==-8||m.h[p2]-m.h[p1]==-9) x+=1;
         else x+=3;
      }
      p1=p2;
      p2++;
   }
   for(i=0;i<(M-2)*N/2-1;i++){//强拍和谐音程，弱拍甚至允许旋律交叉，这里对中间片段有效
      j=k=N+2+2*i;
      while(m.h[j]==38) j--;
      while(m.l[k]==38) k--;
      if(m.h[j]-m.l[k]==0||m.h[j]-m.l[k]==3||m.h[j]-m.l[k]==4||m.h[j]-m.l[k]==7||m.h[j]-m.l[k]==8||m.h[j]-m.l[k]==9||m.h[j]-m.l[k]==12||m.h[j]-m.l[k]==3+12||m.h[j]-m.l[k]==4+12||m.h[j]-m.l[k]==7+12||m.h[j]-m.l[k]==8+12||m.h[j]-m.l[k]==9+12||m.h[j]-m.l[k]==24||m.h[j]-m.l[k]==3+24||m.h[j]-m.l[k]==4+24||m.h[j]-m.l[k]==7+24||m.h[j]-m.l[k]==8+24||m.h[j]-m.l[k]==9+24\
         ||m.h[j]-m.l[k]==-3||m.h[j]-m.l[k]==-4||m.h[j]-m.l[k]==-7||m.h[j]-m.l[k]==-8||m.h[j]-m.l[k]==-9||m.h[j]-m.l[k]==-12||m.h[j]-m.l[k]==-3-12||m.h[j]-m.l[k]==-4-12||m.h[j]-m.l[k]==-7-12||m.h[j]-m.l[k]==-8-12||m.h[j]-m.l[k]==-9-12||m.h[j]-m.l[k]==-24||m.h[j]-m.l[k]==-3-24||m.h[j]-m.l[k]==-4-24||m.h[j]-m.l[k]==-7-24||m.h[j]-m.l[k]==-8-24||m.h[j]-m.l[k]==-9-24) x+=0;
      else if(m.h[j]-m.l[k]==5||m.h[j]-m.l[k]==5+12||m.h[j]-m.l[k]==5+24||m.h[j]-m.l[k]==-5||m.h[j]-m.l[k]==-5-12||m.h[j]-m.l[k]==-5-24) x+=1;
      else x+=5;
   }
   for(i=(M-1)*N;i<M*N-3;i++){//结束和谐，所有音程均和谐以中止
      j=k=i;
      while(m.h[j]==38) j--;
      while(m.l[k]==38) k--;
      if(m.h[j]-m.l[k]==0||m.h[j]-m.l[k]==3||m.h[j]-m.l[k]==4||m.h[j]-m.l[k]==7||m.h[j]-m.l[k]==8||m.h[j]-m.l[k]==9||m.h[j]-m.l[k]==12||m.h[j]-m.l[k]==3+12||m.h[j]-m.l[k]==4+12||m.h[j]-m.l[k]==7+12||m.h[j]-m.l[k]==8+12||m.h[j]-m.l[k]==9+12||m.h[j]-m.l[k]==24||m.h[j]-m.l[k]==3+24||m.h[j]-m.l[k]==4+24||m.h[j]-m.l[k]==7+24||m.h[j]-m.l[k]==8+24||m.h[j]-m.l[k]==9+24\
         ||m.h[j]-m.l[k]==-3||m.h[j]-m.l[k]==-4||m.h[j]-m.l[k]==-7||m.h[j]-m.l[k]==-8||m.h[j]-m.l[k]==-9||m.h[j]-m.l[k]==-12||m.h[j]-m.l[k]==-3-12||m.h[j]-m.l[k]==-4-12||m.h[j]-m.l[k]==-7-12||m.h[j]-m.l[k]==-8-12||m.h[j]-m.l[k]==-9-12||m.h[j]-m.l[k]==-24||m.h[j]-m.l[k]==-3-24||m.h[j]-m.l[k]==-4-24||m.h[j]-m.l[k]==-7-24||m.h[j]-m.l[k]==-8-24||m.h[j]-m.l[k]==-9-24) x+=0;
      else x+=5;
   }
   //结束在主音上
   if(m.l[M*N-4]!=1||m.l[M*N-4]!=13||m.l[M*N-4]!=25) x+=5;
   if(m.h[M*N-4]!=37||m.h[M*N-4]!=13||m.h[M*N-4]!=25) x+=5;
   for(i=0;i<M;i++){//疏密对比
      int r,t;
      r=t=0;
      for(j=N*i;j<N*i+N;j++){
         if(m.l[j]==38) r++;
         if(m.h[j]==38) t++;
      }//这里还需要略做修改才能适应可变M
      if(i==0){
         if(r>4) x+=(r-4)*10;
         else x+=(4-r)*10;
      }else if(i==1){
         if(r>8) x+=(r-8)*10;
         else x+=(8-r)*10;
         if(t>4) x+=(t-4)*10;
         else x+=(4-t)*10;
      }else if(i==2){
         if(r>4) x+=(r-4)*10;
         else x+=(4-r)*10;
         if(t>8) x+=(t-8)*10;
         else x+=(8-t)*10;
      }else{
         if(r>7) x+=(r-7)*10;
         else x+=(7-r)*10;
         if(t>7) x+=(t-7)*10;
         else x+=(7-t)*10;
      }
   }
   //上下行模仿
   p1=p2=N+1;
   p2++;
   p3=p4=1;
   p4++;
   while(p1<M*N-4&&p3<M*N-4){
      while(p2<=M*N-4){
         if(m.h[p2]==38) p2++;
         else break;
      }
      while(p4<=M*N-4){
         if(m.l[p4]==38) p4++;
         else break;
      }
      if((m.h[p2]-m.h[p1])*(m.l[p4]-m.l[p3])<0) x+=1;//同向模仿
      p1=p2;
      p2++;
      p3=p4;
      p4++;
   }
   //高低声部范围
   for(i=0;i<M*N-3;i++){
      if(m.h[i]!=0||m.h[i]!=38){
         if(m.h[i]<13) x+=1;
      }
      if(m.l[i]!=0||m.l[i]!=38){
         if(m.l[i]>25) x+=1;
      }
   }
   //调式音
   for(i=0;i<M*N-3;i++){
      if(m.h[i]!=0||m.h[i]!=38){
         if(m.h[i]==2||m.h[i]==4||m.h[i]==7||m.h[i]==9||m.h[i]==11||m.h[i]==14||m.h[i]==16||m.h[i]==19||m.h[i]==21||m.h[i]==23||m.h[i]==26||m.h[i]==28||m.h[i]==31||m.h[i]==33||m.h[i]==35) x+=3;
      }
      if(m.l[i]!=0||m.l[i]!=38){
         if(m.l[i]==2||m.l[i]==4||m.l[i]==7||m.l[i]==9||m.l[i]==11||m.l[i]==14||m.l[i]==16||m.l[i]==19||m.l[i]==21||m.l[i]==23||m.l[i]==26||m.l[i]==28||m.l[i]==31||m.l[i]==33||m.l[i]==35) x+=3;
      }
   }
   return x;
}

struct music change(struct music m){
   int x,y;
   x=rand()%2;
   if(x==0){//随机修改高声部
      y=N+1+rand()%(M*N-3-N-1);
      if(y==N+1){
         if(m.l[N+1]==38) m.h[N+1]=rand()%37+1;
         else m.h[N+1]=m.l[N+1]+rand()%(38-m.l[N+1]);
      }else{
            if(m.l[y]==38) m.h[y]=rand()%38+1;
            else m.h[y]=m.l[y]+rand()%(39-m.l[y]);
      }
   }else{
      y=1+rand()%(M*N-3-1);
      if(y==1) m.l[1]=rand()%37+1;
      else{
            if(m.h[y]==38||m.h[y]==0) m.l[y]=rand()%38+1;
            else{
               m.l[y]=rand()%m.h[y]+2;
               if(m.l[y]==m.h[y]+1) m.l[y]=38;
            }
      }
   }
   return m;
}

