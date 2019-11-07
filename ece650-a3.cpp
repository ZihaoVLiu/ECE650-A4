#include<vector>
#include <unistd.h>
#include <signal.h>
#include <iostream>


int rgen(){
    char * argvrgen[2] = {"rgen", NULL}; // argument for rgen.cpp.
    return execv ("rgen", argvrgen);
}

int a1(){
    char * argva1[3] = {"python", "ece650-a1.py", NULL}; // argument for a1.py.
    return execvp("python", argva1);
}

int a2(){
    char * argva2[2] = {"ece650-a2", NULL}; // argument for a2.py.
    return execv("ece650-a2",argva2);
}

int Input(){
    // Process writing to ece650-a2.
    // read a line of input until EOL and store in a string
    while (!std::cin.eof()) {
        std::string line;
        std::getline(std::cin, line);
    }
    return 0;
}

int opt(int s, int n, int l, int c){
    std::cout << "s=" << s
              << " n=" << n
              << " l=" << l
              << " c=" << c << " " << std::endl;
}

int main(int argc, char **argv)
{
    // command line argument define.
    std::string sValue;
    int s = 10;
    std::string nValue;
    int n = 5;
    std::string lValue;
    int l = 5;
    std::string cValue;
    int c = 20;
    int index;
    int command;

    opterr = 0;

    // expected options are '-s', '-n', '-l' and '-c' values.
    while ((command = getopt (argc, argv, "s:n:l:c:")) != -1)
        switch (command)
        {
            case 's':
                sValue = optarg;
                s = atoi(sValue.c_str());
                break;
            case 'n':
                nValue = optarg;
                n = atoi(nValue.c_str());
                break;
            case 'l':
                lValue = optarg;
                l = atoi(lValue.c_str());
                break;
            case 'c':
                cValue = optarg;
                c =atoi(cValue.c_str());
                break;
            default:
                return 0;
        }

    // pipe starts here.
    std::vector<pid_t> kids;
    pid_t child_pid;

    // create a pipe between Input and rgen.
    int OtoR[2];
    pipe(OtoR);
    child_pid = fork ();
    if (child_pid == 0)
    {
        // redirect the Input stdout to the pipe
        dup2(OtoR[1], STDOUT_FILENO);
        close(OtoR[0]);
        close(OtoR[1]);

        // start process A
        return opt(s, n, l, c);
    }
    else if (child_pid < 0) {
        std::cerr << "Error: could not fork\n";
        return 1;
    }
    kids.push_back(child_pid);


    int pipe2[2];
    pipe(pipe2);
    child_pid = fork();
    if (child_pid == 0)
    {
        // redirect stdin from the pipe
        dup2(OtoR[0], STDIN_FILENO);
        close(OtoR[1]);
        close(OtoR[0]);
        dup2(pipe2[1], STDOUT_FILENO);
        close(pipe2[0]);
        close(pipe2[1]);
        return rgen();
    }
    else if (child_pid < 0) {
        std::cerr << "Error: could not fork\n";
        return 1;
    }
    kids.push_back(child_pid);


    int pipe3[2];
    pipe(pipe3);
    child_pid = fork();
    if (child_pid == 0)
    {
        dup2(pipe2[0], STDIN_FILENO);
        close(pipe2[1]);
        close(pipe2[0]);
        dup2(pipe3[1], STDOUT_FILENO);
        close(pipe3[0]);
        close(pipe3[1]);
        return a1();
    }
    else if (child_pid < 0) {
        std::cerr << "Error: could not fork\n";
        return 1;
    }
    kids.push_back(child_pid);


    child_pid = fork();
    if (child_pid == 0)
    {
        dup2(pipe3[0], STDIN_FILENO);
        close(pipe3[1]);
        close(pipe3[0]);
        return a2();
    }
    else if (child_pid < 0) {
        std::cerr << "Error: could not fork\n";
        return 1;
    }
    kids.push_back(child_pid);

    int res = Input();
    // send kill signal to all children
    for (pid_t k : kids) {
        int status;
        kill (k, SIGTERM);
        waitpid(k, &status, 0);
    }

    return res;
}