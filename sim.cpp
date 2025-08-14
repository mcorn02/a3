/*
PLEASE RUN ON cssc1415

#Team Names: Michael Corn and Simmon Quan

#Edoras Usernames: cssc1415 and cssc1450

#Class Info: CS 480 Operating Systems, Summer Session 2 2025

#Assignment 3: Memory Allocation/Deallocation Simulation

#File Name: sim.cpp

*/


#include "sim.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <climits> //for INT_MAX
#include <random>
#include <fstream>
#include <iomanip>

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

void Stats::print_summary(const string& name) {
    int total_requests = REQUESTS;
    int successful_allocs = total_requests - denied_requests;

    double avg_fragments = static_cast<double>(total_fragments) / total_requests;
    double avg_nodes = successful_allocs > 0 ? static_cast<double>(total_nodes_traversed) / successful_allocs : 0.0;
    double deny_pct = (static_cast<double>(denied_requests) / total_requests) * 100.0;

    cout << "End of " << name << " Allocation\n";
    cout << "Average External Fragments Each Request: " << fixed << setprecision(6) << avg_fragments << "\n";
    cout << "Average Nodes Traversed Each Allocation: " << fixed << setprecision(6) << avg_nodes << "\n";
    cout << "Percentage Allocation Requests Denied Overall: " << fixed << setprecision(6) << deny_pct << "%\n";
    cout << "\n";
}

bool allocate_or_not(int percentage) {
    return (rand() % 100) < percentage;
}

void run_simulation(MemoryManager& mem, Stats& stats, const string& csv_filename) {
   ofstream csv(csv_filename);
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
            next_pid++;
            
        } else {
            //perform deallocation
            int i = rand() % pids.size();
            int pid = pids[i];
            mem.deallocate_mem(pid);
            
            //remove pid from list
            pids[i] = pids.back();
            pids.pop_back();
        }

        int fragments = mem.fragment_count();
        
        stats.record_fragment_count(fragments);

        csv << step << ","
            << stats.denied_requests << ","
            << stats.total_nodes_traversed << ","
            << stats.total_fragments << "\n";
    }

    csv.close();
}



int main() {    
    srand(static_cast<unsigned>(time(nullptr)));

    Stats ff_stats;
    FirstFitMemory ff;
    run_simulation(ff, ff_stats, "first_fit_results.csv");
    ff_stats.print_summary("First Fit");

    BestFitMemory bf;
    Stats bf_stats;
    run_simulation(bf, bf_stats, "best_fit_results.csv");
    bf_stats.print_summary("Best Fit");

    return 0;
}
