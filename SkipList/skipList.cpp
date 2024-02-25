#include<iostream>
#include<cstdio>
#include<ctime>
#include<random>
using namespace std;


template<class T>
struct skipNode//跳表节点;
{
    int key;//关键字，定为int型;
    T val;//关键字所对应的值;
    skipNode<T> **next;//一个可以指向多个节点的多层节点指针;

    skipNode(int _key,T _val,int size):key(_key),val(_val)
    {
        next=new skipNode<T>* [size];
    }

    skipNode(int _key,int size):key(_key)
    {
        next=new skipNode<T>* [size];
    }

    ~skipNode()
    {
        delete[] next;
    }

    bool operator<(const skipNode &tempNode) const//重载<号，只比较key；
    {
        return this->key<tempNode.key;
    }
};


template<class T>
class skipList
{

private:

    skipNode<T>* headNode;//跳表的头节点,视为minKey;

    skipNode<T>* tailNode;//跳表尾节点，视为maxKey;

    int MaxLevel;//最大层数,从[0,MaxLevel);

    int levels;//当前最大层数，数据范围从[0,MaxLevel);

    int nodeSize;//除headNode和tailNode外的有效节点个数;
    
    int maxKey,minKey;//最大和最小关键字;

    default_random_engine randomEngine;//随机数引擎;

    int randomLevel(int maxLevel);//返回[0,maxLevel)以此确定该数的最高层;

    int optimize(skipNode<T>* firNode,skipNode<T>* secNode,int nowLevel);//对第nowLevel层firNode到secNode节点(secNode为firNode第nowLevel层下一节点)间进行时间复杂度为O(1)的局部优化，使其部分满足严格跳表;

public:

    skipList(int _maxKey,int _minKey,int _maxLevel);//初始化;

    ~skipList();

    bool empty();//判断是否为空，为空返回true;

    int size();//返回节点个数;

    bool push(int _key,T _val);//将关键字为_key,值为_val的节点加入跳表中;

    int findIndex(T _val);//找到值为_val的关键字并返回其关键字;

    T findValue(int _key);//找到关键字为_key的值;

    void pop_back();//将关键字最大的节点删除;

    void pop_front();//将关键字最小的节点删除;

    void erase(int _key);//将关键字为_key的节点删除;

    void clear();//清空跳表内所有元素;

    void output();//展示跳表中每层的元素;

    void StrictSkip();//将整个跳表进行严格化;

};

template<class T>
skipList<T>::skipList(int _minKey,int _maxKey,int _maxLevel):maxKey(_maxKey),minKey(_minKey),MaxLevel(_maxLevel),nodeSize(0),levels(1)
{
    headNode=new skipNode<T>(minKey,MaxLevel);//初始化headNode；
    tailNode=new skipNode<T>(maxKey,MaxLevel);//初始化tailNode;
    for(int i=0;i<MaxLevel;i++)//使headNode的节点指针每一层都指向tailNode，而tailNode的节点指针每一层都指向NULL;
    {
        headNode->next[i]=tailNode;
        tailNode->next[i]=NULL;
    }
    randomEngine.seed(time(0));//初始化随机引擎;
}

template<class T>
skipList<T>::~skipList()
{
    skipNode<T>* tempNode;
    while(headNode->next[0]!=NULL)//从最底层开始不断删除节点;
    {
        tempNode=headNode->next[0];
        delete headNode;
        headNode=tempNode;
    }
    delete tailNode;
}

/*
1/2概率: 1
1/4概率: 2
1/8概率: 3
...
*/
template<class T>
int skipList<T>::randomLevel(int maxLevel)
{
    uniform_int_distribution<int> randomNum(0,1);//确定随机数范围为1或0，即判定为1/2概率;
    int now=0;
    while(now<maxLevel&&randomNum(randomEngine)==0)
    {
        now++;
    }
    if(now==maxLevel)//当now>maxLevel时即剩下1/2^(maxLevl)概率超出该层，则将其回到第0层以维持平衡，因为第0层数据远远多于第maxLevel层，最好维护平衡;
    {
        now=0;
    }
    return now;
}

/*
0:两节点无变化;
1:删除了第nowLevel层的secNode;
2:增加了另外节点;
*/
template<class T>
int skipList<T>::optimize(skipNode<T>* firNode,skipNode<T>* secNode,int nowLevel)
{
    if(nowLevel==0)//第0层无需优化;
    {
        return 0;
    }
    skipNode<T>* tempNode=firNode->next[nowLevel-1];
    if(tempNode==secNode)//当第nowLevel与nowLevel-1层secNode都为firNode下一节点，即存在局部重复问题,删除第nowLevel层的secNode，但无法将secNode更高层同时删除,只能在下次局部优化时删除;
    {
        firNode->next[nowLevel]=secNode->next[nowLevel];
        secNode->next[nowLevel]=NULL;
        return 1;
    }
    else
    {
        skipNode<T>* tempNode2=tempNode->next[nowLevel-1];
        if(tempNode2!=secNode)//当第nowLevel-1层firNode下第二个节点tempNode2不为secNode即说明对第nowLevel层firNode和secNode距离过大，可在其中增加tempNode2进行优化;
        {
            firNode->next[nowLevel]=tempNode2;
            tempNode2->next[nowLevel]=secNode;
            return 2;
        }
        return 0;
    }
}

