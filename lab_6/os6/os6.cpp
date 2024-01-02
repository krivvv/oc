#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <string>
using namespace std;

vector<char> globalArray;  
mutex mtx1,mtx2, mtx3, mtx4;
condition_variable_any startWorkCondition;
condition_variable_any readytoOutArray;
condition_variable_any readytoCount;
condition_variable_any resultReadyCondition;
bool b1 = false, b2 = false;
int count1 = 0;
void workThread() {
    {
        unique_lock<mutex> lock(mtx1);
        startWorkCondition.wait(lock);
    }
    int restInterval;
    cout << "Enter rest interval (in milliseconds): ";
    cin >> restInterval;
    vector<char> tempArray(globalArray.size(), '_');
    for (size_t i = 0; i < globalArray.size(); ++i) {
        if (isdigit(globalArray[i])) {
            tempArray[i] = globalArray[i];
            i++;
            while (i < globalArray.size() && globalArray[i] != ' ' && globalArray[i] != '-' ) {
                tempArray[i] = globalArray[i];
                i++;
            }
            
        }
    }
 /*   cout << "**********************" << endl;*/
    for (int i = 0; i < globalArray.size(); i++) {
        globalArray[i] = tempArray[i];
 /*       cout << globalArray[i] << ' ';*/
    }
    //cout << "**********************" << endl;
    this_thread::sleep_for(chrono::milliseconds(restInterval));
    /*readytoOutArray.notify_one();*/
    readytoCount.notify_one();
    b1 = true;
}

void countElementThread() {
    {
        unique_lock<mutex> lock1(mtx2);
        readytoCount.wait(lock1);
    }
    /*cout << "------------------------" << endl << "count elems working with" << endl;
    for (char ch : globalArray) {
        cout << ch << ' ';
    }
    cout << "----------------------------------" << endl;*/
    for (char ch : globalArray) {
        if (ch != '_') {
            count1++;
        }
    }
    b2 = true;
    /*cout << "CountElement: Count of elements in the array: " << count1 << std::endl;*/
}

int main() {
    size_t arraySize;
    cout << "Enter array size: ";
    cin >> arraySize;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    globalArray.resize(arraySize);
    cout << "Enter array elements: ";
    string input;
    getline(cin, input);
    for (size_t i = 0; i < arraySize; ++i) {
        globalArray[i] = input[i];
    }

    cout << "Original array: ";
    for (char ch : globalArray) {
        cout << ch << " ";
    }
    cout << endl;
    thread work(workThread);
    thread countElement(countElementThread);
    this_thread::sleep_for(chrono::milliseconds(100));
    startWorkCondition.notify_one();
    work.join();
    countElement.join();
    
        unique_lock<mutex> lock3(mtx3);
        

    while (!b1) {
        lock3.lock();
        this_thread::sleep_for(chrono::milliseconds(300));
        lock3.unlock();
    }
    cout << "Final Array: ";
    for (int i = 0; i < globalArray.size(); i++) {
        cout << globalArray[i] << ' ';
    }
    unique_lock<mutex> lock4(mtx4);
    while (!b2) {
        lock4.lock();
        this_thread::sleep_for(chrono::milliseconds(300));
        lock4.unlock();
    }
    cout << endl  << "Count of elems: " << count1 << endl;
    return 0;
}
