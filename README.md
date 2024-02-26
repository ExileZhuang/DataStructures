## 关于项目
### 背景
该项目实现了数据结构中的四种常见数据结构：  
[目录树(CatalogTree)](https://github.com/ExileZhuang/DataStructures/tree/main/CatalogTree)；  
[外排序：(ExternalSorting)](https://github.com/ExileZhuang/DataStructures/tree/main/ExternalSorting);  
[网络放大器(NetworkerAmplifier)](https://github.com/ExileZhuang/DataStructures/tree/main/NetworkerAmplifier);  
[跳表(SkipList)](https://github.com/ExileZhuang/DataStructures/tree/main/SkipList);  
### 开发语言
[C++](https://isocpp.org/) 20  
### 介绍
#### 目录树
首先实现目录树节点——文件(file)的构造：
```
struct file
{
    string fileName;

    bool isDirectory;//是否是目录,true为目录,false为文件;

    file* parent;//记录其父文件;

    file** childrens;//记录其所有子节点;

    int childrenSize;//儿子节点个数，只记录childrens中的节点个数;

    int maxChildrenSize;//最大儿子节点数量，用于变化childrens的大小;

    file(bool _isDirectory,file* _parent,string _fileName);//构造函数;

    file();//构造函数;

    void dir();//列出该文件下所有的文件;

    bool mkFile(string childFileName,bool _isDirectory);//在该目录下创建一个名为childFileName的文件或目录;

    void cdAbsolutePath();//输出该文件的绝对路径;

    file* cdParent();//当前目录变为当前目录的父目录;

    file* cdChildFile(string childFileName);//进入当前目录的名为childFileName子目录(子文件不进入),未成功进入返回NULL;

    bool deleteChildFile(string childFileName,bool _isDirectory);//删除当前目录的名为childFileName子文件或目录,false代表未找到，true代表删除成功;

    file* findFile(string findFileName,bool _isDirectory);//在file的子文件中寻找名为findFileName的文件或目录;

};
```
目录树(CatalogTree)相关功能都是由文件(file)相关功能的并集或交集来实现，因此顺着文件(file)的相关函数实现目录树(CatalogTree)的构造：  
```
class CatalogTree
{
private:

    file** roots;//记录所有的根节点文件;

    int rootsSize;//根节点文件个数;

    int maxRootsSize;//最大根节点个数，初始默认为为5;

    file* now;//记录当前节点所在的位置;

    string* split(string path);//对path进行所有文件和路径的拆分,末尾返回end;

public:

    CatalogTree();//构造函数;

    void cdDirectory(string path);//进入相对路径或绝对路径为path的目录并更新当前now;

    void dir();//列出当前目录下的所有目录项;

    void cdAbsolutePath();//列出当前目录下的绝对路径;

    void cdParent();//进入当前目录的父目录;

    bool mkFile(string path,bool isDirectory);//在绝对路径为path的目录里创建一个目录,isDirectory表示最后创建为dir，否则为file;

    bool deleteFile(string path,bool _isDirectory);

    file* findFile(string findFileName,bool _isDirectory);//找到名为findFileName的文件或目录;

    void dfsFiles(string* allFiles,int& allFileSize,int &maxFileSize,string now,file* nowFile);//利用递归求出所有叶节点文件的路径;

    bool saveFiles(string saveFilePath);//将该目录树保存到指定路径saveFilePath的文件中;

    bool loadFiles(string loadFilePath);//读取指定loadFilePath中的目录树;

    void output();//显示所有目录及文件;

    void output(file *temp);//同上;
};
```
最后实现基础的显示界面(CatalogApp):
```
class CatalogApp
{
private:

    CatalogTree myFiles;

public:

    CatalogApp();//构造函数;

    void StartApp();//启动该app;

    void getHelp();//帮助键;
};
```  
#### 外排序
首先实现最小输者树(MinLoserTree):
```
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
```
通过最小输者树(MinLoserTree)实现外排序(ExternalSort):
```
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
```  
#### 网络放大器
首先实现节点(Node):
```
struct Node
{
    int Num;//序号;

    Node** children;//存储子节点及其花费;

    int* childrenCost;//对应节点的花费;

    int childrenSize;//子节点个数;

    int degreeToLeaf;

    bool isBooster;//是否放置了网络放大器;

    Node(int _Num):Num(_Num);

    void push(Node* pNode,int cost);
};
```
然后实现网络放大器(NetworkerAmplifier):
```
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
```  
#### 跳表
跳表(SkipList)是链表(List)的改进，用空间换时间。   
首先实现跳表(SkipList)的节点(SkipNode)，存储为键-值方式：
```
template<class T>
struct skipNode//跳表节点;
{
    int key;//关键字，定为int型;
    T val;//关键字所对应的值;
    skipNode<T> **next;//一个可以指向多个节点的多层节点指针;

    skipNode(int _key,T _val,int size):key(_key),val(_val);

    skipNode(int _key,int size):key(_key);

    ~skipNode();

    bool operator<(const skipNode &tempNode) const;//重载<号，只比较key；
};
```
随后实现跳表(SkipList),规定对于跳表每一层，每两个节点具有一个索引，即对于跳表从上层到底层，严格意义上的节点数是1，2，4，8···：
```
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
```
## 贡献者
[ExileZhuang](https://github.com/ExileZhuang)
