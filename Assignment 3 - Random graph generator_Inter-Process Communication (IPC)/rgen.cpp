#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <fstream>

using namespace std;


//obtain random number from /dev/urandom
int random_num_gen(int min, int max) { 
	unsigned int random;

	unsigned random_value;
	int fd = -1;

	ifstream urandom("/dev/urandom");
	if (urandom.fail()) {
		cerr << "Error: cannot open /dev/urandom\n";
		return 1;
	}

	int c = 0;
	while(1) {
		urandom.read((char*)&c,1);
		//read(fd, &random, sizeof(unsigned int));
		c = min + (c % (max - min + 1));
		if(c>=min && c<=max) {
			//cout << "Random value: " << c <<endl;
			urandom.close();
			break;
		}
	}
	return c;
}

string random_street(int l) {
	//srand(time(0));
	string str = "abcdefghijklmnopqrstuvwxyz";
	string name_st;
	int p;
	while (name_st.size() != l) {
		p = ((rand() % (str.size() - 1)));
		name_st += str.substr(p, 1);
	}
	return name_st;
}


bool same_pt(int x1, int y1, int x2, int y2) {

	if (x1 == x2 && y1 == y2) {
		return true;
	}
	else {
		return false;
	}
}

bool Linesoverlap(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3)
{

	//cout << "in Linesoverlap fn "<<endl;
	int Minx01 = min(x0, x1);
	int Miny01 = min(y0, y1);
	int Minx23 = min(x2, x3);
	int Miny23 = min(y2, y3);
	int Maxx01 = max(x0, x1);
	int Maxy01 = max(y0, y1);
	int Maxx23 = max(x2, x3);
	int Maxy23 = max(y2, y3);

	if (x1 != x0 && x2 != x3)
	{
		float k1 = (y1 - y0) / (x1 - x0);
		float k2 = (y3 - y2) / (x3 - x2);
		float Den = (y1 - y0) * (x3 - x2) - (y3 - y2) * (x1 - x0);
		if (k1 == k2)
		{ //parllel
			float d1 = abs(y0 * (x1 - x0) - x0 * (y1 - y0) - y2 * (x3 - x2) + x2 * (y3 - y2)); //??d = abs(c1-c2) / sqrt(a*a+b*b)
			if (d1 == 0)
			{//overlap
				if ((x2 > Minx01&& x2<Maxy01 && y2>Miny01&& y2 < Maxy01) || (x3 > Minx01&& x3<Maxy01 && y3>Miny01&& y3 < Maxy01)
					|| (x0 > Minx23&& x0<Maxy23 && y0>Miny23&& y0 < Maxy23) || (x1 > Minx23&& x1<Maxy23 && y1>Miny23&& y1 < Maxy23))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else {
			return false;
		}
	}
	else
		return false;
}

bool is_valid_point(vector <int> point_list, int x, int y) {
	int len = point_list.size();
	//cout << "in is_valid_point fn" <<endl;
	if (len == 0) {
		return true;
	}
	else if (len == 2) {
		if ((same_pt(x, y, point_list[0], point_list[1]))) {
			//cout << "1st condtn" <<endl;
			return false;
		}
		return true;
	}
	else {
		//cout << "in last loop" <<endl;
		for (int i = 0; i < point_list.size(); i = i + 2) {
			if (same_pt(x, y, point_list[i], point_list[i + 1])) {
				//cout << "2nd condtn" <<endl;
				return false;
			}
		}
		return true;

	}
	//return true;
}

bool is_overlap(vector <int> point_list,vector< vector<int> > all_pts ) {
	//std::cout <<"in overlap fn" <<endl;
	for (int i = 2; i < point_list.size(); i = i + 2) {
		//cout << " all_pts.size() " << all_pts.size() <<endl;
		for (int j = 0; j < all_pts.size(); j = j + 2) {
			//cout << " all_pts[j].size() " << all_pts[j].size() <<endl;
			for (int k = 2; k < all_pts[j].size(); k = k + 2) {
				//std::cout <<"all_pts[j].size(): " <<all_pts[j].size();
				//std:: cout<<" inside last loop " <<endl;
				if (Linesoverlap(point_list[i], point_list[i + 1],point_list[i-2],point_list[i-1],all_pts[j][k],all_pts[j][k+1],all_pts[j][k-2],all_pts[j][k-1] )) {
					return true;
				}
				// else
					// return false;
			}
		}
	}
	return false;
}

int main(int argc, char* argv[]) {
	
	int ip;
    int max_st = 10, max_lnseg = 5, max_wait = 5, max_coord = 20;

	while ((ip = getopt(argc, argv, "s:n:l:c:")) != EOF) {
        //s,n,l,c options and error handling for these options
        switch (ip) {
            case 's':
                if ((atoi(optarg)) < 2) {
                    cerr<< "Error: Invalid input for streets";
					//cout << "max_st: " << max_st;
                    break;
                }
                if (atoi(optarg) >= 2) {
					max_st = atoi(optarg);
					//cout << "max_st: " << max_st;
                	break;
				}
            case 'n':
                if ((atoi(optarg)) < 1) {
                    cerr<< "Error: Invalid input for number of line segments in a street";
					//cout << "max_lnseg: " << max_lnseg;
                    break;
                }
                if ((atoi(optarg)) >= 1) {
					max_lnseg = atoi(optarg);
					//cout << "max_lnseg: " << max_lnseg;
                	break;
				}
            case 'l':
                if ((atoi(optarg)) < 5) {
                    cerr<< "Error: Invalid input for wait time";
					//cout << "max_wait: " << max_wait;
                    break;
                }
                if ((atoi(optarg)) >= 5){
					max_wait = atoi(optarg);
					//cout << "max_wait: " << max_wait;
                	break;
				}
            case 'c':
                if ((atoi(optarg)) < 1) {
                    cerr<< "Error: Invalid input for coordinates";
					//cout << "Max_coord: " << max_coord;
                    break;
                }
                if ((atoi(optarg))>= 1){
					max_coord = atoi(optarg);
					//cout << "Max_coord: " << max_coord;
                	break;
				}
			default:
				break;
        }
    }

	int st_num, ln_num, wait_time, pt_num;
	int s = 0;
	string name_st;
	vector <string> lst_st;

	std::vector< std::vector<int> > all_pts;

	while (1) {
		// std::vector< std::vector<int> > all_pts;
		st_num = random_num_gen(2, max_st);
		wait_time = random_num_gen(5, max_wait);

		// std::cout << "st_num: " << st_num <<endl;
		
		while (s < st_num) {
			vector <int> point_list, xs, ys;
			ln_num = random_num_gen(1, max_lnseg);
			// std::cout << "Line segments random value: " << ln_num <<endl;

			int try_cnt = 0;
			int temp_cnt = 0;
			for (int l = 0; l <=  ln_num; l++) {
				int x_val, y_val;
				// std::cout << "Try cnt: " <<try_cnt <<endl;
				// std::cout << "Temp cnt: " <<temp_cnt <<endl;
				
				temp_cnt = try_cnt;
				x_val = random_num_gen(-max_coord, max_coord);
				y_val = random_num_gen(-max_coord, max_coord);
				// std::cout << "x_value: " <<x_val << " y_val: " << y_val <<endl;

				// std::cout << "condition: " << (is_valid_point(point_list, x_val, y_val)&&!is_overlap(point_list,all_pts)) <<endl ;
				// std::cout << "is valid point: " << is_valid_point(point_list, x_val, y_val) << endl;
				// std::cout << "not is overlap: " << !is_overlap(point_list,all_pts) <<endl;

				if (is_valid_point(point_list, x_val, y_val)&&!is_overlap(point_list,all_pts))
				{	
					//cout << "l value: " << l <<endl;
					point_list.push_back(x_val);
					point_list.push_back(y_val);

					try_cnt = 0;
					temp_cnt = 0;
					
				}

				else
				{
					//cout << "In try cnt loop" <<endl;
					++try_cnt;
					if (try_cnt == 25)
					{
						cerr << "Error: failed to generate valid input for 25 simultaneous attempts" << endl;
						exit(1);
					}
					//continue;
				}

				if(temp_cnt != try_cnt)
					l= l-1;
				// cout << "l value: " <<l <<endl;
			}
			// std::cout << "Point list: ";
			// for (int i = 0; i <=  sizeof(point_list); i++) {
			// 	std::cout << " " << point_list[i]; }
			// std::cout <<endl;
				

			name_st = random_street(4);
			lst_st.push_back(name_st);

			all_pts.push_back(point_list);

			std::cout << "add " << "\"" << name_st << "\" ";
			for (int i = 0; i <= ln_num; i++) {
				std::cout << "(" << point_list[2*i] << "," << point_list[2*i + 1] << ") ";
			}
			std::cout << endl;
			s++;
		}
		std::cout << "gg" << endl;

		sleep(wait_time);

		for (int i = 0; i < lst_st.size(); i++) {
			std::cout << "rm " << "\"" << lst_st[i] << "\"" << std::endl;
		}
		lst_st.clear();
		s = 0;
		std::cout<< flush;
	}

	return 0;
}