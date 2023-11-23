//badger
#include <iostream>
#include <cstring>
#include <random>
#include <fstream>

using namespace std;

typedef unsigned long long ULL;

const bool is_even(const ULL a1){return a1%2==0;}
const ULL get_odd(const ULL a1){return (3*a1)+1;}
const ULL get_even(const ULL a1){return a1/2;}
const ULL get_num(const ULL a1){return is_even(a1)?get_even(a1):get_odd(a1);}

class clr{
public:
	enum types{	end=0,
			bold=1,
			dim=2,
			under_line=4,
			blink=5,
			strike_through=9,
			fg=38,
			bg=48,
			over_line=53};
	int num=0;
	types type=types::fg;

	clr(const int num2=0,const types type2=types::fg):num(num2),type(type2){}
	clr(const clr& a1):num(a1.num),type(a1.type){}
	clr(const clr& a1,const types type2):num(a1.num),type(type2){}
	operator string() const {
		string beginz="\033[";
		if(type==types::end){return beginz+to_string(type)+"m";}
		return beginz+to_string(type)+";5;"+to_string(num)+"m";
	}
};

const clr black=	clr(0b0000);
const clr white=	clr(0b1111);
const clr red=		clr(0b1001);
const clr orange=	clr(0b1011);
const clr yellow=	clr(0b0011);
const clr green=	clr(0b1010);
const clr blue=		clr(0b1100);
const clr indigo=	clr(0b1101);
const clr violet=	clr(0b0101);
const clr endz=		clr(0b0000,clr::types::end);

string get_ui(const string a1,const clr a2=white){
	return (string)a2+a1+(string)endz;
}

struct text{
public:
	string txt="";
	clr fg=white;

	operator string() const {
		return (string)fg+txt+(string)endz;
	}
};

int get_random(const int min=0,const int max=100){
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> dist(min,max);
	return dist(mt);
}

string get_num_fmt(const string str){
	string ans="";
	int count=0;
	for(int a=str.length()-1;a>=0;a--){
		if(count==3){
			ans=","+ans;
			count=0;
		}
		ans=str[a]+ans;
		count++;
	}
	return ans;
}

string get_file_clr(const string file_name){
	ifstream file(file_name);
	string line;
	int a=0;
	string ans="";
	while(getline(file,line)){
		ans+=get_ui(line,clr((a%0b1111)+1))+"\n";
		a++;
	}
	return ans.substr(0,ans.length()-1);
}

int get_len(const string str){
	int count=0;
	int pos=0;
	for(char c:str){
		if(c=='\t'){
			int spaces=8-(pos%8);
			count+=spaces;
			pos+=spaces;
		}
		else{
			count++;
			pos++;
		}
		if(c=='\n'){
			pos=0;
		}
	}
	return count;
}

string get_plain_txt(const string str){
	string ans="";
	bool is_clr=false;
	for(char c:str){
		if(c=='\033'){
			is_clr=true;
		}
		else if(is_clr){
			if(c=='m'){
				is_clr=false;
			}
		}
		else{
			ans+=c;
		}
	}
	return ans;
}

void print(const string a1,const char end_chr='\n'){cout<<a1<<end_chr;}
void print(const string a1,const clr a2,const char end_chr='\n'){cout<<get_ui(a1,a2)<<end_chr;}
void print(const text* a1,const char end_chr='\n'){cout<<(string)*a1<<end_chr;}
void print_per(const string a1,const char a2,const char end_chr='\n'){
	for(int a=0;a<a1.length();a++){cout<<a2;}
	cout<<end_chr;
}
void print_per(const int a1,const char a2,const char end_chr='\n'){
	for(int a=0;a<a1;a++){cout<<a2;}
	cout<<end_chr;
}
void print_box(const string txt,const char chr){
	char opp=chr;
	if(chr=='-'){opp='|';}
	string plain_txt=get_plain_txt(string(1,opp)+" "+txt+" "+string(1,opp));
	const int len=get_len(plain_txt);
	print_per(len,chr);
	print(string(1,opp)+" "+txt+" "+string(1,opp));
	print_per(len,chr);
}

