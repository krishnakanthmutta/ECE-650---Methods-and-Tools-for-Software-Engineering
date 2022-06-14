#include<iostream>
#include<string>
#include<list>
#include<vector>
#include<climits>
#include <algorithm>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
using namespace std;

std::vector< std::vector<int> > adj;

void prnt_vertexcover (vector<int> vertex_cover_lst)
{
    //if((!vertex_solution.empty())&&(!edg_list.empty())) {
    for (int i = 0;i<vertex_cover_lst.size();i++)
    {
        cout<<vertex_cover_lst[i]+1<<" ";
    }
    cout<<endl;
}

void vertex_cover(std::vector<int> edg_list, int vertices) 
{   
    vector<int> vertex_cover_lst;
    vertex_cover_lst.clear();
    int k,lower,upper;
    upper = vertices;
    lower = 1; 

    while(lower<=upper)
    {
        k=int(lower+upper)/2;
        std::vector<std::vector<Minisat::Lit> > res(vertices); 

        Minisat::Solver solver;
        
        for(int i = 0; i < vertices; i++)
        {
            for(int j = 0; j < k; j++)
            {
                Minisat::Lit li = Minisat::mkLit(solver.newVar());
                res[i].push_back(li);
            }
        }

        //clause 1
        for(int i=0;i<k;i++)
        {
	        Minisat::vec<Minisat::Lit> temp;
        	for(int j=0;j<vertices;j++)
        	{
        		temp.push(res[j][i]);
        	}
        	solver.addClause(temp);
        	temp.clear();       
        }

        //clause 2
        for(int m=0;m<vertices;m++)
        {
        	for(int p=0;p<k-1;p++)
        	{
        		for(int q=p+1;q<k;q++)
        		{
        			solver.addClause(~res[m][p],~res[m][q]);
        		}
        	}
        }

        //clause 3
        for(int m=0;m<k;m++)
        {
        	for(int p=0;p<vertices-1;p++)
        	{
        		for(int q=p+1;q<vertices;q++)
        		{
        			solver.addClause(~res[p][m],~res[q][m]);
        		}
        	}
        }
        
        //clause 4
        for(int i=0;i<edg_list.size();i=i+2) 
        {
            //int a1 = edg_list[i];
            //int a2 = edg_list[i+1];
            Minisat::vec<Minisat::Lit> temp1;
        	for(int j=0;j<k;j++)
        	{
        		temp1.push(res[edg_list[i]][j]);
        		temp1.push(res[edg_list[i+1]][j]);
        	}
        	solver.addClause(temp1);
        	temp1.clear();
        }

        auto sol = solver.solve();
        if(sol) 
        {
            upper = k-1;
            vertex_cover_lst.clear();
            for(int i = 0; i < vertices; i++)
            {
                for(int j = 0; j < k; j++)
                {
                    Minisat::lbool tf=solver.modelValue(res[i][j]);
                    if(tf==Minisat::l_True)
                    {   
                        vertex_cover_lst.push_back(i);
                    }
                }
            }
        }
        else
        {
            lower = k+1;
        }
        std::sort(vertex_cover_lst.begin(),vertex_cover_lst.end()); //added this line

    }

    prnt_vertexcover (vertex_cover_lst);
    
    vertex_cover_lst.clear();

}

int main() {
    int vertices = 1;
    int k, l, m, x, y;
    //int s_v1, s_v2;
    string st;
    vector<int> edg_list;
    //vector <int> final_path;

    while (!std::cin.eof()) { // might not need to run this while loop -- instead it will take only one input and then keeps 
        
        std::string line;
        std::getline(std::cin, line);
        //edg_list.clear();
        

        //Taking vertices input and storing the data
        if (line.find("V ") == 0) { // (line1.find("V ") == 0) 
            //cout << line <<endl;
            //cout << "in V loop" << endl;

            //printing vertices
            //cout << line1<<endl;

            line.erase(0,2);
            adj.clear();
            edg_list.clear();
            vertices = stoi(line);
            

            //std::cout<<vertices;
            adj.resize(vertices);
        }

        //cout << "Vertices " << vertices << endl;
         
        
        // Taking Edges input and storing the data
        if (line.find("E ") == 0) { //line2.find("E ") == 0
            //cout << line <<endl;
            //printing edges
            //cout<< line2 << endl;
            /*if (line == "E {}") {
                cerr << "" <<endl;
            }*/
            if (vertices <= 0) {
                cerr << "Error: Invalid vertices input."<<endl;
            }
            line.erase(0,2);
            //k = line.find("<");
            while((k = line.find("<")) != string::npos) {
                //cout<<"inside while k loop"<<endl;
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
                edg_list.push_back(y-1);
                edg_list.push_back(x-1);
                line.erase(k,l+1);
                if (x> vertices or y> vertices) 
                    cerr<< "Error: Vertex does not exist"<<endl;  
            }
            if (!edg_list.empty()) {
                vertex_cover(edg_list,vertices);
            }
            else if(vertices != 0)
                cout <<endl;
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

        adj.clear();
        edg_list.clear();


    }
    return 0;
}