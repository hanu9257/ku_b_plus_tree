#include "slot_header.hpp"

void slot_header::set_num_data(uint32_t ndata){
	num_data = ndata;
}

uint32_t slot_header::get_num_data(){
	return num_data;
}

void slot_header::set_data_region_off(uint16_t off){
	data_region_off = off;		
}

uint16_t slot_header::get_data_region_off(){
	return data_region_off;
}

void *slot_header::get_offset_array(){
	return offset_array;
}

void slot_header::set_offset_array(void *offset){
	offset_array = offset;
}

void slot_header::set_page_type(uint16_t type){
	page_type = type;
}
uint16_t slot_header::get_page_type(){
	return page_type;
}
