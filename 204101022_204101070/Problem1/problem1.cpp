/*
usage :
	./routing -top topologyfile.txt -conn connectionfile.txt -rt routingtablefile.txt -ft forwardingtablefile.txt -path pathsfile.txt -flag hop/  
	dist -p 0/1
*/

#include<bits/stdc++.h>
using namespace std;

string topologyfile;		//stores topology file name
string connectionfiles;		//stores connection file name
string routingtablefile;	//stores routingtable file name
string forwardingtablefile;	//stores forwardingtable file name
string pathfiles;		//stores pathfile file name
string HopDist;			//tells whether we use hop or distance
bool flag;			
int p;				//tells whether approach is optimistic or pessimistic
int nodes = 0, edges = 0;	
int** costMatrix;		//store the cost of every edge
double** linkCap;		//store the bandwidth of every link
double** connections;		//store the connection info given by the user
int No_of_conn;			
int cost = 0;
int NoOfConnEst = 0;
#define INF 9999

//This Structure is use to store first ans second link-disjoined shortest path between every two nodes.
typedef struct RTinfo{
	pair<vector<int>, int> p1;//contains first shortest path;
	pair<vector<int>, int> p2;//contains second shortest path;
	
}RT;
RT **pr;//pr is the pointer to the Structure that stores Routng Info.

//This Structure is use to store Forwarding information for every node
typedef struct FTInfo{
	vector<vector<int>> forward;
}FT;
FT *fr;//fr is the pointer to the Structure that stores Forwarding Info.

//This Structure is use to store path info for all established connections
typedef struct pathInfo{
	int ConnId;
	int Source;
	int Destination;
	vector<int> Label_List;
	int Path_Cost;
}PI;
PI *pathIn;//pathIn is the pointer to the Structure that stores Path Info.

//This Structure is use to convert character array into a string.
string convertToString(char* a, int size) { 
	int i; 
	string s = ""; 
	for(i=0; i<size; i++) { 
		s = s + a[i]; 
	} 
	return s; 
} 

//This Structure is use to find the character array size given by the user
int argvSize(char* a){
	int i = 0;
	while(a[i] != '\0'){
		i++;	
	}
	return i;
}

//use to Initialize the arrays where we store our routing and forwarding information
void Init(){
	costMatrix = new int *[nodes];
	linkCap = new double *[nodes];
	pr = new RT*[nodes];
	for(int i=0;i<nodes;i++){
		pr[i] = new RT[nodes];
	}
	fr = new FT[nodes];
}

//use to handle Commandline arguments.----------------------------------------------
void CommandlineHandler(char** a, int n){
	for(int i=1; i<n; i=i+2){
		string s;
		s = convertToString(a[i], argvSize(a[i]));
		if(s.compare("-top") == 0){
			topologyfile = convertToString(a[i+1], argvSize(a[i+1]));	
		}else if(s.compare("-conn") == 0){
			connectionfiles	= convertToString(a[i+1], argvSize(a[i+1]));	
		}else if(s.compare("-rt") == 0){
			routingtablefile = convertToString(a[i+1], argvSize(a[i+1]));		
		}else if(s.compare("-ft") == 0){
			forwardingtablefile = convertToString(a[i+1], argvSize(a[i+1]));		
		}else if(s.compare("-path") == 0){
			pathfiles = convertToString(a[i+1], argvSize(a[i+1]));		
		}else if(s.compare("-flag") == 0){
			HopDist = convertToString(a[i+1], argvSize(a[i+1]));
			if(HopDist.compare("hop") == 0){
				flag = false;			
			}else if(HopDist.compare("dist") == 0){
				flag = true;			
			}else{
				cout<<"flag value is either hop or dist"<<endl;
				exit(0);			
			}		
		}else if(s.compare("-p") == 0){
			p = atoi(a[i+1]);
			if(p != 0 && p != 1){
				cout<<"p value is either 0 or 1"<<endl;
				exit(0);			
			}		
		}		
	}
}

