#include<iostream>
#include<cstdio>
#include<cmath>
using namespace std;
int MaxN=1e9;
struct Node
{
    int key;
    int val;

    Node(int _key,int _val):key(_key),val(_val)
    {}

    Node():key(-1),val(-1)
    {}

    bool operator<(const Node& tempNode) const
    {
        return this->val<tempNode.val;
    }
};


template<class T>
class MinLoserTree
{
private:

    int arraySize;//元素个数;

    T *loser;//输者树;

    T theWiner;

public:

    MinLoserTree(int _size);//初始化函数，构造最小输者树;

    void output();//输出整个输者树;

    T returnWiner();//返回赢者;

    void replay(int key,T val);//对原本数组中第key个元素改为val并重构输者树;

    void play(T *array);//实现最小输者树;

    ~MinLoserTree();
};

template<class T>
MinLoserTree<T>::MinLoserTree(int _size):arraySize(2*_size)
{
    return;
}


template<class T>
void MinLoserTree<T>::play(T *array)
{
    T *tempLoser=new T[arraySize];
    loser=tempLoser;
    T * tempWiner=new T[arraySize/2];
    for(int i=0;i<arraySize/2;i++)//将传入数组中所有数放入tempWiner中；
    {
        tempWiner[i]=array[i];
        loser[arraySize/2+i]=array[i];
    }
    int nowSize=arraySize/2;
    int beg=nowSize;
    while(nowSize>0)//利用循环实现最小输者树的构建;
    {
        if(nowSize==1)//只有一个元素时;
        {
            loser[0]=tempWiner[0];
            break;
        }
        else
        {
            int nextSize=nowSize/2;//下次循环中赢者树的元素个数;
            int length=nextSize;//此次在输者树中需要构建的元素个数;
            beg-=length;//输者树数组中开始的位置;
            if(nowSize%2==1)//当nowSize为奇数时，则nextSize需要+1,将最后一个数直接插入到赢者树中;
            {
                nextSize++;
            }
            T* newWiner=new T[nextSize];
            if(nowSize%2==1)
            {
                newWiner[nextSize-1]=tempWiner[nowSize-1];
            }
            for(int i=0;i<length;i++)//对赢者树和输者树不断进行更新;
            {
                newWiner[i]=min(tempWiner[i*2],tempWiner[i*2+1]);
                loser[beg+i]=max(tempWiner[i*2],tempWiner[i*2+1]);
            }
            nowSize=nextSize;//准备下一次循环;
            T* currentWiner=tempWiner;
            delete[] currentWiner;
            tempWiner=newWiner;
            /*
            cout<<"loser:";
            for(int i=0;i<arraySize;i++)
            {
                cout<<loser[i]<<" ";
            }
            cout<<endl;
            cout<<"newWiner:";
            for(int i=0;i<nowSize;i++)
            {
               cout<<newWiner[i]<<" ";
            }
            cout<<endl;
            */
        }
    }
    //output();
    theWiner=loser[0];
    return;
}

template<class T>
void MinLoserTree<T>::output()
{
    cout<<"loser:";
    for(int i=0;i<arraySize;i++)
    {
        cout<<loser[i]<<" ";
    }
    cout<<endl;
    return;
}


template<class T>
T MinLoserTree<T>::returnWiner()
{
    return theWiner;
}

template<class T>
void MinLoserTree<T>::replay(int key,T val)
{
    T theChaneVal=loser[arraySize/2+key-1];
    loser[arraySize/2+key-1]=val;
    T* winer=new T[2];
    int nowSize=arraySize/2;
    int nowkey=key;
    T change=val;
    winer[0]=change;
    while(nowSize>0)
    {
        if(nowkey==nowSize/2&&nowSize%2==1)
        {
            winer[1]=change;
        }
        else
        {
            if(nowkey%2==1)
            {
                winer[1]=loser[nowSize/2+nowkey];
                //cout<<"nowSize/2+nowkey:"<<nowSize/2+nowkey<<endl;
            }
            else
            {
               winer[1]=loser[nowSize/2+nowkey-2];
               //cout<<"nowSize/2+nowkey-2:"<<nowSize/2+nowkey-2<<endl;
            }
        }
        if(nowSize==1)
        {
            loser[0]=min(winer[0],winer[1]);
            break;
        }
        int nextSize=nowSize/2;
        if(nowSize%2==1)
        {
            nextSize++;
        }
        int nextkey=0;
        if(nowkey!=1)
        {
            nextkey=nowkey/2;
            if(nowkey%2==1&&nowkey==nowSize)
            {
                nextkey++;
            }
        }
        else
        {
            nextkey=1;
        }
        //cout<<"nextkey:"<<nextkey<<endl;
        loser[nextSize+nextkey-1]=max(winer[0],winer[1]);
        //cout<<nextSize+nextkey-1<<endl;
        change=min(winer[0],winer[1]);
        winer[0]=change;
        nowkey=nextkey;
        nowSize=nextSize;
    }
    theWiner=loser[0];
    return;
}

