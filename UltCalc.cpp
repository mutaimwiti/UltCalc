//PREPROCESSOR
#include<iostream>
#include<string>
#include<cmath>
#include<algorithm>
#include<vector>
#include<iomanip>
#ifdef __WINDOWS__
	#include<Windows.h>
#endif

using namespace std;

//FUNCTION PROTOTYPES
void get_Input();
void check_syntax();
void syntax_error();
void simplify_operations();
void filter_input(int,int);
void separate_elements();
void calculate();
void reset_calc();
void clearScreen();

//STRUCTURE operation
struct operation
{
    char operation;
    int position;
};

//GLOBAL VARIBLES

string input;
vector<operation>op_data;
vector<char>filter_State,operations;
vector<double>operands;
short int d;
double filter_True;
bool negate_first=false,prev_filter=false,negate_Roperand=false;
unsigned short int input_length,op_data_size,filter_Size,filter_Count,dec_pos,divider,dec_count,tmp,x,adjacent_signs,final_op_pos;

//MAIN
int main()
{
    cout<<setprecision(10);
	cout<<"DynamicInputCalc.byMUTAI v1.0.0"<<endl<<endl<<"clear screen: 'cl' ; exit: 'ext'"<<endl<<endl
        <<": ";
    get_Input();
}

//FUNCTION DEFINITIONS

//syntax_error
void syntax_error()
{
    cout<<endl<<"Syntax Error!"<<endl<<endl
        <<": ";
    get_Input();
}
void get_Input()
{
    getline(cin,input);
    check_syntax();
}

//check_syntax
void check_syntax()
{
    operation op_temp;
	short int previous_op_pos=-2;
	unsigned short int c,prev_dec;
    input.erase(remove(input.begin(),input.end(),' '),input.end());
    input_length=input.size();
    int input_end=input_length-1;
    if(input[0]=='*'||input[0]=='/')
    {
        reset_calc();
        syntax_error();
    }
    if((input.compare(0,2,"cl")==0||input.compare(0,2,"CL")==0)&&input_length==2)
    {
        clearScreen();
	    cout<<"DynamicInputCalc.byMUTAI "<<endl<<endl<<"clear screen: 'cl' ; exit: 'ext'"<<endl<<endl
            <<": ";
        get_Input();
    }
    else if((input.compare(0,3,"ext")==0||input.compare(0,3,"EXT")==0)&&input_length==3)
        exit(0);
    else
    {
        if(input[0]=='/'||input[0]=='*')
        {
            reset_calc();
            syntax_error();
        }
        //checking whether input begins with multiplication or division sign
        if(input[input_end]=='*'||input[input_end]=='/'||input[input_end]=='+'||input[input_end]=='-')
        {
            reset_calc();
            syntax_error();
        }
        //checking whether the input ends with a multiplication, division, addition or subtraction sign
        for(c=0;c<input_length;c++)
            {
                input[c]=input[c];
                if(!(input[c]>','&&input[c]<':')&&!(input[c]>')'&&input[c]<','))
                {
                    reset_calc();
                    syntax_error();
                }//checking whether the input entails only 1,2,3,4,5,6,7,8,9,0,".","+","-","/","*"
            }
        for(c=0;c<input_length;c++)
        {
            if(input[c]=='+'||input[c]=='*'||input[c]=='-'||input[c]=='/')
            {
                if(c-previous_op_pos==1&&input[c]=='*')
                {
                    reset_calc();
                    syntax_error();
                }
                //checking for [op],*
                else if(c-previous_op_pos==1&&input[c]=='/')
                {
                    reset_calc();
                    syntax_error();
                }
                //checking for [op],/
                previous_op_pos=c;
                op_temp.operation=input[c];
                op_temp.position=c;
                op_data.push_back(op_temp);
            }//locating positions of operation signs and placing them in a data structure array
        }
        op_data_size=op_data.size();
    }
    simplify_operations();
    op_data_size=op_data.size();//update
    if(op_data_size==0||op_data_size==1&&input[0]=='-')
    {
        cout<<input<<endl<<endl<<": ";
        reset_calc();
        get_Input();
    }//checking whether no operations have been provided
    final_op_pos=op_data[op_data_size-1].position;
    if(op_data[op_data_size-1].position-op_data[op_data_size-2].position==1)
        final_op_pos=op_data[op_data_size-2].position;
    separate_elements();
}

