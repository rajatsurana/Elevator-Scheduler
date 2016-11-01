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
    	int N;//number of floors
    	int K;//number of elevators
    	vector<int> pos;// = [0]*K  initial positions of all elevators
    	vector<int> BU;// = [0]*N  # button up on each floor   (always 0 for top floor)
    	vector<int> BD;// button down on each floor (always 0 for first floor)
    	vector<vector<int> >BF;//[K][N]  = [[0]*N for i in range(K)] -- floor buttons pressed inside elevator, for each elevator
    	vector<int> dirn;//1 or 0 or -1 for up or rest or down---dirn for all elevators
		vector<bool> stopped;//is lift open or stopped at some floor
    	State(int N, int K):pos(K),BU(N), BD(N),BF(K),dirn(K),stopped(K){
			this->N=N;
    	    this->K=K;
    	    for(int l=0;l<K;l++){
                for(int i=0;i<N;i++){
                    BF[l].push_back(0);
                }
				dirn[l]=1;
				stopped[l]=1;
			}
    	};
};
bool hasRequestsInDirectionInsideLift(State state, int lift, int dirn){
	int liftPosition=state.pos[lift];
	if(dirn==1){
		for(int i=liftPosition+1;i<state.N;i++){
			if(state.BF[lift][i]==1){
				return true;
			}
		}
	}else if(dirn==-1){
		for(int i=liftPosition-1;i>=0;i--){/////////////liftPos-1 ?????///
			if(state.BF[lift][i]==1){//////////////who updates BF when a transition happens----when we read input we are doing it
				return true;
			}
		}
	}

	return false;
}
bool checkReqInHall(State state,int lift,int dirn){
	int liftPosition=state.pos[lift];
	if(dirn==1){
		for(int l=liftPosition+1; l<state.N;l++){
			if(state.BU[l]==1){
				return true;
			}else if(state.BD[l]==1){
				return true;
			}

		}
	}else{
		for(int l=liftPosition-1; l>=0;l--){
			if(state.BU[l]==1){
				return true;
			}else if(state.BD[l]==1){
				return true;
			}
		}
	}
	return false;
}
string makeAction(string action, int pos){
	string res = action;
	string lift="";
	int ii=pos+1;
	stringstream ss;
	ss<< ii;ss>>lift;
	res+=lift;
	return res;
}
string takeAction(State &state){
	string action="";
	for(int i=0;i<state.K;i++){//FOR EACH LIFT
		bool shouldStop=false;
		bool shouldContinue=false;
		bool shouldMoveOpposite =false;
		bool openInDirnUp=false;
		bool openInDirnDown=false;
		if(state.dirn[i]==1){
			bool stopsUp =hasRequestsInDirectionInsideLift(state,i,1);//hasReqUpDirn
			//bool stopsDown =hasRequestsInDirectionInsideLift(state,i,-1);
			int liftPos=state.pos[i];
			if(state.BF[i][liftPos]==1){//button pressed inside lift for this floor than I have to stop at this floor
				shouldStop=true;
			}else if(stopsUp){
				if(state.pos[i]!=state.N-1){//this condition will never rise
					shouldContinue=true;
				}
			}
			if(!stopsUp){
				if(state.pos[i]!=0){
					shouldMoveOpposite=true;
				}
			}
			bool reqInHallInUp = checkReqInHall(state,i,1);
			bool reqInHallInDown= checkReqInHall(state,i,-1);
			if(state.BD[liftPos]==1){
				//open in opposite direction on this floor
				openInDirnDown=true;
			}
			if(state.BU[liftPos]==1){
				openInDirnUp=true;
				//open in same direction
			}
			//isSomeLiftAlreadyStopped
			for(int l=0;l<state.K;l++){
				if(i!=l && state.pos[l]==state.pos[i] && state.stopped[l]==1 && state.dirn[l]==1){
					shouldStop=false;
					break;
				}
			}
			if(action!=""){
				action+=" ";
			}
			if(shouldStop){
				action+=makeAction("AOU",i);
				state.dirn[i]=1;
				state.stopped[i]=1;
			}else if(shouldContinue){
				action+=makeAction("AU",i);
				//update state of lift after taking action
				state.dirn[i]=1;
				state.pos[i]+=1;
				state.stopped[i]=0;
			}else if(openInDirnDown){

				action+=makeAction("AOD",i);
				state.dirn[i]=-1;
				state.stopped[i]=1;
			}else if(openInDirnUp){
				action+=makeAction("AOU",i);
				//update state of lift after taking action
				state.dirn[i]=1;
				state.stopped[i]=1;
			}else if(reqInHallInUp){
				action+=makeAction("AU",i);//
				state.dirn[i]=1;
				state.pos[i]+=1;
				state.stopped[i]=0;
			}else if((shouldMoveOpposite && !reqInHallInUp) || reqInHallInDown){//req in up dirn by hall buttons
				//move down to pick passengers right?
				action+=makeAction("AD",i);
				state.dirn[i]=-1;
				state.pos[i]-=1;
				state.stopped[i]=0;
			}else{
				if(liftPos!=0){
					action+=makeAction("AOD",i);
				}else if(liftPos!=state.N-1){
					action+=makeAction("AOU",i);
				}else{
					action+=makeAction("AS",i);
				}

				state.stopped[i]=1;
			}
		}else{//if(state.dirn[i]==-1)
			//bool stopsUp =hasRequestsInDirectionInsideLift(state,i,1);//hasReqUpDirn
			bool stopsDown =hasRequestsInDirectionInsideLift(state,i,-1);
			int liftPos=state.pos[i];
			if(state.BF[i][liftPos]==1){//button pressed inside lift for this floor than I have to stop at this floor
				shouldStop=true;
			}else if(stopsDown){
				if(state.pos[i]!=0){//this condition will never rise
					shouldContinue=true;
				}
			}
			if(!stopsDown){
				if(state.pos[i]!=state.N-1){
					shouldMoveOpposite=true;
				}
			}
			bool reqInHallInUp = checkReqInHall(state,i,1);
			bool reqInHallInDown= checkReqInHall(state,i,-1);
			if(state.BD[liftPos]==1){
				//open in same direction on this floor
				openInDirnDown=true;
			}
			if(state.BU[liftPos]==1){
				openInDirnUp=true;
				//open in opposite direction
			}
			//isSomeLiftAlreadyStopped
			for(int l=0;l<state.K;l++){
				if(i!=l && state.pos[l]==state.pos[i] && state.stopped[l]==1 && state.dirn[l]==1){
					shouldStop=false;
					break;
				}
			}
			if(action!=""){
				action+=" ";
			}
			if(shouldStop){
				action+=makeAction("AOD",i);
				state.dirn[i]=-1;
				state.stopped[i]=1;
			}else if(shouldContinue){
				action+=makeAction("AD",i);
				//update state of lift after taking action
				state.dirn[i]=-1;
				state.pos[i]-=1;
				state.stopped[i]=0;
			}else if(openInDirnDown){
				action+=makeAction("AOD",i);
				state.dirn[i]=-1;
				state.stopped[i]=1;
			}else if(openInDirnUp){
				action+=makeAction("AOU",i);
				//update state of lift after taking action
				state.dirn[i]=1;
				state.stopped[i]=1;
			}else if(reqInHallInUp){
				action+=makeAction("AU",i);//
				state.dirn[i]=1;
				state.pos[i]+=1;
				state.stopped[i]=0;
			}else if((shouldMoveOpposite && !reqInHallInUp) || reqInHallInDown){//req in up dirn by hall buttons
				//move down to pick passengers right?
				action+=makeAction("AD",i);
				state.dirn[i]=-1;
				state.pos[i]-=1;
				state.stopped[i]=0;
			}else{
				if(liftPos!=0){
					action+=makeAction("AOD",i);
				}else if(liftPos!=state.N-1){
					action+=makeAction("AOU",i);
				}else{
					action+=makeAction("AS",i);
				}
				state.stopped[i]=1;
			}
		}

	}
	return action;
}
void simpleAgent(Argument args){
	State state(args.N,args.K);
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
		for(int i=0;i<length;i++){
			if(result[i]!="0"){
				if(result[i][0]=='B'){
					if(result[i][1]=='D'){
						int floor = (int)result[i][3]- (int)'1';
						state.BD[floor]=1;
					}else if(result[i][1]=='U'){
						int floor = (int)result[i][3]- (int)'1';
						state.BU[floor]=1;
					}else if(result[i][1]=='_'){
						if(args.K<10 && args.N<10){
							int button= (int)result[i][2]- (int)'1';
							int lift = (int)result[i][4]- (int)'1';
							state.BF[lift][button]=1;
						}
					}
				}
			}
		}
	}
}
int main(int argc, char *argv[]){
	Argument args;//5,2,0.2,0.7,0.9,1
	sscanf(argv[1],"%d",&args.N);
	sscanf(argv[2],"%d",&args.K);
	sscanf(argv[3],"%f",&args.p);
	sscanf(argv[4],"%f",&args.q);
	sscanf(argv[5],"%f",&args.r);
	sscanf(argv[6],"%f",&args.t);
	cout<<"0\n"<<flush;
	simpleAgent(args);
	return 0;
}
