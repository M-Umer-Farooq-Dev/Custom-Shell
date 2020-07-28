#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <cstring>
#include <signal.h>
#include <sys/wait.h>
#include <pwd.h>
#include <fcntl.h>
#include <dirent.h>
using namespace std;
void sigintHandler(int sig_num)
{

	signal(SIGINT, sigintHandler);
	printf("\n Cannot be terminated using Ctrl+C \n");
}

void EXEC(string first, string input, bool flag)
{
	int count = 2;
	int pid = fork();
	if (pid == 0)
	{
		for (int i = 0; input[i] != '\0'; i++)
		{
			if (input[i] == ' ')
			{
				count++;
			}
		}

		char **buffer = new char *[count + 1];
		buffer[0] = new char(first.length());
		if (flag == false)
		{
			first[first.length() - 1] = '\0';
		}
		strcpy(buffer[0], first.c_str());

		for (int a = 1; a < count; a++)
		{
			buffer[a] = new char[input.substr(0, input.find(' ')).length()];
			strcpy(buffer[a], input.substr(0, input.find(' ')).c_str());
			input.erase(0, input.find(' ') + 1);
		}

		buffer[count] = NULL;
		execvp(buffer[0], buffer);
		perror("exec failed");
	}
	else
	{
		if (flag == true)
		{
			int status;
			(void)waitpid(pid, &status, 0);
		}
	}
}