//this is the pre processing function that is use to take input from the file given by the user----------------------------------------------
void preprocessing(){
	ifstream file;
	file.open(topologyfile);//reading Topology file
	if(file.is_open()){
		file >> nodes >> edges;
		Init();
		for(int i=0;i<nodes;i++){
			costMatrix[i] = new int[nodes]();		
		}
		for(int i=0;i<nodes;i++){
			linkCap[i] = new double[nodes]();		
		}
		int x;
		while(file >> x){
			int y, w, b;
			file >> y >> w >> b;
			if(flag == true){
				costMatrix[x][y] = w;
				costMatrix[y][x] = w;
			}else{
				costMatrix[x][y] = 1;
				costMatrix[y][x] = 1;
			}
			linkCap[x][y] = b;
			linkCap[y][x] = b;
		}
		for(int i=0;i<nodes;i++){
			for(int j = 0;j<nodes;j++){
				if(i == j){
					costMatrix[i][j] = 0;
				}else{
					if(costMatrix[i][j] == 0){
						costMatrix[i][j] = 9999;					
					}				
				}			
			}		
		}
		file.close();
	}else{
		cout<<"file not found"<<endl;
	}

	file.open(connectionfiles);//reading connection file
	if(file.is_open()){
		file >> No_of_conn;
		connections = new double *[No_of_conn];
		for(int i=0;i<No_of_conn;i++){
			connections[i] = new double[5]();		
		}
		double x;
		int i = 0;
		while(file >> x){
			double y, min, avg, max;
			file >> y >> min >> avg >> max;
			connections[i][0] = x;
			connections[i][1] = y;
			connections[i][2] = min;
			connections[i][3] = avg;
			connections[i][4] = max;
			i++;
		}
		file.close();
	}else{
		cout<<"file not found"<<endl;
	}
}
//----------------------------------------------
void DisplayInput(){
	cout<<"Input :"<<endl;
	cout<<"Topology : "<<topologyfile<<endl;
	cout<<"Connection File : "<<connectionfiles<<endl;
	cout<<"Hop/dist : "<<flag<<"   0-Hopcount and 1-Distance"<<endl;
	cout<<"Approach : "<<p<<"   0-Optimistic and 1-Pessimistic"<<endl<<endl;
	cout<<"Output: "<<endl;
}

/*============================ finding first and second shortest using Dijkstra's Algorithm ===========================*/
int src;
int dist[10000];
bool visited[10000] = {0};
int parent[10000];

void initailization(){
	for(int i=0;i<nodes;i++){
		parent[i] = i;
		dist[i] = INF; 
	}
	dist[src] = 0;
}


int getNearest(){
	int minvalue = INF;
	int minnode = 0;
	for(int i = 0;i<nodes;i++){
		if(!visited[i] && dist[i] < minvalue){
			minvalue = dist[i];
			minnode = i;	
		}
	} 
	return minnode;
}

//Dijkstra's Algo
void Dijkstras(){
	for(int i=0;i<nodes;i++){
		int nearest = getNearest();
		visited[nearest] = true;
		for(int j = 0;j<nodes;j++){
			if((costMatrix[nearest][j] != INF) && (dist[j] > (dist[nearest] + costMatrix[nearest][j]))){
				dist[j] = dist[nearest] + costMatrix[nearest][j];	
				parent[j] = nearest;	
			}
		}
	}
}

//storing first shortest path
void store(){
	for(int i=0;i<nodes;i++){
		if(i == src){
			continue;		
		}else{
			pr[src][i].p1.second = dist[i];
		}
		vector<int> temp;
		temp.push_back(i);
		int parnode = parent[i];
		while(parnode != src){
			temp.push_back(parnode);
			parnode = parent[parnode];			
		}
		temp.push_back(src);
		reverse(temp.begin(), temp.end());
		pr[src][i].p1.first = temp;
		temp.clear();
	}
}

//finding first shortest path
void findingfirstShortestpath(){
	for(int i=0;i<nodes;i++){
		src = i;
		initailization();
		Dijkstras(); //call for Dijkstra's Algo
		store();	
		for(int i=0;i<nodes;i++){
			visited[i] = false;		
		}	
	}
}

//storing second shortest path
void storeSecond(int dest){
	pr[src][dest].p2.second = dist[dest];
	vector<int> temp;
	temp.push_back(dest);
	int parnode = parent[dest];
	while(parnode != src){
		temp.push_back(parnode);
		parnode = parent[parnode];			
	}
	temp.push_back(src);
	reverse(temp.begin(), temp.end());
	pr[src][dest].p2.first = temp;
	temp.clear();
}

