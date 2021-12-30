#include <iostream>
#include <string>
#include<fstream>
#include<cstring>
//c:/php/php.exe
using namespace std;

int ala_index=0, mnt_index=0, mdt_index=0,macStatus=0,mdtStatus=0,a=0,op_line=0; 
string expOutput[40][20];char Argument[20][30];

//Declare require Classes

class MNT{
    public:
        int mnIndex=0;
        string macroName;
        int mdtIndex;
        //int mdtendindex;
}_mnt[25];

class MDT{
    public:
        int mdIndex;
        string card;
}_mdt[25];
    
class ALA{
    public:
        string name;
        string formalArg;
        int positionIndex;
        string actArg;
}_ala[25];

class Output{
    public:
        string output;
}op[40];

//Push initial values into MNT Table

void pushMNT(int index, string mac){
    _mnt[index].mnIndex=index+1;
    _mnt[index].macroName=mac;
    mnt_index+=1;
}

//Push initial values into ALA Table

void pushALA(string macro, string f_arg,int posIndex, int line){
    _ala[line].name=macro;
    f_arg.erase(f_arg.find_last_not_of("\n\r") + 1);
    if(f_arg.length()>4)
        f_arg[4]=0;
    _ala[line].formalArg=f_arg;
    _ala[line].positionIndex=posIndex;
    ala_index+=1;
}

//Push initial values into MDT Table

void pushMDT(string str){
    _mdt[mdt_index].mdIndex=mdt_index+1;
    _mdt[mdt_index].card=str;
    
    if(mdtStatus==1){
        _mnt[mnt_index-1].mdtIndex=mdt_index+1;//cout<<"\nIn card:"<<str;
    }
    mdtStatus=3;
    mdt_index++;
}

//Fill actual parameters in ALA Table

void push_actualParaALA(string macroName, string actual_argument, int posIndex){
    int line=0;
    for(int i=0;i<ala_index;i++){
        if(macroName==_ala[i].name && _ala[i].positionIndex==posIndex){
            line =i;
        }
        
    }
    _ala[line].actArg= actual_argument;
}

//Push Actual Parameters in ALA during Nested Call
void push_actualNestedParaALA(string macroName, string argument,int posIndex){
    int line=0,line1=0,found=0;
    string Val;
    cout<<"\nArgument:"<<argument<<" PosIndex:"<<posIndex;
    for(int i=0;i<ala_index;i++){
        if(argument== _ala[i].formalArg){
            line =i;
        //    cout<<"\nAt line:"<<line<<"Pos Index:"<<posIndex<<endl;
        }
        if(macroName== _ala[i].name && found==0){
            line1 =i;
            found=1;
            //cout<<"\nAt line:"<<line<<"Pos Index:"<<posIndex<<endl;
        }
    }
    Val=_ala[line].actArg;
    //cout<<" Val:"<<Val<<" Line1:"<<line1<<" lineno."<<line1-1+posIndex;
    _ala[line1-1+posIndex].actArg=Val;
    
}


//Find all the arguments in a given line
void findArguments(string macro, string mainstr){
    int pos2=0,i=0, argStr_size=0,isNested=0,same=1,similar=0;
    char arg_str[25];
    string argstr,actVal;
    //38 32 >64
    for(i=0;i<20;i++){
        for(int j=0;j<25;j++){
            Argument[i][j]=0;
        }
    }
    a=0;
    //pos2=mainstr.find(" ");
    argstr[0]='\0';
    //cout<<"Main str:"<<mainstr<<endl;
    argstr=mainstr;//.substr(pos2+1);;
    argStr_size=argstr.size();
    for(i=0;i<argstr.size();i++){
            arg_str[i]=argstr[i];
    }
    //cout<<"ARg size:"<<argstr;
    arg_str[argStr_size]=0;
    char *token=strtok(arg_str," ,");
    while(token!=NULL){
      //  cout<<" Token:"<<token<<endl;
        for(i=0;i<strlen(token);i++){
            Argument[a][i]=token[i];
            
        }
        
        Argument[a][i]=0;
        push_actualParaALA(macro,Argument[a],a+1);
        a+=1;
                //posIndex+=1;
        token[0]='\0';
        token=strtok (NULL, " ,");
    } 
}

