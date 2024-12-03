#include <bits/stdc++.h>
using namespace std;
class Union {
	int* parent;
	int* rank;
	public:
		Union (int n)
		{
			parent = new int[n];
			rank = new int[n];
			for (int i = 0; i < n; i++) {
				parent[i] =-1;
				rank[i] = 1;
			}
		}
		
		int find(int i)
		{
			if (parent[i] ==-1){
				return i;

			}
			return parent[i] = find(parent[i]);
		}
		
		void edge_union(int x, int y)
		{
			int s1 = find(x);
			int s2 = find(y);
			if (s1 != s2) {
				if (rank[s1] < rank[s2]) {
					parent[s1] = s2;
				}
				else if (rank[s1] > rank[s2]) {
					parent[s2] = s1;
				}
				else {
					parent[s2] = s1; 
					rank[s1] += 1; 
				}
			}
		}
	};
class Graph {
	
	vector<vector<int> > edgelist;
	int V;
	
	public:
		Graph(int V) { this->V = V; }
		
		void addEdge(int x, int y, int w)
		{
			int arr[3] = {w, x , y};
			vector<int> vec;
			for(int i = 0; i<3; i++){
				vec.push_back(arr[i]);
			}
			edgelist.push_back(vec);
		}
		
		void kruskals_mst()
		{
			sort(edgelist.begin(), edgelist.end());
			
			Union s(V);
			int ans = 0;
			cout<<"\n------------------------------"<<endl;
			cout<<"\n>> Edges of constructed MST:-"<< endl;
			for (int i=0; i<edgelist.size(); i++) {
				vector<int> edge = edgelist[i];
				int w = edge[0];
				int x = edge[1];
				int y = edge[2];
				if (s.find(x) != s.find(y)) {
					s.edge_union(x, y);
					ans += w;
					cout << x << "-- " << y << " == " << w<< endl;
				}
			}
			cout << "\n-> Cost of MST : " << ans;
		}
};
int main()
{
	cout<<"=========== KRUSKAL'S MST ALGORITHM =========== \n"<<endl;
	int vert,e;
	cout<<"-> Enter no. of vertices : ";
	cin>>vert;
	cout<<"-> Enter no. of edges : ";
	cin>>e;
	Graph g(vert);
	cout<<"\n-> Enter edges (source,destination,weight):-"<<endl;
	for (int i=0;i<e;i++){
		int a,b,c;
		cout<<"\n>> Enter Edge "<<i<<" : ";
		cin>>a>>b>>c;
		
		g.addEdge(a,b,c);
	}
	g.kruskals_mst();
	
	return 0;
}
