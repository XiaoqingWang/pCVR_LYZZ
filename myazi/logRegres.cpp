#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>
#include "matrix.h"
#include <fstream>
#include <sstream>
#include <stack>
using namespace std;
#define MAX 1000000
#define MIN -100000


struct creativeID
{
    int id;
    int sum;
    int ssum;
    double pr;
    struct creativeID *next;
    int len;
};

///ͳ��ÿ��ʱ��ε������ĸ��ʣ�ֱ�Ӵ洢�ڱ�����
double clicktime[24]= {0.0246654,
                       0.0301104,
                       0.028314,
                       0.0229519,
                       0.259183,
                       0.0264976,
                       0.0245146,
                       0.0217758,
                       0.0231793,
                       0.0384133,
                       0.0339939,
                       0.0304794,
                       0.0255811,
                       0.0233522,
                       0.0227103,
                       0.0248416,
                       0.0237889,
                       0.0227326,
                       0.0246128,
                       0.0263791,
                       0.0270367,
                       0.0247819,
                       0.0261875,
                       0.0244826
                      };
///ͳ�Ʋ�ͬ������ʽ�������ĸ��ʣ�ֱ�Ӵ洢���ڴ�
double connection[5]= {0.00479176,
                       0.02966,
                       0.008357,
                       0.00831307,
                       0.00684698
                      };
///ͳ�Ʋ�ͬ��Ӫ�̵������ĸ���
double telecoms[4]= {0.0324223,
                     0.0218315,
                     0.0322667,
                     0.0261772
                    };
/**
sigmad����,����û��ֱ�ӽ�Ԥ�����ת��Ϊ���ͣ����ǻ��Ƿ���һ��doubleֵ
*/
double sigmoid(double z)
{
    return 1.0/(1+exp(-z));
}
/**
�ݶ��½��㷨����Ҫ��ȷ�����ݶȷ��򣬲��������õ�����˼�����n��������
��Ŀ�꺯����͹�滮���⣬��ô�ֲ���Сֵ����ȫ����Сֵ

**/
int gradAscent(Matrix x,Matrix y)
{
    Matrix weights;
    weights.initMatrix(&weights,x.row,1,1);///��ʼ���Ż���������Ϊ1

    Matrix xT;
    xT.initMatrix(&xT,x.row,x.col);
    xT.transposematrix(x,&xT);

    Matrix z;
    z.initMatrix(&z,x.col,1);

    Matrix w1;
    w1.initMatrix(&w1,x.row,y.row);

    double alpha=0.01;///��������
    double error;///��¼������
    int k,c=0;
    int i,j;
    double loss=0;
    for(c=0; c<1000000; c++)
    {
        z.multsmatrix(&z,x,weights);

        for(i=0; i<x.col; i++)
            z.mat[i][0]=sigmoid(z.mat[i][0]);///Ԥ�����
        loss=0;
        for(i=0; i<x.col; i++)
        {
            if(y.mat[i][0]==1)
            {
                loss+=log(z.mat[i][0]);
            }
            if(y.mat[i][0]==0)
            {
                loss+=log(1-z.mat[i][0]);
            }
        }
        cout<<"loss="<<loss/z.col<<endl;

        z.submatrix(&z,y,z);///���㸺�ݶȷ���ͬʱ������ΪԤ���������ʵ�������

        error=0;
        for(k=0; k<x.col; k++)///ͳ�ƴ�����
            error+=z.mat[k][0];
        cout<<"error="<<error<<endl;
        if(error<2&&error>-2)///���ô�����С��һ��ֵʱ�˳�����
            break;
        w1.multsmatrix(&w1,xT,z);///���㸺�ݶȷ���
        for(j=0; j<x.row; j++)
            w1.mat[j][0]*=alpha;///���ݶȷ����벽���ĳ˻�ȷ������ֵ
        weights.addmatrix(&weights,weights,w1);///�����ݶȷ�����һ������
        cout<<"weights"<<endl;
        weights.print(weights);
    }
    int er=0;
    for(i=0; i<x.col; i++)
    {
        er+=y.mat[i][0];
    }
    cout<<"label1sum="<<er<<endl;
    /**
    ��֤�㷨����ȷ��
    **/
    double sum=0;
    Matrix test;
    test.initMatrix(&test,y.col,y.row);
    test.multsmatrix(&test,x,weights);
    for(i=0; i<y.col; i++)
    {
        if(test.mat[i][0]>0)
        {
            sum+=1-y.mat[i][0];
        }

        else
        {
            sum+=y.mat[i][0]-0;
        }

    }
    cout<<"sum="<<sum<<endl;
}
/**
����ݶ��½����ݶ��½�����ͬ�����ڸ��ݶȷ����ȷ�����ݶ��½��Ǹ������е�������ȷ�����ݶȷ���
������ݶ��½�ÿ��ֻ��һ����������ȷ�����ݶȷ�����Ȼ����ȫ���ţ������ŵ����������ӣ�ͬ������

**/
int stoGradAscent(Matrix x,Matrix y)//����ݶ��½�ÿһ��ѡ��m�������������ݶ��½����򣬸ô�����ֻѡ��һ��������������ݶ��½���������ֵ
{
    int i,j,c=0;
    Matrix weights;
    weights.initMatrix(&weights,x.row,1,1);

    Matrix z;
    z.initMatrix(&z,1,1);

    Matrix xOne;
    xOne.initMatrix(&xOne,1,x.row);

    Matrix xOneT;
    xOneT.initMatrix(&xOneT,xOne.row,xOne.col);

    Matrix w1;
    w1.initMatrix(&w1,x.row,y.row);

    double alpha=0.01;///����
    double error;
    double loss;
    for(c=0; c<5000; c++)
    {
        loss=0;
        for(i=0; i<x.col; i++)
        {
            xOne.getOneCol(xOne,x,i);///���ѡ��һ�������㣬����û�������ѡ�񣬶��ǰ���ѡ��
            z.multsmatrix(&z,xOne,weights);
            z.mat[0][0]=sigmoid(z.mat[0][0]);
            if(y.mat[i][0]==1)
            {
                loss+=log(z.mat[0][0]);
            }
            if(y.mat[i][0]==0)
            {
                loss+=log(1-z.mat[0][0]);
            }
            z.mat[0][0]=y.mat[i][0]-z.mat[0][0];
            xOneT.transposematrix(xOne,&xOneT);
            w1.multsmatrix(&w1,xOneT,z);///����һ��������Ԥ�������ȷ�����ݶȷ���
            for(j=0; j<w1.row; j++)
                w1.mat[j][0]*=alpha;
            weights.addmatrix(&weights,weights,w1);///����

        }
        cout<<"weights"<<endl;
        weights.print(weights);
        cout<<"loss="<<loss/x.col<<endl;
    }
    /**
    ��֤�㷨����ȷ��
    */
    double sum=0;
    Matrix test;
    test.initMatrix(&test,y.col,y.row);
    test.multsmatrix(&test,x,weights);
    for(i=0; i<y.col; i++)
    {
        if(test.mat[i][0]>0)
        {
            sum+=1-y.mat[i][0];
        }

        else
        {
            sum+=y.mat[i][0]-0;
        }

    }
    cout<<"sum="<<sum<<endl;
    cin>>i;
}