template<class T>
MinLoserTree<T>::~MinLoserTree()
{
    delete[] loser;
}

class ExternalSort
{
private:

    int k;

    int **nums;

    int bufSize;

    int* buf;


public:

    ExternalSort(int **_nums,int _bufSize,int _k);//构造函数;

    ~ExternalSort();//析构函数;

    void sort(int* _nums,int numsSize);//对单个数组进行最小生成树的排序;

    void externalSort();//对所有元素进行外排序;

    int* returnBuf();

    void clearBuf();
};

ExternalSort::ExternalSort(int **_nums,int _bufSize,int _k=20):k(_k),bufSize(_bufSize)
{
    nums=_nums;
    int *newBuf=new int[bufSize];
    buf=newBuf;
}

ExternalSort::~ExternalSort()
{
    delete[] buf;
}

void ExternalSort::sort(int *_nums,int numsSize)
{
    MinLoserTree<Node>* tempTree=new MinLoserTree<Node>(numsSize);
    Node* nodes=new Node[numsSize];
    for(int j=0;j<numsSize;j++)
    {
        nodes[j].key=j;
        nodes[j].val=_nums[j];
    }
    tempTree->play(nodes);
    int *tempnums=new int[numsSize];
    int now=0;
    while(now<numsSize)
    {
        tempnums[now]=tempTree->returnWiner().val;
        int _key=tempTree->returnWiner().key;
        Node current(_key,MaxN);
        tempTree->replay(_key,current);
        now++;
    }
    for(int i=0;i<numsSize;i++)
    {
        _nums[i]=tempnums[i];
    }
    delete tempTree;
    delete nodes;
    delete tempnums;
}

void ExternalSort::externalSort()
{
    int numsSize=bufSize/k;
    for(int i=0;i<k;i++)
    {
        sort(nums[i],numsSize);//对所有单个数组内元素排序;
    }
    Node* nodes=new Node[k];
    for(int i=0;i<k;i++)
    {
        nodes[i].key=i;
        nodes[i].val=nums[i][0];
    }
    int* rec=new int[k];
    for(int i=0;i<k;i++)
    {
        rec[i]=0;
    }
    MinLoserTree<Node>* tree=new MinLoserTree<Node>(k);
    tree->play(nodes);
    int now=0;
    while(now<bufSize)
    {
        buf[now]=tree->returnWiner().val;
        int _key=tree->returnWiner().key;
        now++;
        rec[_key]++;
        Node tempNode;
        tempNode.key=_key;
        if(rec[_key]<numsSize)
        {
            tempNode.val=nums[_key][rec[_key]];
        }
        else
        {
            tempNode.val=MaxN;
        }
        tree->replay(_key,tempNode);
    }
    return;
}

int* ExternalSort::returnBuf()
{
    return buf;
}

void ExternalSort::clearBuf()
{
    int* del=buf;
    delete[] del;
    int* newBuf=new int[bufSize];
    buf=newBuf;
}

class FileExternalSort
{
private:

    ExternalSort* mySort;

    int k;

    int bufSize;

    int numsSize;

public:

    FileExternalSort(int _k,int _bufSize,int _numsSize);

    void externalSort();
};

FileExternalSort::FileExternalSort(int _k,int _bufSize,int _numsSize):k(_k),bufSize(_bufSize),numsSize(_numsSize)
{
    int **nums=new int *[k];
    for(int i=0;i<k;i++)
    {
        int* temp=new int[10000];
        nums[i]=temp;
    }
    FILE *fp1=fopen("in1.txt","w");
    if(!fp1)
    {
        cout<<"Error"<<endl;
        return;
    }
    else
    {
        fscanf(fp1,"%d",nums[0]);
    }
    fclose(fp1);
    FILE* fp2=fopen("in2.txt","w");
    if(!fp2)
    {
        cout<<"Error!"<<endl;
        return;
    }
    else
    {
        fscanf(fp2,"%d",nums[1]);
    }
    fclose(fp2);
    ExternalSort* temp=new ExternalSort(nums,bufSize,2);
    mySort=temp;
}

void FileExternalSort::externalSort()
{
    int now=0;
    while(now<=numsSize)
    {
        mySort->externalSort();
        int *nowNums=mySort->returnBuf();
        FILE *fp=fopen("out.txt","a");
        if(!fp)
        {
            cout<<"Error"<<endl;
            return;
        }
        fprintf(fp,"%d",nowNums);
        fclose(fp);
        now+=bufSize;
        mySort->clearBuf();
    }
}

int main()
{
    return 0;
}