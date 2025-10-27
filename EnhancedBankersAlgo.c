#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define N 5   // Number of processes
#define M 3   // Number of resources

// -------------------- Global Variables --------------------
int allocation[N][M];
int maxNeed[N][M];
int need[N][M];
int available[M];
int total[M] = {10, 5, 7};  // total resources
bool finished[N] = {false};

// -------------------- Function Declarations --------------------
void initialize_system();
bool is_system_safe();
bool request_resources(int pid, int request[]);
void detect_deadlock();
int find_victim_process();
void recover_from_deadlock(int victim_id);
void display_system_state();
void execute_jobs();

// -------------------- Function Definitions --------------------

// Step 1: Initialize system with hardcoded example
void initialize_system() {
    int alloc[N][M] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    int maxn[N][M] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };

    // Copy to globals
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            allocation[i][j] = alloc[i][j];
            maxNeed[i][j] = maxn[i][j];
            need[i][j] = maxNeed[i][j] - allocation[i][j];
        }
    }

    // Calculate available resources
    for (int j = 0; j < M; j++) {
        int sum = 0;
        for (int i = 0; i < N; i++)
            sum += allocation[i][j];
        available[j] = total[j] - sum;
    }

    printf("System Initialized.\n");
    display_system_state();
}

// Step 2: Safety Check
bool is_system_safe() {
    int work[M];
    bool finish[N] = {false};

    for (int i = 0; i < M; i++)
        work[i] = available[i];

    int count = 0;
    while (count < N) {
        bool found = false;
        for (int p = 0; p < N; p++) {
            if (!finish[p]) {
                int j;
                for (j = 0; j < M; j++)
                    if (need[p][j] > work[j])
                        break;

                if (j == M) {
                    for (int k = 0; k < M; k++)
                        work[k] += allocation[p][k];
                    finish[p] = true;
                    found = true;
                    count++;
                }
            }
        }
        if (!found)
            break;
    }

    return (count == N);
}

// Step 3: Request resources
bool request_resources(int pid, int request[]) {
    printf("\nProcess %d requesting: [", pid);
    for (int j = 0; j < M; j++) printf("%d ", request[j]);
    printf("]\n");

    for (int j = 0; j < M; j++) {
        if (request[j] > need[pid][j]) {
            printf("Error: Process %d requested more than its need!\n", pid);
            return false;
        }
        if (request[j] > available[j]) {
            printf("Process %d must wait (insufficient resources).\n", pid);
            return false;
        }
    }

    // Tentatively allocate
    for (int j = 0; j < M; j++) {
        available[j] -= request[j];
        allocation[pid][j] += request[j];
        need[pid][j] -= request[j];
    }

    // Check if safe
    if (is_system_safe()) {
        printf("System remains in safe state after allocation.\n");
        return true;
    } else {
        // Rollback
        for (int j = 0; j < M; j++) {
            available[j] += request[j];
            allocation[pid][j] -= request[j];
            need[pid][j] += request[j];
        }
        printf("Unsafe state detected. Process %d blocked.\n", pid);
        return false;
    }
}

// Step 4: Deadlock detection
void detect_deadlock() {
    printf("\nChecking for deadlocks...\n");
    if (!is_system_safe()) {
        printf("Deadlock detected!\n");
        int victim = find_victim_process();
        recover_from_deadlock(victim);
    } else {
        printf("No deadlock detected.\n");
    }
}

// Step 5: Choose victim (minimum allocated resources)
int find_victim_process() {
    int min_sum = 9999;
    int victim = -1;

    for (int i = 0; i < N; i++) {
        if (!finished[i]) {
            int sum = 0;
            for (int j = 0; j < M; j++)
                sum += allocation[i][j];
            if (sum < min_sum) {
                min_sum = sum;
                victim = i;
            }
        }
    }

    printf("Victim selected: Process %d (min resource loss = %d)\n", victim, min_sum);
    return victim;
}

// Step 6: Recovery - release victim resources
void recover_from_deadlock(int victim_id) {
    printf("Recovering from deadlock by terminating process %d...\n", victim_id);
    for (int j = 0; j < M; j++)
        available[j] += allocation[victim_id][j];

    for (int j = 0; j < M; j++) {
        allocation[victim_id][j] = 0;
        need[victim_id][j] = 0;
    }
    finished[victim_id] = true;
    display_system_state();
}

// Step 7: Display state
void display_system_state() {
    printf("\nCurrent System State:\n");
    printf("PID\tAlloc\t\tMax\t\tNeed\t\tFinished\n");
    for (int i = 0; i < N; i++) {
        printf("%d\t", i);
        for (int j = 0; j < M; j++) printf("%d ", allocation[i][j]);
        printf("\t\t");
        for (int j = 0; j < M; j++) printf("%d ", maxNeed[i][j]);
        printf("\t\t");
        for (int j = 0; j < M; j++) printf("%d ", need[i][j]);
        printf("\t\t%s\n", finished[i] ? "Yes" : "No");
    }
    printf("Available: ");
    for (int j = 0; j < M; j++) printf("%d ", available[j]);
    printf("\n");
}

// Step 8: Main Execution Loop
void execute_jobs() {
    int cycle = 0;
    while (cycle < 5) {
        printf("\n=== Cycle %d ===\n", cycle + 1);
        for (int i = 0; i < N; i++) {
            if (finished[i]) continue;
            
            int req[M];
            // Simulate random small requests
            for (int j = 0; j < M; j++) req[j] = (need[i][j] > 0 && rand() % 2) ? 1 : 0;

            if (!request_resources(i, req)) printf("Process %d is blocked this cycle.\n", i);
            
            else {
                bool done = true;
                for (int j = 0; j < M; j++) if (need[i][j] != 0) done = false;

                if (done) {
                    finished[i] = true;
                    for (int j = 0; j < M; j++) available[j] += allocation[i][j];
                    printf("Process %d has finished execution.\n", i);
                }
            }
        }
        detect_deadlock();
        cycle++;
    }
}

// -------------------- Main --------------------
int main() {
    initialize_system();
    execute_jobs();
    printf("\nAll cycles complete.\nFinal System State:\n");
    display_system_state();
    return 0;
}
