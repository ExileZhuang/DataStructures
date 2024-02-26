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
#### 网络放大器
#### 跳表
