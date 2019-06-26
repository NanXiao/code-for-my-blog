#include <unistd.h>
#include <vector>
#include <iostream>

using namespace std;

int main(void) {
    vector<vector<char>> vec(1024 * 1024, vector<char>(1024));

    cout << "Before freeing memory, sleep 30 seconds ..." << endl;
    sleep(30);
    vector<vector<char>>().swap(vec);

    cout << "Sleep now ..." << endl;
    sleep(300);
    return 0;
}
