#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
int s1[1000][1000];
int s2[1000][1000];
int area[1000000][2];
int result[1000][1000];
pthread_t tid[1000000];
int n=10;//weishu
int p=1;//bingfadu
int num;
int sum;
int min;
double start,finish;
void *Thread(void *x)
{
	int *xx = (int*)x;
	for(int i=xx[0];i<=xx[1];i++){
		int row=(i-1)/n;
		int col=(i-1)%n;
		//multip(row,col);
		result[row][col]=0;
		for(int t=0;t<n;t++){
			result[row][col]+=s1[row][t]*s2[t][col];
		}
	}
	if(xx[1]/num<=min){
		int i=n*n-xx[1]/num+1;
 		int row=(i-1)/n;
 		int col=(i-1)%n;
		//multip(row,col);
		result[row][col]=0;
		for(int t=0;t<n;t++){
			result[row][col]+=s1[row][t]*s2[t][col];
		}
	}
}
int main(){
	while(1){
		printf("请输入矩阵的阶数：\n");
	scanf("%d",&n);
	printf("请输入并发度的值：\n");
	scanf("%d",&p);
	srand((unsigned) time(NULL));
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
		  	s1[i][j]=rand()%11;
		  	s2[i][j]=rand()%11;
		}
	}
	printf("\n");
	num=n*n/p;
	sum=1;
	min=n*n%p;
	int r;
	start=clock();
	for(int i=0;i<p;i++){	
		area[i][0]=sum;
		sum=sum+num;
		area[i][1]=sum-1;
  		r=pthread_create(&tid[i],NULL,Thread,(void*)(area[i]));
  		pthread_join(tid[i],NULL);
	}
	finish=clock();
	printf("finished: %lf\n",finish-start);
	}
	
}
