class CopyClass
{
    int a;
    char *str;
    
public:
    friend void display();
    CopyClass();
    CopyClass(int,char*);
    CopyClass(CopyClass const &);
    ~CopyClass();
    void setA(int);
    int getA();
    void setStr(char*);
    char* getStr();

};
