#include "sim.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <climits> //for INT_MAX
#include <random>
#include <fstream>


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
            split_block(current, num_units, process_id);
            return nodes_traversed;
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
            return 0;
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

                //if perfect fit we can break early
                if(waste == 0) break;
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
    Block* current = head;
    Block* prev = nullptr;

    while(current) {
        if(current->process_id == process_id) {
            //mark block as free
            current->allocated = false;
            current->process_id = -1;

            merge_block(prev, current);
            return 0;
        }
        prev = current;
        current = current->next;
    }

    return -1;
}


int BestFitMemory::fragment_count() const {
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
        total_nodes_traversed += nodes;
    }
}

void Stats::record_fragment_count(int fragments) {
    total_fragments += fragments;
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

    cout << "\n";
    cout << "\n";
}


void print_summary(const string& name) {
}

bool allocate_or_not(int percentage) {
    return (rand() % 100) < percentage;
}

void run_simulation(MemoryManager& mem, Stats& stats) {
   ofstream csv("sim_results.csv");
   csv << "step,denied_requests,total_nodes_traversed,total_fragments\n";

    vector<int> pids;
    int next_pid = 1;

    const int percentage = 60; //60% allocate, 40% deallocate

    for(int step = 1; step <= REQUESTS; ++step) {
        bool perform_allocate = allocate_or_not(percentage);

        //if no memory allocated, we must allocate
        if(pids.empty()) {
            perform_allocate = true;
        }

        if(perform_allocate) {
            //perform allocation
            //generate units to allocate 
            //between 3 and 10
            int num_units = MIN_UNITS + rand() % (MAX_UNITS - MIN_UNITS + 1);            
            //allocate
            int traversed = mem.allocate_mem(next_pid, num_units);
            bool success = (traversed != -1);
            //track nodes traversed or denied request
            stats.record_allocation(success ? traversed : 0, success);
            
            if(success) {
                pids.push_back(next_pid);
            }

            cout << "After allocation of PID: " << next_pid << "\n";
            print_blocks(mem.get_head());
            next_pid++;
            
        } else {
            //perform deallocation
            int i = rand() % pids.size();
            int pid = pids[i];
            mem.deallocate_mem(pid);
            pids.erase(pids.begin() + i);
            
            //remove pid from list
            pids[i] = pids.back();
            pids.pop_back();

            cout << "After deallocation of PID: " << i << "\n";
            print_blocks(mem.get_head());
        }

        int fragments = mem.fragment_count();
        
        cout << "\nCURRENT FRAGMENT COUNT: " << fragments << "\n";

        stats.record_fragment_count(fragments);

        csv << step << ","
            << stats.denied_requests << ","
            << stats.total_nodes_traversed << ","
            << stats.total_fragments << "\n";
    }

    csv.close();
}



int main() {
    //BestFitMemory mem;
    
    srand(static_cast<unsigned>(time(nullptr)));

    Stats stats;
    FirstFitMemory mem;

    run_simulation(mem, stats);
    
    cout << "\nFRAGMENT COUNT" << mem.fragment_count() << "\n";

    return 0;
}
