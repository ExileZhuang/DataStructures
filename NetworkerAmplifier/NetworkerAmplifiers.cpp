#include<iostream>
#include<cstdio>
#include<vector>
#include<queue>
#include<stack>
#include<random>
#include<Windows.h>
#include<ctime>
using namespace std;

struct Node
{
    int Num;//序号;

    Node** children;//存储子节点及其花费;

    int* childrenCost;//对应节点的花费;

    int childrenSize;//子节点个数;

    int degreeToLeaf;

    bool isBooster;//是否放置了网络放大器;

    Node(int _Num):Num(_Num)
    {
        childrenSize=0;
        //cout<<children.size()<<endl;
        degreeToLeaf=-1;
        Node** tempChildren=new Node*[100];
        children=tempChildren;
        int* tempChildrenCost=new int[100];
        childrenCost=tempChildrenCost;
        for(int i=0;i<100;i++)
        {
            children[i]=NULL;
            childrenCost[i]=0;
        }
    }

    void push(Node* pNode,int cost)
    {
        children[childrenSize]=pNode;
        childrenCost[childrenSize]=cost;
        childrenSize++;
    }
};

class NetAmplifiers
{
private:

    Node* root;//根节点;

    vector<Node> nodes;//存储所有图结构;

    int nodeSize;//所有节点范围：1-nodeSize;

    int PMin;//最小压力;

    int PMax;//最大压力;

    void degrees(Node* nowNode,int& cnt);//递归遍历整棵树求出所需放大器个数;

public:

    NetAmplifiers(int _noedeSize,int _PMin,int _PMax);

    int SolutionA();//第一种方法求解所需的放大器个数;

    int SolutionB();//第二种方法求解所需的放大器个数;

    void output(Node* tempNode);

    Node* returnRoot();
};

NetAmplifiers::NetAmplifiers(int _nodeSize,int _PMin,int _PMax):nodeSize(_nodeSize),PMin(_PMin),PMax(_PMax)
{
    for(int i=1;i<=nodeSize;i++)
    {
        Node tempNode(i);
        nodes.push_back(tempNode);
    }
    int wSize;//边数;
    cin>>wSize;
    for(int i=0;i<wSize;i++)//输入图;
    {
        int beg=0,end=0,cost=0;
        cin>>beg>>end>>cost;
        nodes[beg-1].push(&nodes[end-1],cost);
        //output((&nodes[beg-1]));
        /*
        for(auto i:nodes)
        {
            if(i.childrenSize!=0)
            {
                cout<<i.Num<<endl;
            }
        }
        */
    }
    Node* tempRoot=NULL;//寻找根节点,将值赋给root；
    vector<pair<int,int>> degree(nodeSize,make_pair(0,0));//first表示出度,second表示入度;
    for(int i=0;i<nodeSize;i++)
    {
        degree[i].first=nodes[i].childrenSize;
        //cout<<"Num:"<<nodes[i].Num<<" size:"<<nodes[i].childrenSize<<endl;
        for(int j=0;j<nodes[i].childrenSize;j++)
        {
            degree[nodes[i].children[j]->Num-1].second++;
        }
    }
    for(int i=0;i<nodeSize;i++)
    {
        if(degree[i].first>0&&degree[i].second==0)
        {
            tempRoot=&nodes[i];
            break;
        }
    }
    /*
    for(auto i:degree)
    {
        cout<<i.first<<" "<<i.second<<endl;
    }
    */
    root=tempRoot;
    //output(root);
}

void NetAmplifiers::degrees(Node* nowNode,int& cnt)
{
    //cout<<"Num:"<<nowNode->Num<<"  childrenSize:"<<nowNode->childrenSize<<endl;
    if(nowNode->childrenSize==0)//当为叶节点时;
    {
        nowNode->degreeToLeaf=0;
        return;
    }
    int nowDegree=0;
    for(int i=0;i<nowNode->childrenSize;i++)//非叶节点进行递归将子节点的degreeToLeaf求出后，再判断子节点是否需要放置放大器;
    {
        //cout<<cnt<<endl;
        if(nowNode->children[i]->degreeToLeaf==-1)
        {
            degrees(nowNode->children[i],cnt);
        }
    }
    for(int i=0;i<nowNode->childrenSize;i++)
    {
        //cout<<nowNode->childrenCost[i]+nowNode->children[i]->degreeToLeaf<<endl;
        if(nowNode->childrenCost[i]+nowNode->children[i]->degreeToLeaf>PMax-PMin)//需设置放大器的情况;
        {
            nowNode->children[i]->isBooster=true;
            cnt++;
            nowDegree=max(nowDegree,nowNode->childrenCost[i]);
        }
        else
        {
            nowDegree=max(nowDegree,nowNode->childrenCost[i]+nowNode->children[i]->degreeToLeaf);
        }
    }
    nowNode->degreeToLeaf=nowDegree;
    return;
}

