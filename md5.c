#include <stdio.h>
#include <math.h>
unsigned int r[64]={0},k[64]={0};
unsigned int h0 = 0x67452301;
unsigned int h1 = 0xEFCDAB89;
unsigned int h2 = 0x98BADCFE;
unsigned int h3 = 0x10325476;
#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))
void print_arr(unsigned int* arr,int len){
    int i;
    printf("%s\n",__func__);
    for(i=0;i<len;i++)
    {
        if(i>0 && i%16==0 ) 
            printf("\n");

        printf("%08x ",arr[i]);

    }
    printf("\n");
}
void init_r(){
    unsigned int tmp1[][4]={{7,12,17,22},
                {5, 9,14,20},
                {4,11,16,23},
                {6,10,15,21}};
    
    int i,j;
    for(i=0,j=0;i<64;i++){
        if(i>0 && i%16==0 ) j++;

        r[i]=tmp1[j][i%4];
    }
    print_arr(r,64);
}
double fabs(double d){
    return ((d<0) ? -d:d);
}
void init_k(){
    int i;
    unsigned long long int BIG_INT=0x100000000LLU;
    for(i=0;i<64;i++){
        double t = sin((double)(i+1));
        //printf("t %lf \n",t);
        k[i] = (unsigned int)(fabs(t)*BIG_INT); 
    }
    //printf(" %llu \n",BIG_INT);
    print_arr(k,64);
}
void test_enc(){
    unsigned char msg[512/8]={0};
    unsigned int w[16]={0};
    unsigned int tmp,i;
    unsigned int a,b,c,d,f,g;
    a = h0;
    b = h1;
    c = h2;
    d = h3;

    msg[0]=0x31;   //msg
    msg[1]=1<<7;//append '1'

    msg[448]=1;

    for(i=0;i<16;i++)
    {
        tmp = msg[i*4];
        w[i] = tmp<<24;

        tmp = msg[i*4+1];
        w[i] += tmp<<16;

        tmp = msg[i*4+2];
        w[i] += tmp<<8;
        
        w[i] += msg[i*4+3];
    }
    print_arr(w,16);
    for(i=0;i<64;i++){
        if( i >=0 && i<=15){
            f =  (c & b) |( (~b) & d);
            g = i;
        }
        else if( i >=16 && i<=31){
            f =  (d  & b) |( (~d) & c);
            g = ((i*5) + 1) % 16;
        }
        else if( i >=32 && i<=47){
            f = ((b ^ c) ^ d);
            g = ((i*3) + 5) % 16;
        }
        else if( i >=48 && i<=63){
            f = c ^ (b  | (~d));
            g = (i*7) %16 ;
        }
        tmp = d;
        d = c;
        c = b;
        b = b +LEFTROTATE( a + f + k[i] + w[g],r[i]);
        a = tmp;
    }   
    h0+=a;
    h1+=b;
    h2+=c;
    h3+=d;
printf("%x %x %x %x \n",h0,h1,h2,h3);
}
int main(int argc,char** argv)
{
    init_r();
    init_k();
    test_enc();
    return 0;
}
