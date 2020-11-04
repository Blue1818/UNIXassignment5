/*	Dale Westberg z1910234
	CSCI 330
	Assignment #3
	9/18/20
*/
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <sstream>
#include <cstring>
using namespace std;

//returns false if array contains quit
bool parseCmd(string*, string);

bool driver();

int main()
{
    bool flag = true;
    while (flag == true) flag = driver();

    return 0;
}


bool driver()
{
    //Variables:
	string cmd1[25];
    string cmd2[25];
    char cmd1Format[25][255];
    char cmd2Format[25][255];
    int pipefd[2];
	int rs;
	char buffer[256];
    pid_t pid;
    char *x[25];
    char *y[25];

    
        cmd1[24] = "endHere";
        cmd2[24] = "endHere";
        for (int i = 0; i < 25; i++)
        {
            x[i] = nullptr;
            y[i] = nullptr;
        }

        //Get commands from user.
        cout << "command1? ";
        getline(cin,cmd1[0]);
        cout << "command2? ";
        getline(cin,cmd2[0]);

        if ((parseCmd(cmd1, cmd1[0]) == false) || (parseCmd(cmd2, cmd2[0]) == false))
        {
            return false;
        }

        //Turn string arrays into c_str arrays.
        for (int i = 0;(i < 25) && (cmd1[i] != "endHere"); i++)
        {
            strcpy(cmd1Format[i], cmd1[i].c_str());
        }
        for (int i = 0;(i < 25) && (cmd2[i] != "endHere"); i++)
        {
            strcpy(cmd2Format[i], cmd2[i].c_str());
        }
        for (int i = 0; cmd1[i] != "endHere"; i++) x[i] = cmd1Format[i];
        for (int i = 0; cmd2[i] != "endHere"; i++) y[i] = cmd2Format[i];

        rs = pipe(pipefd);
        if (rs < 0)
        {
            perror("pipe");
            exit(1);
        }
        
        pid = fork();
        
        if(pid == 0)
        {
            //child: receave input from parent's output
            //close write end of pipe
            close(pipefd[1]);
            close(0);
            //duplicate read end of pip to standard input file discriptor.
            dup(pipefd[0]);
            close(pipefd[0]);
        
            execvp(cmd2Format[0], x);
        } else
        {
            //parent or error. output to be placed into child's input
            //close read end of pipe
            close(pipefd[0]);
            close(1); //standard output
            //duplicate write end of pipe into standard output file discriptor.
            dup(pipefd[1]);
            close(pipefd[1]);
            
            execvp(cmd1Format[0], y);
            wait(nullptr); //wait for child to finish.
        }
    
	
	
	return true;
}


bool parseCmd(string* cmds, string bulk)
{
    //make last one nullptr.
    istringstream stream;
    string temp = "";
    stream.str(bulk);
    int counter = 1;

    for(int i = 0; i < bulk.length(); i++)
    {
        if (bulk.at(i) == ' ') counter++;
    }

    for(int i = 0; i < counter; i++)
    {
        stream >> temp;
        cmds[i] = temp;
    }
    //Mark end of array;
    cmds[counter] = "endHere";

    //if quit is any of the commands, return false;
     for(int i = 0; i < counter; i++)
    {
        if (cmds[i] == "quit") return false;
    }

    return true;
}