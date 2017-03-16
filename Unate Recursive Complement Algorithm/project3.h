
struct cubelistT{
	vector<vector<int>>	cube;
	//slot=0, complement; slot=1, true; slot=2, don't care
};

struct variableT{
	bool binate;
	int	 TC; //#True - #Comp
	int	 times; // # of cubes that the variable appears in
};

