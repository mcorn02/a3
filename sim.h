#ifndef SIM_H
#define SIM_H

#include <string>
using namespace std;

const int TOTAL_UNITS = 128; //256 KB / 2 KB
const int MIN_UNITS = 3;
const int MAX_UNITS = 10;
const int REQUESTS = 10000;

struct Block {
    int start;
    int size;
    bool allocated;
    int process_id;
    Block* next;

    Block(int s, int sz, bool a, int pid = -1);
};

//abstract base class for memory manager
class MemoryManager {
public:
    virtual int allocate_mem(int process_id, int num_units) = 0;
    virtual int deallocate_mem(int process_id) = 0;
    virtual int fragment_count() const = 0;
};

//first fit memory manager
class FirstFitMemory : public MemoryManager {
private:
    Block* head;
public: 
    FirstFitMemory();
    int allocate_mem(int process_id, int num_units) override;
    int deallocate_mem(int process_id) override;
    int fragment_count() const override;
};

//Best Fit memory manager
class BestFitMemory : public MemoryManager {
private:
    Block* head;
public: 
    BestFitMemory();
    int allocate_mem(int process_id, int num_units) override;
    int deallocate_mem(int process_id) override;
    int fragment_count() const override;
};

//statistics tracking 
struct Stats {
    int total_requests = 0;
    int denied_requests = 0;
    int total_nodes_traversed = 0;
    int total_fragments = 0;

    void record_allocation(int nodes, bool success);
    void record_fragment_count(int fragments);
    void print_summary(const string& name);
};

//simulation function
void run_simulation(MemoryManager& mem, Stats& stats);

#endif // SIM_H
