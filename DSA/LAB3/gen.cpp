#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cstdlib>
#include <ctime>

using namespace std;

string filename;
int insertOperations;
int queryOperations;
bool alternating;
ofstream outfile("data.in");

struct UserData
{
    string id;
    int rank;
};

void readPojData(const string &filename, vector<UserData> &data)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }

    string id;
    int rank, d1, d2;
    while (file >> d1 >> id >> rank >> d2)
        data.push_back({id, rank});

    file.close();
}

string generateRandomString(const string &base)
{
    string randomString = base;
    randomString += "_";
    randomString += to_string(rand() % 1000); // 添加随机数字
    return randomString;
}

void generateTestData(const vector<UserData> &data)
{
    unordered_map<string, int> hashTable;
    srand(time(0));

    vector<string> keys;
    int insertCounter = 0;
    int queryCounter = 0;
    if (alternating)
    {
        while (insertCounter < insertOperations || queryCounter < queryOperations)
        {
            bool op = rand() % 2;
            if (op && (insertCounter < insertOperations))
            {
                int index;
                do
                    index = rand() % data.size();
                while (hashTable.find(data[index].id) != hashTable.end());

                const UserData &user = data[index];
                outfile << "0 " << user.id << " " << user.rank << endl;
                hashTable[user.id] = user.rank;
                keys.push_back(user.id);
                insertCounter++;
            }

            if (!op && (queryCounter < queryOperations))
            {
                int index = rand() % keys.size();
                const string &key = keys[index];
                if (rand() % 3 == 0)
                    outfile << "1 " << generateRandomString(key) << endl; // 查询不存在的字符串，但与key有naive哈希冲突
                else
                    outfile << "1 " << key << endl; // 查询存在的字符串
                queryCounter++;
            }
        }
    }
    else
    {
        for (int i = 0; i < insertOperations; i++)
        {
            int index;
            do
                index = rand() % data.size();
            while (hashTable.find(data[index].id) != hashTable.end());

            const UserData &user = data[index];
            outfile << "0 " << user.id << " " << user.rank << endl;
            hashTable[user.id] = user.rank;
            keys.push_back(user.id);
        }
        for (int i = 0; i < queryOperations; i++)
        {
            int index = rand() % keys.size();
            const string &key = keys[index];
            if (rand() % 3 == 0)
                outfile << "1 " << generateRandomString(key) << endl; // 查询不存在的字符串
            else
                outfile << "1 " << key << endl; // 查询存在的字符串
        }
    }
    // End of input
    outfile << "2" << endl;
}

int main(int argc, char *argv[])
{
    insertOperations = atoi(argv[1]);
    queryOperations = atoi(argv[2]);
    alternating = atoi(argv[3]);

    vector<UserData> data;
    readPojData("poj.txt", data);

    generateTestData(data);

    outfile.close();
    return 0;
}