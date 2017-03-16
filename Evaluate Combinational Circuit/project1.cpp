#include <vector>
#include <stack>
#include <deque>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string.h>
#include <unordered_map>

using namespace std;

string pureWord(string &word);//eliminate all spaces

int main(int argc, char *argv[]){

string netfile=argv[1];
string valuefile=argv[2];

unordered_map<string,bool> valuelist;
//unordered_map<string,netT> outlist;
vector<string>	outnames;
vector<string>  innames;
string temp;

ifstream iFile;

iFile.open(valuefile);//.c_str()
while (!iFile.eof()){
	string inputname;
	string inputvalue = "1"; //string test="1";
	bool valueb=0; int value = 0;
	getline(iFile,inputname,' ');
	getline(iFile,inputvalue);
	//cout<<inputname<<' '<<inputvalue<<endl;
	value=std::stoi(inputvalue);//bool value
	if (value==1) valueb=1;
	else valueb =0;
	valuelist[inputname]=valueb;

}
iFile.close();
iFile.open(netfile);
while (getline(iFile,temp)){
	string netlist = pureWord(temp);
	if (netlist[0]=='#'){
	}
	else if (netlist[0]=='\n'){
	}
	else if ((netlist.find("input")!=std::string::npos) || (netlist.find("INPUT")!=std::string::npos)){
		//cout<<"getting inputs"<<endl;
		size_t sigstart = netlist.find('(');
		size_t siglength = netlist.find(')') - sigstart -1;
		string signame =  netlist.substr(sigstart+1,siglength);
		innames.push_back(signame);
		//cout<<signame<<endl;	
	}
	else if ((netlist.find("output")!=std::string::npos) || (netlist.find("OUTPUT")!=std::string::npos)){
		//cout<<"getting outputs"<<endl;
		size_t sigstart = netlist.find('(');
		size_t siglength = netlist.find(')') - sigstart -1;
		string signame =  netlist.substr(sigstart+1,siglength);
		//valuelist[signame].value = 0;//initial value	
		outnames.push_back(signame);//record output names for latter use
		//cout<<signame<<endl;	
	}
	else if (netlist.find("=")!=std::string::npos){
		size_t sigstart = netlist.find("=");
		string wire = netlist.substr(0,sigstart);
		//cout<<"Gate:"<<wire<<endl;

		string twoletter = netlist.substr(sigstart+1,2); 
		string threeletter = netlist.substr(sigstart+1,3); 
		string fourletter = netlist.substr(sigstart+1,4); 
		//cout<<"2,3,4:"<<twoletter<<threeletter<<fourletter<<endl;

		size_t input_pos = netlist.find('(');
		size_t input_end = netlist.find(')');
		size_t lastcomma = netlist.find_last_of(',');
		bool result = 0;
		bool done = 0;

		if ((twoletter=="or") || (twoletter=="OR") ){
			result = 0;
			while ((input_pos <= lastcomma)&& (done==0)){
				size_t comma =netlist.find_first_of(",",input_pos+1);
				size_t inlength = comma - input_pos -1;
				if (input_pos == lastcomma)	{
					done = 1;
					inlength = input_end-input_pos-1;
				}
				string inputstr = netlist.substr(input_pos+1,inlength);
				if (result==1) {}	
				else 	result = valuelist[inputstr];

				input_pos = comma;
			}			
		}
		else if ((threeletter=="AND") || (threeletter=="and") ){
			//cout<<"AND GATE"<<endl;
			result =1;
			//cout<<"start; end :"<<input_pos<<' '<<lastcomma<<endl;
			while ((input_pos <= lastcomma)&& (done==0)){
				size_t comma =netlist.find_first_of(",",input_pos+1);
				size_t inlength = comma - input_pos -1;
				if (input_pos == lastcomma)	{
					done = 1;
					inlength = input_end-input_pos-1;
				}
				string inputstr = netlist.substr(input_pos+1,inlength);
				if (result==1) { result = valuelist[inputstr]; }	
				else 	{}

				input_pos = comma;

			//cout<<"start; end :"<<input_pos<<' '<<lastcomma<<endl; cout<<done<<endl;
			}
		}
		else if ((threeletter=="NOR") || (threeletter=="nor") ){
			result =0;
			//bool start = 1;
			while ((input_pos <= lastcomma)&&(done==0)){
				size_t comma =netlist.find_first_of(",",input_pos+1);
				size_t inlength = comma - input_pos -1;
				if (input_pos == lastcomma)	{
					done = 1;
					inlength = input_end-input_pos-1;
				}
				string inputstr = netlist.substr(input_pos+1,inlength);
				if (result==1) {}	
				else 	result = valuelist[inputstr];

				input_pos = comma;
			}
			result = !result;//not or
		}
		else if ((threeletter=="XOR") || (threeletter=="xor") ){
			result =0;
			while ((input_pos <= lastcomma)&& (done==0)){
				size_t comma =netlist.find_first_of(",",input_pos+1);
				size_t inlength = comma - input_pos -1;
				if (input_pos == lastcomma)	{
					done = 1;
					inlength = input_end-input_pos-1;
				}
				string inputstr = netlist.substr(input_pos+1,inlength);
				if (result==1) { result = !valuelist[inputstr]; }	
				else 	{ result = valuelist[inputstr]; }
				//cout<<"input "<<inputstr<<",result "<<result<<endl;

				input_pos = comma;
			}
		}
		else if ((fourletter=="XNOR") || (fourletter=="xnor") ){
			result =1;
			while ((input_pos <= lastcomma)&& (done==0)){
				size_t comma =netlist.find_first_of(",",input_pos+1);
				size_t inlength = comma - input_pos -1;
				if (input_pos == lastcomma)	{
					done = 1;
					inlength = input_end-input_pos-1;
				}
				string inputstr = netlist.substr(input_pos+1,inlength);
				if (result==1) { result = valuelist[inputstr]; }	
				else 	{ result = !valuelist[inputstr]; }

				input_pos = comma;
			}
		}
		else if ((fourletter=="nand") || (fourletter=="NAND") ){
			result =1; 
			//bool start= 1;
			while ((input_pos <= lastcomma)&& (done==0)){
				size_t comma =netlist.find_first_of(",",input_pos+1);
				size_t inlength = comma - input_pos -1;
				if (input_pos == lastcomma)	{
					done = 1;
					inlength = input_end-input_pos-1;
				}
				string inputstr = netlist.substr(input_pos+1,inlength);
				if (result==1) { result = valuelist[inputstr]; }	
				else 	{}

				input_pos = comma;
			}
			result = !result;
		}
		else if ((threeletter=="not") || (threeletter=="NOT") ){
			string inputstr = netlist.substr(input_pos+1,input_end-1-input_pos);
			result = !valuelist[inputstr]; 
		}
		else if ((threeletter=="buf") || (threeletter=="BUF") ){
			string inputstr = netlist.substr(input_pos+1,input_end-1-input_pos);
			result = valuelist[inputstr]; 
		}

		valuelist[wire] = result;//store the calculated value to the output_sig 
		//cout<<"WIRE :"<<wire<<'='<<result<<endl;
	}

}

iFile.close();
for (unsigned i=0; i<outnames.size();i++){
	//cout<<"OUTPUTS: "<<outnames.size()<<endl;
	cout<<outnames[i]<<' '<<valuelist[outnames[i]]<<endl;
}

return 0;
}

string pureWord(string &word){
//get the word without character
	unsigned int i=0;
	unsigned int j=0;
	string pureword;

	while (i<word.length()){
		if ( (word[i]!=' ') && (word[i]!='\t') ){
			pureword.push_back(word[i]);
			//cout<<"pureword "<<pureword[j]<<" word "<<word[i]<<" "<<pureword<<' '<<(word[i]==' ')<<endl;
			j++;	
		}
		//else	cout<<"THE COLUMN NAME INCLUDES A SPACE:"<<word<<' '<<(word[i]==' ');
		i++;
	}
	return pureword;
}


