/* #include "random_number.h"


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
#include <algorithm>
using namespace std;

// s is the street number
// n is the number of line-segments in each street
// l is the waiting time before generating next random input
// c is the coordinates values
int s, n, l, c;
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

// define a function to initialize coordinate of each street.
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

// these two functions are used to detect if crosspoints exist. -1 means there is no intersection between two segments.
float crossPoint[2] = {0,0};
int CPinSegment (int line1[4], int line2[4]){
    // detect line is vertical
    if (line1[0] == line1[2]){
        if (crossPoint[1] >= min(line1[1],line1[3]) && crossPoint[1] <= max(line1[1],line1[3])){
            if (crossPoint[0] >= min(line2[0], line2[2]) && crossPoint[0] <= max(line2[0], line2[2])) return 0;
        }
    }
    // detect line is horizontal
    if (line1[1] == line1[3]){
        if (crossPoint[0] >= min(line1[0], line1[2]) && crossPoint[0] <= max(line1[0], line1[2])){
            if (crossPoint[1] >= min(line2[1], line2[3]) && crossPoint[1] <= max(line2[1], line2[3])) return 0;
        }
    }

    if (crossPoint[0] >= min(line1[0], line1[2]) && crossPoint[0] <= max(line1[0], line1[2])){
        if (crossPoint[1] >= min(line2[1], line2[3]) && crossPoint[1] <= max(line2[1], line2[3]) &&
        crossPoint[0] >= min(line2[0], line2[2]) && crossPoint[0] <= max(line2[0], line2[2])) return 0;
    }
    else return -1;
}
int getCrossPoint(int line1[4], int line2[4]){
    float a1, a2, b1, b2, c1, c2;
    float d;
    a1 = line1[1] - line1[3];
    b1 = line1[2] - line1[0];
    c1 = line1[0] * line1[3] - line1[2] * line1[1];
    a2 = line2[1] - line2[3];
    b2 = line2[2] - line2[0];
    c2 = line2[0] * line2[3] - line2[2] * line2[1];
    d = a1 * b2 - a2 * b1;
    if (d == 0) return -1; //two lines are horizontal
    else {
        crossPoint[0] = ((b1 * c2 - b2 * c1) / d);
        crossPoint[1] = ((c1 * a2 - c2 * a1) / d);
    }
    if (CPinSegment(line1, line2) == 0) return 0;
    else {
        return -1;
    }
}

// define a function to detect street is a sub street. 0 means this is a sub street.
int getSub(int line1[4], int line2[4]){
    bool l10 = line1[0] >= min(line2[0],line1[2]) && line1[0] <= max(line2[0],line2[2]);
    bool l12 = line1[2] >= min(line2[0],line1[2]) && line1[2] <= max(line2[0],line2[2]);
    bool l11 = line1[1] >= min(line2[1],line1[3]) && line1[1] <= max(line2[1],line2[3]);
    bool l13 = line1[3] >= min(line2[1],line1[3]) && line1[3] <= max(line2[1],line2[3]);
    if (line1[0] - line1[2] == 0 || line2[0] - line2[2] == 0){
        return 1;
    }
    float k1 = ((line1[1] - line1[3]) / (line1[0] - line1[2]));
    float b1 = (line1[1] - k1 * line1[0]);
    float k2 = ((line2[1] - line2[3]) / (line2[0] - line2[2]));
    float b2 = (line2[1] - k2 * line2[0]);
    if (l10 == true && l11 == true && l12 == true && l13 == true && k1 == k2 && b1 == b2) return 0;
    else return -1;
}

int crossPointInSegment(int line[4]){
    if (line[0] - line[2] != 0){
        float k = ((line[1] - line[3]) / (line[0] - line[2]));
        float b = (line[1] - k * line[0]);
        if (((crossPoint[1] - (k * crossPoint[0] + b)) < 0.0001 || (crossPoint[1] - (k * crossPoint[0] + b)) > 0.0001) &&
        crossPoint[0] >= min(line[0], line[2]) && crossPoint[0] <= max(line[0],line[2]) &&
        crossPoint[1] >= min(line[1], line[3]) && crossPoint[1] <= max(line[1],line[3]))
            return 0;
        else return -1;
    } else{
        if (crossPoint[0] == line[0] && crossPoint[1] >= min(line[1], line[3]) && crossPoint[1] <= max(line[1],line[3]))
            return 0;
        else return -1;
    }
}

// define a function to detect repeat and overlap problems.
int detectOverlap(){
    int tempCoord[4];
    int tempCoord2[4];
    int segNumber;

    // detect same node exist in each street.
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

    // detect crosspoint exist in same street.
    for (int i = 0; i < streetNumber; ++i) {
        segNumber = segNumbers[i];
        for (int j = 0; j < (2 * (segNumber + 1)); ++j) {
            tempCoord[0] = coords[i][j];
            tempCoord[1] = coords[i][j+1];
            tempCoord[2] = coords[i][j+2];
            tempCoord[3] = coords[i][j+3];
            if (j < (2 * (segNumber + 1)) - 3){
                for (int k = j + 4; k < (2 * (segNumber + 1)) ; ++k) {
                    if (k + 3 >= (2 * (segNumber + 1))) break;
                    tempCoord2[0] = coords[i][k];
                    tempCoord2[1] = coords[i][k+1];
                    tempCoord2[2] = coords[i][k+2];
                    tempCoord2[3] = coords[i][k+3];
                    if (getCrossPoint(tempCoord, tempCoord2) == 0) {
                        cerr << "There is intersection between segment in a street." << i << endl;
                        cerr << "the crosspoint is: " << crossPoint[0] << " " << crossPoint[1] << endl;
                        cerr << "seg1 " << tempCoord[0] << "," << tempCoord[1] << "  " << tempCoord[2] << "," << tempCoord[3] << endl;
                        cerr << "seg2 " << tempCoord2[0] << "," << tempCoord2[1] << "  " << tempCoord2[2] << "," << tempCoord2[3] << endl;
                        return -1;
                    }
                    k = k + 3;
                }
            }
            j = j + 1;
        }
    }

    // detect overlap segment among all of the street.
    bool intersection = false;
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
                        if (k + 3 >= (nodeNumber)) break;
                        tempCoord2[0] = coords[j][k];
                        tempCoord2[1] = coords[j][k+1];
                        tempCoord2[2] = coords[j][k+2];
                        tempCoord2[3] = coords[j][k+3];
                        if (getCrossPoint(tempCoord,tempCoord2) == 0) intersection = true;
                        if (tempCoord[0] == tempCoord2[0] && tempCoord[1] == tempCoord2[1]
                        && tempCoord[2] == tempCoord2[2] && tempCoord[3] == tempCoord2[3]){
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
        // detect one street is a sub street.
        if (segNumber == 1){
            tempCoord[0] = coords[m][0];
            tempCoord[1] = coords[m][1];
            tempCoord[2] = coords[m][2];
            tempCoord[3] = coords[m][3];
            for (int i = 0; i < streetNumber; ++i) {
                if (i == m) continue;
                for (int j = 0; j < (2 * (segNumbers[i] + 1)) - 3; ++j) {
                    if (j + 3 >= (2 * (segNumbers[i] + 1))) break;
                    tempCoord2[0] = coords[i][j];
                    tempCoord2[1] = coords[i][j+1];
                    tempCoord2[2] = coords[i][j+2];
                    tempCoord2[3] = coords[i][j+3];
                    j = j + 1;
                    if (getSub(tempCoord, tempCoord2) == 0){
                        cerr << "There is a complete overlap(this street is a sub street)" << endl;
                        cerr << "the substreet is " << m << endl;
                        cerr << "the street is " << i << endl;
                        cerr << tempCoord2[0] << tempCoord2[1] << tempCoord2[2] << tempCoord2[3] << endl;
                        return -1;
                    }
                    j = j + 1;
                }
            }
        }
    }
    if (intersection == true) return 0;
    else{
        cerr << "There is no intersection among these street." << endl;
        return -1;
    }
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
        if (detectOverlap() == -1) {
            countNumber = 1 + countNumber;
            if (countNumber == 100) {
                cerr << "Error: Failed to generate valid input for 100 simultaneous attempts." << endl;
                exit(0);
            }
        }
        cout << endl;
        if (detectOverlap() == 0) sleep(getWaitTime());
    }

    /*
    int line1[4] = {-5,1,6,-5};
    int line2[4] = {-19,-7,-12,-4};
    crossPoint[0] = 0;
    crossPoint[1] = 0;
    cout << getCrossPoint(line1,line2) << endl;
    cout << crossPoint[0] << crossPoint[1] << endl;
     */
}