//Find all the arguments in a line in nested call(macro defination)
void findNestedArguments(string macro, string mainstr){
    int pos2=0,i=0, argStr_size=0,isNested=0,same=1,similar=0;
    char arg_str[30];
    string argstr,actVal;
    //38 32 >64
    for(i=0;i<10;i++){
        for(int j=0;j<10;j++){
            Argument[i][j]=0;
        }
    }
    a=0;
    argstr[0]='\0';
    argstr=mainstr;//.substr(pos2+1);;
    argStr_size=argstr.size();
    for(i=0;i<argstr.size();i++){
            arg_str[i]=argstr[i];
    }
    //cout<<"ARg size:"<<argstr;
    arg_str[argStr_size]=0;
    char *token=strtok(arg_str," ,");
    while(token!=NULL){
        //cout<<" Token:"<<token<<endl;
        for(i=0;i<strlen(token);i++){
            Argument[a][i]=token[i];
            
        }
        
        Argument[a][i]=0;
        //cout<<"\nmacro:"<<macro<<"  Argument:"<<Argument[a]<<" a+1:"<<a+1 ; 
            //cout<<"actual_Arg:"<<a+1<<" Argument:"<<Argument[a]<<endl;
            push_actualNestedParaALA(macro,Argument[a],a+1);
            
        
        a+=1;
                //posIndex+=1;
        token[0]='\0';
        token=strtok (NULL, " ,");
    } 
}


//Expanded Code Output
void fillOutput(string macro){
    int start=0,stat=0,endstate=0,j=0,ln=0,i=0,nstat=0, pos=0,alapos=0,storagelen,argpos;
    int length=0,nested=0;
    string check1,storage,check_arg,impstore,check2;
    for(i=0;i<mnt_index;i++){           //Finding the MACRO called
        check1=_mnt[i].macroName;
        if(macro==check1){
            start=_mnt[i].mdtIndex;
        }
    }
    
    start-=1;
     while(endstate==0){

        alapos='\0';
        storage=_mdt[start].card;
        storage.erase(storage.find_last_not_of("\n\r") + 1);
        storagelen=storage.length();
        string newMacro;
        nested=0;
        // newMacro=storage;
        int newPos=0;
        newPos=storage.find(" ");
        newMacro=storage.substr(0,newPos);
        //cout<<"New Macro:"<<newMacro<<endl;
        for(i=0;i<mnt_index;i++){           //Finding the MACRO called in MDT card if encountered
            check2=_mnt[i].macroName;
            if(newMacro==check2){
                nested=1;
            }
        }
        
        if(nested==1){                                           //For recursive call
            check_arg=storage.substr(newPos+1);
            cout<<"Nested Macro:"<<newMacro<<endl;
            findNestedArguments(newMacro, check_arg);
            //push_actualParaALA(newMacro,Argument[a],a+1);
            fillOutput(newMacro);
            cout<<"\nI'm out!"<<endl;
            start+=1;
           // nested=0;
        }
        if(nested!=1){
            for(i=0;i<storagelen;i++){
                if(storage[i]=='&')
                    argpos=i;
            }
            length=storagelen-argpos;
            for(i=0;i<ala_index;i++){
                //cout<<"Storage:";
                for(j=0;j<length;j++){
                    if(storage[argpos+j]==_ala[i].formalArg[j]){
                        
                        //alapos=i;
                        //cout<<storage[argpos+j]<<" "<<_ala[i].formalArg[j]<<endl;
                        ln+=1;
                        if(ln>=3){
                           // cout<<"I have entered";
                            alapos=i;
                            stat=1;
                            break;
                        }
                        //continue;
                    }
                   
                }
                //cout<<endl;
                ln=0;
        
            }
            
            impstore.erase(impstore.find_last_not_of("\n\r") + 1);
            //cout<<"At final moment!";
            
            impstore=storage.substr(0,argpos);
            impstore.append(_ala[alapos].actArg); 
            
            op[op_line].output=impstore;
            op_line++;
            start+=1;
            stat=0;
           // nstat=0;
            if(_mdt[start].card=="MEND")
                 endstate=1;
        }

    }
}

//Process each line of string to fill the respective tables etc.

