#include <iostream>
#include "utility.h"

using namespace std;

int main() {
    cout << Utility::convertTime(8515) << endl;
    cout << Utility::getProgressBar("57") << endl;
    return 0;
}

