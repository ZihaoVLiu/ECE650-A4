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

// define a function to detect repeat and overlap problems. -1 means invalid.
int detectOverlap(){
    int tempCoord[4];
    int tempCoord2[4];
    int segNumber;

    // detect loop.
    for (int j1 = 0; j1 < streetNumber; ++j1) {
        segNumber = segNumbers[j1];
        if (coords[j1][0] == coords[j1][2 * (segNumber + 1) - 2] && coords[j1][1] == coords[j1][2 * (segNumber + 1)-1])
            return -1;
    }


    // detect same node exist in each street.
    for (int i = 0; i < streetNumber; ++i) {
        segNumber = segNumbers[i];
        for (int j = 0; j < (2 * (segNumber + 1)); ++j) {
            tempCoord[0] = coords[i][j];
            tempCoord[1] = coords[i][j+1];
            if (j != (2 * (segNumber + 1))) {
                for (int k = (j + 2); k < (2 * (segNumber + 1)); ++k) {
                    if (tempCoord[0] == coords[i][k] && tempCoord[1] == coords[i][k + 1]) {
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
                        if (k + 3 >= (2 * (segNumbers[j] + 1))) break;
                        tempCoord2[0] = coords[j][k];
                        tempCoord2[1] = coords[j][k+1];
                        tempCoord2[2] = coords[j][k+2];
                        tempCoord2[3] = coords[j][k+3];
                        if (getCrossPoint(tempCoord,tempCoord2) == 0) intersection = true;
                        if (tempCoord[0] == tempCoord2[0] && tempCoord[1] == tempCoord2[1]
                            && tempCoord[2] == tempCoord2[2] && tempCoord[3] == tempCoord2[3]){
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
                        return -1;
                    }
                    j = j + 1;
                }
            }
        }
    }

    // detect no intersection among the street.
    bool DifferStInter = false;
    for (int i1 = 0; i1 < streetNumber; ++i1) {
        segNumber = segNumbers[i1];
        int nodeNumber = (2 * (segNumber + 1));
        for (int i = 0; i < nodeNumber; ++i) {
            tempCoord[0] = coords[i1][i];
            tempCoord[1] = coords[i1][i+1];
            tempCoord[2] = coords[i1][i+2];
            tempCoord[3] = coords[i1][i+3];
            for (int j = (i1 + 1); j < streetNumber; ++j) {
                for (int k = 0; k < (2 * (segNumbers[j] + 1)); ++k) {
                    if (k + 3 >= (2 * (segNumbers[j] + 1))) break;
                    tempCoord2[0] = coords[j][k];
                    tempCoord2[1] = coords[j][k+1];
                    tempCoord2[2] = coords[j][k+2];
                    tempCoord2[3] = coords[j][k+3];
                    if (getCrossPoint(tempCoord, tempCoord2) != -1)
                         DifferStInter = true;
                    k = k + 1;
                }
            }
            i = i + 1;
        }
    }

    if (intersection == true && DifferStInter == true) return 0;
    else{
        return -1;
    }
}

int array_opt[4];
int getopt() {
    string opt;
    int count = 0;
    int result;
    getline(cin, opt);
    for (int i = 0; i < opt.size(); ++i) {
        if (opt[i] == '=') {
            for (int j = i + 1; j < opt.size(); ++j) {
                if (opt[j] >= '0' && opt[j] <= '9') {
                    result = result * 10 + (opt[j] - 48);
                }
                if (opt[j] == ' ') {
                    array_opt[count] = result;
                    count = count + 1;
                    result = 0;
                    break;
                }
            }
        }
    }
}



int main1(int argc, char **argv)
{
    // command line argument define.
    std::string sValue;
    s = 10;
    std::string nValue;
    n = 5;
    std::string lValue;
    l = 5;
    std::string cValue;
    c = 20;
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


    int line1[4] = {0,0,2,2};
    int line2[4] = {0,1,0,5};

    int identity = 0;
    int countNumber = 0;
    while (true) {
        // remove the existed street for next loop.
        if (identity == 1){
            identity = 0;
            for (int i = 0; i < streetNumber; ++i) {
                cout << "r " << "\"" <<streetName[i] << "\"" << endl;
            }
        }
        getStreetNumber();
        getStreetName(streetNumber);
        getSegNumber(streetNumber);
        getCoords(streetNumber);

        if (detectOverlap() == 0) {
            identity = 1;
            // standard cout to be gathered by a1.
            // for loop can be deleted afterall
            for (int i = 0; i < streetNumber; ++i) {
                cout << "a \"" << streetName[i] << "\" ";
                int segNumber = segNumbers[i];
                for (int j = 0; j < (2 * (segNumber + 1)); ++j) {
                    if (j % 2 == 0) {
                        cout << "(";
                        cout << coords[i][j] << ",";
                    }
                    if (j % 2 == 1) {
                        cout << coords[i][j] << ") ";
                    }
                }
                cout << endl;
            }
            countNumber = 0; // if code can be executed. set the countnumber back to 0.
            cout << "g" << endl;
            sleep(getWaitTime());
        }


        if (detectOverlap() == -1) {
            countNumber = 1 + countNumber;
            if (countNumber == 25) {
                cerr << "Error: Failed to generate valid input for 25 simultaneous attempts." << endl;
                exit(0);
            }
        }
    }
}