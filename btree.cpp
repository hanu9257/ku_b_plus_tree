#include "btree.hpp"
#include <iostream> 
#include <cstring>

btree::btree(){
	root = new page(LEAF);
	height = 1;
};

void btree::insert(char *key, uint64_t val){
	// Please implement this function in project 2.
	page *current_page = root;
	while(current_page->get_type() == INTERNAL) {
		current_page = (page *)current_page->find(key);
	}
	char *medium_address;
	if(current_page->insert(key, val) == false) {
		current_page->split(key, val, &medium_address);
	}
	/* current page is leaf */
}

uint64_t btree::lookup(char *key){
	// Please implement this function in project 2.
	uint64_t val = 0;
	page *current_page = root;
	while(current_page->get_type() == INTERNAL) {
		current_page = (page *)current_page->find(key);
	}
	val = current_page->find(key);
	return val;
}
