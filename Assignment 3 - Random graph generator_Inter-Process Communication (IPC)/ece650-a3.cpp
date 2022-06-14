#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <fcntl.h>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char** argv) {
    int ip;
    int max_st = 10, max_lnseg = 5, max_wait = 5, max_coord = 20;

    while ((ip = getopt(argc, argv, "s:n:l:c:")) != EOF) {
        //s,n,l,c options and error handling for these options
        switch (ip) {
            case 's':
                if ((atoi(optarg)) < 2) {
                    cerr<< "Error: Invalid input for streets"<<endl;
                    exit(1);
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
                    cerr<< "Error: Invalid input for number of line segments in a street"<<endl;
                    exit(1);
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
                    cerr<< "Error: Invalid input for wait time"<<endl;
                    exit(1);
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
                    cerr<< "Error: Invalid input for coordinates"<<endl;
                    exit(1);
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

    vector<pid_t> kids;
    int pipe1[2];
    pipe(pipe1);
    int pipe2[2];
    pipe(pipe2);
    pid_t p1 = fork();
    if (p1 == 0) {
        dup2(pipe1[1], STDOUT_FILENO);
        close(pipe1[0]);
        close(pipe2[0]);
        close(pipe2[1]);
        close(pipe1[1]);
        execv("rgen", argv);
        //execl("./rgen", "rgen", "-s", to_string(max_st).c_str(), "-n", to_string(max_lnseg).c_str(), "-l", to_string(max_wait).c_str(), "-c", to_string(max_coord).c_str(), NULL);
    } else {
        kids.push_back(p1);
        pid_t p2 = fork();
        if (p2 == 0) {
            dup2(pipe2[1], STDOUT_FILENO);
            close(pipe2[0]);
            close(pipe2[1]);
            dup2(pipe1[0], STDIN_FILENO);
            close(pipe1[0]);
            close(pipe1[1]);

            char *argv1[3];
            argv1[0] = (char *)"python3";
            argv1[1] = (char *)"ece650-a1.py";
            argv1[2] = NULL;

            execvp(argv1[0], argv1);
            //execl("/usr/bin/python3", "python3", "ece650-a1.py", NULL);
        } else {
            pid_t p3 = fork();
            if (p3 == 0) {
                dup2(pipe2[0], STDIN_FILENO);
                close(pipe1[0]);
                close(pipe1[1]);
                close(pipe2[0]);
                close(pipe2[1]);
                execv("ece650-a2",argv);
                //execl("./ece650-a2", "ece650-a2", NULL);
            } else {
                pid_t p4 = fork();
                if (p4 == 0) {
                    dup2(pipe2[1], STDOUT_FILENO);
                    close(pipe1[0]);
                    close(pipe1[1]);
                    close(pipe2[0]);
                    close(pipe2[1]);
                    while (true) {
                        string line;
                        getline(cin, line);
                        if (cin.eof()) {
                            exit(0);
                        }
                        cout << line << endl;
                    }

                } else {
                    kids.push_back(p4);
                    int status = 1;
                    while (1) {
                        for (auto it = kids.begin(); it != kids.end(); ++it) {
                            waitpid((*it), &status, WNOHANG);
                            if (status == 0) {
                                if ((*it) == p1) {
                                    kill(p2, SIGTERM);
                                    kill(p3, SIGTERM);
                                    kill(p4, SIGTERM);
                                    return 0;
                                } else {
                                    kill(p2, SIGTERM);
                                    kill(p3, SIGTERM);
                                    kill(p1, SIGTERM);
                                    return 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}