template<class T>
bool skipList<T>::empty()
{
    return nodeSize==0;
}

template<class T>
int skipList<T>::size()
{
    return nodeSize;
}

template<class T>
bool skipList<T>::push(int _key,T _val)
{
    if(_key<=minKey||_key>=maxKey)//当不符合范围的key出现时，无法push并返回false;
    {
        return false;
    }
    nodeSize++;
    int nowLevel=randomLevel(MaxLevel);//确定该节点的最大层数,范围[0,levels)
    if(levels<nowLevel+1)//重新确定当前最大层数;
    {
        levels=nowLevel+1;
    }
    skipNode<T>* nowNode=new skipNode<T>(_key,_val,MaxLevel);
    skipNode<T>* firstNode=headNode;
    for(int i=levels-1;i>=0;i--)//从当前最高层开始查找插入节点所在位置;
    {
        skipNode<T>* secNode=firstNode->next[i];
        while(firstNode->key<_key&&secNode->key<_key)//当插入节点在当前节点右边时；
        {
            int temp=optimize(firstNode,secNode,i);//对上两个节点进行局部优化;
            if(temp==0)//当部分节点没有变化时;
            {
                firstNode=secNode;
                secNode=firstNode->next[i];
            }
            else//当局部优化使得该层增加或减少节点时，firstNode不变，secNode指向firstNode的下一节点,从而避免新插入\删除后节点被跳过问题;
            {
                firstNode=firstNode;
                secNode=firstNode->next[i];
            }
        }
        if(secNode->key==_key)//找到关键字相同的节点;
        {
            return false;
        }
        if(i>nowLevel)//当该层仍处于该节点所在的最大层数时,使该节点的该层指针指向NULL;
        {
            nowNode->next[i]=NULL;
        }
        else//处于该层并且在两节点之中时，插入该节点;
        {
            firstNode->next[i]=nowNode;
            nowNode->next[i]=secNode;
        }
    }
    return true;
}

template<class T>
int skipList<T>::findIndex(T _val)
{
    int res=-1;
    skipNode<T>* firNode=headNode->next[0];//从最底层节点开始寻找值为_val的关键字;
    while(firNode!=tailNode)
    {
        if(firNode->val==_val)
        {
            res=firNode->key;
        }
        firNode=firNode->next[0];
    }
    return res;
}

template<class T>
T skipList<T>::findValue(int _key)
{
    T res;
    skipNode<T>* firNode=headNode;
    for(int i=levels-1;i>=0;i--)//从最高层开始查找关键字为_key的节点;
    {
        skipNode<T>* secNode=firNode->next[i];
        while(firNode->key<_key&&secNode->key<_key)//当查找节点在当前节点右边时;
        {
            int temp=optimize(firNode,secNode,i);//局部优化这两个节点;
            if(temp==0)
            {
                firNode=secNode;
                secNode=firNode->next[i];
            }
            else
            {
                firNode=firNode;
                secNode=firNode->next[i];
            }
        }
        if(secNode->key==_key)//找到该节点;
        {
            res=secNode->val;
            return res;
        }
        else
        {
            continue;
        }
    }
    return false;
}

template<class T>
void skipList<T>::pop_back()
{
    if(empty())
    {
        return;
    }
    else
    {
        skipNode<T>* firNode=headNode;
        while(firNode->next[0]->next[0]!=tailNode)//当该节点的底层指针指向的下一个节点的下一个节点为tailNode则此时节点为最后一个节点的父节点;
        {
            firNode=firNode->next[0];
        }
        skipNode<T>* delNode=firNode->next[0];
        firNode->next[0]=delNode->next[0];
        for(int i=levels-1;i>0;i--)//从当前最高层开始，找到该节点的最大层数;
        {
            skipNode<T>* tempNode=headNode;
            while(tempNode->next[i]!=tailNode)//从最大层数开始以此删除每一层节点;
            {
                if(tempNode->next[i]==delNode)
                {
                    tempNode->next[i]=delNode->next[i];
                    break;
                }
                tempNode=tempNode->next[i];
            }
        }
        delete delNode;
        nodeSize--;
    }
    return;
}

template<class T>
void skipList<T>::pop_front()
{
    if(empty())
    {
        return;
    }
    skipNode<T>* delNode=headNode->next[0];
    for(int i=levels-1;i>=0;i--)//从最底层开始寻找判断是否每一层存在首节点;
    {
        if(headNode->next[i]==delNode)
        {
            headNode->next[i]=delNode->next[i];
        }
    }
    delete delNode;
    nodeSize--;
    return;
}

