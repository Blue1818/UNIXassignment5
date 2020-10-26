#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <sstream>
using namespace std;

//returns false if array contains quit
bool parseCmd(string*, string);

int main()
{
    //Variables:
	string cmd1[25];
    string cmd2[25];
    int pipefd[2];
	int rs;
	char buffer[256];

    //Get commands from user.
    cout << "command1? ";
    getline(cin,cmd1[0]);
    cout << "command2? ";
    getline(cin,cmd2[0]);

    if ((parseCmd(cmd1, cmd1[0]) == false) || (parseCmd(cmd2, cmd2[0]) == false))
    {
        return 0;
    }

	
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
        execvp(cmd1[0].c_str, cmd1, nullptr);
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
        execvp(cmd2[0], cmd2, nullptr);
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
    //this keeps comming up as segmentation fault
    //cmds[counter] = nullptr;

    //if quit is any of the commands, return false;
     for(int i = 0; i < counter; i++)
    {
        if (cmds[i] == "quit") return false;
    }

    return true;
}