/* #include "random_number.h"

***/

/*
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <cstdio>

void writer (const char * message, int count, FILE * stream){
    for (;count > 0; --count){
        fprintf (stream, "%s\n", message);
        fflush (stream);
        sleep(1);
    }
}

void reader (FILE * stream){
    char buffer[1024];
    while(!feof (stream) && !ferror(stream) && fgets(buffer, sizeof (buffer), stream) != NULL)
        fputs (buffer, stdout);
}
int main(){
    int fds[2];
    pid_t pid;

    pipe(fds);
    pid = fork();
    if (pid == (pid_t) 0) {
        FILE * stream;
        close(fds[1]);
        stream = fdopen(fds[0], "r");
        reader(stream);
        close(fds[0]);
    }
    else{
        FILE * stream;
        close(fds[0]);
        stream = fdopen(fds[1], "w");
        writer("wocaonima", 5, stream);
        close(fds[1]);
    }
    return 0;
}
 */


#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

// s is the street number
// n is the number of line-segments in each street
// l is the waiting time before generating next random input
// c is the coordinates values
int s, n, l, c=2;
int smin = 2;
int nmin = 1;
int lmin = 5;
int cmin = -c;
int sDefault = 10;
int nDefault = 5;
int lDefault = 5;
int cDefault = 20;
int streetNumber;

// build a string array to store street name.
string streetName[676];
// build a array to store the number of segment line of each street.
int segNumbers[676];
// build a 2D array to store the coordinates value of each street.
int coords[676][200];


// define a random function to output positive random value.
int randV(int kmin, int kmax){
    // open /dev/urandom to read
    ifstream urandom("/dev/urandom");
    // check whether it fail
    if (urandom.fail()) {
        cerr << "Error: Cannot open /dev/urandom" << endl;
        return 1;
    }
    // read a random 8-bit value.
    // Have to use read() method for low-level reading
    char ch = 'a';
    while(true){
        urandom.read(&ch, 1);
        if (kmin-1 < (unsigned int)ch && (unsigned int)ch < kmax+1) {
            break;
        }
    }
    // can be deleted afterall
    cout << "Random character is: " << (unsigned int)ch << endl;
    // close random stream
    urandom.close();
    return (unsigned int)ch;
}

// define a random function to output positive random value.
int randC(int c){
    // open /dev/urandom to read
    ifstream urandom("/dev/urandom");
    // check whether it fail
    if (urandom.fail()) {
        cerr << "Error: Cannot open /dev/urandom" << endl;
        return 1;
    }
    // read a random 8-bit value.
    // Have to use read() method for low-level reading
    char ch = 'a';
    while(true){
        urandom.read(&ch, 1);
        if (-c - 1 < ch && ch < c + 1) {
            break;
        }
    }
    // can be deleted afterall
    cout << "Random coordinate is: " << (unsigned int)ch << endl;
    // close random stream
    urandom.close();
    return ch;
}

// define a function to get the number of street.
int getStreetNumber(){
    if (s == 0){
        streetNumber = randV(smin, sDefault);
    } else streetNumber = randV(smin, s);
    return streetNumber;
}

// define a function to get pointed number of street name.
int getStreetName(int nNumber){
    int sort = 0;
    char first;
    char second;
    char initial = 'a';
    string value1, value2;
    // generate the name of street.
    for (int j = 0; j < 26; ++j) {
        first = initial + j;
        for (int i = 0; i < 26; ++i) {
            second = initial + i;
            value1 = first;
            value2 = second;
            streetName[sort] = (value1 + value2);
            if(sort == (nNumber)) {
                break;
            }
            sort =  sort + 1;
        }
    }
    // can be deleted afterall
    for (int k = 0; k < nNumber; ++k) {
        cout << streetName[k] << endl;
    }
    return 0;
}

// define a function to get the number of segment line of each street.
int getSegNumber(int nNumber){
    for (int i = 0; i < nNumber; ++i) {
        int segNumber;
        if (n == 0){
            segNumber = randV(nmin, nDefault);
        } else segNumber = randV(nmin, n);
        segNumbers[i] = segNumber;
    }
    return 0;
}

// define a function to get the waiting time.
int getWaitTime(){
    int waitTime;
    if (l == 0){
        waitTime = 5;
    } else waitTime = randV(lmin, l);
    return waitTime;
}

int getCoords(int nNumber){
    int segNumber;
    cout << "cdefault is: " << cDefault << endl;
    for (int i = 0; i < nNumber; ++i) {
        segNumber = segNumbers[i];
        for (int j = 0; j < (2 * (segNumber+1)); ++j) {
            if (c == 0){
                coords[i][j] = randC(cDefault);
            } else coords[i][j] = randC(c);
        }
    }
    return 0;
}

int detectOverlop(){
    int tempCoord[4];
    int segNumber;
    // detect same node exisit in each street.
    for (int i = 0; i < streetNumber; ++i) {
        segNumber = segNumbers[i];
        for (int j = 0; j < (2 * (segNumber + 1)); ++j) {
            tempCoord[0] = coords[i][j];
            tempCoord[1] = coords[i][j+1];
            if (j != (2 * (segNumber + 1))) {
                for (int k = (j + 2); k < (2 * (segNumber + 1)); ++k) {
                    if (tempCoord[0] == coords[i][k] && tempCoord[1] == coords[i][k + 1]) {
                        // can be deleted afterall
                        cout << "the valus is: " << i << endl;
                        return -1;
                    }
                    k = k + 1;
                }
            }
            j = j + 1;
        }
    }

    // detect overlap segment among all of the street.
    for (int m = 0; m < streetNumber; ++m) {
        segNumber = segNumbers[m];
        int nodeNumber = (2 * (segNumber + 1));
        for (int i = 0; i < nodeNumber; ++i) {
            tempCoord[0] = coords[m][i];
            tempCoord[1] = coords[m][i+1];
            tempCoord[2] = coords[m][i+2];
            tempCoord[3] = coords[m][i+3];
            if (i < nodeNumber - 3){
                for (int j = (m + 1); j < streetNumber; ++j) {
                    for (int k = 0; k < (2 * (segNumbers[j] + 1)); ++k) {
                        if (tempCoord[0] == coords[j][k] && tempCoord[1] == coords[j][k+1]
                        && tempCoord[2] == coords[j][k+2] && tempCoord[3] == coords[j][k+3]){
                            cerr << "the lines are " << m << " and " << j << endl;
                            cout << endl;
                            return -1;
                        }
                        k = k + 1;
                    }
                }
            }
            i = i + 1;
        }
    }
    return 0;
}

int main(){
    int countNumber = 0;
    while (true) {
        getStreetNumber();
        getStreetName(streetNumber);
        getSegNumber(streetNumber);
        // can be deleted afterall.
        cout << "waiting time is: " << getWaitTime() << endl;
        getCoords(streetNumber);
        // for loop can be deleted afterall
        for (int i = 0; i < streetNumber; ++i) {
            int segNumber = segNumbers[i];
            for (int j = 0; j < (2 * (segNumber + 1)); ++j) {
                cout << coords[i][j] << " ";
            }
            cout << endl;
        }
        // can be deleted afterall but remember to add the detectOverlop.
        if (detectOverlop() == -1) {
            countNumber = 1 + countNumber;
            cerr << "Error: the generated output is invalid." << endl;
            if (countNumber == 25) {
                cerr << "Error: Failed to generate valid input for 25 simultaneous attempts." << endl;
                exit(0);
            }
        }
        if (detectOverlop() == 0) sleep(getWaitTime());
    }
}