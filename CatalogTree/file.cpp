#include<iostream>
#include<cstdio>
#include<string>
using namespace std;

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

file::file(bool _isDirectory,file* _parent,string _fileName):isDirectory(_isDirectory),parent(_parent),childrenSize(0),fileName(_fileName)
{
    if(isDirectory)
    {
        maxChildrenSize=5;
        file **newFile=new file*[maxChildrenSize];
        for(int i=0;i<maxChildrenSize;i++)
        {
            newFile[i]=NULL;
        }
        childrens=newFile;
    }
    else
    {
        childrens=NULL;
    }
}

file::file():isDirectory(false),parent(NULL),childrenSize(0),maxChildrenSize(5)
{
    fileName=nullptr;
    file** newFile=new file*[maxChildrenSize];
    for(int i=0;i<maxChildrenSize;i++)
    {
        newFile[i]=NULL;
    }
    childrens=newFile;
}

void file::dir()
{
    cout<<fileName<<":"<<endl;
    if(childrenSize==0)
    {
        cout<<"Empty."<<endl;
    }
    else
    {
        for(int i=0;i<childrenSize;i++)
        {
            printf("(%d):",i+1);
            if(childrens[i]->isDirectory)
            {
                cout<<"Directory:";
            }
            else
            {
                cout<<"file:";
            }
            cout<<childrens[i]->fileName<<endl;
        }
        cout<<"End."<<endl;
    }
    return;
}

bool file::mkFile(string childFileName,bool _isDirectory)
{
    for(int i=0;i<childrenSize;i++)
    {
        if(childrens[i]->fileName==childFileName&&childrens[i]->isDirectory==_isDirectory)
        {
            return false;
        }
    }
    file* childFile=new file(_isDirectory,this,childFileName);
    childrenSize++;
    if(childrenSize>maxChildrenSize)
    {
        int newMaxChildrenSize=2*maxChildrenSize;
        file** newFile=new file*[newMaxChildrenSize];
        for(int i=0;i<maxChildrenSize;i++)
        {
            newFile[i]=childrens[i];
        }
        file** delFiles=childrens;
        childrens=newFile;
        delete[] delFiles;
        maxChildrenSize=newMaxChildrenSize;
    }
    childrens[childrenSize-1]=childFile;
    return true;
}

void file::cdAbsolutePath()
{
    string* parentNames=new string[100];
    int namesLength=0;
    file* now=this;
    while(now!=NULL)
    {
        parentNames[namesLength]=now->fileName;
        namesLength++;
        now=now->parent;
    }
    for(int i=namesLength-1;i>=0;i--)
    {
        cout<<"/"<<parentNames[i];
    }
    cout<<"/"<<endl;
    delete[] parentNames;
    return;
}

file* file::cdParent()
{
    return this->parent;
}

file* file::cdChildFile(string childFileName)
{
    file* childFile=NULL;
    for(int i=0;i<childrenSize;i++)
    {
        if(childrens[i]->fileName==childFileName&&childrens[i]->isDirectory)
        {
            childFile=childrens[i];
        }
    }
    return childFile;
}

bool file::deleteChildFile(string childFileName,bool _isDirectory)
{
    file* delFile=NULL;
    int index=-1;
    for(int i=0;i<childrenSize;i++)
    {
        if(childrens[i]->fileName==childFileName&&childrens[i]->isDirectory==_isDirectory)
        {
            delFile=childrens[i];
            index=i;
            break;
        }
    }
    if(index==-1)
    {
        return false;
    }
    if(_isDirectory)
    {
        for(int i=0;i<delFile->childrenSize;i++)//递归将需删除子目录下的文件全部删除;
        {
            delFile->deleteChildFile(delFile->childrens[i]->fileName,delFile->childrens[i]->isDirectory);
        }
    }
    if(index!=childrenSize-1)
    {
        childrens[index]=childrens[childrenSize-1];
    }
    childrens[childrenSize-1]=NULL;
    delete delFile;
    childrenSize--;
    return true;
}

file* file::findFile(string findFileName,bool _isDirectory)
{
    file* ansFile=NULL;
    for(int i=0;i<childrenSize;i++)
    {
        if(childrens[i]->fileName==findFileName&&childrens[i]->isDirectory==_isDirectory)
        {
            ansFile=childrens[i];
            break;
        }
        else
        {
            if(childrens[i]->childrenSize!=0)
            {
                ansFile=childrens[i]->findFile(findFileName,_isDirectory);
                if(ansFile!=NULL)
                {
                    break;
                }
            }
        }
    }
    return ansFile;
}


ostream& operator<<(ostream &out,const file &f)
{
    if(f.isDirectory)
    {
        cout<<"Directory:";
    }
    else
    {
        cout<<"File:";
    }
    cout<<f.fileName<<" ";
    return out;
}




int main1()
{
    file root(true,NULL,"root");
    root.mkFile("file1",true);
    root.mkFile("file2",true);
    root.mkFile("file1",true);
    root.mkFile("file1.txt",false);
    root.mkFile("file3.txt",false);
    //root.mkfile("file1.txt");
    root.cdChildFile("file1")->mkFile("file4",true);
    //root.deleteChildFile("file1.txt",false);
    //root.dir();
    //root.cdChildFile("file1")->dir();
    root.findFile("file4",true)->cdAbsolutePath();
    root.dir();
    return 0;
}