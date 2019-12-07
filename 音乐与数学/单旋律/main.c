#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//���������ɵ�д��
//��0������ֹ��1����C3��37����C6��38�����ӳ�

#define M 4//��С��
#define N 8//�˷�����Ϊ��С��λ
#define T 50000//�Ŵ�����
#define P 30//������
#define X 400//������ռ�ȣ�Ȼ����Ȼ���˹����������;ͻ�ը

struct music{
   int m[M*N];//����
   int f;//��Ӧ�ȵ÷�
};

struct music create(void);//����һ������
int judge(struct music m);//�����ɴ�֣�Խ��Խ�ã�
struct music change(struct music m);//�򻯵ı���

int main(void){
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
         printf(" %2d",m[i].m[j]);
      printf("\n");
   }
   system("pause");
   return 0;
}

struct music create(void){//�������
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

int judge(struct music m){//��Ӧ�Ⱥ���
   int x=0,i,j,k,p1,p2,p3,p4;
   //���������ڵ��ϣ��˴�����C������൱������ֻ�ܳ���C�������
   for(i=0;i<M*N;i++)
      if(m.m[i]==2||m.m[i]==4||m.m[i]==7||m.m[i]==9||m.m[i]==11||m.m[i]==14||m.m[i]==16||m.m[i]==19||m.m[i]==21||m.m[i]==23||m.m[i]==26||m.m[i]==28||m.m[i]==31||m.m[i]==33||m.m[i]==35) x+=1;
   //������Բ����������װ棨ʹ��ʱע�͵�����Ĵ���棬�������װ���������ɫ��������
   //for(i=0;i<M*N;i++)
   //   if(m.m[i]==38||m.m[i]==1||m.m[i]==3||m.m[i]==5||m.m[i]==8||m.m[i]==10||m.m[i]==13||m.m[i]==15||m.m[i]==17||m.m[i]==20||m.m[i]==22||m.m[i]==25||m.m[i]==27||m.m[i]==29||m.m[i]==32||m.m[i]==34||m.m[i]==37);
   //   else x+=1;
   //�������̣��˴�ֻ�����ٵ�Ҫ�󣬼���Ȳ��˹����Լ����������ļ��壩
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
   //�������ɵĿ��ƣ���ʱû��̫�õİ취���������£���ÿС���а�ռ�ȸ�������
   for(i=0;i<M;i++){
      k=0;
      for(j=N*i;j<N*i+N;j++)
         if(m.m[j]==38) k++;
      if(k>N/X) x+=k-N/X;
      else x+=N/X-k;
   }
   //��β����ֹʽ����ֹ������������һ���������ơ�������������������ӵ����ɺ�����ȥ
   if(m.m[M*N-1]==38){
      p1=M*N-2;
      while(m.m[p1]==38) p1--;
      if(m.m[p1]==13||m.m[p1]==25);
      else x+=5;
   }else x+=5;
   //��β��ֹʽ��ϣ��Э�ͽ������������루�Ⲣ����Ҫ��������
   p1=M*N-1;
   while(p1>0&&m.m[p1]==38) p1--;
   p2=p1;
   p1--;
   while(p1>0&&m.m[p1]==38) p1--;
   if(p1>=0){
      if(m.m[p1]-m.m[p2]==1||m.m[p1]-m.m[p2]==-1||m.m[p1]-m.m[p2]==4||m.m[p1]-m.m[p2]==-4||m.m[p1]-m.m[p2]==3||m.m[p1]-m.m[p2]==-3);
      else x+=1;
   }
   //�ӳ����ĵ��ԣ������ڴ�С����123�����ϣ���Ҳ��һ���������ƣ��������Բ�Ҫ��
   p1=0;
   while(p1<M*N-2){
      while(p1<M*N-2&&(m.m[p1]==38||(m.m[p1+1]!=38||m.m[p1+2]!=38))) p1++;
      if(p1==M*N-2) break;
      if(m.m[p1]==8||m.m[p1]==10||m.m[p1]==13||m.m[p1]==17||m.m[p1]==20||m.m[p1]==22||m.m[p1]==25||m.m[p1]==29||m.m[p1]==32||m.m[p1]==34);
      else x+=5;
      p1++;
   }
   //���ɵ������ԣ�����ɷ����ĳ���ʧ���ˡ�����������Ƚ�ǿ�еķ��������������н�����һ��Ҫ��
   //�ڶ�С�����һС�ڷ����н�������ġ�����Ҫ�ˣ���Ҫʱ�ɵ�����ӷַ��������Ǳ����ڸ����У�
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
   //����С����ǰ��С�ڻ�����״һ��
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
   //����С����ǰ��С�ڽ�������
   for(i=0;i<(M/2)*N;i++){
      if(m.m[i]==38&&m.m[i+(M/2)*N]!=38) x+=0;
   }
   return x;
}

struct music change(struct music m){//����ͻ��
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

