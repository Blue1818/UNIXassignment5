#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h> //wait
using namespace std;

int main()
{
	cout << "Before pipe\n";
	
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
		
		//child reads
		ssize_t nr = read(0,  buffer, sizeof(buffer));
		write(1, buffer, nr);
	} else
	{
		//parent or error
		//close read end of pipe
		close(pipefd[0]);
		close(1); //standard output
		//duplicate write end of pipe into standard output file discriptor.
		dup(pipefd[1]);
		close(pipefd[1]);
		
		
		//parent writes
		//write(1, "Hello", 6);
		//now that the pipe is using standard output, you can use cout into pipe.
		cout << "Caffeine is better than... zzzzzzzzz" << endl;
		wait(nullptr);
	}
	
	
	
	return 0;
}
