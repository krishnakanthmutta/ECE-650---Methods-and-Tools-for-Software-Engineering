#include<iostream>
#include<string>
#include<list>
#include<vector>
#include<climits>
using namespace std;

std::vector< std::vector<int> > adj;

int vec_size(std::vector <std::vector <int> > adj, int node) {
    int i =0;
    //cout << adj[node][0] << endl;
    while(adj[node][i]>=0) {
        i++;
    }
    return i;
}

// BFS - Breadth First Search
int bfs(int v1, int v2, std::vector <std::vector <int> > adj, int ver, int d[], int pred[]) {
    list <int> ls;
    int visit_status[ver];

    // initializing values for the above lists
    for (int i =0; i<ver; i++) {
        visit_status[i] = 0;
        d[i] = INT_MAX;
        pred[i] = -1;
    }

    ls.push_back(v1);
    visit_status[v1] = 1;
    d[v1] = 0;

    //BFS algorithm with additional check of node v2
    while(!ls.empty()) {
        int temp = ls.front();
        ls.pop_front();
        //traversing all the adjacent neighbor nodes of v1 and then traversing for other neighbor nodes until we reach v2
        for (int i = 0; i < adj[temp].size(); i++) {
            if (visit_status[adj[temp][i]] == 0 ) {
                ls.push_back(adj[temp][i]);
                d[adj[temp][i]] = d[temp] + 1;
                pred[adj[temp][i]] = temp;
                visit_status[adj[temp][i]] = 1;

                if (adj[temp][i] == v2)
                    return 1;
            }
        }
    }
    return 0;

}

//Shortest Path determination
vector <int> shortestpath(int v1, int v2, std::vector< std::vector<int> > adj, int ver) {
    int d[ver], pred[ver];
    for (int i =0; i<ver; i++) {
        d[i] = INT_MAX;
        pred[i] = -1;
    }
    vector <int> s_path;
    int temp1 = v2;
    
    if (bfs(v1, v2, adj, ver, d, pred) == 0) {
        return s_path;
    }

    s_path.push_back(temp1);
    while (pred[temp1] != -1) {
        s_path.push_back(pred[temp1]);
        temp1 = pred[temp1];
    }
    return s_path;
}

int main() {
    int vertices = 1;
    int k, l, m, x, y, a, b;
    int s_v1, s_v2;
    string st;
    vector <int> final_path;

    while (!std::cin.eof()) { // might not need to run this while loop -- instead it will take only one input and then keeps 
        
        std::string line;
        std::getline(std::cin, line);
        

        //Taking vertices input and storing the data
        if (line.find("V ") == 0) { // (line1.find("V ") == 0) 
            cout << line <<endl;
            //cout << "in V loop" << endl;

            //printing vertices
            //cout << line1<<endl;

            line.erase(0,2);
            adj.clear();
            vertices = stoi(line);
            

            //std::cout<<vertices;
            adj.resize(vertices);
        }
         
        
        // Taking Edges input and storing the data
        if (line.find("E ") == 0) { //line2.find("E ") == 0
            cout << line <<endl;
            //printing edges
            //cout<< line2 << endl;
            if (vertices <= 0) {
                cerr << "Error: Invalid vertices input."<<endl;
            }
            line.erase(0,2);
            //k = line.find("<");
            while((k = line.find("<")) != string::npos) {
                l = line.find(">");
                st = line.substr(k+1, l-2);
                m = st.find(",");
                x = stoi(st.substr(0, m));
                y = stoi(st.substr(m+1, l-1));
                if (x> vertices or y> vertices or x<=0 or y<=0) { 
                    cerr<< "Error: Vertex/Vertices does not exist"<<endl;
                    adj.clear();
                    break;
                }
                adj[x-1].push_back(y-1);
                adj[y-1].push_back(x-1);   
                line.erase(k,l+1);
                if (x> vertices or y> vertices) 
                    cerr<< "Error: Vertex does not exist"<<endl;  
            }
        }

        // For displaying Adjacency list
        /*for (int p = 0; p < vertices; p++ ) {
            cout << "Vertex " << p << "->";
            if (adj[p].size()!=0){
                for (int q = 0; q< adj[p].size(); q++ ) {
                    cout << adj[p][q] << ",";
                }
            }
            cout <<endl;
        }*/


        // read a line of input until EOL and store in a string


        // Taking shortest path input
        if (line.find("s ") == 0) {
            line.erase(0,2);
            //cout << line << endl;
            a = line.find(" ");
            //cout << "a " << a <<endl;
            s_v1 = stoi(line.substr(0, a));
            line.erase(0,a);
            //cout << line <<endl;
            s_v2 = stoi(line);
            //cout << "s_v1 " << s_v1 << "s_v2 " << s_v2 << endl;
            /*if ((s_v1 > vertices) or (s_v2 > vertices)) {
                cerr<< "Error: Invalid input, please try again" << endl;
            }*/
            if (adj.size() == 0)
                cerr << "Error: No data"<<endl;
            else {
                if ((s_v1 == s_v2) and (s_v1 <= vertices)){
                    cout << s_v1 << endl;
                }
                else if (s_v1 == s_v2 and (s_v1 > vertices)){
                    cerr<< "Error: Invalid nodes"<<endl;
                }
                else {
                    //cout<< "final path size " << final_path.size()<<endl;
                    //if (final_path.size() == 0)
                    /*if (s_v1 > vertices or s_v2 > vertices)
                        cerr<< "Error: No path between the nodes"<<endl;*/
                    if (s_v1 <= vertices and s_v1 > 0 and s_v2 <= vertices and s_v2 > 0 )    
                        final_path = shortestpath(s_v1-1, s_v2-1, adj, vertices);
                    if (final_path.size() == 0)
                        cerr<< "Error: No path between the nodes"<<endl;
                    if (final_path.size() != 0) {
                        for (int i = final_path.size() -1; i>=0; i--) {
                            if (i != 0) 
                                cout << final_path[i]+1 << "-";
                            else
                                cout << final_path[i]+1;
                        }
                        cout << endl;
                        final_path.clear();
                    }
                }
            }
        }

    }
    return 0;
}
