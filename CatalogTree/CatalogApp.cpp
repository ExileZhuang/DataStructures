#include "CatalogTree.cpp"

class CatalogApp
{
private:

    CatalogTree myFiles;

public:

    CatalogApp();//构造函数;

    void StartApp();//启动该app哩;

    void getHelp();//帮助键;
};

CatalogApp::CatalogApp()
{
    StartApp();
}

void CatalogApp::StartApp()
{
    while(1)
    {
        myFiles.cdAbsolutePath();
        cout<<">>";
        string command;
        getline(cin,command);
        //cout<<command;
        int index=-1;
        for(int i=0;i<command.size();i++)
        {
            if(command[i]==' ')
            {
                index=i;
                break;
            }
        }
        string opt,path;
        if(index==-1)
        {
            opt=command;
            path="";
        }
        else
        {
            opt=command.substr(0,index);
            path=command.substr(index+1,command.size()-index-1);
        }
        //cout<<opt<<"     "<<path<<endl;
        if(path=="")
        {
            if(opt=="dir")
            {
                myFiles.dir();
            }
            else if(opt=="cd")
            {
                myFiles.cdAbsolutePath();
            }
            else if(opt=="cd..")
            {
                myFiles.cdParent();
            }
            else if(opt=="help")
            {
                getHelp();
            }
            else if(opt=="quit")
            {
                break;
            }
            else
            {
                cout<<"No Such Command. Enter 'help' to get help."<<endl;
            }
        }
        else
        {
            if(opt=="cd")
            {
                myFiles.cdDirectory(path);
            }
            else if(opt=="mkfile")
            {
                myFiles.mkFile(path,false);
            }
            else if(opt=="mkdir")
            {
                myFiles.mkFile(path,true);
            }
            else if(opt=="delete")
            {
                myFiles.deleteFile(path,true);
                myFiles.deleteFile(path,false);
            }
            else if(opt=="save")
            {
                bool flag=myFiles.saveFiles(path);
                if(!flag)
                {
                    cout<<"Save Failed, Retry Next."<<endl;
                }
            }
            else if(opt=="load")
            {
                bool flag=myFiles.loadFiles(path);
                if(!flag)
                {
                    cout<<"Load Failed, Retry Next."<<endl;
                }
            }
            else
            {
                cout<<"No Such Command. Enter 'help' to get help."<<endl;
            }
        }
    }
}

void CatalogApp::getHelp()
{
    cout<<"---dir"<<endl;
    cout<<"---cd"<<endl;
    cout<<"---cd.."<<endl;
    cout<<"---cd path"<<endl;
    cout<<"---mkfile path"<<endl;
    cout<<"---mkdir path"<<endl;
    cout<<"---delete path"<<endl;
    cout<<"---save path"<<endl;
    cout<<"---load path"<<endl;
    cout<<"---quit"<<endl;
    return;
}


int main()
{
    CatalogApp myApp;
    return 0;
}