//simplify_operations
void simplify_operations()
{
    unsigned short int n,temp_pos;
    for(int c=0;c<op_data.size();c++)
    {//adjacent ++ removal
        if(op_data.size()>(c+1)&&op_data[c+1].position-op_data[c].position==1&&op_data[c].operation=='+'&&op_data[c+1].operation=='+')
        {
            temp_pos=op_data[c+1].position;
            op_data.erase(op_data.begin()+(c+1));
            n=c+1;
            while(n<op_data.size())
            {
                op_data[n].position--;
                n++;
            }
            input.erase(input.begin()+temp_pos);
            if(c==op_data.size()-1)
                break;
            c--;
        }//adjacent -+ removal
        else if(op_data.size()>(c+1)&&op_data[c+1].position-op_data[c].position==1&&op_data[c].operation=='-'&&op_data[c+1].operation=='+')
        {
            temp_pos=op_data[c+1].position;
            op_data.erase(op_data.begin()+(c+1));
            n=c+1;
            while(n<op_data.size())
            {
                op_data[n].position--;
                n++;
            }
            input.erase(input.begin()+temp_pos);
            if(c==op_data.size()-1)
                break;
            c--;
        }//adjacent -- removal
        else if(op_data.size()>(c+1)&&op_data[c+1].position-op_data[c].position==1&&op_data[c].operation=='-'&&op_data[c+1].operation=='-')
        {
            temp_pos=op_data[c+1].position;
            op_data[c].operation='+';
            input[op_data[c].position]='+';
            op_data.erase(op_data.begin()+(c+1));
            n=c+1;
            while(n<op_data.size())
            {
                op_data[n].position--;
                n++;
            }
            input.erase(input.begin()+temp_pos);
            if(c==op_data.size()-1)
                break;
            c--;
        }//adjacent +- removal
        else if(op_data.size()>(c+1)&&op_data[c+1].position-op_data[c].position==1&&op_data[c].operation=='+'&&op_data[c+1].operation=='-')
        {
            temp_pos=op_data[c+1].position;
            op_data[c].operation='-';
            input[op_data[c].position]='-';
            op_data.erase(op_data.begin()+(c+1));
            n=c+1;
            while(n<op_data.size())
            {
                op_data[n].position--;
                n++;
            }
            input.erase(input.begin()+temp_pos);
            if(c==op_data.size()-1)
                break;
            c--;
        }
    }
    if(input[0]=='+')
    {
        input.erase(input.begin());
        op_data.erase(op_data.begin());
        int n=0;
        while(n<op_data.size())
        {
            op_data[n].position--;
            n++;
        }
    }
}

//separate_elements
void separate_elements()//separates the operations and the operands
{
    int starting_posA,starting_posB;
    bool skip_next=false;
    input_length=input.size();//update after simplifying
    if(input[0]=='-')
        negate_first=true;
    for(int c=1;c<input_length;c++)
    {
        if(input[c]=='+'||input[c]=='-'||input[c]=='*'||input[c]=='/')
        {
            operations.push_back(input[c]);
            starting_posA=c;
            starting_posB=c;
            if(input[c+1]=='-')
            {
                starting_posB++;
                negate_Roperand=true;
                skip_next=true;
                adjacent_signs++;
            }
            else if(input[c+1]=='+')
            {
                starting_posB++;
                skip_next=true;
                adjacent_signs++;
            }
            if(prev_filter==true&&c!=final_op_pos)
            {
                prev_filter=false;
                c+=1;
                continue;
            }
            c+=skip_next;
            filter_input(starting_posA,starting_posB);
        }
    }
    if(negate_first==true)
        operands[0]=-abs(operands[0]);
    calculate();
}