int main(int argc, char **argv, char **envp)
{
	// ...
	while (1)
	{
		char *hostName = new char[500], *cwd = new char[500];
		string input;
		DIR *d;
		size_t t;

		signal(SIGINT, sigintHandler);
		{
			//--------------------------------------------------------------//
			//					PART (A)
			//--------------------------------------------------------------//

			//--------------------------------------------------------------//
			//		Current Login Name,HostName and Directory ;)
			//--------------------------------------------------------------//

			getcwd(cwd, 500);
			gethostname(hostName, 500);
			printf("_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_- \n");
			cout << getpwuid(getuid())->pw_name << "@" << hostName << cwd << endl;
			printf("_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_- \n");

			//--------------------------------------------------------------//
			getline(cin, input);
			string first = input.substr(0, input.find(' '));
			input.erase(0, input.find(' ') + 1);
			if (first == "exit")
			{
				printf("---------------- Exiting Shell --------------- \n");
				exit(10);
			}
			else if (first == "pwd")
			{
				printf("Current Working Directory : ");
				cout << getpwuid(getuid())->pw_dir << endl;
			}
			else if (first == "clear")
			{
				system("clear");
			}

			//--------------------------------------------------------------//
			//					P A R T ( B )
			//--------------------------------------------------------------//

			else
			{

				//--------------------------------------------------------------//
				//					"  ls  "
				//--------------------------------------------------------------//

				if (first == "ls")
				{
					string directory = input.substr(0, input.find('\0'));
					DIR *d;
					struct dirent *dir;
					d = opendir(directory.c_str());
					int file, current_output;
					bool check = false;
					if (input[0] == '>')
					{
						input.erase(0, 2);
						check = true;
						cout << input.c_str() << endl;
						file = open(input.c_str(), O_CREAT | O_WRONLY, 0777);
						current_output = dup(1);
						dup2(file, 1);
					}
					//cout << "directory" << directory.c_str() << endl;
					if (d)
					{
						while ((dir = readdir(d)) != NULL)
						{
							printf("%s\n", dir->d_name);
						}
						closedir(d);
					}
					else
					{
						printf("Error in opening directory \n");
					}
					if (check == true)
					{
						dup2(current_output, 1);
					}
				}
				//--------------------------------------------------------------//
				//			B->	( C D)
				//--------------------------------------------------------------//
				else if (first == "cd")
				{
					string directory = input.substr(0, input.find('\0'));
					int file, current_output;
					bool check = false;
					if (input[0] == '>')
					{
						input.erase(0, 2);
						check = true;
						cout << input.c_str() << endl;
						file = open(input.c_str(), O_CREAT | O_WRONLY, 0777);
						current_output = dup(1);
						dup2(file, 1);
					}
					if (chdir(directory.c_str()) == 0)
					{
						cout << "Current DIrectory changed from { " << cwd << " } to { ";
						getcwd(cwd, 500);
						cout << cwd << " } \n";
					}
					else
					{
						cout << "Directory not found \n";
					}
					if (check == true)
					{
						dup2(current_output, 1);
					}
				}

				//--------------------------------------------------------------//

				//--------------------------------------------------------------//
				//      	C->> (Environment Variables)
				//--------------------------------------------------------------//

				//--------------------------------------------------------------//

				else if (first == "environ")
				{
					int file, current_output;
					bool check = false;
					if (input[0] == '>')
					{
						input.erase(0, 2);
						check = true;
						cout << input.c_str() << endl;
						file = open(input.c_str(), O_CREAT | O_WRONLY, 0777);
						current_output = dup(1);
						dup2(file, 1);
					}
					for (short i = 0; envp[i] != 0; i++)
					{
						cout << (envp[i]) << endl;
					}
					if (check == true)
					{
						dup2(current_output, 1);
					}
				}

				else if (first == "setenv")
				{
					string envName = input.substr(0, input.find(' '));
					input.erase(0, input.find(' ') + 1);
					string NameToBE = "";
					NameToBE = input.substr(0, input.find(' '));
					cout << envName << " " << NameToBE << endl;
					input.erase(0, input.find(' ') + 1);
					if (setenv(envName.c_str(), NameToBE.c_str(), true) == 0)
					{
						cout << "Environment variable name changed from { " << envName << " } to { " << NameToBE << " } successfully \n";
					}
					else
					{
						cout << "Could not find the environment Variable -> " << envName << " \n";
					}
				}
				else if (first == "unsetenv")
				{
					string envName = input.substr(0, input.find(' '));
					input.erase(0, input.find(' ') + 1);
					if (unsetenv(envName.c_str()) == 0)
					{
						cout << "Environment variable { " << envName << " } removed successfully \n";
					}
					else
					{
						cout << "Could not find the environment Variable -> " << envName << " \n";
					}
				}
				//--------------------------------------------------------------//

				else if (first == "exit")
				{
					exit(0);
				}
				//--------------------------------------------------------------//

				//--------------------------------------------------------------//
				//      	Part D && Part F->> (FORK)
				//--------------------------------------------------------------//
				else if (first[first.length() - 1] == '&')
				{
					//   "Part "F"
					EXEC(first, input, 0); // Function definition is above main
				}
				else
				{

					bool checkFlag = false;
					for (int i = 0; i < input.length() && !checkFlag; i++)
					{
						if (input[i] == '<' || input[i] == '>')
						{
							checkFlag = true;
						}
					}
					if (!checkFlag)
					{
						//   "Part "D"
						EXEC(first, input, 1);
					}
					else
					{
						//--------------------------------------------------------------//
						// 					Part (E)		(I/O Redirection)
						//--------------------------------------------------------------//
						if (input[0] == '>')
						{
							int current_out = dup(1);
							int count = 2;
							int pid = fork();
							if (pid == 0)
							{
								for (int i = 0; input[i] != '\0'; i++)
								{
									if (input[i] == ' ')
									{
										count++;
									}
								}

								char **buffer = new char *[count + 1];
								buffer[0] = new char(first.length());

								strcpy(buffer[0], first.c_str());
								short miniCheck = 1;
								string outputFile = input.substr(input.find('>') + 2, input.find('\0'));
								cout << outputFile << endl;
								int file = open(outputFile.c_str(), O_CREAT | O_WRONLY, 0777);
								dup2(file, 1);
								for (int a = 1; a < count && miniCheck >= 0; a++)
								{

									if (input[0] == '>')
									{
										input.erase(0, 2);
										miniCheck =-1;
										count=a;
										buffer[a]=NULL;
										break;
									}
									else
									{

										buffer[a] = new char[input.substr(0, input.find(' ')).length()];
										strcpy(buffer[a], input.substr(0, input.find(' ')).c_str());
										input.erase(0, input.find(' ') + 1);
									}
								}
								

								buffer[count] = NULL;
								execvp(buffer[0], buffer);
								perror("exec failed");
							}
							else
							{

								int status;
								(void)waitpid(pid, &status, 0);
							}
						}

						else
						{

							int current_out = dup(1);
							int count = 2;
							int pid = fork();
							if (pid == 0)
							{
								for (int i = 0; input[i] != '\0'; i++)
								{
									if (input[i] == ' ')
									{
										count++;
									}
								}

								char **buffer = new char *[count + 1];
								buffer[0] = new char(first.length());

								strcpy(buffer[0], first.c_str());
								short miniCheck = 1;
								string outputFile = input.substr(input.find('>') + 2, input.find('\0'));
								cout << outputFile << endl;
								int file = open(outputFile.c_str(), O_CREAT | O_WRONLY, 0777);
								dup2(file, 1);
								for (int a = 1; a < count && miniCheck >= 0; a++)
								{
									if (input[0] == '<')
									{
										input.erase(0, 2);
										miniCheck--;
									}
									if (miniCheck == 0)
									{
										miniCheck--;
									}
									buffer[a] = new char[input.substr(0, input.find(' ')).length()];
									strcpy(buffer[a], input.substr(0, input.find(' ')).c_str());
									input.erase(0, input.find(' ') + 1);
								}

								buffer[count] = NULL;
								execvp(buffer[0], buffer);
								perror("exec failed");
							}
							else
							{

								int status;
								(void)waitpid(pid, &status, 0);
							}
						}
					}
				}

				//--------------------------------------------------------------//
			}
		}

		// ...
	}
}