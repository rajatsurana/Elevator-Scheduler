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
class State{
    public:
    	int N;							//number of floors
    	int K;							//number of elevators
    	vector<int> pos;				// = [0]*K  initial positions of all elevators
    	vector<int> BU;					// = [0]*N  # button up on each floor   (always 0 for top floor)
    	vector<int> BD;					// button down on each floor (always 0 for first floor)
    	vector<vector<int> >BF;			//[K][N]  = [[0]*N for i in range(K)] -- floor buttons pressed inside elevator, for each elevator
    	vector<int> dirn;				//1 or 0 or -1 for up or rest or down---dirn for all elevators
    	State(int N, int K):pos(K),BU(N), BD(N),BF(K),dirn(K){
			this->N=N;
    	    this->K=K;
    	    for(int l=0;l<K;l++)
                for(int i=0;i<N;i++){
                    BF[l].push_back(0);
                }
    	};
};
bool findStopsInDirection(State state, int lift, int dirn){
	int liftPosition=state.pos[lift];
	if(dirn==1){
		for(int i=liftPosition+1;i<state.N;i++){
			if(state.BF[lift][i]==1){
				return true;
			}
		}
	}else if(dirn==-1){
		for(int i=liftPosition;i>=0;i--){/////////////liftPos-1 ?????///
			if(state.BF[lift][i]==1){//////////////who updates BF when a transition happens----when we read input we are doing it
				return true;
			}
		}
	}

	return false;
}
string takeAction(State &state){
	string action="";
	for(int i=0;i<state.K;i++){
		if(state.dirn[i]==1){
			bool stopsUp =findStopsInDirection(state,i,1);
			if(stopsUp){
				if(action!=""){
					action+=" ";
				}
				action+="AU";
				string lift="";
				int ii=i+1;
				stringstream ss;
				ss<< ii;ss>>lift;
				action+=lift;
				//update state of lift after taking action
				state.pos[i]+=1;
				state.dirn[i]=1;
			}else{
				if(action!=""){
					action+=" ";
				}
				action+="AS";
				int ii=i+1;
				string lift;
				stringstream ss;
				ss<< ii;ss>>lift;
				action+=lift;
				state.dirn[i]=0;
			}
		}else if(state.dirn[i]==-1){
			bool stopsDown = findStopsInDirection(state,i,-1);
			if(stopsDown){
				if(action!=""){
					action+=" ";
				}
				action+="AD";
				string lift="";
				int ii=i+1;
				stringstream ss;
				ss<< ii;ss>>lift;
				action+=lift;
				//update state of lift after taking action
				state.pos[i]-=1;
				state.dirn[i]=-1;
			}else{
				if(action!=""){
					action+=" ";
				}
				action+="AS";
				int ii=i+1;
				string lift;
				stringstream ss;
				ss<< ii;ss>>lift;
				action+=lift;
				state.dirn[i]=0;
			}
		}else{
			//we have to take decision when there are no buttons pressed in direction of lift inside the lift
			if(action!=""){
				action+=" ";
			}
			action+="AS";
			int ii=i+1;
			string lift;
			stringstream ss;
			ss<< ii;ss>>lift;
			action+=lift;
			state.dirn[i]=0;
		}
	}
	return action;
}
void simpleAgent(Argument args){
	cerr<<"args"<<args.N<<args.K<<args.p<<args.q<<args.r<<args.t<<endl;
	State state(args.N,args.K);
	cerr<<"state args "<<state.N<<" "<<state.K<<endl;
	string ready;
  	getline(cin,ready);
  	cerr<<ready<<"ready"<<endl;
	vector<string> repeat;
	for(int i=0;i<args.N-1; i++){repeat.push_back("AU");repeat.push_back("AOU");}
	repeat.push_back("AOD");
	for(int i=0;i<args.N-1; i++){repeat.push_back("AD");repeat.push_back("AOD");}
	repeat.push_back("AOU");
	int i = 0;
	while(1){
		// vector<string> actions;
		// string suff="";
		// stringstream sss;
		// for(int k=0;k<args.K;k++){
		// 	int kk=k+1;
		// 	sss<< kk;sss>>suff;
		// 	actions.push_back("AOU" + suff);
		// 	//cerr<<(char)(k+1)<<" k+1 "<<k+1<<endl;
		// }
        // //actions = ['AOU' + str(k+1) for k in range(args.K)]
		// string actionsOut="";
		// for (int l=0;l< args.K;l++){
        // 	if (i>(args.N/args.K+1)*l*2){
		// 	string end="";
		// 	int ll=l+1;
		// 	stringstream ss;
		// 	ss<< ll;ss>>end;
	    //     actions[l] = repeat[(i - (args.N/args.K+1)*l*2 - 1) % repeat.size()] +end;//+ string(1,1);
        //     //cerr<<(char)(l+1)<<" l+1 "<<l+1<<endl;
		// 	}
		// }
		// actionsOut= actions[0];
		// for(int ll=1;ll<actions.size();ll++){
		// 	actionsOut+= " "+ actions[ll];
		// }
	    //i+=1;
		//cout<<actionsOut<<'\n'<<flush;
		string actionsOut=takeAction(state);
		cerr<<actionsOut<<" this action taken"<<endl;
		cout<<actionsOut<<'\n'<<flush;
		string updates;
	  	getline(cin,updates);
		cerr<<updates<<"update"<<endl;
		istringstream iss(updates);
		vector<string> result;
		for(string s; iss >> s;)result.push_back(s);
		cerr<<result[0]<<"res"<<endl;
		int length = result.size();
		if (length ==1){
			if(result[0]=="0"){

			}else{
				if(result[0][0]=='B'){
					if(result[0][1]=='D'){
						int floor = (int)result[0][3]- (int)'1';
						state.BD[floor]=1;
					}else if(result[0][1]=='U'){
						int floor = (int)result[0][3]- (int)'1';
						state.BU[floor]=1;
					}else if(result[0][1]=='_'){
						if(args.K<10 && args.N<10){
							int button= (int)result[0][2]- (int)'1';
							int lift = (int)result[0][4]- (int)'1';
							state.BF[button][lift]=1;
						}
					}
				}
			}
		}else if(length==2){
			if(result[0]=="0"){
				if(result[1][0]=='B'){
					if(result[1][1]=='D'){
						int floor = (int)result[1][3]- (int)'1';
						state.BD[floor]=1;
					}else if(result[1][1]=='U'){
						int floor = (int)result[1][3]- (int)'1';
						state.BU[floor]=1;
					}else if(result[1][1]=='_'){
						if(args.K<10 && args.N<10){
							int button= (int)result[1][2]- (int)'1';
							int lift = (int)result[1][4]- (int)'1';
							state.BF[button][lift]=1;
						}
					}
				}
			}else{
				if(result[0][0]=='B'){
					if(result[0][1]=='D'){
						int floor = (int)result[0][3]- (int)'1';
						state.BD[floor]=1;
					}else if(result[0][1]=='U'){
						int floor = (int)result[0][3]- (int)'1';
						state.BU[floor]=1;
					}else if(result[0][1]=='_'){
						if(args.K<10 && args.N<10){
							int button= (int)result[0][2]- (int)'1';
							int lift = (int)result[0][4]- (int)'1';
							state.BF[button][lift]=1;
						}
					}
				}
			}
		}else if(length==3){//length==3
			if(result[0]=="0"){
				if(result[1][0]=='B'){
					if(result[1][1]=='D'){
						int floor = (int)result[1][3]- (int)'1';
						state.BD[floor]=1;
					}else if(result[1][1]=='U'){
							int floor = (int)result[1][3]- (int)'1';
							state.BU[floor]=1;
					}else if(result[1][1]=='_'){
						if(args.K<10 && args.N<10){
							int button= (int)result[1][2]- (int)'1';
							int lift = (int)result[1][4]- (int)'1';
							state.BF[button][lift]=1;
						}
					}
				}
				if(result[2][0]=='B'){
					if(result[2][1]=='D'){
						int floor = (int)result[2][3]- (int)'1';
						state.BU[floor]=1;
					}else if(result[2][1]=='U'){
						int floor = (int)result[2][3]- (int)'1';
						state.BU[floor]=1;
					}else if(result[2][1]=='_'){
						if(args.K<10 && args.N<10){
							int button= (int)result[2][2]- (int)'1';
							int lift = (int)result[2][4]- (int)'1';
							state.BF[button][lift]=1;
						}
					}
				}
			}else{
				if(result[0][0]=='B'){
					if(result[0][1]=='D'){
						int floor = (int)result[0][3]- (int)'1';
						state.BD[floor]=1;
					}else if(result[0][1]=='U'){
						int floor = (int)result[0][3]- (int)'1';
						state.BU[floor]=1;
					}else if(result[0][1]=='_'){
						if(args.K<10 && args.N<10){
							int button= (int)result[0][2]- (int)'1';
							int lift = (int)result[0][4]- (int)'1';
							state.BF[button][lift]=1;
						}
					}
				}
				if(result[1][0]=='B'){
					if(result[1][1]=='D'){
						int floor = (int)result[1][3]- (int)'1';
						state.BD[floor]=1;
					}else if(result[1][1]=='U'){
							int floor = (int)result[1][3]- (int)'1';
							state.BU[floor]=1;
					}else if(result[1][1]=='_'){
						if(args.K<10 && args.N<10){
							int button= (int)result[1][2]- (int)'1';
							int lift = (int)result[1][4]- (int)'1';
							state.BF[button][lift]=1;
						}
					}
				}
			}
		}
	}
		//update state
		//take action according to policy for states
}
int main(int argc, char *argv[]){
	Argument args;//5,2,0.2,0.7,0.9,1
	sscanf(argv[1],"%d",&args.N);
	sscanf(argv[2],"%d",&args.K);
	sscanf(argv[3],"%f",&args.p);
	sscanf(argv[4],"%f",&args.q);
	sscanf(argv[5],"%f",&args.r);
	sscanf(argv[6],"%f",&args.t);
	//cerr<<"args"<<args.N<<args.K<<args.p<<args.q<<args.r<<args.t<<endl;
	cout<<"0\n"<<flush;
	simpleAgent(args);
	return 0;
}
