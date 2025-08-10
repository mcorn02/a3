#include "sim.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits> //for INT_MAX
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
    Block* current = head;
    int nodes_traversed = 0;

    //traverse the list
    while(current) {
        nodes_traversed++;

        //if free and big enough
        if(current->process_id == -1 && current->size >= num_units) {
            if(current->size > num_units) {
                split_block(current, num_units, process_id);
                return nodes_traversed;
            }
        }
        current = current->next;
    }

    return -1; //no block found
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
    int min_waste = INT_MAX;
    Block* current = head;
    Block* best_fit = nullptr;


    //traverse the list
    while(current) {
        nodes_traversed++;

        //if free and big enough
        if(current->process_id == -1 && current->size >= num_units) {
            //calculate waste
            int waste = current->size - num_units;
            //if waste is smaller than min
            //update best_fit
            //mark nodes traversed
            if(waste < min_waste) {
                min_waste = waste;
                best_fit = current;
                best_fit_traversal = nodes_traversed;
            }
        }
        current = current->next;
    }

    //if best fit is found, split blocks
    //return nodes traversed for best fit
    if(best_fit) {
        split_block(best_fit, num_units, process_id);
        return best_fit_traversal;
    }

    return -1; //no block found
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
    BestFitMemory mem;

    //try to allocate 10 units for process 1
    int nodes = mem.allocate_mem(1, 10);
    cout << "Allocating 10 units for process 1, nodes traversed: " << nodes << endl;

    //try to allocate 8 units for process 2
    nodes = mem.allocate_mem(2, 8);
    cout << "Allocating 8 units for process 2, nodes traversed: " << nodes << endl;
    
    //a third allocation
    nodes = mem.allocate_mem(3, 15);
    cout << "Allocating 15 units for process 3, nodes traversed: " << nodes << endl;
    
    //an impossible one
    nodes = mem.allocate_mem(4, 200);
    cout << "Allocating 200 units for process 4, nodes traversed: " << nodes << endl;

    return 0;
}