string get_input(const string a1){
	string a2="";
	print(a1);
	getline(cin,a2);
	return a2;
}

unsigned long long get_arg(const string cmd_arg){
	unsigned long long num=0;
	int pos=0;
	int exp=0;
	for(char c:cmd_arg){
		if(c=='^'){
			exp=stoll(cmd_arg.substr(pos+1,cmd_arg.length()-pos));
			break;
		}
		pos++;
	}
	if(exp!=0){
		num=pow(stoll(cmd_arg.substr(0,pos)),exp);
	}
	else{
		num=stoll(cmd_arg);
	}
	return num;
}

string pad_text(string text,int width,int pad=0,char padding_char=' '){
	int text_length=get_len(text);
	int padding_length=width-text_length;
	padding_length-=pad;
	if(padding_length<=0){
		return text;
	}
	int left_padding_length=padding_length/2;
	int right_padding_length=padding_length-left_padding_length;
	string left_padding=string(left_padding_length,padding_char);
	string right_padding=string(right_padding_length,padding_char);
	return left_padding+text+right_padding;
}

string get_table(string** table,int rows,int cols){
	string ans="";
	int* col_widths=new int[cols];
	for(int a=0;a<cols;a++){
		col_widths[a]=0;
		for(int b=0;b<rows;b++){
			int len=get_len(table[b][a]);
			if(len>col_widths[a]){
				col_widths[a]=len;
			}
		}
	}
	for(int a=0;a<rows;a++){
		for(int b=0;b<cols;b++){
			ans+=pad_text(table[a][b],col_widths[b],1);
			if(b!=cols-1){
				ans+=" | ";
			}
		}
		ans+="\n";
		if(a!=rows-1){
			for(int b=0;b<cols;b++){
				ans+=string(col_widths[b]+3,'-');
				if(b!=cols-1){
					ans+="+-";
				}
			}
			ans+="\n";
		}
	}
	return ans;
}

