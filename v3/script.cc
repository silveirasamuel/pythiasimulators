//Samuel PP Silveira, Universidade Federal do ABC, 2018
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
int script(string argv, string argv2){
	ifstream codes;
	codes.open(argv);
	if(!codes.is_open()){
		cout<<"Failed to find file!";
		return 1;		
	}
	int n=0;
	int pCodes[100];
	while(!codes.eof()){
		codes>>pCodes[n];
		n++;
	}
	codes.close();
	string arg = "";
	for(int i=0;i<n-1;i++){
		arg = ".x treereader.cc(\""+argv2+"\","+ to_string(pCodes[i])+")";
		cout<<arg<<endl;	
		gROOT->ProcessLine(arg.c_str());
	}
	return 0;
}