void processString(string str){
    string teststr,mainstr,argstr, macroName,argument,actualmacro,mname;
    int posIndex=0;
    char Arguments[10][10],arg_str[25];
    int noArg=1,pos=0,pos1=0,i=0,pos2=0,a=0,j=0,finalStatus=0;
    pos=str.find(" ");
    teststr=str.substr(0,pos);
    if(teststr=="MACRO"){
        macStatus=1;
        mdtStatus=1;
    }
    else
        macStatus=0;
    if(macStatus==0 && mdtStatus==0){   //Triggered when dealing with normal strings /All the tables have been filled/MACRO call
        int mntpos,index1=0,stat=0;string check;
        
        teststr.erase(teststr.find_last_not_of("\n\r") + 1);
        
        for(i=0;i<mnt_index;i++){           //Finding the MACRO called
            check=_mnt[i].macroName;
            
            if(teststr==check){
                finalStatus=1;
                index1=i;   
                //cout<<"&&&&&";
                
            }
        }
        if(finalStatus==1){
            mainstr=str.substr(pos+1);
            findArguments(teststr,mainstr);
            finalStatus=0;
            fillOutput(teststr);
        }
        else{
            //int len;
            op[op_line].output=str;
            op_line++;
        }
    }
    
    if(macStatus==1){       //Triggered when Encounter a MACRO defination line
        posIndex=1;
        mainstr=str.substr(pos+1);
        //cout<<"\nmainstr:"<<mainstr;
        pos2=mainstr.find(" ");
        macroName=mainstr.substr(0,pos2);
        pushMNT(mnt_index,macroName);
        for(i=0;i<mainstr.size();i++){
            int x=0;
            if(mainstr[i]=='&'){
                noArg=0;
               // pushALA(macroName,"000",x,ala_index);
            }
        }
        
        if(noArg==0){
            argstr=mainstr.substr(pos2+1);
            argstr[argstr.size()]=0;
           // cout<<"\nargstr:"<<argstr;
            for(i=0;i<argstr.length();i++){
                arg_str[i]=argstr[i];
            }
            arg_str[i]=0;
            char *token=strtok(arg_str," ,");
            while(token!=NULL){
                //cout<<endl;
                for(i=0;i<strlen(token);i++){
                    Arguments[a][i]=token[i];
                }
                //cout<<"i:"<<i;
                
                Arguments[a][i]=0;
                
                pushALA(macroName,Arguments[a],posIndex,ala_index);
                a+=1;
                posIndex+=1;
                token[0]=0;
                token=strtok (NULL, " ,");
            }
            macStatus=2;
        }
        else{
            pushALA(macroName,"000",0,ala_index);
        }
    }
    
    if((mdtStatus==1 || mdtStatus==3) && macStatus==0){ //Triggered when inside macro defination
        pushMDT(str);
        //mdtStatus=0;
        if(teststr=="MEND"){
            mdtStatus=0;
        }
    }
    
}


//Functions for displaying each table
void displayMNT(){
    cout<<"\n\n\t\t########## MNT Table ###########";
    cout<<"\n\nMNT Index\t\tMacro Name\t\tMDT index\n";
    for(int i=0; i<mnt_index;i++){
        cout<<"\n"<<_mnt[i].mnIndex<<"\t\t\t"<<_mnt[i].macroName;
        cout<<"\t\t\t"<<_mnt[i].mdtIndex;
    }       
}
void displayALA(){
    cout<<"\n\n\t\t############ ALA Table ############";
    cout<<"\n\nALA MACname\t\tFormal Parameters\t\tPositional Arg\t\tActual Parameters\n";
    for(int i=0; i<ala_index;i++){
        cout<<"\n"<<_ala[i].name<<"\t\t\t"<<_ala[i].formalArg<<"\t\t\t\t";
        cout<<"#"<<_ala[i].positionIndex<<"\t\t\t"<<_ala[i].actArg;
    }        
}

void displayMDT(){
    //cout<<mdt_index;
    cout<<"\n\n\t\t\t############MDT Table############";
    cout<<"\n\nMDT Index\t\tCard\n";
    for(int i=0; i<mdt_index;i++){
        cout<<"\n"<<_mdt[i].mdIndex<<"\t\t\t"<<_mdt[i].card;
    }        
}

void readInput(){
    string inputString;
    fstream file;
    file.open("input.txt",ios::in);
    if(!file){
        cout<<"Input File not present.";
    }
    else{
        while(!file.eof()){
            getline(file,inputString);
            //cin.ignore();
            //cout<<inputString<<endl;
            inputString.erase(inputString.find_last_not_of("\n\r") + 1);
            processString(inputString);
        }
    }
    file.close();
}

int main(){
    
    readInput();
    displayALA();
    displayMDT();
    displayMNT();
    //cout<<"().()";
    cout<<"\n\n\n\nEXPANDED CODE:-\n";
    //cout<<"\nPositional Arg: #"<<_ala[3].positionIndex;
    for(int i=0;i<op_line;i++)
        cout<<op[i].output<<endl;
    return 0;
}