//finding second shortest path
void findingSecondShortestpath(){
	for(int i=0;i<nodes;i++){
		for(int j = 0;j<nodes;j++){
			if(i == j){
				continue;			
			}
			src = i;
			initailization();
			vector<int> old;
			for(int k = 0;k< pr[i][j].p1.first.size()-1;k++){ //Removing first Shortest path
				old.push_back(costMatrix[pr[i][j].p1.first[k]][pr[i][j].p1.first[k+1]]);
				costMatrix[pr[i][j].p1.first[k]][pr[i][j].p1.first[k+1]] = INF;			
			}
			Dijkstras(); //call for Dijkstra's Algo
			for(int k = 0;k< pr[i][j].p1.first.size()-1;k++){ //Adding first Shortest path
				costMatrix[pr[i][j].p1.first[k]][pr[i][j].p1.first[k+1]] = old[k];			
			}
			storeSecond(j);//storing the second shortest 
			for(int i=0;i<nodes;i++){
				visited[i] = false;		
			}
		}	
	}
}
/*============================ End of finding first and second shortest path ===========================*/


//use to Export Routing table into .txt file----------------------------------------------
void ExportRoutingTable(){
	ofstream of;
	of.open(routingtablefile);
	if(of.is_open()){
		for(int i = 0;i<nodes;i++){
			of<<"routing table of node "<<i<<endl;
			for(int j = 0;j<nodes;j++){
				if(i == j){
					continue;				
				}
				of<<j<<"\t";
				for(int l = 0;l<pr[i][j].p1.first.size();l++){
					if(l == pr[i][j].p1.first.size()-1){
						of<<pr[i][j].p1.first[l];					
					}else{
						of<<pr[i][j].p1.first[l]<<"-->";	
					}
					
				}
				of<<"\t"<<pr[i][j].p1.second<<endl;
				of<<j<<"\t";
				for(int l = 0;l<pr[i][j].p2.first.size();l++){
					if(l == pr[i][j].p2.first.size()-1){
						of<<pr[i][j].p2.first[l];					
					}else{
						of<<pr[i][j].p2.first[l]<<"-->";	
					}
				}
				of<<"\t"<<pr[i][j].p2.second<<endl<<endl;		
			}
			of<<endl;
		}
		of.close();
		cout<<"Routing table generated successfully"<<endl;
	}else{
		cout<<"file not found"<<endl;
	}
}

//use to store forwarding Info into the structure----------------------------------------------
int label = 5001;
void StoreForwardingInfo(vector<int> p1, int s, int d, int connID, int cost){
	pathIn[connID].ConnId = connID;
	pathIn[connID].Source = s;
	pathIn[connID].Destination = d;
	vector<int> temp1;
	vector<int> temp2;
	for(int i = 0;i<p1.size();i++){
		if(i == 0){
			temp2.push_back(-1);
			temp2.push_back(-1);
			temp2.push_back(p1[i+1]);
			temp2.push_back(label);
			temp1.push_back(label);	
			fr[p1[i]].forward.push_back(temp2);
			temp2.clear();		
		}else if(i == p1.size()-1){
			temp2.push_back(p1[i-1]);
			temp2.push_back(label);
			temp2.push_back(-1);
			temp2.push_back(-1);
			fr[p1[i]].forward.push_back(temp2);
			temp2.clear();	
		}else{
			temp2.push_back(p1[i-1]);
			temp2.push_back(label++);
			temp2.push_back(p1[i+1]);
			temp2.push_back(label);
			temp1.push_back(label);
			fr[p1[i]].forward.push_back(temp2);
			temp2.clear();		
		}  		
	}
	pathIn[connID].Label_List = temp1;
	pathIn[connID].Path_Cost = cost;
}

//update the link capacity after establishinf a connection----------------------------------------------
void UpdateLinkCap(vector<int> path, double beq){
	for(int i=0;i<path.size()-1;i++){
		linkCap[path[i]][path[i+1]] -= beq;
	}
}

