#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//���տ�ũ���Ϸ���Ȱ棬����������
//��0������ֹ��1����C3��37����C6��38�����ӳ�

//Ȼ����Щ���������޸ģ��Ŵ������͸������ɱ�
#define M 4//��С��
#define N 16//ʮ��������Ϊ��С��λ
#define T 50000//�Ŵ�����
#define P 30//������

//���������ͣ���Ϊ������������������ũ�������һС��
const char *p="00111-1-1-111-1-111---11111111111-1-1-1-1-1-1---00111-1-1-111-1-";//0Ϊ��ֹ1Ϊ��-Ϊ�ӳ�

struct music{
   int ml[M*N];//����������
   int mh[M*N];//����������
   int f;//��Ӧ�ȵ÷�
};

struct music create(void);//����һ������
int judge(struct music m);//�����ɴ�֣�Խ��Խ�ã�
struct music change(struct music m);//�򻯵ı���

int main(void){//��ȫ�뵥������ͬ
   srand((unsigned)time(NULL));
   struct music m[2*P];//ǰ��ʮ��Ϊ�ױ�������ʮ��Ϊ������
   int i,j,k,l;
   for(i=0;i<P;i++){//������ʼ����
      m[i]=create();
      m[i].f=judge(m[i]);
   }
   for(i=1;i<P;i++){//��ʼ��������
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
   for(i=0;i<T;i++){//�Ŵ����̵ļ�ʵ��
      for(j=0;j<P;j++){//��������
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
   for(i=0;i<P;i++){//��������ǰP����
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

struct music create(void){//���ո�������������
   struct music m;
   int i;
   for(i=0;i<M*N-N;i++){
      if(p[i]=='0') m.ml[i]=0;
      else if(p[i]=='-') m.ml[i]=38;
      else m.ml[i]=rand()%30+1;//��������Ҫ�������������Χ��
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
   //���ڵ���
   for(i=0;i<M*N;i++)
      if(m.ml[i]==2||m.ml[i]==4||m.ml[i]==7||m.ml[i]==9||m.ml[i]==11||m.ml[i]==14||m.ml[i]==16||m.ml[i]==19||m.ml[i]==21||m.ml[i]==23||m.ml[i]==26||m.ml[i]==28||m.ml[i]==31||m.ml[i]==33||m.ml[i]==35) x+=10;
   //��������Ҫ̫�ߣ���������̫�ͣ����鰴ϲ��ѡ��
   for(i=0;i<M*N;i++){
      if(m.ml[i]<38&&m.ml[i]>25) x+=0;
      if(m.mh[i]>0&&m.mh[i]<13) x+=0;
   }
   //����������
   p1=p2=2;
   p2++;
   while(p1<M*N){
      while(p2<M*N&&(p[p2]=='0'||p[p2]=='-')) p2++;
      if(p2==M*N) break;
      if(p2-p1==1){//�����ʱ�����н�ƽ�ȵ�������
         if(m.ml[p2]-m.ml[p1]>2||m.ml[p2]-m.ml[p1]<-2) x+=1;
         else if(m.ml[p2]-m.ml[p1]==0) x+=1;
      }else{//�����ڴ�����ڣ�����������
         if(m.ml[p2]-m.ml[p1]>7||m.ml[p2]-m.ml[p1]<-7) x+=1;
         else if(m.ml[p2]-m.ml[p1]==6||m.ml[p2]-m.ml[p1]==-6) x+=1;
      }
      p1=p2;
      p2++;
   }
   //���ɼ䲻��������
   for(i=N;i<4*N;i++){
      if(m.mh[i]!=0&&m.ml[i]!=0){
         p1=p2=i;
         while(m.mh[p1]==38) p1--;
         while(m.ml[p2]==38) p2--;
         if(m.mh[p1]>m.mh[p2]);
         else x+=1;
      }
   }
   //���ɼ���ͣ�Ϊ��г���̣�����Ϊͬʱ�û��������������г
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
