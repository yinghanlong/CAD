#include <vector>
#include <stack>
#include <deque>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <cassert>
#include <ctime>
#include <climits>
#include <list>
#include <map>
#include <set>
#include <valarray>
#include <algorithm>

using namespace std;

#include "project3.h"
//pseudo code
/*
cubeList Complement( cubeList 𝐹 ) {
	// check if 𝐹 is simple enough to complement it directly and quit
	if ( 𝐹 is simple and we can complement it directly )
		return( directly computed complement of 𝐹 )
	else {
		// do recursion
		let 𝑥 = most binate variable for splitting
		cubeList 𝑃 = Complement( positiveCofactor( 𝐹, 𝑥 ) )
		cubeList 𝑁 = Complement( negativeCofactor( 𝐹, 𝑥 ) )
		𝑃 = AND( 𝑥, 𝑃 )
		 𝑁 = AND( 𝑥̅, 𝑁 )
		return( OR( 𝑃, 𝑁 ) )
	} // end recursion
} // end function
*/
cubelistT Complement(cubelistT& F, int num);
int terminate(cubelistT& F);
cubelistT positiveCofactor(cubelistT& F, int x);
cubelistT negativeCofactor(cubelistT& F, int x);
cubelistT AND(int x, bool pos, cubelistT & F);
cubelistT OR(cubelistT& P, cubelistT & N);

int main(int argc, char *argv[]){

	int num;
	cubelistT	cubelist;
	vector<int>	onecube;
	string			temp;
	int cubenum=0;

	string cubefile=argv[1];
	
	ifstream iFile;

	iFile.open(cubefile);
/////Get inputs/////////
	getline(iFile,temp);	num = std::stoi(temp); //cout<<"Variable NUM="<<num<<endl;

	while (getline(iFile,temp)){
		cubelist.cube.push_back(onecube);

		for (int j=0; j<num; j++){
			switch (temp[j]){
				case '0': cubelist.cube[cubenum].push_back(0);	break;
				case '1': cubelist.cube[cubenum].push_back(1);	break;
				case '-': cubelist.cube[cubenum].push_back(2);	break;

			}
			//cout<<cubelist.cube[cubenum][j]<<' ';//debug
		}
		cubenum++;
		//cout<<endl;//debug
	}
	iFile.close();

///////Get Complement///////////
	cubelistT comp = Complement(cubelist, num);

///////OUTPUT/////////
	cout<<num<<endl;
	for (unsigned i=0; i<comp.cube.size(); i++){
		for (unsigned j=0; j<comp.cube[i].size();j++){
			if (comp.cube[i][j]==2)	cout<<'-';
			else					cout<<comp.cube[i][j];
		}
		cout<<endl;
	}

	return 0;
}