int NetAmplifiers::SolutionA()
{
    int cnt=0;
    //output(root);
    degrees(root,cnt);
    return cnt;
}

void NetAmplifiers::output(Node* tempNode)
{
    cout<<"Num:"<<tempNode->Num<<"  childrenSize:"<<tempNode->childrenSize<<"  isBooster:"<<tempNode->isBooster<<endl;
    for(int i=0;i<tempNode->childrenSize;i++)
    {
        cout<<"cost:"<<tempNode->childrenCost[i]<<" ";
        output(tempNode->children[i]);
    }
}

int NetAmplifiers::SolutionB()
{
    int cnt=0;
    queue<Node*> myque;
    myque.push(root);
    vector<bool> flg(nodeSize,false);
    flg[root->Num-1]=true;
    stack<Node*> myStack;
    while(!myque.empty())
    {
        Node* now=myque.front();
        myque.pop();
        myStack.push(now);
        for(int i=0;i<now->childrenSize;i++)
        {
            if(!flg[now->children[i]->Num-1])
            {
                flg[now->children[i]->Num-1]=true;
                myque.push(now->children[i]);
            }
        }
    }
    while(!myStack.empty())
    {
        Node* now=myStack.top();
        if(now->degreeToLeaf==-1)
        {
            if(now->childrenSize==0)
            {
                now->degreeToLeaf=0;
            }
            else
            {
                int nowDegree=0;
                for(int i=0;i<now->childrenSize;i++)
                {
                    if(now->children[i]->degreeToLeaf+now->childrenCost[i]>PMax-PMin)
                    {
                        cnt++;
                        now->children[i]->isBooster=true;
                        nowDegree=max(nowDegree,now->childrenCost[i]);
                    }
                    else
                    {
                        nowDegree=max(nowDegree,now->childrenCost[i]+now->children[i]->degreeToLeaf);
                    }
                }
                now->degreeToLeaf=nowDegree;
            }
        }
        myStack.pop();
    }
    return cnt;
}

Node* NetAmplifiers::returnRoot()
{
    return root;
}

int main()
{
    //freopen("input1.in","r",stdin);
    int _nodeSize,_PMax;
    cin>>_nodeSize>>_PMax;
    NetAmplifiers NB(_nodeSize,0,_PMax);
    LARGE_INTEGER cpu;
    LARGE_INTEGER start;
    LARGE_INTEGER end;
    QueryPerformanceFrequency(&cpu);
    QueryPerformanceCounter(&start);
    int ans=NB.SolutionB();
    QueryPerformanceCounter(&end);
    cout<<"SolutionB:"<<ans<<endl;
    double last=(end.QuadPart-start.QuadPart)*1000000.0/cpu.QuadPart;
    cout<<"time:"<<double(last)<<endl;
    return 0;
}
int main1()
{
    //freopen("input9.in","r",stdin);
    int _nodeSize,_PMax;
    cin>>_nodeSize>>_PMax;
    NetAmplifiers NA(_nodeSize,0,_PMax);
    LARGE_INTEGER cpu;
    LARGE_INTEGER start;
    LARGE_INTEGER end;
    QueryPerformanceFrequency(&cpu);
    QueryPerformanceCounter(&start);
    int ans=NA.SolutionA();
    QueryPerformanceCounter(&end);
    cout<<"SolutionA:"<<ans<<endl;
    double last=(end.QuadPart-start.QuadPart)*1000000.0/cpu.QuadPart;
    cout<<"time:"<<double(last)<<endl;
    return 0;
}

int main3()
{
    LARGE_INTEGER cpu;
    LARGE_INTEGER start;
    LARGE_INTEGER end;
    QueryPerformanceFrequency(&cpu);
    QueryPerformanceCounter(&start);
    Sleep(10);
    QueryPerformanceCounter(&end);
    double last=(end.QuadPart-start.QuadPart)*1000000.0/cpu.QuadPart;
    cout<<"time:"<<double(last)<<endl;
    return 0;
}