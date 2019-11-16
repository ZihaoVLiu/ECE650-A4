#include<vector>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <stdlib.h>

// execute the rgen to get random streets.
int rgen(int argc, char * argv[]){
    argv[0] = (char*) "rgen";
    return execv ("rgen", argv);
}
// execute ece650-a1.py to generate vertex and edge.
int a1(){
    char * argva1[3] = {"python", "ece650-a1.py", NULL}; // argument for a1.py.
    return execvp("python", argva1);
}
// execute ece650-a2.cpp to compute the shortest path.
int a2(){
    char * argva2[2] = {"ece650-a2", NULL}; // argument for a2.py.
    return execv("ece650-a2",argva2);
}
// cin the command s (the shortest path)
int Input(){
    // Process writing to ece650-a2.
    // read a line of input until EOL and store in a string
    while (!std::cin.eof()) {
        std::string line;
        std::getline(std::cin, line);
        std::cout << line << std::endl;
    }
    return 0;
}


int main(int argc, char** argv)
{

    std::vector<pid_t> kids;
    pid_t child_pid;

    // create a pipe between a3 and rgen.
    int OtoR[2];
    pipe(OtoR);
    child_pid = fork();
    if (child_pid == 0)
    {
        // redirect the command line argument to the of in of first pipe.
        dup2(OtoR[1], STDOUT_FILENO);
        close(OtoR[0]);
        close(OtoR[1]);
        return 0;
    }
    else if (child_pid < 0) {
        std::cerr << "Error: could not fork\n";
        return 1;
    }
    kids.push_back(child_pid);

    // pipe between rgen and a1
    int pipe2[2];
    pipe(pipe2);
    child_pid = fork();
    if (child_pid == 0)
    {
        // redirect command line argument from pipe to the rgen.
        dup2(OtoR[0], STDIN_FILENO);
        close(OtoR[1]);
        close(OtoR[0]);
        // refirect the output of rgen to the second pipe.
        dup2(pipe2[1], STDOUT_FILENO);
        close(pipe2[0]);
        close(pipe2[1]);
        return rgen(argc, argv);
        //return execv("rgen", spec);
    }
    else if (child_pid < 0) {
        std::cerr << "Error: could not fork\n";
        return 1;
    }
    kids.push_back(child_pid);

    // create the pipe between a1 and a2.
    int pipe3[2];
    pipe(pipe3);
    child_pid = fork();
    if (child_pid == 0)
    {
        // redirect the rgen output from pipe to the python script.
        dup2(pipe2[0], STDIN_FILENO);
        close(pipe2[1]);
        close(pipe2[0]);
        // redirect the output of python to the third pipe.
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
        // redirect the output of python from pipe to a2.cpp.
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

    // redirect the Input value of this script to the third pipe.
    dup2(pipe3[1], STDOUT_FILENO);
    close(pipe3[0]);
    close(pipe3[1]);
    int res = Input();

    // send kill signal to all children to avoid error.
    for (pid_t k : kids) {
        int status;
        kill (k, SIGTERM);
        waitpid(k, &status, 0);
    }

    return res;
}