//filter_input
void filter_input(int pos_a,int pos_b)//filters the string to produce real numbers
{
    filter_State.clear();
    prev_filter=true;
    filter_True=0;
	filter_Count=0;
	dec_pos=0;
	dec_count=0;
    if(!(pos_a==final_op_pos&&(op_data_size-negate_first-adjacent_signs)%2==0))
    {
        for(d=pos_a-1;((input[d]>'/'&&input[d]<':')||input[d]=='.');d--)
        {
			if(input[d]=='.')
            {
                dec_pos=filter_Count;
                dec_count++;
            }
            else
                filter_State.push_back(input[d]);
            if(dec_count>1)
            {
                reset_calc();
                syntax_error();
            }//checking whether there is more than one decimal in one number
            filter_Count++;
        }
        reverse(filter_State.begin(),filter_State.end());
        filter_Size=filter_State.size();
        if(dec_pos>0)
        {
            tmp=1;
            for(x=0;x<filter_Size;x++)
            {
                tmp++;
                filter_True+=(filter_State[x]-48)*pow(10,(filter_Count-tmp));
            }
            filter_True/=pow(10,dec_pos);
        }
        else
        {
            tmp=0;
            for(x=0;x<filter_Size;x++)
            {
                tmp++;
                filter_True+=(filter_State[x]-48)*pow(10,(filter_Count-tmp));
            }
        }
        operands.push_back(filter_True);
    }
    //updating some of the variables shared by both filtering loops
    filter_State.clear();
    filter_True=0;
    filter_Count=0;
    dec_pos=0;
    dec_count=0;//to determine whether there are is more than one decimal in a number
    for(d=pos_b+1;((input[d]>'/'&&input[d]<':')||input[d]=='.');d++)
    {
        filter_Count++;
        if(input[d]=='.')
        {
            dec_pos=filter_Count;
            dec_count++;
        }
        else
            filter_State.push_back(input[d]);
        if(dec_count>1)
        {
            reset_calc();
            syntax_error();
        }//checking whether there is more than one decimal in one number
    }
    filter_Size=filter_State.size();
    if(dec_pos>0)
    {
        tmp=1;
        divider=filter_Count-dec_pos;
        for(x=0;x<filter_Size;x++)
        {
            tmp++;
            filter_True+=(filter_State[x]-48)*pow(10,(filter_Count-tmp));
        }
        filter_True/=pow(10,divider);
    }
    else
    {
        tmp=0;
        for(x=0;x<filter_Size;x++)
        {
            tmp++;
            filter_True+=(filter_State[x]-48)*pow(10,(filter_Count-tmp));
        }
    }
    if(negate_Roperand==true)
    {
        filter_True=-abs(filter_True);
        negate_Roperand=false;
    }
    operands.push_back(filter_True);
}

//calculate
void calculate()
{
    unsigned short int c;
    double temp;
    for(c=0;c<operations.size();c++)
    {
        if(operations[c]=='/')
        {
            temp=operands[c]/operands[c+1];
            operations.erase(operations.begin()+c);
            operands.erase(operands.begin()+c);
            operands.erase(operands.begin()+c);
            operands.insert(operands.begin()+c,temp);
            c--;
        }
    }//division
    for(c=0;c<operations.size();c++)
    {
        if(operations[c]=='*')
        {
            temp=operands[c]*operands[c+1];
            operations.erase(operations.begin()+c);
            operands.erase(operands.begin()+c);
            operands.erase(operands.begin()+c);
            operands.insert(operands.begin()+c,temp);
            c--;
        }
    }//multiplication
    int negative_pos=-1;
    for(c=0;c<operations.size();c++)
    {
        if(operations[c]=='-')
        {
            negative_pos=c;
            break;
        }
    }//locating first negative sign
    if(negative_pos>-1)
    {
        for(c=negative_pos+1;c<operations.size();c++)
        {
            if(operations[c]=='+')
            {
                if(c>negative_pos)
                {
                    operands.insert(operands.begin()+(negative_pos+1),operands[c+1]);
                    operands.erase(operands.begin()+(c+2));
                    operations.insert(operations.begin()+negative_pos,'+');
                    operations.erase(operations.begin()+(c+1));
                    negative_pos++;
                }
            }
        }
    }/*shifting operands to be subtracted to the right. If this block is removed some calculations will have errors e.g.
    12/2-5+3*7 without it the result of 3*7 i.e. 21 would be added to 5 before subtracting the later result i.e. 26
    from the result of 12/2 i.e 6. This is mathematically WRONG hence the need to shift whatever should be subtracted to
    the right*/
    for(c=0;c<operations.size();c++)
    {
        if(operations[c]=='+')
        {
            temp=operands[c]+operands[c+1];
            operations.erase(operations.begin()+c);
            operands.erase(operands.begin()+c);
            operands.erase(operands.begin()+c);
            operands.insert(operands.begin()+c,temp);
            c--;
        }
    }//addition
    for(c=0;c<operations.size();c++)
    {
        if(operations[c]=='-')
        {
            temp=operands[c]-operands[c+1];
            operations.erase(operations.begin()+c);
            operands.erase(operands.begin()+c);
            operands.erase(operands.begin()+c);
            operands.insert(operands.begin()+c,temp);
            c--;
        }
    }//subtraction
    cout<<endl<<"Ans="<<operands[0]<<endl<<endl<<": ";
    reset_calc();
    get_Input();
}

//reset_calc
void reset_calc()
{
    op_data.clear();
    op_data.clear();
    negate_first=false;
    operations.clear();
    prev_filter=false;
    negate_Roperand=false;
    operands.clear();
    adjacent_signs=0;
}

void clearScreen()
{
	#ifdef __LINUX__
		cout<<"\x1B[2J\x1B[H";
	#endif
	#ifdef __WINDOWS__
		system("cls");
	#endif
}
