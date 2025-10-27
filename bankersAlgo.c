#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

void bankersAlgorithm(int allocation[], int maxNeed[], int available[], int n, int m, int total[]) {
    int need[n * m];
    bool finish[n];
    int safeSeq[n];
    int count = 0;

    // Calculate Need matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            need[i * m + j] = maxNeed[i * m + j] - allocation[i * m + j];
        }
        finish[i] = false;
    }

    while (count < n) {
        bool found = false;
        for (int p = 0; p < n; p++) {
            if (!finish[p]) {
                int j;
                for (j = 0; j < m; j++) {
                    if (need[p * m + j] > available[j]) break;
                }

                if (j == m) {
                    for (int k = 0; k < m; k++)
                        available[k] += allocation[p * m + k];

                    safeSeq[count++] = p;
                    finish[p] = true;
                    found = true;
                }
            }
        }

        if (!found) {
            printf("System is not in safe state\n");
            return;
        }
    }

    printf("System is in safe state.\nSafe sequence is: ");
    for (int i = 0; i < n; i++)
        printf("%d ", safeSeq[i]);
    printf("\n");
}

int main() {
    int n = 5; // Number of processes
    int m = 3; // Number of resources

    int allocation[]= {0, 1, 0, 2, 0, 0, 3, 0, 2, 2, 1, 1, 0, 0, 2};
    int maxNeed[]= {7, 5, 3, 3, 2, 2, 9, 0, 2, 2, 2, 2, 4, 3, 3};

    int total[3] = {10, 5, 7};
    int available[3];

    // Calculate available resources
    for (int j = 0; j < m; j++) {
        int sum = 0;
        for (int i = 0; i < n; i++) {
            sum += allocation[i * m + j];
        }
        available[j] = total[j] - sum;
    }

    bankersAlgorithm(allocation, maxNeed, available, n, m, total);

    return 0;
}
