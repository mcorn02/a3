#ifndef SIM_H
#define SIM_H

#include <string>
using namespace std;

const int TOTAL_UNITS = 128; //256 KB / 2 KB
const int MIN_UNITS = 3;
const int MAX_UNITS = 10;
const int REQUESTS = 10000;

struct Block {
    int start; //starting unit index
    int size; //num of units
    bool allocated; 
    int process_id; //pid = -1 means free block
    Block* next;

    Block(int s, int sz, bool a, int pid = -1);
};

//abstract base class for memory manager
class MemoryManager {
protected:
    Block* head;
public:
    MemoryManager() {
        head = new Block(0, TOTAL_UNITS, false);
    }
    
    Block* get_head() const { return head; }

    virtual int allocate_mem(int process_id, int num_units) = 0;
    virtual int deallocate_mem(int process_id) = 0;
    virtual int fragment_count() const = 0;
    virtual ~MemoryManager() {
        //clean linked list
        Block* current = head;
        while(current) {
            Block* temp = current;
            current = current->next;
            delete temp;
        }
    }

protected:
    void split_block(Block* block, int size_needed, int process_id) {
        if(block->size > size_needed) {
            //allocate new block
            Block* new_block = new Block(block->start + size_needed, block->size - size_needed, false, -1 );

            //reuse the current block
            block->size = size_needed;
            block->allocated = true;
            block->process_id = process_id;
            block->next = new_block;
        }
    }
};

    virtual void merge_block() {}
        
//first fit memory manager
class FirstFitMemory : public MemoryManager {
public: 
    FirstFitMemory();
    int allocate_mem(int process_id, int num_units) override;
    int deallocate_mem(int process_id) override;
    int fragment_count() const override;
protected:
    void merge_block() override;
};

//Best Fit memory manager
class BestFitMemory : public MemoryManager {
public: 
    BestFitMemory();
    int allocate_mem(int process_id, int num_units) override;
    int deallocate_mem(int process_id) override;
    int fragment_count() const override;
 protected:
    void merge_block() override;
   
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
