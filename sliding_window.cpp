#include <iostream>
using namespace std;
int main() {
    cout << "Sender side" <<endl;

    int ws, fs;
    cout << "Enter window size: ";
    cin >> ws;
    cout << "Enter framesize: ";
    cin >> fs;

    for (int i = 0; i < ws; ++i) {
        cout << "send: " << i <<endl;
    }

    int k = ws;
    for (int j = 0; j <= fs; ++j) {
        cout << "ack " << j << " is received" << endl;
        if (k <= fs) {
            cout << "send " << k <<endl;
            k++;
        }
    }
    cout << "connection terminated" <<endl;

    cout << "Receiver side" << endl;
    for (int i = 0; i <= fs; ++i) {
        cout << "message received " << i <<endl;
        cout << "ack " << i << " sent" <<endl;
    }
    cout << "connection terminated" <<endl;

    return 0;
}