//Establishing the connections one by one----------------------------------------------
void establishingConnections(){
	pathIn = new PI[No_of_conn];
	for(int i = 0;i<No_of_conn;i++){
		double source = connections[i][0];
		double destination = connections[i][1];
		double bmin = connections[i][2];
		double bavg = connections[i][3];
		double bmax = connections[i][4];
		vector<int> path1; //here we store first shortest path Temporarily
		int cost1;
		vector<int> path2; //here we store Second shortest path Temporarily
		int cost2;
		path1 = pr[(int)source][(int)destination].p1.first;
		path2 = pr[(int)source][(int)destination].p2.first;
		cost1 = pr[(int)source][(int)destination].p1.second;
		cost2 = pr[(int)source][(int)destination].p2.second;
		double beq;
		beq = min(1.0 * bmax, bavg + 0.25 * (bmax - bmin));
		bool IsEstablished = true;
		if(p == 0){
			for(int j = 0;j<path1.size()-1;j++){
				if(beq > linkCap[path1[j]][path1[j+1]]){
					IsEstablished = false;
					break;
				}			
			}
			if(IsEstablished == true){
				NoOfConnEst++;
				StoreForwardingInfo(path1, source, destination, i, cost1);//stor forwarding info 
				UpdateLinkCap(path1, beq); //update link capacity
				
			}else{
				for(int j = 0;j<path2.size()-1;j++){
					if(beq > linkCap[path2[j]][path2[j+1]]){
						IsEstablished = false;
						break;
					}			
				}
				if(IsEstablished == true){
					NoOfConnEst++;
					StoreForwardingInfo(path2, source, destination, i, cost2); //stor forwarding info
					UpdateLinkCap(path2, beq); //update link capacity
				}			
			}					
		}else if(p == 1){
			for(int j = 0;j<path1.size()-1;j++){
				if(bmax > linkCap[path1[j]][path1[j+1]]){
					IsEstablished = false;
					break;
				}			
			}
			if(IsEstablished == true){
				NoOfConnEst++;
				StoreForwardingInfo(path1, source, destination, i, cost1); //stor forwarding info
				UpdateLinkCap(path1, bmax); //update link capacity
				
			}else{
				for(int j = 0;j<path2.size()-1;j++){
					if(bmax > linkCap[path2[j]][path2[j+1]]){
						IsEstablished = false;
						break;
					}			
				}
				if(IsEstablished == true){
					NoOfConnEst++;
					StoreForwardingInfo(path2, source, destination, i, cost2); //stor forwarding info
					UpdateLinkCap(path2, bmax); //update link capacity
				}			
			}		
		}
	}
}

//use to Export Forwarding table into .txt file----------------------------------------------
void ExportForwardingTable(){
	ofstream of;
	of.open(forwardingtablefile);
	if(of.is_open()){
		for(int i=0;i<nodes;i++){
			of<<"forwardingtable of node "<<i<<endl;
			of<<"==================================="<<endl;;
			for(int j = 0;j<fr[i].forward.size();j++){
				for(int k = 0;k<fr[i].forward[j].size();k++){
					of<<fr[i].forward[j][k]<<" ";			
				}
				of<<endl;		
			}	
			of<<endl<<endl;
		}
		of.close();
		cout<<"forwarding table generated successfully"<<endl;
	}else{
		cout<<"file not found"<<endl;
	}
}

//use to Export Path into .txt file for all established connections----------------------------------------------
void ExportPaths(){
	ofstream of;
	of.open(pathfiles);
	if(of.is_open()){
		for(int i=0;i<No_of_conn;i++){
			if(pathIn[i].Source == pathIn[i].Destination || pathIn[i].Source > nodes){
				continue;		
			}
			of<<pathIn[i].ConnId<<"  ";
			of<<pathIn[i].Source<<"  ";
			of<<pathIn[i].Destination<<"  ";
			for(int j = 0;j<pathIn[i].Label_List.size();j++){
				if(j == pathIn[i].Label_List.size()-1){
					of<<pathIn[i].Label_List[j]<<" ";				
				}else{
					of<<pathIn[i].Label_List[j]<<"-->";
				}								
			}
			of<<" ";
			of<<pathIn[i].Path_Cost<<"  "<<endl;
		}
		of<<"no. of connections established : "<<NoOfConnEst<<endl;
		of.close();
		cout<<"path file generated successfully"<<endl;
	}else{
		cout<<"file not found"<<endl;
	}
}

//=========================================== main ============================================
int main(int argc, char* argv[]){
	
	if(argc < 15){
		cout<<"usage "<<argv[0]<<" -top topologyfile.txt -conn connectionfile.txt"; 
		cout<<" -rt routingtablefile.txt -ft forwardingtablefile.txt -path pathsfile.txt"; 
		cout<<" -flag hop/dist -p 0/1"<<endl;
      		exit(0);	
	}
	CommandlineHandler(argv, argc); 
	preprocessing();		//storing the input info into arrays
	DisplayInput();			//Display inputs given by the user
	findingfirstShortestpath();	//finding the first shortest path using Dijkstra's Algo
	findingSecondShortestpath();	//finding the second shortest path using Dijkstra's Algo
	ExportRoutingTable();		//Export the routing table
	establishingConnections();	//establishing the connections
	ExportForwardingTable();	//Export the Forwarding table
	ExportPaths();			//Export the Path information
	cout<<"no. of connections established : "<<NoOfConnEst<<endl;
	return 0;
}
