#include "sim.h"
#include <iostream>
#include <vector>
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
    Block* current = head;
    Block* prev = nullptr;

    while(current) {
        if(current->process_id == process_id) {
            //mark block as free
            current->allocated = false;
            current->process_id = -1;

            merge_block(prev, current);
        }
        prev = current;
        current = current->next;
    }

    return -1;
}

int FirstFitMemory::fragment_count() const {
    //TODO
    int count = 0;
    Block* current = head;

    while(current) {
        //if block is unallocated and size 1 or 2
        //count it as a fragment
        if(current->process_id == -1 && (current->size == 1 || current->size == 2)) {
            count++;
        }
        current = current->next;
    }
    return count;
}

//implementation of BestFit
BestFitMemory::BestFitMemory() {
    head = new Block(0, TOTAL_UNITS, false);
}

int BestFitMemory::allocate_mem(int process_id, int num_units) {
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
    Block* current = head;
    Block* prev = nullptr;

    while(current) {
        if(current->process_id == process_id) {
            //mark block as free
            current->allocated = false;
            current->process_id = -1;

            merge_block(prev, current);
        }
        prev = current;
        current = current->next;
    }

    return -1;
}


int BestFitMemory::fragment_count() const {
    //TODO
    int count = 0;
    Block* current = head;

    while(current) {
        //if block is unallocated and size 1 or 2
        //count it as a fragment
        if(current->process_id == -1 && (current->size == 1 || current->size == 2)) {
            count++;
        }
        current = current->next;
    }
    return count;
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



//prints out blocks to give visual
void print_blocks(Block* head) {
    Block* current = head;

    while(current) {
        cout << "[start " << current->start
        << ", size: " << current->size
        << ", allocated: " << current->allocated
        << ", process id: " << current->process_id
        << "] -> ";
        current = current->next;
    }
    cout << "NULL\n";
}



int main() {
    //BestFitMemory mem;
    FirstFitMemory mem;

    vector<int> pids;

    int next_pid = 1;
    for(int i = 0; i < 2; ++i) {
        int size = 3 + (rand() % 8);
        int result = mem.allocate_mem(next_pid, size);
        if(result != -1) {
            pids.push_back(next_pid);
        }
        ++next_pid;
    }
    cout << "FIRST FIT TEST\n";
    cout << "----------------------------------\n";
    print_blocks(mem.get_head());
    cout << "----------------------------------\n";

    cout << "Fragments: " << mem.fragment_count() << endl;

    cout << "DEALLOCATING MEMORY\n";

    mem.deallocate_mem(1);
    print_blocks(mem.get_head());

    return 0;
}