cubelistT Complement(cubelistT& F, int num) {
	cubelistT comp;
	vector<int>	onecube;
	for (int i=0; i<num; i++){
				onecube.push_back(2);// don't cares
	}

	// check if 𝐹 is simple enough to complement it directly and quit
	if (terminate(F)!=0){//𝐹 is simple and we can complement it directly )
		if (terminate(F)==1){// case 1, empty cube list
			comp.cube.push_back(onecube);//all don't cares
			return comp;
		}
		else if (terminate(F)==2){//case 3, Cube list contains alldoncares cube
			return comp; //return an empty cube
		}
		else if (terminate(F)==3){//case 3, Cube list contains just one cube
			//using DeMorgan Laws
			for (unsigned i=0; i<F.cube.size(); i++){
				for (unsigned j=0; j<F.cube[i].size();j++){
					if (F.cube[i][j]!=2){
						vector<int> newcube = onecube;
						if (F.cube[i][j]) newcube[j] = 0;//complement
						else			  newcube[j] = 1;
						comp.cube.push_back(newcube);
					}
				}
			}
			return comp;
		}
		//return( directly computed complement of 𝐹 )
	}
	else {
	/////////////////////////////////////////////////
	///////////////Select variable///////////////////
		//vector<bool>	binate;
		vector<variableT>	variable;	
		int				minTC;
		int				minIDX=0;
		bool			allunate=1;
		int				pickvar=0;
//INITIALIZE!
		variableT		var;
		var.TC=0;		var.binate=0;	var.times=0;

		for (int j=0; j<num; j++){ //for each variable
			variable.push_back(var);//initialize
			bool	first=1;//first non don care 
			int  	T=0;	int	C=0;
			bool	positive=0;//record the unate direction
			for (unsigned i=0; i<F.cube.size(); i++){
				if (F.cube[i][j]!=2){
					if (first){
						positive = F.cube[i][j];
						first = 0;
					}
					else{
						if (positive!=F.cube[i][j]){
							variable[j].binate = 1;
							allunate =0;
						}
					}
						if (F.cube[i][j]==1) T++;
						else				 C++;
					variable[j].times++; // count the times that the variable appears in the cubelist
				}
			}
			if (T-C>0)	variable[j].TC= T - C;
			else		variable[j].TC= C - T;
			//cout<<"Variable "<<j<<" appeared "<<variable[j].times<<" times, #T-C="<<variable[j].TC<<endl;
		}


		if (allunate==0){	//there are binate variables
			//pick the binate variable in the most cubes
			int most=0;
			int mostIDX =0;
			for (int j=0; j<num; j++){
				if ((variable[j].times>most) && (variable[j].binate)){
					most = variable[j].times;
					mostIDX = j;	
				}
			}
			//find if there are ties	
			vector<int>	ties;
			for (int j=mostIDX; j<num; j++){
				if (variable[j].times==most)
					ties.push_back(j);
			}
			//break ties with the smallest |𝑇−𝐶|, and then with the smallest variable index;
			bool firstbi=1;
			for (unsigned j=0; j<ties.size(); j++){
				if ((variable[ties[j]].binate==1)&& ((firstbi==1)||(minTC>variable[ties[j]].TC)) ) {
					firstbi = 0;//init
					minTC = variable[ties[j]].TC;
					minIDX = ties[j];
				}
			}
			pickvar = minIDX;
			//cout<<"binate variable with smallest T-C is"<<minIDX<<endl;
		} 
		else{// there are no binate variables
			//pick the unate variable in the most cubes
			int mostu=0;
			int mostuIDX =0;
			for (int j=0; j<num; j++){
				if (variable[j].times>mostu){
					mostu = variable[j].times;
					mostuIDX = j;	
				}
			}
			pickvar = mostuIDX;//	cout<<"unate variable in the most cubes is"<<pickvar<<endl;
		}
/*
		if (there are binate variables) {
		pick the binate variable in the most cubes, and if necessary,
		break ties with the smallest |𝑇−𝐶|, and then with the smallest variable index;
		}
		else { // there are no binate variables
		pick the unate variable in the most cubes, and if necessary,
		break ties with the smallest variable index;
		}
*/

		// do recursion
		cubelistT  Fpco= positiveCofactor(F, pickvar);
		cubelistT  Fnco= negativeCofactor(F, pickvar);

		cubelistT P= Complement( Fpco, num );
		cubelistT N= Complement( Fnco, num );
		P= AND(pickvar,1,P);
		N= AND(pickvar,0,N);
		return	(OR(P,N));
	} // end recursion

	return comp;
} // end function

cubelistT AND(int x, bool pos, cubelistT & F){
	cubelistT newcube = F;
	for (unsigned i=0; i<F.cube.size(); i++){
		if (pos)	newcube.cube[i][x] = 1;
		else		newcube.cube[i][x] = 0;
	}
	return newcube;
}

cubelistT OR(cubelistT& P, cubelistT & N){
	cubelistT newcube = P;
	for (unsigned i=0; i<N.cube.size(); i++){
		newcube.cube.push_back(N.cube[i]);
	}
	return newcube;
}

cubelistT positiveCofactor(cubelistT & F, int x){
	cubelistT	newcube;
	vector<int>	onecube;
	for (unsigned i=0; i<F.cube.size(); i++){
		onecube = F.cube[i];
		if (F.cube[i][x]==1){
			onecube[x] = 2; //x becomes don't care
			newcube.cube.push_back(onecube);
		}
		else if (F.cube[i][x]==0){ // this cube is eliminated
		}
		else if (F.cube[i][x]==2){ // this cube is unchanged
			newcube.cube.push_back(onecube);
		}
	}
	return newcube;
}

cubelistT negativeCofactor(cubelistT& F, int x){
	cubelistT	newcube;
	vector<int>	onecube;
	for (unsigned i=0; i<F.cube.size(); i++){
		onecube = F.cube[i];
		if (F.cube[i][x]==0){
			onecube[x] = 2; //x becomes don't care
			newcube.cube.push_back(onecube);
		}
		else if (F.cube[i][x]==0){ // this cube is eliminated
		}
		else if (F.cube[i][x]==2){ // this cube is unchanged
			newcube.cube.push_back(onecube);
		}
	}
	return newcube;
}

int terminate( cubelistT& F) {
	bool alldontcare;

	if (F.cube.empty()){
		return 1;	//case 1, empty cube list
	}
	else if (F.cube.size()==1){
		return 3;	//case 3, Cube list contains just one cube
	}
	else {
		for (unsigned i=0; i<F.cube.size(); i++){
			alldontcare = 1;
			for (unsigned j=0; j<F.cube[i].size(); j++){
				if (F.cube[i][j]!=2){	//if not a don't care slot
					alldontcare = 0;
				}
			}
			if (alldontcare==1)	return 2; //case 2, Cube list contains All-Don’t-Cares Cube
		}
	
		return 0; //default, not terminate condition
	}
}