template<class T>
void skipList<T>::erase(int _key)
{
    skipNode<T>* firNode=headNode;
    skipNode<T>* delNode=headNode;
    for(int i=levels-1;i>=0;i--)//从最高层开始寻找所需要删除节点所在的位置;
    {
        skipNode<T>* secNode=firNode->next[i];
        while(firNode->key<_key&&secNode->key<_key)//当所需节点在当前节点右边时;
        {
            int temp=optimize(firNode,secNode,i);//进行局部优化;
            if(temp==0)
            {
                firNode=secNode;
                secNode=firNode->next[i];
            }
            else
            {
                firNode=firNode;
                secNode=firNode->next[i];
            }
        }
        if(secNode->key==_key)//但找到该节点时，使其父节点该层指针指向其自节点;
        {
            delNode=secNode;
            firNode->next[i]=secNode->next[i];
        }
    }
    if(delNode==headNode)
    {
        return;
    }
    else
    {
        nodeSize--;
        delete delNode;
        return;
    }
}

template<class T>
void skipList<T>::clear()
{
    skipNode<T>* firNode=headNode;
    for(int i=levels-1;i>0;i--)//从最高层开始，到第1层不断将每一层节点指向的指针指向tailNode;
    {
        skipNode<T>* secNode=firNode->next[i];
        while(secNode!=tailNode)
        {
            firNode->next[i]=secNode->next[i];
            secNode=firNode->next[i];
        }
    }
    skipNode<T>* secNode=firNode->next[0];
    while(secNode!=tailNode)//将最底层所有节点删除;
    {
        firNode->next[0]=secNode->next[0];
        delete secNode;
        secNode=firNode->next[0];
    }
    levels=1;
    nodeSize=0;
    return;
}

template<class T>
void skipList<T>::output()
{
    for(int i=levels-1;i>=0;i--)
    {
        skipNode<T>* firNode=headNode;
        while(firNode!=NULL)
        {
            if(firNode->key==minKey||firNode->key==maxKey)
            {
                cout<<"F/E"<<" ";
            }
            else
            {
                cout<<firNode->key<<":"<<firNode->val<<" ";
            }
            firNode=firNode->next[i];
        }
        cout<<endl;
    }
    cout<<endl;
}

template<class T>
void skipList<T>::StrictSkip()
{
    int _levels=1;
    int _nodeSize=nodeSize;
    while(_nodeSize>0)//根据节点个数判断当前应在的最大层数;
    {
        _levels++;
        _nodeSize/=2;
    }
    if(_levels>=MaxLevel)
    {
        levels=MaxLevel;
    }
    else
    {
        levels=_levels;
    }
    for(int i=1;i<levels;i++)//从第1层开始，不断根据其下一层判断该层所在的节点;
    {
        skipNode<T>* firNode=headNode;
        skipNode<T>* SecNode=firNode->next[i];
        skipNode<T>* theSecNode=firNode->next[i-1];
        int rec=1;
        while(theSecNode!=tailNode)
        {
            if(rec%2==0)//当该节点应当在上一层时;
            {
                if(theSecNode!=SecNode)
                {
                    firNode->next[i]=theSecNode;
                    theSecNode->next[i]=SecNode;
                }
                firNode=firNode->next[i];
                SecNode=firNode->next[i];
            }
            else//当该节点不该在上一层时，将上一层存在的该节点删除;
            {
                if(theSecNode==SecNode)
                {
                    firNode->next[i]=SecNode->next[i];//将第i+1层到levels层该节点全部删除;
                    for(int j=i+1;j<levels;j++)//找到第i+1层到levels层的所有节点并删除;
                    {
                        skipNode<T>* tempNode=headNode;
                        bool flag=false;
                        while(tempNode->next[j]!=SecNode)
                        {
                            tempNode=tempNode->next[j];
                            if(tempNode==tailNode)
                            {
                                flag=true;
                                break;
                            }
                        }
                        if(flag==true)
                        {
                            break;
                        }
                        else
                        {
                            tempNode->next[j]=SecNode->next[j];
                        }
                    }
                    firNode=firNode;
                    SecNode=firNode->next[i];
                }
            }
            theSecNode=theSecNode->next[i-1];
            rec++;
        }
    }
}

int main()
{
    skipList<int> sk(0,1001,15);
    for(int i=1;i<=100;i++)
    {
        sk.push(i,i);
    }
    sk.output();
    cout<<sk.findIndex(24)<<endl;
    cout<<sk.findValue(64)<<endl;
    cout<<sk.size()<<endl;
    cout<<sk.empty()<<endl;
    sk.pop_back();
    sk.pop_front();
    cout<<sk.size()<<endl;
    sk.erase(48);
    sk.output();
    cout<<endl;
    sk.StrictSkip();
    sk.output();
    return 0;
}

int main2()
{
    skipList<int> sk(0,101,10);
    for(int i=1;i<80;i++)
    {
        sk.push(i,i);
    }
    sk.StrictSkip();
    sk.output();
    return 0;
}

int main1()
{
    skipList<int> sk(0,101,10);
    for(int i=100;i>0;i--)
    {
        sk.push(i,i);
    }
    sk.output();
    return 0;
}