#include <bits/stdc++.h>
#include <string>
using namespace std;

class Argument{
	public:
		int N;
		int K;
		float p;
		float q;
		float r;
		float t;
		Argument();
};
Argument::Argument(){
}
void simpleAgent(Argument args){
    //ready = sys.stdin.readline().strip()
 	char ready[100];
    cin.getline(ready,100);
    //int data[3];
    //int i = 0;
    cerr<<ready<<"ready"<<endl;

    //stringstream ssin(line);
    //while (ssin.good() && i < 3){
    //    ssin >> data[i];
     //   cerr<<data[i]<<" data"<<endl;
     //   ++i;
    //}
	vector<string> repeat;
	for(int i=0;i<args.N-1; i++){
		repeat.push_back("AU");
		repeat.push_back("AOU");
	}
		repeat.push_back("AOD");
	for(int i=0;i<args.N-1; i++){
		repeat.push_back("AD");
		repeat.push_back("AOD");
	}
		repeat.push_back("AOU");
    //repeat = ['AU','AOU']*(args.N-1)
    //repeat[-1] = 'AOD'
   // repeat += ['AD','AOD']*(args.N-1)
   // repeat[-1] = 'AOU'

    int i = 0;

    while(1){
		vector<string> actions;
		string suff="";
				
				stringstream sss;
				
		for(int k=0;k<args.K;k++){
			int kk=k+1;
			sss<< kk;
			sss>>suff;
			actions.push_back("AOU" + suff);
			//cerr<<(char)(k+1)<<" k+1 "<<k+1<<endl;
		}
        //actions = ['AOU' + str(k+1) for k in range(args.K)]
		string actionsOut="";
        for (int l=0;l< args.K;l++){
            if (i>(args.N/args.K+1)*l*2){
				string end="";
				int ll=l+1;
				stringstream ss;
				ss<< ll;
				ss>>end;
                actions[l] = repeat[(i - (args.N/args.K+1)*l*2 - 1) % repeat.size()] +end;//+ string(1,1);
                //cerr<<(char)(l+1)<<" l+1 "<<l+1<<endl;
			}
		}
		actionsOut= actions[0];
		for(int ll=1;ll<actions.size();ll++){
			actionsOut+= " "+ actions[ll];
		}

        i+=1;
 
		cout<<actionsOut<<'\n'<<std::flush;

		char updates[100];
    	cin.getline(updates,100);
	}
}


int main(int argc, char *argv[]){
	Argument args;//= new Argument();
	//5,2,0.2,0.7,0.9,1	
	args.N=5;//argv[1];
	args.K=2;//argv[2];
	args.p=0.2;//argv[3];
	args.q=0.7;//argv[4];
	args.r=0.9;//argv[5];
	args.t=1;//argv[6];
	cout<<"0\n"<<std::flush;
	simpleAgent(args);
	return 0;
}