/**
�߼��ع飬������Ҫ�����䳣�õ�����������㷨��һ�����ݶ��½���һ��������ݶ��½�

*/

///������ѵ����õ��Ĳ�������Ӧ���߼��ع��е�wx+b�е�w��b,
///��һ����ʾclicktime,con,creativeID,userID,positionID,connection,teles
///��Ҫ˵�����ǣ�����userID��ʱû�ã����Զ�ӦuserID�Ĳ�����b
double weights[8]= {-0.445365,0,11.7753,-596.598,17.3998,46.2444,4.75333};

///���ߺ�����ֱ�Ӷ�test.csv�е��������и���Ԥ�⣬д��pr.csv
int predict(Matrix x,Matrix y)
{
    int i=0;
    ofstream ofile;
    ofile.open("pr.csv");
    for(i=0; i<x.col; i++)
    {
        y.mat[i][0]=x.mat[i][2]*weights[0]+x.mat[i][3]*weights[2]+x.mat[i][5]*weights[4]+x.mat[i][6]*weights[5]+x.mat[i][7]*weights[6]+weights[3]*0.01;
        y.mat[i][0]=sigmoid(y.mat[i][0]);
        ofile<<i+1<<','<<y.mat[i][0]<<'\n';
    }
}


void fect_tj(Matrix x,Matrix y)
{
    ///��clicktime creativeID postionID conenction teles��������ͳ�Ƶ������ĸ���
    int i,j;
    double sum[5]= {0,0,0,0,0};
    int len[5]= {0,0,0,0,0};
    creativeID *ct;
    ct=new creativeID;
    ct->len=0;
    ct->next=NULL;
    creativeID *p;
    p=ct;
    creativeID *tmpct;

    int tsum[24];
    int l1tsum[24];
    int t=0;


    int snum[10000];
    int ssnum[10000];
    double pr[10000];
    for(i=0; i<10000; i++)
    {
        snum[i]=0;
        ssnum[i]=0;
    }
    for(i=0; i<y.col; i++)
    {
        ssnum[(int)x.mat[i][5]]++;//���ｫ�±�
        {
            snum[(int)x.mat[i][5]]+=y.mat[i][0];
        }
    }

    ofstream ofile;
    ofile.open("positionID.txt");
    for(j=0; j<10000; j++)
    {
        if(snum[j]==0&&ssnum[j]==0)
        {
            pr[j]=0.025;
        }
        if(snum[j]==0&&ssnum[j]!=0)
        {
            pr[j]=1.0/((ssnum[j]+1)*1.0/0.025);
        }
        if(snum[j]!=0&&ssnum[j]==0)
        {
            cout<<"error"<<endl;
            exit(-1);
        }
        if(snum[j]!=0&&ssnum[j]!=0)
        {
            pr[j]=(double)snum[j]/ssnum[j];
        }

        ofile<<j<<"  "<<snum[j]<<"   "<<ssnum[j]<<"   "<<pr[j]<<'\n';
    }
    for(t=0; t<24; t++)
    {
        tsum[t]=0;
        l1tsum[t]=0;
    }

    for(i=0; i<y.col; i++)
    {
        for(t=0; t<24; t++)
        {
            if(((int)x.mat[i][1]%10000)/100==t)
            {
                tsum[t]++;
                if(y.mat[i][0]==1)
                {
                    l1tsum[t]++;
                }
            }
        }
    }


    for(t=0; t<24; t++)
    {
        cout<<tsum[t]<<"&"<<l1tsum[t]<<"&"<<(double)l1tsum[t]/tsum[t]<<endl;
    }

    j=7;//����6��ʾconnection��7��ʾteles
    for(i=0; i<y.col; i++)
    {
        {

            if(x.mat[i][j]==0)
            {
                sum[0]+=y.mat[i][0];
                len[0]++;

            }
            if(x.mat[i][j]==1)
            {
                len[1]++;
                sum[1]+=y.mat[i][0];
            }

            if(x.mat[i][j]==2)
            {
                sum[2]+=y.mat[i][0];
                len[2]++;
            }
            if(x.mat[i][j]==3)
            {
                sum[3]+=y.mat[i][0];
                len[3]++;
            }
            if(x.mat[i][j]==4)
            {
                sum[4]+=y.mat[i][0];
                len[4]++;
            }
        }
    }
    for(i=0; i<5; i++)
        cout<<sum[i]<<"  "<<len[i]<<"  "<<(double)sum[i]/len[i]<<endl;
}

