#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//������һ��������λ��д�������൱�ڸ����˽�����
//��0������ֹ��1����C3��37����C6��38�����ӳ�

#define M 4//��С��
#define N 8//�˷�����Ϊ��С��λ
#define T 50000//�Ŵ�����
#define P 30//������

struct music{
   int ml[M*N];//���������ɣ�ȡΪ�̶���
   int mh[M*N];//���������ɣ�ȡΪ��λ����
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

struct music create(void){//���������ڵ����������ɺ����ͱ��캯����֤������Ӧ�Ⱥ������Խ���������
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
   //���ڵ���
   for(i=0;i<M*N/2;i++)
      if(m.ml[2*i]==2||m.ml[2*i]==4||m.ml[2*i]==7||m.ml[2*i]==9||m.ml[2*i]==11||m.ml[2*i]==14||m.ml[2*i]==16||m.ml[2*i]==19||m.ml[2*i]==21||m.ml[2*i]==23||m.ml[2*i]==26||m.ml[2*i]==28||m.ml[2*i]==31||m.ml[2*i]==33||m.ml[2*i]==35) x+=1;
   for(i=1;i<M*N-1;i++)
      if(m.mh[i]==2||m.mh[i]==4||m.mh[i]==7||m.mh[i]==9||m.mh[i]==11||m.mh[i]==14||m.mh[i]==16||m.mh[i]==19||m.mh[i]==21||m.mh[i]==23||m.mh[i]==26||m.mh[i]==28||m.mh[i]==31||m.mh[i]==33||m.mh[i]==35) x+=1;
   //�������ɵ���������
   for(i=0;i<M*N/2-1;i++){
      if(m.ml[2*i]-m.ml[2*i+2]>9||m.ml[2*i]-m.ml[2*i+2]<-9) x+=1;
   }
   for(i=1;i<M*N-1;i++){//��������ƽ��
      if(m.mh[i]-m.mh[i+1]>4||m.mh[i]-m.mh[i+1]<-4) x+=1;
   }
   //˫���������ͣ�ǿ����г
   for(i=1;i<M*N/2-1;i++){
      if(m.mh[2*i]-m.ml[2*i]==3||m.mh[2*i]-m.ml[2*i]==4||m.mh[2*i]-m.ml[2*i]==7||m.mh[2*i]-m.ml[2*i]==8||m.mh[2*i]-m.ml[2*i]==9||m.mh[2*i]-m.ml[2*i]==15||m.mh[2*i]-m.ml[2*i]==16||m.mh[2*i]-m.ml[2*i]==19||m.mh[2*i]-m.ml[2*i]==20||m.mh[2*i]-m.ml[2*i]==21||m.mh[2*i]-m.ml[2*i]==27||m.mh[2*i]-m.ml[2*i]==28||m.mh[2*i]-m.ml[2*i]==31||m.mh[2*i]-m.ml[2*i]==32||m.mh[2*i]-m.ml[2*i]==33);
      else x+=1;
   }
   //��������Ҫ̫���Լ���������Ҫ̫��
   for(i=1;i<M*N-1;i++)
      if(m.mh[i]<13) x+=1;
   for(i=0;i<M*N/2;i++)
      if(m.ml[2*i]>25) x+=1;
   //���²�������ĳ���������顣��֪���ɡ�
   //����ʹ��ƽ������н��������������У�
   for(i=1;i<M*N/2-2;i++){
      if((m.mh[2*i]-m.ml[2*i]==7||m.mh[2*i]-m.ml[2*i]==19||m.mh[2*i]-m.ml[2*i]==31)&&(m.mh[2*i+2]-m.ml[2*i+2]==7||m.mh[2*i+2]-m.ml[2*i+2]==19||m.mh[2*i+2]-m.ml[2*i+2]==31)) x+=1;
   }
   //��λ�����������ͬ������������ģ�������ɵ�����
   //ͬ������
   for(i=1;i<M*N-2;i++){
      if(m.mh[i]==m.mh[i+1]) x+=1;
   }
   //����ģ��
   for(i=1;i<M*N/2-2;i++){
      if(m.mh[2*i]-m.ml[2*i]==m.mh[2*i+2]-m.ml[2*i+2]&&m.mh[2*i]-m.ml[2*i]==m.mh[2*i+4]-m.ml[2*i+4]) x+=1;
   }
   //��ͷҪ��
   if(m.mh[1]-m.ml[0]==0||m.mh[1]-m.ml[0]==7||m.mh[1]-m.ml[0]==12);
   else x+=1;
   //��βҪ��
   //�Ժ�г���̽�����ֹʽ
   if(m.mh[M*N-2]-m.ml[M*N-2]==0||m.mh[M*N-2]-m.ml[M*N-2]==12||m.mh[M*N-2]-m.ml[M*N-2]==24||m.mh[M*N-2]-m.ml[M*N-2]==36);
   else x+=1;
   if(m.mh[M*N-3]-m.ml[M*N-4]==3||m.mh[M*N-3]-m.ml[M*N-4]==4||m.mh[M*N-3]-m.ml[M*N-4]==7||m.mh[M*N-3]-m.ml[M*N-4]==8||m.mh[M*N-3]-m.ml[M*N-4]==9||m.mh[M*N-3]-m.ml[M*N-4]==15||m.mh[M*N-3]-m.ml[M*N-4]==16||m.mh[M*N-3]-m.ml[M*N-4]==19||m.mh[M*N-3]-m.ml[M*N-4]==20||m.mh[M*N-3]-m.ml[M*N-4]==21||m.mh[M*N-3]-m.ml[M*N-4]==27||m.mh[M*N-3]-m.ml[M*N-4]==28||m.mh[M*N-3]-m.ml[M*N-4]==31||m.mh[M*N-3]-m.ml[M*N-4]==32||m.mh[M*N-3]-m.ml[M*N-4]==33);
   else x+=1;
   //��ֹ��������
   if(m.mh[M*N-2]!=13&&m.mh[M*N-2]!=25) x+=1;
   //������һЩɧ����
   //�����н�
   for(i=1;i<M*N/2-1;i++){
      if((m.mh[2*i]-m.mh[2*i+2])*(m.ml[2*i]-m.ml[2*i+2])>=0) x+=1;
   }
   //ģ��
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
