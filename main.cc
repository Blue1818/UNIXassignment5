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

int main()
{
    //Variables:
	string cmd1[25];
    cmd1[24] = "endHere";
    string cmd2[25];
    cmd2[24] = "endHere";
    char cmd1Format[25][255];
    //cmd1Format[24] = NULL;
    char cmd2Format[25][255];
    //cmd2Format[24] = NULL;
    int pipefd[2];
	int rs;
	char buffer[256];

    char *x[25];
    char *y[25];
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
        return 0;
    }

    //Turn string arrays into c_str arrays.
    for (int i = 0;(i < 25) && (cmd1[i] != "endHere"); i++)
    {
        //cmd1Format[i] = cmd1[i].c_str();
        strcpy(cmd1Format[i], cmd1[i].c_str());

        /* if (cmd1[i+1] == "endHere")
        {
            //cmd1Format[i+1] = NULL;
            x[i+1] = nullptr;
        } */
    }
	for (int i = 0;(i < 25) && (cmd2[i] != "endHere"); i++)
    {
        //cmd2Format[i] = cmd2[i].c_str();
        strcpy(cmd2Format[i], cmd2[i].c_str());

        /* if (cmd2[i+1] == "endHere")
        {
            //cmd2Format[i+1] = NULL;
            x[i+1] = nullptr;
        } */
    }
    for (int i = 0; cmd1[i] != "endHere"; i++) x[i] = cmd1Format[i];
    for (int i = 0; cmd2[i] != "endHere"; i++) y[i] = cmd2Format[i];

	rs = pipe(pipefd);
	if (rs < 0)
	{
		perror("pipe");
		exit(1);
	}
	
	pid_t pid = fork();
	
	if(pid == 0)
	{
        //child: receave input from parent's output
		//close write end of pipe
		close(pipefd[1]);
		close(0);
		//duplicate read end of pip to standard input file discriptor.
		dup(pipefd[0]);
		close(pipefd[0]);
		
        //execlp("wc", "wc", nullptr);
        execvp(cmd1Format[0], x);
	} else
	{
		//parent or error. output to be placed into child's input
		//close read end of pipe
		close(pipefd[0]);
		close(1); //standard output
		//duplicate write end of pipe into standard output file discriptor.
		dup(pipefd[1]);
		close(pipefd[1]);
		
		//execlp("ls", "ls", nullptr);
        execvp(cmd2Format[0], y);
        wait(nullptr); //wait for child to finish.
	}
	
	
	
	return 0;
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