int main()
{
    int i,j=0;
    dataToMatrix dtm;
    cout<<"loadData"<<endl;
    cout<<"----------------------"<<endl;
    /**
    ��ѵ�����е�������������ӳ�䣬����ӳ��������������д��train_fx.txt�У�
    ͬ�����Բ��Լ��е�������������ӳ�䣬����ӳ��������������д��test_fx.txt��
    ***/

    dtm.loadData(&dtm,"creativeID.txt",1);
    Matrix cID;
    cID.loadMatrix(&cID,dtm);

    dtm.loadData(&dtm,"positionID.txt",1);
    Matrix pID;
    pID.loadMatrix(&pID,dtm);


    //char file[20]="pre\\test.csv";
    char file[20]="pre\\train.csv";
    dtm.loadData(&dtm,file);

    ///�����ѵ����������������������任֮��
    ///�Ϳ��԰�֮ǰע�͵���ֱ�Ӷ�txt�е�����������ѵ�����߲��Ծ����ˣ�����train_fx.txt��ʾѵ��������test_fx.txt��ʾ���ԣ�Ԥ��ʵ�ֲ���
    ///��Ҫ˵������ͳ�Ƶ��ת���ĸ����Ƕ�csv�ļ���ֱ�Ӷ�200�����ݣ���ʵ������ѵ��ʱ���������ݵĺ�����loadData�л���һ�ַ�ʽ��ֻ��20��
    ///���������Ķ��룬����ȫ�����룬������Ҫȥ��loadData.h�ļ���loadData�����е�if��i%10==2�����
    //dtm.loadData(&dtm,"train_fx.txt",1);

    //cout<<"ok"<<endl;

    Matrix x;
    x.loadMatrix(&x,dtm);
    cout<<"col="<<x.col<<endl;

    Matrix y;
    y.initMatrix(&y,x.col,1);
    y=y.getOneRow(x,1);

    Matrix fx;
    fx.initMatrix(&fx,x.col,x.row);
    cout<<"col="<<y.col<<endl;
    for(i=0; i<y.col; i++)
    {
        fx.mat[i][1]=clicktime[(int)((int)x.mat[i][1]%10000)/100];
        fx.mat[i][6]=connection[(int)x.mat[i][6]];
        fx.mat[i][7]=telecoms[(int)x.mat[i][7]];
        fx.mat[i][3]=cID.mat[(int)x.mat[i][3]][3];
        fx.mat[i][5]=pID.mat[(int)x.mat[i][5]][3];
        fx.mat[i][4]=0.01;
        fx.mat[i][0]=x.mat[i][0];
        fx.mat[i][2]=0;
    }

    ofstream ofile;
    ofile.open("train_fx.txt");
    for(i=0; i<fx.col; i++)
    {
        for(j=0; j<fx.row; j++)
        {
            ofile<<fx.mat[i][j]<<"  ";
        }
        ofile<<'\n';
    }
    ///ѵ����κ���ʵ��
    //x.deleteOneRow(&x,1);
    //gradAscent(x,y);
    //stoGradAscent(x,y);


    ///Ԥ������������ʺ���
    //predict(x,y);
    return 0;
}
