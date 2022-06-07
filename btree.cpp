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
	page *new_page = nullptr;
	char *medium_key = (char *)malloc(20);
	char input_key[20];
	uint64_t input_value;
	int path_count = 1;

	/* 현재 k를 insert할 때 split이 발생했음에도 부모 page의 key가 insert되지 않았음. */

	while (current_page->get_type() == INTERNAL) /* traversing */
	{
		stack[path_count] = current_page;
		path_count++;
		page *next_page = (page *)current_page->find(key);
		current_page = next_page;
	}
	strcpy(input_key, key);
	input_value = val;
	while (path_count != 0)
	{
		if (current_page->insert(input_key, input_value) == false) /* leaf */
		{
			new_page = current_page->split(input_key, input_value, &medium_key); /* after split */
			strcpy(input_key, medium_key);
			input_value = (uint64_t)new_page;
			if (current_page == root)
			{
				page *new_root = new page(INTERNAL);
				new_root->set_leftmost_ptr(root);
				new_root->insert(medium_key, (uint64_t)new_page);
				root = new_root;
				height++;
				break;
			}
		}
		else 
		{
			break;
		}

		current_page = stack[--path_count];
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
