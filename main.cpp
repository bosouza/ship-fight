#include <string>
#include <vector>
#include <sstream> //istringstream
#include <iostream> // cout
#include <fstream> // ifstream
 
using namespace std;
 
vector<vector<int>> tilesScenario;

/**
 * Reads csv file into table, exported as a vector of vector of ints.
 * @param inputFileName input file name (full path).
 * @return data as vector of vector of ints.
 */
vector<vector<int>> parse2DCsvFile(string inputFileName);
int getTile(unsigned int line, unsigned int column, unsigned int depth);
int getTileXYZ(unsigned int x, unsigned int y, unsigned int z);

int main()
{
    tilesScenario = parse2DCsvFile("config/avalon-main-map.csv");
 
    // for (auto l : tilesScenario) {
    //     for (auto x : l){
    //         cout << x << " ";
    //     }       
    //     cout << endl;
    // }
 
    cout << getTile(2,3,1) << endl;
    cout << getTile(2,4,2) << endl;

    cout << getTileXYZ(0,0,0) << endl;
    cout << getTileXYZ(1,1,0) << endl;
    cout << getTileXYZ(2,2,0) << endl;
    
    return 0;
}

vector<vector<int>> parse2DCsvFile(string inputFileName) {
    vector<vector<int> > data;
    ifstream inputFile(inputFileName);
    int l = 0;
    while (inputFile) {
        l++;
        string s;
        if (!getline(inputFile, s)) break;
        if (s[0] != '#') {
            istringstream ss(s);
            vector<int> record;
            while (ss) {
                string line;
                if (!getline(ss, line, ','))
                    break;
                try {
                    record.push_back(stof(line));
                }
                catch (const std::invalid_argument e) {
                    cout << "Not a Number found in file " << inputFileName << " line " << l
                         << endl;
                    e.what();
                }
            }
            data.push_back(record);
        }
    }
    if (!inputFile.eof()) {
        cerr << "Could not read file " << inputFileName << "\n";
        //__throw_invalid_argument("File not found.");
    }
    return data;
}

int getTile(unsigned int line, unsigned int column, unsigned int depth){
    int tile = tilesScenario[line][column];

    if ((tile==0 || tile==27 || tile==54 || tile==81 || tile==108 || tile==135 || tile==162 || tile==189 || tile==216 || tile==243 || tile==270 || tile==297 || tile==324 || tile==351 || tile==378 || tile==405 || tile==432 || tile==459 || tile==486 || tile==513 || tile==540 || tile==567 || tile==594 || tile==621) && depth>0 && depth<5) 
    return tile+depth;
    else 
    return tile;
}

int getTileXYZ(unsigned int x, unsigned int y, unsigned int z){
    int line = (tilesScenario.size() - 1) - x;
    return getTile(line,y,z);
}