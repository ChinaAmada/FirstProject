#include <stdio.h>
#include <stdlib.h>

#define DATA_BEGIN      0
#define DATA_BACKUP     256
#define MAX_DATA        512

static unsigned char E2PROM[MAX_DATA];
 /*
data:   input pointer of data
len:    input length of data
return: output result of sumcheck
*/
int CalSum(char* data,int len){
    unsigned char sum=0;
    int i;

    for(i=0;i<len;i++){
        sum += data[i];
    }
    return 0xff-sum;
}


/*

*/
int Check(char* data,int len){
    unsigned char sum=0;
    int i;

    for(i=0;i<len;i++){
        sum += data[i];
    }
    if(0xff == sum)
        return 0;
    else
        return -1;
}

int ReadE2prom(int p,char* data,int len)
{
    unsigned char tmp[2][len+1];

    memcpy(tmp[0],E2PROM+DATA_BEGIN+p,len+1);
    memcpy(tmp[1],E2PROM+DATA_BACKUP+p,len+1);

    if(Check(tmp[0],len+1)==0){    //data is good
        memcpy(data,tmp[0],len);
        return 0;
    }else if(Check(tmp[1],len+1)==0){ //data is bad,but backup is good
        memcpy(data,tmp[1],len);    //return data
        memcpy(E2PROM+DATA_BEGIN,tmp[1],len+1); //back data
        return 0;
    }else{
        return -1;
    }
}

int WriteE2rom(int p,char* data,int len)
{
    unsigned char tmp[len+1];

    if(p<0||p>MAX_DATA||(p+len-1)>MAX_DATA/2)
        return -1;
    memcpy(tmp,data,len);
    tmp[len] = CalSum(data,len);
    memcpy(E2PROM+DATA_BEGIN+p,tmp,len+1);
    memcpy(E2PROM+DATA_BACKUP+p,tmp,len+1);
    return 0;
}

int main()
{
    unsigned char test[5] = {1,2,3,4,5};
    int ret=0;
    int i=0;
    //printf("Hello world!\n");

    ret = WriteE2rom(0,test,5);
    if(ret<0)
        printf("WriteE2rom Error!\n");


    //E2PROM[DATA_BEGIN+1] = 0xff;
    E2PROM[DATA_BACKUP+3] = 0xff;


    ret = ReadE2prom(0,test,5);
    if(ret<0)
        printf("ReadE2prom Error!\n");

    for(i=0;i<5;i++)
        printf("%.2x ",test[i]);
    printf("\n");

    printf("DATA_BEGIN:");
    for(i=0;i<(5+1);i++)
        printf("%.2x ",E2PROM[DATA_BEGIN+i]);
    printf("\n");

    printf("DATA_BACKUP:");
    for(i=0;i<(5+1);i++)
        printf("%.2x ",E2PROM[DATA_BACKUP+i]);
    printf("\n");
    return 0;
}




