#include"file.cpp"
#include<fstream>

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

string* CatalogTree::split(string path)
{
    string* ans=new string[100];
    int ansSize=0;
    string now="";
    for(auto i:path)
    {
        if(i=='/')
        {
            if(now!="")
            {
                ans[ansSize]=now;
                ansSize++;
                now="";
            }
        }
        else
        {
            now+=i;
        }
    }
    if(now!="")
    {
        ans[ansSize]=now;
        ansSize++;
    }
    ans[ansSize]="end";
    return ans;
}

CatalogTree::CatalogTree():maxRootsSize(5),rootsSize(0)
{
    file** newRoots=new file*[maxRootsSize];
    for(int i=0;i<maxRootsSize;i++)
    {
        newRoots[i]=NULL;
    }
    roots=newRoots;
    now=NULL;
}

void CatalogTree::cdDirectory(string path)
{
    if(rootsSize==0)
    {
        cout<<"Error:No Directory!"<<endl;
        return;
    }
    string* pathFiles=split(path);
    string firstFile=pathFiles[0];
    int index=-1;
    for(int i=0;i<rootsSize;i++)
    {
        if(roots[i]->fileName==firstFile&&roots[i]->isDirectory)
        {
            index=i;
            break;
        }
    }
    int beg=-1;
    if(index!=-1)//绝对路径;
    {
        beg=1;
        now=roots[index];
    }
    else
    {
        beg=0;
    }
    if(now==NULL)
    {
        cout<<"Error Path"<<endl;
        return;
    }
    for(int i=beg;;i++)
    {
        if(pathFiles[i]=="end")
        {
            break;
        }
        file* temp=now->cdChildFile(pathFiles[i]);
        if(temp==NULL)
        {
            break;
        }
        else
        {
            now=temp;
        }
    }
    return;
}

void CatalogTree::dir()
{
    if(now==NULL)
    {
        for(int i=0;i<rootsSize;i++)
        {
            printf("(%d):",i);
            cout<<"Directory "<<roots[i]->fileName<<":"<<endl;
        }
    }
    else
    {
        now->dir();
    }
    return;
}

void CatalogTree::cdAbsolutePath()
{
    if(now==NULL)
    {
        cout<<"Empty."<<endl;
    }
    else
    {
        now->cdAbsolutePath();
    }
    return;
}

void CatalogTree::cdParent()
{
    if(now!=NULL)
    {
        now=now->cdParent();
    }
    return;
}

bool CatalogTree::mkFile(string path,bool _isDirectory)
{
    string* pathFiles=split(path);
    string firstFile=pathFiles[0];
    int index=-1;//false代表为相对路径，true代表为绝对路径;
    for(int i=0;i<rootsSize;i++)
    {
        if(firstFile==roots[i]->fileName)
        {
            index=i;
            break;
        }
    }
    file* filePosition=NULL;
    int begIndex=0;
    if(index!=-1)//绝对路径时;
    {
        filePosition=roots[index];
        begIndex=1;
    }
    else//相对路径时;
    {
        if(now==NULL)//now为NULL时，新建一个root文件;
        {
            file* newFile=new file(true,NULL,firstFile);
            rootsSize++;
            if(rootsSize>maxRootsSize)
            {
                int newMaxRootsSize=2*maxRootsSize;
                file** newRoots=new file*[newMaxRootsSize];
                for(int i=0;i<maxRootsSize;i++)
                {
                    newRoots[i]=roots[i];
                }
                file** delRoots=roots;
                roots=newRoots;
                delete[] delRoots;
                maxRootsSize=rootsSize;
            }
            roots[rootsSize-1]=newFile;
            filePosition=roots[rootsSize-1];
            begIndex=1;
        }
        else
        {
            filePosition=now;
            begIndex=0;
        }
    }
    for(int i=begIndex;;i++)
    {
        if(pathFiles[i]=="end")
        {
            break;
        }
        bool flag=false;//代表该目录下存在下一个绝对路径上的文件或目录;
        for(int j=0;j<filePosition->childrenSize;j++)
        {
            if(filePosition->childrens[j]->fileName==pathFiles[i]&&filePosition->childrens[j]->isDirectory)
            {
                flag=true;
                break;
            }
        }
        if(flag)
        {
            filePosition=filePosition->cdChildFile(pathFiles[i]);
        }
        else
        {
            if(pathFiles[i+1]=="end"&&!_isDirectory)//这是文件;
            {
                filePosition->mkFile(pathFiles[i],false);
            }
            else
            {
                filePosition->mkFile(pathFiles[i],true);
                filePosition=filePosition->cdChildFile(pathFiles[i]);
            }
        }
    }
    return true;
}

bool CatalogTree::deleteFile(string path,bool _isDirectory)
{
    string* pathFiles=split(path);
    string firstFile=pathFiles[0];
    int index=-1;
    for(int i=0;i<rootsSize;i++)
    {
        if(firstFile==roots[i]->fileName)
        {
            index=i;
            break;
        }
    }
    file* filePosition=NULL;
    int beg=-1;
    if(index!=-1)
    {
        beg=1;
        filePosition=roots[index];
    }
    else
    {
        if(now==NULL)
        {
            return false;
        }
        else
        {
            filePosition=now;
            beg=0;
        }
    }
    int index2=-1;
    for(int i=beg;;i++)
    {
        if(pathFiles[i+1]=="end")
        {
            index2=i;
            break;
        }
        bool flag=false;
        for(int j=0;j<filePosition->childrenSize;j++)
        {
            if(filePosition->childrens[j]->fileName==pathFiles[i]&&filePosition->childrens[j]->isDirectory)
            {
                flag=true;
                break;
            }
        }
        if(flag)
        {
            filePosition=filePosition->cdChildFile(pathFiles[i]);
        }
        else
        {
            return false;
        }
    }
    if(now==filePosition->cdChildFile(pathFiles[index2]))
    {
        now=NULL;
    }
    filePosition->deleteChildFile(pathFiles[index2],_isDirectory);
    return true;
}