int main(int args,char* argv[]){
	unsigned long long num=get_random();
	if(args>1){
		if((string)argv[1]=="-h"){
			print(get_file_clr("./logo.txt")+"\n");
			print(new text{.txt="usage:\t"+(string)argv[0]+" <#>",.fg=red});
			print("");
			print(new text{.txt="-h:\thelp.",.fg=orange});
			print(new text{.txt="-a:\tabout.",.fg=yellow});
			print(new text{.txt="-i:\tinfo.",.fg=green});
			print(new text{.txt="-c:\tcontext.",.fg=blue});
			exit(0);
		}
		else if((string)argv[1]=="-a"){
			print(get_file_clr("./logo.txt")+"\n");
			print(new text{.txt="* #: inputted number.",.fg=red});
			print(new text{.txt="\t* restriction: 0<#<∞.",.fg=orange});
			print(new text{.txt="* n: iterations.",.fg=yellow});
			print(new text{.txt="\n* rules:",.fg=green});
			print(new text{.txt="\t* even: #/2.",.fg=blue});
			print(new text{.txt="\t* odd: (3*#)+1.",.fg=indigo});
			print(new text{.txt="\n* info.:",.fg=violet});
			print(new text{.txt="\t* #=>n ; 2*#=>n+1",.fg=red});
			exit(0);
		}
		else if((string)argv[1]=="-i"){
			print(get_file_clr("./logo.txt")+"\n");
			print(new text{.txt="* author: Badger Code (https://github.com/321BadgerCode).",.fg=red});
			print(new text{.txt="* version: 1.0.0.",.fg=orange});
			print(new text{.txt="* license: MIT.",.fg=yellow});
			print(new text{.txt="* description: collatz conjecture.",.fg=green});
			print(new text{.txt="* usage: "+(string)argv[0]+" <#>",.fg=blue});
			exit(0);
		}
		else if((string)argv[1]=="-c"){
			print(get_file_clr("./logo.txt")+"\n");
			print(get_ui("The Collatz conjecture",orange)+", also known as the "+get_ui("3n+1",orange)+" problem, is one of the most famous unsolved problems in mathematics. It was first proposed by "+get_ui("Lothar Collatz",orange)+" in "+get_ui("1937",orange)+", although it had been independently discovered by others earlier. The conjecture is very simple to state, but it has proven to be extremely difficult to prove.");
			print("\n"+get_ui("Summary",green)+":\n\t"+get_ui("The Collatz conjecture",orange)+" states that if you take any positive integer and repeatedly apply the following rules, you will eventually reach the number "+get_ui("1",orange)+":\n\t"+get_ui("*",indigo)+" If the number is "+get_ui("even",violet)+", "+get_ui("divide it by 2",yellow)+".\n\t"+get_ui("*",indigo)+" If the number is "+get_ui("odd",violet)+", "+get_ui("multiply it by 3 and add 1",yellow)+".\n\n\tFor example, if you start with the number "+get_ui("7",orange)+", you would get the following sequence: "+get_ui("7, 22, 11, 34, 17, 52, 26, 13, 40, 20, 10, 5, 16, 8, 4, 2, 1",clr(white,clr::types::under_line))+".");
			print("\n"+get_ui("History",green)+":\n\t"+get_ui("The Collatz conjecture",orange)+" was first proposed by "+get_ui("Lothar Collatz",orange)+" in "+get_ui("1937",orange)+" in a talk at the University of Marburg. However, it had been independently discovered by others earlier. For example, the mathematician "+get_ui("Mario Luciani",orange)+" had proposed a similar conjecture in "+get_ui("1950",orange)+".\n\n\tThe conjecture has been extensively studied by mathematicians. For example, the conjecture has been verified for all positive integers up to at least "+get_ui("2^68",orange)+".\n\n\tIn "+get_ui("1985",orange)+", the mathematician "+get_ui("Jeffrey Lagarias",orange)+" offered a "+get_ui("$1,000",orange)+" prize for a proof of the "+get_ui("Collatz conjecture",orange)+". However, the prize has not yet been awarded.");
			print("\n"+get_ui("References:",green)+":\n\t* "+get_ui("https://en.wikipedia.org/wiki/Collatz_conjecture",blue));
			exit(0);
		}
		else{
			num=get_arg(argv[1]);

			while(num<=0){
				print(new text{.txt="0<#<∞!",.fg=red});
				print("(if ur seeing this message & ur input is a natural #, then it may be > (2^64)-1)");
				num=get_arg(get_input("#: "));
			}
		}
	}
	const int a1=num;

	int a=0;
	unsigned long long last_num=0;
	//string** table=new string*[num];
	while(num!=1){
print_row:
		string num_fmt=get_num_fmt(to_string(num));
		string row=	get_ui(get_num_fmt(to_string(a))+".\t",red)+
				get_ui(num_fmt,orange)+"\t"+
				(is_even(num)?get_ui("even",yellow):get_ui("odd",yellow))+"\t"+
				(num!=1?(is_even(num)?get_ui(num_fmt,green)+"/2":"(3*"+get_ui(num_fmt,green)+")+1")+get_ui("=",clr(white,clr::types::dim))+get_ui(get_num_fmt(to_string(get_num(num))),clr(blue,clr::types::bold)):get_ui("end",indigo));
		(a==0?print_box(row,'-'):print(row));

			if(num<=0||num==last_num){print(new text{.txt="[._.]: AN ERROR OCCURED IN THE PROGRAM!!!",.fg=red});break;}
			if(a%2==0){last_num=num;}

			if(num==1){break;}
			num=get_num(num);
			a++;
			if(num==1){goto print_row;}

			//table[a]=new string[4]{to_string(a),to_string(num),is_even(num)?"even":"odd",to_string(get_num(num))};
	}
	// TODO: fix table (segfault).
	//print(get_table(table,a+1,4));

	print("\n"+get_ui(get_num_fmt(to_string(a1)),red)+" is 1 after "+get_ui(get_ui(get_ui(get_num_fmt(to_string(a)),clr(green,clr::types::under_line)),clr(green,clr::types::bold)),clr(green,clr::types::over_line))+" iterations.");

	return 0;
}
// TODO: color types revert text to default white color.