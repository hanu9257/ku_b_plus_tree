#include "slot_header.hpp"

#define PAGE_SIZE 256
//#define PAGE_SIZE 4096
class page{
	private:
		slot_header hdr;
		char data[PAGE_SIZE-sizeof(slot_header)-sizeof(page*)];
		page *leftmost_ptr;
	public:
		page(uint16_t);

		uint64_t find(char *);
		bool insert(char *, uint64_t);
		void print();
		bool is_full(uint64_t);
		uint16_t get_record_size(void*);
		char *get_key(void *);
		uint64_t get_val(void *);
		uint16_t get_type();
		page* split(char *, uint64_t, char**);
		void set_leftmost_ptr(page *);
		page *get_leftmost_ptr();
		void defrag();
};


