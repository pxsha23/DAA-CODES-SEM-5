// Scenario: University Timetable Scheduling A university is facing challenges in
// scheduling exam timetables due to overlapping student enrolments in multiple courses. To
// prevent clashes, the university needs to assign exam slots efficiently, ensuring that no two exams
// taken by the same student are scheduled at the same time.
// To solve this, the university decides to model the problem as a Graph Colouring Problem, where:
// ● Each course is represented as a vertex.
// ● An edge exists between two vertices if a student is enrolled in both courses.
// ● Each vertex (course) must be assigned a colour (time slot) such that no two adjacent vertices
// share the same colour (no two exams with common students are scheduled in the same slot).
// As a scheduling system developer, you must:
// 1. Model the problem as a graph and implement a graph colouring algorithm (e.g., Greedy
// Colouring or Backtracking).
// 2. Minimize the number of colours (exam slots) needed while ensuring conflict-free scheduling.
// 3. Handle large datasets with thousands of courses and students, optimizing performance.
// 4. Compare the efficiency of Greedy Colouring, DSATUR, and Welsh-Powell algorithms for
// better scheduling.
// Extend the solution to include room allocation constraints where exams in the same slot should
// fit within available classrooms.

#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <algorithm>
using namespace std;

struct Graph {
int n; vector<vector<int>> adj;
Graph(int n): n(n), adj(n) {}
void addEdge(int u,int v){ if(u!=v){ adj[u].push_back(v); adj[v].push_back(u);} }
};

// Greedy
vector<int> greedy(const Graph &G,const vector<int> &order={}) {
int n=G.n; vector<int> col(n,-1), mark(n,-1);
for(int i=0;i<n;i++){
int v = order.empty()? i: order[i];
for(int nb:G.adj[v]) if(col[nb]!=-1) mark[col[nb]]=i;
int c=0; while(mark[c]==i) c++; col[v]=c;
} return col;
}

// Welsh-Powell
vector<int> welsh_powell(const Graph &G){
vector<pair<int,int>> deg; for(int i=0;i<G.n;i++) deg.push_back({(int)G.adj[i].size(),i});
sort(deg.rbegin(),deg.rend());
vector<int> order; for(auto &p:deg) order.push_back(p.second);
return greedy(G,order);
}

// DSATUR
vector<int> dsatur(const Graph &G){
int n=G.n; vector<int> col(n,-1), deg(n);
for(int i=0;i<n;i++) deg[i]=G.adj[i].size();
set<pair<pair<int,int>,int>> pq;
for(int i=0;i<n;i++) pq.insert({{0,-deg[i]},i});
vector<unordered_set<int>> used(n);
while(!pq.empty()){
int v=pq.rbegin()->second; pq.erase(--pq.end());
int c=0; while(used[v].count(c)) c++; col[v]=c;
for(int nb:G.adj[v]) if(col[nb]==-1){
pq.erase({{-(int)used[nb].size(),-deg[nb]},nb});
used[nb].insert(c);
pq.insert({{-(int)used[nb].size(),-deg[nb]},nb});
}
} return col;
}

// Utility
int colorCount(const vector<int>& c){ return c.empty()?0:*max_element(c.begin(),c.end())+1; }

// Room allocation
bool allocateRooms(const vector<int>& color,const vector<int>& size,const vector<int>& cap){
int slots=colorCount(color); bool ok=true;
for(int s=0;s<slots;s++){
vector<int> exams;
for(int i=0;i<color.size();i++) if(color[i]==s) exams.push_back(i);
sort(exams.begin(),exams.end(),[&](int a,int b){return size[a]>size[b];});
vector<int> used(cap.size(),0);
cout << "\nTime Slot " << s+1 << ": ";
for(int c:exams){
bool placed=false;
for(int r=0;r<cap.size();r++) if(!used[r] && cap[r]>=size[c]){
used[r]=1; placed=true;
cout << "(Course " << c+1 << "->Room " << r+1 << ") ";
break;
}
if(!placed){ cout << "(Course " << c+1 << "->NO ROOM) "; ok=false; }
}
} return ok;
}

int main(){
ios::sync_with_stdio(false);
cin.tie(nullptr);

int n,m;
cout << "Enter number of courses: " << endl; cout.flush();

if(!(cin>>n)) return 0;
cout << "Enter number of conflicts (edges): " << endl; cout.flush();
cin >> m;
Graph G(n);
cout << "Enter conflicting course pairs (u v), 1-based indices:" << endl; cout.flush();
for(int i=0;i<m;i++){int u,v;cin>>u>>v;G.addEdge(u-1,v-1);}

int R;
cout << "Enter number of rooms: " << endl; cout.flush();
cin >> R;
vector<int> cap(R);
cout << "Enter capacities of rooms (space separated):" << endl; cout.flush();
for(int &x:cap) cin>>x;

vector<int> size(n);
cout << "Enter number of students in each course (n integers):" << endl; cout.flush();
for(int &x:size) cin>>x;

auto cG = greedy(G);
auto cW = welsh_powell(G);
auto cD = dsatur(G);
int sG=colorCount(cG), sW=colorCount(cW), sD=colorCount(cD);

cout << "\n--- RESULTS ---\n";
cout << "Greedy uses " << sG << " slots\n";
cout << "Welsh-Powell uses " << sW << " slots\n";
cout << "DSATUR uses " << sD << " slots\n";

vector<pair<int,string>> alg = {{sD,"DSATUR"},{sW,"Welsh-Powell"},{sG,"Greedy"}};
sort(alg.begin(),alg.end());
string best = alg[0].second;
cout << "\nBest Algorithm: " << best << "\n";

vector<int> bestCol = (best=="DSATUR"?cD:(best=="Welsh-Powell"?cW:cG));
cout << "\nCourse Slot Assignments:\n";
for(int i=0;i<n;i++) cout << "Course " << i+1 << " -> Slot " << bestCol[i]+1 << "\n";

cout << "\n--- ROOM ALLOCATION ---\n";
bool ok = allocateRooms(bestCol,size,cap);
cout << "\n\nFinal Allocation " << (ok? "successful" : "failed (not enough room capacity)") <<
"\n";
return 0;
}
