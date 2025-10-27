#include <bits/stdc++.h>

using namespace std;

void bankersAlgorithm(vector<vector<int>> allocation, vector<vector<int>> maxNeed, vector<int> available, int n, int m, vector<int> total) {
    vector<vector<int>> need(n, vector<int>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            need[i][j] = maxNeed[i][j] - allocation[i][j];
        }
    }

    vector<bool> finish(n, false);
    vector<int> safeSeq;

    for (int count = 0; count < n; ) {
        bool found = false;
        for (int p = 0; p < n; p++) {
            if (!finish[p]) {
                int j;
                for (j = 0; j < m; j++)
                    if (need[p][j] > available[j])
                        break;

                if (j == m) {
                    for (int k = 0; k < m; k++)
                        available[k] += allocation[p][k];

                    safeSeq.push_back(p);
                    finish[p] = true;
                    found = true;
                    count++;
                }
            }
        }

        if (!found) {
            cout << "System is not in safe state\n";
            return;
        }
    }

    cout << "System is in safe state.\nSafe sequence is: ";
    for (int i = 0; i < safeSeq.size(); i++) cout << safeSeq[i] << " ";
    cout << endl;
}

int main(){
    int n = 5; // Number of processes
    int m = 3; // Number of resources

    vector<vector<int>> allocation = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };
    vector<vector<int>> maxNeed = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };

    vector<int> total = {10, 5, 7};

    vector<int> available(m);
    
    for (int j = 0; j < m; j++) {
        int sum = 0;
        for (int i = 0; i < n; i++) {
            sum += allocation[i][j];
        }
        available[j] = total[j] - sum;
    }

    bankersAlgorithm(allocation, maxNeed, available, n, m, total);

    return 0;
}