file* CatalogTree::findFile(string findFileName,bool _isDirectory)
{
    file* ansFile=NULL;
    for(int i=0;i<rootsSize;i++)
    {
        if(roots[i]->fileName==findFileName&&roots[i]->isDirectory==_isDirectory)
        {
            ansFile=roots[i];
            break;
        }
        else
        {
            ansFile=roots[i]->findFile(findFileName,_isDirectory);
            if(ansFile!=NULL)
            {
                break;
            }
        }
    }
    return ansFile;
}

void CatalogTree::dfsFiles(string* allFiles,int& allFileSize,int& maxFileSize,string now,file* nowFile)
{
    now=now+nowFile->fileName+"/";
    if(nowFile->childrenSize==0)
    {
        allFileSize++;
        if(allFileSize>maxFileSize)
        {
            int newMaxFileSize=3*maxFileSize;
            string *temp=new string[newMaxFileSize];
            for(int i=0;i<maxFileSize;i++)
            {
                temp[i]=allFiles[i];
            }
            string* delFiles=allFiles;
            allFiles=temp;
            delete[] delFiles;
            maxFileSize=newMaxFileSize;
        }
        if(nowFile->isDirectory)//记录目录树叶节点是文件还是目录;
        {
            now+="1/";
        }
        else
        {
            now+="0/";
        }
        allFiles[allFileSize-1]=now;
    }
    else
    {
        for(int i=0;i<nowFile->childrenSize;i++)
        {
            dfsFiles(allFiles,allFileSize,maxFileSize,now,nowFile->childrens[i]);
        }
    }
    return;
}

bool CatalogTree::saveFiles(string saveFilePath) 
{
    int MaxFileSize=1000,nowFileSize=0;
    string* allFiles=new string[MaxFileSize];
    for(int i=0;i<rootsSize;i++)
    {
        string now="/";
        dfsFiles(allFiles,nowFileSize,MaxFileSize,now,roots[i]);
    }
    bool flag=false;
    string* tempFilePath=split(saveFilePath);
    string exactFilePath="";
    for(int i=0;;i++)
    {
        if(tempFilePath[i+1]=="end")
        {
            exactFilePath=exactFilePath+tempFilePath[i];
            break;
        }
        else
        {
            exactFilePath=exactFilePath+tempFilePath[i]+"/";
        }
    }
    saveFilePath=exactFilePath;
    ofstream out(saveFilePath,ios::out);
    if(out.is_open())
    {
        for(int i=0;i<nowFileSize;i++)
        {
            out<<allFiles[i]<<endl;
        }
        out.close();
        flag=true;
    }
    return flag;
}

bool CatalogTree::loadFiles(string loadFilePath)
{
    string* tempFilePath=split(loadFilePath);
    string exactFilePath="";
    for(int i=0;;i++)
    {
        if(tempFilePath[i+1]=="end")
        {
            exactFilePath=exactFilePath+tempFilePath[i];
            break;
        }
        else
        {
            exactFilePath=exactFilePath+tempFilePath[i]+"/";
        }
    }
    loadFilePath=exactFilePath;
    int nowFileSize=0,maxFileSize=5;
    string *allFiles=new string[maxFileSize];
    fstream in(loadFilePath,ios::in);
    if(in.is_open())
    {
        string line;
        while(getline(in,line))
        {
            nowFileSize++;
            if(nowFileSize>maxFileSize)
            {
                int newMaxFileSize=3*maxFileSize;
                string* newFiles=new string[newMaxFileSize];
                for(int i=0;i<maxFileSize;i++)
                {
                    newFiles[i]=allFiles[i];
                }
                string* delFiles=allFiles;
                allFiles=newFiles;
                delete[] delFiles;
                maxFileSize=newMaxFileSize;
            }
            allFiles[nowFileSize-1]=line;
        }
    }
    else
    {
        return false;
    }
    for(int i=0;i<nowFileSize;i++)
    {
        string* nowPathFiles=split(allFiles[i]);
        string nowPath="/";
        for(int j=0;;j++)
        {
            if(nowPathFiles[j+1]=="end")
            {
                bool _isDirectory=false;
                if(nowPathFiles[j+1]=="1")
                {
                    _isDirectory=true;
                }
                mkFile(nowPath,_isDirectory);
                break;
            }
            else
            {
                nowPath=nowPath+nowPathFiles[j]+"/";
            }
        }
    }
    return true;
}

void CatalogTree::output()
{
    for(int i=0;i<rootsSize;i++)
    {
        output(roots[i]);
    }
    return;
}

void CatalogTree::output(file* temp)
{
    temp->dir();
    for(int i=0;i<temp->childrenSize;i++)
    {
        if(temp->childrens[i]->isDirectory)
        {
            output(temp->childrens[i]);
        }
    }
    return;
}


int main2()
{
    CatalogTree cata;
    cata.mkFile("/root/file1/file2.txt/",false);
    cata.mkFile("/root/file2/file3/",true);
    cata.cdDirectory("/root/");
    cata.mkFile("/file4.txt/",false);
    cata.mkFile("/file2/file3/file5/file6.txt/",false);
    cata.mkFile("file7",true);
    //cata.output();
    //cata.deleteFile("/file2/file3/file5/",true);
    //cata.deleteFile("/root/file1/file2.txt/",false);
    cata.output();
    cata.saveFiles("catalogTree.txt");
    return 0;
}

int main3()
{
    CatalogTree cata;
    cata.loadFiles("catalogTree.txt");
    cata.output();
    return 0;
}