#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

void parseCmd(string*, string);

int main()
{
	string cmd1[25];
    string cmd2[25];

    cout << "command1? ";
    cin >> cmd1[0];
    cout << "command1? ";
    cin >> cmd2[0];
	
	int pipefd[2];
	int rs;
	char buffer[256];
	
	rs = pipe(pipefd);
	if (rs < 0)
	{
		perror("pipe");
		exit(1);
	}
	
	pid_t pid = fork();
	
	if(pid == 0)
	{
		//close write end of pipe
		close(pipefd[1]);
		close(0);
		//duplicate read end of pip to standard input file discriptor.
		dup(pipefd[0]);
		close(pipefd[0]);
		
        execlp("wc", "wc", nullptr);
	} else
	{
		//parent or error
		//close read end of pipe
		close(pipefd[0]);
		close(1); //standard output
		//duplicate write end of pipe into standard output file discriptor.
		dup(pipefd[1]);
		close(pipefd[1]);
		
		execlp("ls", "ls", nullptr);
        wait(nullptr); //wait for child to finish.
	}
	
	
	
	return 0;
}

void parseCmd(string* cmds, string bulk)
{

}
