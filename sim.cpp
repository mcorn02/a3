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
    int nodes_traversed = 0;
    int best_fit_traversal = 0;
    int min_waste = MAX_UNITS;
    Block* current = head;
    Block* best_fit = nullptr;


    //traverse the linked list
    while(current) {
        nodes_traversed++;
        
        //check if block is usable
        if(current->process_id == -1 || current->size >= num_units) {
            //waste calculation
            int waste = current->size - num_units;

            //check if this is new min_waste
            if(waste < min_waste) {
                min_waste = waste;
                best_fit = current;
                best_fit_traversal = nodes_traversed;
            }
        }
        current = current->next;
        
        if(best_fit) {
            split_block(best_fit, num_units, process_id);
            return best_fit_traversal;
        }
    }
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
