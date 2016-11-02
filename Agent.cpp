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
		Argument(){};
};
int timer = 0;
class State{
    public:
    	int N;//number of floors
    	int K;//number of elevators
    	vector<int> pos;// = [0]*K  initial positions of all elevators
    	vector<int> BU;// = [0]*N  # button up on each floor   (always 0 for top floor)
    	vector<int> BD;// button down on each floor (always 0 for first floor)
    	vector<vector<int> >BF;//[K][N]  = [[0]*N for i in range(K)] -- floor buttons pressed inside elevator, for each elevator
    	vector<int> dirn;//1 >or 0 or -1 for up or rest or down---dirn for all elevators
		vector<bool> stopped;//is lift open or stopped at some floor
		vector<vector<float> > arrivalProbUpDown;
    	State(int N, int K):pos(K),BU(N), BD(N),BF(K),dirn(K),stopped(K),arrivalProbUpDown(2){
			this->N=N;
    	    this->K=K;
    	    for(int l=0;l<K;l++){
                for(int i=0;i<N;i++){
                    BF[l].push_back(0);
                }
				dirn[l]=1;
				stopped[l]=1;
			}
			for(int l=0;l<2;l++){
				for(int k=0;k<N;k++){
					arrivalProbUpDown[l].push_back(0);
				}
			}
    	};
};
int sumSquaredWeightedWaitingTime(State state, string actionSequence){
	//for every lift's action--weighted waiting time(due to time of arrival) of all passengers
	//in direction of the lift's action
	istringstream iss(actionSequence);
	vector<string> actions;
	for(string s; iss >> s;)actions.push_back(s);
	int length = actions.size();
	//AOU,AOD,AU,AD,AS
	int sumSquaredTime=0;
	for(int i=0;i<length;i++){
		if(actions[i][1]=='U'){
			//time of all people in up direction(or everywhere) waiting for this lift after decreasing 1 distance
			//or time of all people in all direction after it moves 1 distance
			int liftPos=state.pos[i]+1;
			for(int j=0;j<state.N;j++){
				if(j<liftPos){
					if(state.BU[j]!=0){
						sumSquaredTime+=(j+liftPos)*(j+liftPos)*(timer-state.BU[j])*state.arrivalProbUpDown[1][j];
					}
					if(state.BD[j]!=0){
						sumSquaredTime+=(j+liftPos)*(j+liftPos)*(timer-state.BD[j])*state.arrivalProbUpDown[0][j];
					}
				}else{
					if(state.BU[j]!=0){
						sumSquaredTime+=(j-liftPos)*(j-liftPos)*(timer-state.BU[j])*state.arrivalProbUpDown[1][j];
					}
					if(state.BD[j]!=0){
						sumSquaredTime+=(j-liftPos)*(j-liftPos)*(timer-state.BD[j])*state.arrivalProbUpDown[0][j];
					}
				}

			}
		}else if(actions[i][1]=='D'){
			//time of all people in down direction waiting for this lift
			int liftPos=state.pos[i]-1;
			for(int j=0;j<state.N;j++){
				if(j>liftPos){
					if(state.BU[j]!=0){
						sumSquaredTime+=(j+liftPos)*(j+liftPos)*(timer-state.BU[j])*state.arrivalProbUpDown[1][j];
					}
					if(state.BD[j]!=0){
						sumSquaredTime+=(j+liftPos)*(j+liftPos)*(timer-state.BD[j])*state.arrivalProbUpDown[0][j];
					}
				}else{
					if(state.BU[j]!=0){
						sumSquaredTime+=(j-liftPos)*(j-liftPos)*(timer-state.BU[j])*state.arrivalProbUpDown[1][j];
					}
					if(state.BD[j]!=0){
						sumSquaredTime+=(j-liftPos)*(j-liftPos)*(timer-state.BD[j])*state.arrivalProbUpDown[0][j];
					}
				}
			}
		}else if(actions[i][1]=='O'){
			if(actions[i][1]=='U'){
				//time of all people in up direction waiting for this lift
				int liftPos=state.pos[i];
				for(int j=0;j<state.N;j++){
					if(j<liftPos){
						if(state.BU[j]!=0){
							sumSquaredTime+=(j+liftPos)*(j+liftPos)*(timer-state.BU[j])*state.arrivalProbUpDown[1][j];
						}
						if(state.BD[j]!=0){
							sumSquaredTime+=(j+liftPos)*(j+liftPos)*(timer-state.BD[j])*state.arrivalProbUpDown[0][j];
						}
					}else{
						if(state.BU[j]!=0){
							sumSquaredTime+=(j-liftPos)*(j-liftPos)*(timer-state.BU[j])*state.arrivalProbUpDown[1][j];
						}
						if(state.BD[j]!=0){
							sumSquaredTime+=(j-liftPos)*(j-liftPos)*(timer-state.BD[j])*state.arrivalProbUpDown[0][j];
						}
					}
				}
			}else if(actions[i][1]=='D'){
				//time of all people in down direction waiting for this lift
				int liftPos=state.pos[i];
				for(int j=0;j<state.N;j++){
					if(j>liftPos){
						if(state.BU[j]!=0){
							sumSquaredTime+=(j+liftPos)*(j+liftPos)*(timer-state.BU[j])*state.arrivalProbUpDown[1][j];
						}
						if(state.BD[j]!=0){
							sumSquaredTime+=(j+liftPos)*(j+liftPos)*(timer-state.BD[j])*state.arrivalProbUpDown[0][j];
						}
					}else{
						if(state.BU[j]!=0){
							sumSquaredTime+=(j-liftPos)*(j-liftPos)*(timer-state.BU[j])*state.arrivalProbUpDown[1][j];
						}
						if(state.BD[j]!=0){
							sumSquaredTime+=(j-liftPos)*(j-liftPos)*(timer-state.BD[j])*state.arrivalProbUpDown[0][j];
						}
					}
				}
			}
		}else if(actions[i][1]=='S'){
			//makes everyone else wait other than person on the position itself
			int liftPos=state.pos[i];
			for(int j=0;j<state.N;j++){
				if(state.BU[j]!=0){
					sumSquaredTime+=(j-liftPos)*(j-liftPos)*(timer-state.BU[j])*state.arrivalProbUpDown[1][j];
				}
				if(state.BD[j]!=0){
					sumSquaredTime+=(j-liftPos)*(j-liftPos)*(timer-state.BD[j])*state.arrivalProbUpDown[0][j];
				}
			}
		}
	}

	return sumSquaredTime;
}
bool hasRequestsInsideLiftInDirection(State state, int lift, int dirn){
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
bool checkReqInHallInDirection(State state,int lift,int dirn){
	int liftPosition=state.pos[lift];
	if(dirn==1){
		for(int l=liftPosition+1; l<state.N;l++){
			if(state.BU[l]!=0){
				return true;
			}else if(state.BD[l]!=0){
				return true;
			}

		}
	}else{
		for(int l=liftPosition-1; l>=0;l--){
			if(state.BU[l]!=0){
				return true;
			}else if(state.BD[l]!=0){
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
	bool sentOneLiftUpAlready=false;
	bool sentOneLiftDownAlready=false;
	for(int i=0;i<state.K;i++){//FOR EACH LIFT
		bool shouldStop=false;
		bool shouldContinue=false;
		bool shouldMoveOpposite =false;
		bool openInDirnUp=false;
		bool openInDirnDown=false;

		if(state.dirn[i]==1){
			bool stopsUp =hasRequestsInsideLiftInDirection(state,i,1);//hasReqUpDirn
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
			bool reqInHallInUp = checkReqInHallInDirection(state,i,1);
			bool reqInHallInDown= checkReqInHallInDirection(state,i,-1);
			if(state.BD[liftPos]!=0){
				//open in opposite direction on this floor
				openInDirnDown=true;
			}
			if(state.BU[liftPos]!=0){
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
				state.BU[liftPos]=0;
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
				state.BD[liftPos]=0;
				state.dirn[i]=-1;
				state.stopped[i]=1;
			}else if(openInDirnUp){
				action+=makeAction("AOU",i);
				//update state of lift after taking action
				state.BU[liftPos]=0;
				state.dirn[i]=1;
				state.stopped[i]=1;
			}else if(reqInHallInDown){
				if(!sentOneLiftDownAlready){
					action+=makeAction("AD",i);
					state.dirn[i]=-1;
					state.pos[i]-=1;
					state.stopped[i]=0;
					sentOneLiftDownAlready=true;
				}else{
					action+=makeAction("AOD",i);
					state.dirn[i]=-1;
					//state.pos[i]-=1;
					state.stopped[i]=1;
				}

			}else if(reqInHallInUp){
				if(!sentOneLiftUpAlready){
					action+=makeAction("AU",i);//
					state.dirn[i]=1;
					state.pos[i]+=1;
					state.stopped[i]=0;
					sentOneLiftUpAlready=true;
				}else{
					action+=makeAction("AOU",i);//
					state.dirn[i]=1;
					//state.pos[i]+=1;
					state.stopped[i]=1;
				}

			}else if(shouldMoveOpposite && !reqInHallInUp){//req in up dirn by hall buttons
				//move down to pick passengers right?
				action+=makeAction("AD",i);
				state.dirn[i]=-1;
				state.pos[i]-=1;
				state.stopped[i]=0;
			}else{
				if(liftPos!=0){
					action+=makeAction("AOD",i);
					state.BD[liftPos]=0;
					state.dirn[i]=-1;
				}else if(liftPos!=state.N-1){
					action+=makeAction("AOU",i);
					state.BU[liftPos]=0;
					state.dirn[i]=1;
				}else{
					action+=makeAction("AS",i);
				}
				state.stopped[i]=1;
			}
		}else{//if(state.dirn[i]==-1)
			//bool stopsUp =hasRequestsInDirectionInsideLift(state,i,1);//hasReqUpDirn
			bool stopsDown =hasRequestsInsideLiftInDirection(state,i,-1);
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
			bool reqInHallInUp = checkReqInHallInDirection(state,i,1);
			bool reqInHallInDown= checkReqInHallInDirection(state,i,-1);
			if(state.BD[liftPos]!=0){
				//open in same direction on this floor
				openInDirnDown=true;
			}
			if(state.BU[liftPos]!=0){
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
			}else if(reqInHallInDown){
				if(!sentOneLiftDownAlready){
					action+=makeAction("AD",i);
					state.dirn[i]=-1;
					state.pos[i]-=1;
					state.stopped[i]=0;
					sentOneLiftDownAlready=true;
				}else{
					action+=makeAction("AOD",i);
					state.dirn[i]=-1;
					//state.pos[i]-=1;
					state.stopped[i]=1;
				}

			}else if(reqInHallInUp){
				if(!sentOneLiftUpAlready){
					action+=makeAction("AU",i);//
					state.dirn[i]=1;
					state.pos[i]+=1;
					state.stopped[i]=0;
					sentOneLiftUpAlready=true;
				}else{
					action+=makeAction("AOU",i);//
					state.dirn[i]=1;
					//state.pos[i]+=1;
					state.stopped[i]=1;
				}
			}else if(shouldMoveOpposite && !reqInHallInUp ){//req in up dirn by hall buttons
				//move down to pick passengers right?
				action+=makeAction("AD",i);
				state.dirn[i]=-1;
				state.pos[i]-=1;
				state.stopped[i]=0;
			}else{
				if(liftPos!=0){
					action+=makeAction("AOD",i);
					state.BD[liftPos]=0;
					state.dirn[i]=-1;
				}else if(liftPos!=state.N-1){
					action+=makeAction("AOU",i);
					state.BU[liftPos]=0;
					state.dirn[i]=1;
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
	state.arrivalProbUpDown[0][0]=0;
	state.arrivalProbUpDown[1][0]=args.p*args.q;//groung floor up button---always go in up direction
	state.arrivalProbUpDown[0][state.N-1]=args.p*(1-args.q)/(float)(args.N-1);
	state.arrivalProbUpDown[1][state.N-1]=0;
	for(int l=0;l<2;l++){//0 is down and 1 is up
		for(int j=1;j<state.N-1;j++){
			if(l==0){//down
				state.arrivalProbUpDown[l][j]=args.p*((1-args.q)/(float)(args.N-1))*(args.r+(j-1)*(1-args.r)/(float)(args.N-2));
			}else{//up
				state.arrivalProbUpDown[l][j]=args.p*((1-args.q)/(float)(args.N-1))*(args.N-j-1)*(1-args.r)/(float)(args.N-2);
			}
		}
	}

	string ready;
  	getline(cin,ready);
  	//cerr<<ready<<"ready"<<endl;
	// vector<string> repeat;
	// for(int i=0;i<args.N-1; i++){repeat.push_back("AU");repeat.push_back("AOU");}
	// repeat.push_back("AOD");
	// for(int i=0;i<args.N-1; i++){repeat.push_back("AD");repeat.push_back("AOD");}
	// repeat.push_back("AOU");

	while(1){
		timer++;
		string actionsOut=takeAction(state);
		cerr<<actionsOut<<" take this action"<<endl;
		cout<<actionsOut<<'\n'<<flush;
		string updates;
	  	getline(cin,updates);
		cerr<<updates<<"update state now"<<endl;
		istringstream iss(updates);
		vector<string> result;
		for(string s; iss >> s;)result.push_back(s);
		int length = result.size();
		for(int i=0;i<length;i++){
			if(result[i]!="0"){
				if(result[i][0]=='B'){
					if(result[i][1]=='D'){
						int floor = (int)result[i][3]- (int)'1';
						state.BD[floor]=timer;
					}else if(result[i][1]=='U'){
						int floor = (int)result[i][3]- (int)'1';
						state.BU[floor]=timer;
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
	//simulate mdp then print 0
	cout<<"0\n"<<flush;
	simpleAgent(args);
	return 0;
}
