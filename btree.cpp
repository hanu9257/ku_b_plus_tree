#include "btree.hpp"
#include <iostream>
#include <cstring>

btree::btree()
{
	root = new page(LEAF);
	height = 1;
};

void btree::insert(char *key, uint64_t val)
{
	// Please implement this function in project 2.
	page *current_page = root;
	page *stack[height];
	int path_count = 0;
	while (current_page->get_type() == INTERNAL)
	{
		page *next_page = (page *)current_page->find(key);
		if(next_page == nullptr)
		{
			next_page = current_page->get_leftmost_ptr();
		}
		current_page = next_page;
	}
	char *medium_key;
	page *new_page = nullptr;
	if (current_page->insert(key, val) == false)
	{
		new_page = current_page->split(key, val, &medium_key);
		if (current_page == root)
		{
			page *new_root = new page(INTERNAL);
			new_root->set_leftmost_ptr(root);
			new_root->insert(medium_key, (uint64_t)new_page);
			root = new_root;
			height++;
		}
	}
}

uint64_t btree::lookup(char *key)
{
	// Please implement this function in project 2.
	uint64_t val = 0;
	page *current_page = root;
	while (current_page->get_type() == INTERNAL)
	{
		current_page = (page *)current_page->find(key);
	}
	val = current_page->find(key);
	return val;
}
