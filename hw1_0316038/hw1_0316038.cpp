#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <time.h>
using namespace std;
//failure function
int prekmp(string pattern, int f[])
{
	int m=pattern.length();
	int k; //f value
	f[0]=-1; //first value
	for(int i=1;i<m;i++){
		k=f[i-1];
		while(k>=0){
			if(pattern[k]==pattern[i-1])
				break;
			else
				k=f[k];
		}
		f[i]=k+1; //get failure[i] value
		
	}
}


//check whether target string contains pattern
int KMP(string pattern,string target){

	int m=pattern.length();
	int n=target.length();
	int f[m];
	prekmp(pattern,f);
	int i=0;
	int k=0;
	int count=0;  //find pattern numbers in a target

	while(i<n){
		if(k==-1){   //if fail,move and start again
			i++;
			k=0;
		}
		else if(target[i]==pattern[k]){   //if match,keep matching 
			i++;
			k++;
			if(k==m){
				count++;
				k=0; //pattern reset
				i=i-m+1; //i reset
			}

		}
		else
			k=f[k]; //failure
	}

	return count;

}

int main()
{	
	clock_t time = clock();
	int t=0;
	int i=0; //pattern numbers in a target
	int j=0; //cut times(position)
	char line[1000000];
	char first[1000];
	char input[100];
	char output[100];
	cin >>  input >> output ;

    bool check=0;
	fstream fin;
	fstream fout;
	fin.open(input,ios::in);
	fout.open(output,ios::out);
	
	
	int ms = double(time) / CLOCKS_PER_SEC * 1000;
	fout<< ms << "ms" <<endl;
	 
	while(fin.getline(line,sizeof(line),EOF)){
		//cout<<line<<endl;
		char *test=strtok(line," \n"); 
		if(check==0){
			strcpy(first,test);
			check=1;
			j++;
		}
		
		 //first cut
		while(test!=NULL){
			i=KMP(first,test); //return count
			while(i--){
				fout<<j;
			}
			test=strtok(NULL," \n");
			j++;
		}

	}
	
	time = clock() - time;
	return 0;
}
