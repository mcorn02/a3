#include "sim.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

//implementation of blocks
Block::Block(int s, int sz, bool a, int pid)
    : start(s), size(sz), allocated(a), process_id(pid), next(nullptr) {}

//implementation of FirstFit
FirstFitMemory::FirstFitMemory() {
    head = new Block(0, TOTAL_UNITS, false);
}

int FirstFitMemory::allocate_mem(int process_id, int num_units) {
    //TODO
    return -1;
}

int FirstFitMemory::deallocate_mem(int process_id) {
    //TODO
    return -1;
}

int FirstFitMemory::fragment_count() const {
    //TODO
    return 0;
}

//implementation of BestFit
BestFitMemory::BestFitMemory() {
    head = new Block(0, TOTAL_UNITS, false);
}

int BestFitMemory::allocate_mem(int process_id, int num_units) {
    //TODO
    int pid = process_id;

    return -1;
}

int BestFitMemory::deallocate_mem(int process_id) {
    //TODO
    return -1;
}

int BestFitMemory::fragment_count() const {
    //TODO
    return 0;
}

//Stats
//need average # of external fragments
//average allocation time in terms of the average nodes traversed in allocation
//percentage of times an allocation request is denied
void Stats::record_allocation(int nodes, bool success) {
    total_requests++;
    if(!success) {
        denied_requests++;
    } else {
        
    }
}

void Stats::record_fragment_count(int fragments) {
}

void print_summary(const string& name) {
}

void run_simulation(MemoryManager& mem, Stats& stats) {
}

int main() {
    cout << "running\n";
    return 0;
}
