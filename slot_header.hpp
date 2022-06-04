
#include <stdint.h>

#define LEAF 1
#define INTERNAL 2

class slot_header{
	private:
		uint16_t page_type;
		uint16_t data_region_off;
		uint32_t num_data;
		void *offset_array;
		void *sibling_ptr;
		
	public:
		void set_page_type(uint16_t);
		uint16_t get_page_type();
		void set_num_data(uint32_t);
		uint32_t get_num_data();
		uint16_t get_data_region_off();
		void set_data_region_off(uint16_t);
		void set_offset_array(void *);
		void *get_offset_array();
};


