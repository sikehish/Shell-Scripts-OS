//FCFS Algorithm Scheduling


#include <stdio.h>
#include <stdlib.h>

typedef struct process
{
 int ID,AT,BT,CT,TAT,WT;
}pro;

pro P[15];


void swap(pro *a,pro *b)
{
   pro temp=*a;
   *a=*b;
   *b=temp;
    return ;
}


void sort(int n)
{
 for(int i=0;i<n-1;i++)
 {
  for(int j=0;j<n-i-1;j++)
  {
   if(P[j].AT>P[j+1].AT)
      swap(&P[j],&P[j+1]);
  }
 }
}

void PrintGanttChart(int n)
{
 printf("-----------------------\n");
 for(int i=0;i<n;i++)
 { 
  if(P[i].AT > P[i-1].CT)
    {
     printf("|Idle(%d)%d",P[i].AT-P[i-1].CT,P[i].AT);
    }
  printf("|P%d(%d)%d",P[i].ID,P[i].BT,P[i].CT);
 }
 printf("\n");
 printf("-----------------------\n");
}


void main()
{
 int n,total_WT=0,total_TAT=0;
 float avg_WT=0,avg_TAT=0;
 printf("\n Enter the Number of Processes :\n");
 scanf("%d",&n);
 printf("\n enter the arrival time and burst time of the Processes :\n");
 for(int i=0;i<n;i++)
 {
        printf("Process P%d :\n",i+1);
	P[i].ID=(i+1);
	printf("AT:");
        scanf("%d",&P[i].AT);
	printf("BT:");
        scanf("%d",&P[i].BT);
        printf("\n");
 }
 sort(n);
//finding the completion time of all the processes

P[0].WT=0;
P[0].CT = P[0].AT+P[0].BT;
 
	for(int i=1;i<n;i++)
 	 {
 	  if(P[i].AT>P[i-1].CT)
	   {
            P[i].CT = P[i-1].CT+(P[i].AT-P[i-1].CT)+P[i].BT;
            continue;
           }
           P[i].CT = P[i-1].CT+P[i].BT;
         }


//finding the turn around time 
for(int i=0;i<n;i++)
{
   if(P[i].AT >P[i-1].CT)
   {
     P[i].TAT=P[i].CT - P[i].AT ;
     total_TAT +=P[i].TAT;
     P[i].WT=0;
     continue;
   }
   P[i].TAT=P[i].CT -P[i].AT;
   total_TAT +=P[i].TAT;
   P[i].WT=P[i].TAT - P[i].BT;
   total_WT+=P[i].WT;
}
avg_TAT=(float)total_TAT/n;
avg_WT=(float)total_WT/n;
//printing the table of Processes with details 

printf("PID\tAT\tBT\tCT\tTAT\tWT\n");

for(int i= 0;i<n;i++)
{
 printf("%d\t%d\t%d\t%d\t%d\t%d\n",P[i].ID,P[i].AT,P[i].BT,P[i].CT,P[i].TAT,P[i].WT);
}

printf("\n Average TAT =%.2f\n Average WT=%.2f\n",avg_TAT,avg_WT);

printf("\n Gantt Chart \n\n");

PrintGanttChart(n);

}




