#include "page.hpp"
#include <iostream>
#include <cstring>
#define VALUE_SIZE 8
#define RECORD_SIZE_SIZE 2
#define OFFSET_SIZE 2

void put2byte(void *dest, uint16_t data)
{
	*(uint16_t *)dest = data;
}

uint16_t get2byte(void *dest)
{
	return *(uint16_t *)dest;
}

page::page(uint16_t type)
{
	hdr.set_num_data(0);
	hdr.set_data_region_off(PAGE_SIZE - 1 - sizeof(page *)); /* what is this 1? */
	hdr.set_offset_array((void *)((uint64_t)this + sizeof(slot_header)));
	hdr.set_page_type(type);
}

uint16_t page::get_type()
{
	return hdr.get_page_type();
}

uint16_t page::get_record_size(void *record)
{
	uint16_t size = *(uint16_t *)record;
	return size;
}

char *page::get_key(void *record)
{
	char *key = (char *)((uint64_t)record + sizeof(uint16_t));
	return key;
}

uint64_t page::get_val(void *key)
{
	uint64_t val = *(uint64_t *)((uint64_t)key + (uint64_t)strlen((char *)key) + 1);
	return val;
}

void page::set_leftmost_ptr(page *p)
{
	leftmost_ptr = p;
}

page *page::get_leftmost_ptr()
{
	return leftmost_ptr;
}

uint64_t page::find(char *key)
{
	// Please implement this function in project 1.
	uint64_t val = 0;
	uint32_t num_data = hdr.get_num_data();
	uint16_t off = 0;
	uint16_t record_size = 0;
	void *offset_array = hdr.get_offset_array();
	void *stored_key = nullptr;
	void *data_region = nullptr;

	/* linear search for stored key == key */
	for (int i = 0; i < num_data; i++)
	{
		off = *(uint16_t *)((uint64_t)offset_array + i * 2);
		data_region = (void *)((uint64_t)this + (uint64_t)off);
		stored_key = get_key(data_region);
		if (strcmp(key, (char *)stored_key) == 0)
		{
			val = get_val((void *)stored_key);
			break;
		}
	}
	return val;
}

bool page::insert(char *key, uint64_t val)
{
	// Please implement this function in project 1.
	/* 공간이 있는지 확인 */
	if (!is_full(strlen(key)))
		return false;

	/* 변수 초기화 */
	uint32_t num_data = hdr.get_num_data();
	uint16_t off = 0;
	uint16_t record_size = 0;
	uint16_t where_to_insert = 0;
	void *offset_array = hdr.get_offset_array();
	void *stored_key = nullptr;
	void *data_region = nullptr;

	/* insert할 key보다 큰 key가 나올 떄까지 순차 탐색 */
	for (int i = 0; i < num_data; i++)
	{
		off = *(uint16_t *)((uint64_t)offset_array + i * 2);
		data_region = (void *)((uint64_t)this + (uint64_t)off);
		stored_key = get_key(data_region);
		if (strcmp(key, (char *)stored_key) >= 0)
		{
			where_to_insert = i + 1;
		}
		else
		{
			where_to_insert = i;
			break;
		}
	}

	void *src_pointer = nullptr;
	void *dest_pointer = nullptr;
	offset_array = hdr.get_offset_array();

	/* offset array 값 변경  */
	for (int i = num_data; i > where_to_insert; i--)
	{
		uint16_t data;
		src_pointer = (void *)((uint64_t)offset_array + (i - 1) * 2);
		dest_pointer = (void *)((uint64_t)offset_array + i * 2);
		data = get2byte(src_pointer);
		put2byte(dest_pointer, data);
	}

	/* offset insert */
	record_size = RECORD_SIZE_SIZE + (strlen(key) + 1) + VALUE_SIZE;
	dest_pointer = (void *)((uint64_t)offset_array + (where_to_insert)*2);
	void *record_size_pointer = (void *)((uint64_t)this + (uint64_t)hdr.get_data_region_off() - record_size);
	int64_t input_offset = (int64_t)(record_size_pointer) - (int64_t)this;
	*(uint64_t *)dest_pointer = input_offset;

	/* record size insert */
	*(uint16_t *)record_size_pointer = record_size;

	/* record key insert */
	char *key_pointer = (char *)((uint64_t)record_size_pointer + RECORD_SIZE_SIZE);
	strcpy(key_pointer, key);
	*((char *)key_pointer + strlen(key)) = '\0';
	/* record value insert */
	uint64_t *value_pointer = (uint64_t *)((u_int64_t)record_size_pointer + RECORD_SIZE_SIZE + strlen(key) + 1);
	*value_pointer = val;

	/* edit header */
	hdr.set_data_region_off(input_offset);
	hdr.set_num_data(num_data + 1);
	return true;
}

page *page::split(char *key, uint64_t val, char **parent_key)
{
	// Please implement this function in project 2.
	page *new_page = new page(get_type());
	int num_data = hdr.get_num_data();
	void *offset_array = hdr.get_offset_array();
	void *stored_key = nullptr;
	char *middle_key = nullptr;
	uint16_t off = 0;
	uint64_t stored_val = 0;
	void *data_region = nullptr;

	for (int i = num_data / 2; i < num_data; i++)
	{
		off = *(uint16_t *)((uint64_t)offset_array + i * 2);
		data_region = (void *)((uint64_t)this + (uint64_t)off);
		stored_key = get_key(data_region);
		if (i == num_data / 2)
		{
			middle_key = get_key(data_region);
		}
		stored_val = get_val((void *)stored_key);
		new_page->insert((char *)stored_key, stored_val);
	}

	if (strcmp(key, middle_key) <= 0) /* Inserted key is smaller than middle key. */
	{
		this->insert(key, val);
	}
	else /* Inserted key is larger than middle key. */
	{
		new_page->insert(key, val);
	}
	this->defrag();
	return new_page;
}

bool page::is_full(uint64_t inserted_key_size)
{
	// Please implement this function in project 1.
	uint32_t num_data = hdr.get_num_data();
	uint16_t data_region_off = hdr.get_data_region_off();
	void *offset_array = hdr.get_offset_array();
	void *free_start = (void *)((uint64_t)offset_array + num_data * 2); /* offset type is uint16_t -> num*data * 2 */
	void *free_end = (void *)((uint64_t)this + (u_int64_t)hdr.get_data_region_off());

	/* look at the space left */
	uint64_t needed_size = inserted_key_size + VALUE_SIZE + RECORD_SIZE_SIZE + OFFSET_SIZE + 1;
	uint64_t left_over = (uint64_t)free_end - (uint64_t)free_start;
	if (left_over < needed_size) /* Not enough space here! */
		return false;
	return true;
}

void page::defrag()
{
	page *new_page = new page(get_type());
	int num_data = hdr.get_num_data();
	void *offset_array = hdr.get_offset_array();
	void *stored_key = nullptr;
	uint16_t off = 0;
	uint64_t stored_val = 0;
	void *data_region = nullptr;

	for (int i = 0; i < num_data / 2; i++)
	{
		off = *(uint16_t *)((uint64_t)offset_array + i * 2);
		data_region = (void *)((uint64_t)this + (uint64_t)off);
		stored_key = get_key(data_region);
		stored_val = get_val((void *)stored_key);
		new_page->insert((char *)stored_key, stored_val);
	}
	new_page->set_leftmost_ptr(get_leftmost_ptr());

	memcpy(this, new_page, sizeof(page));
	delete new_page;
}

void page::print()
{
	uint32_t num_data = hdr.get_num_data();
	uint16_t off = 0;
	uint16_t record_size = 0;
	void *offset_array = hdr.get_offset_array();
	void *stored_key = nullptr;
	uint64_t stored_val = 0;

	printf("## slot header\n");
	printf("Number of data :%d\n", num_data);
	printf("offset_array : |");
	for (int i = 0; i < num_data; i++)
	{
		off = *(uint16_t *)((uint64_t)offset_array + i * 2);
		printf(" %d |", off);
	}
	printf("\n");

	void *data_region = nullptr;
	for (int i = 0; i < num_data; i++)
	{
		off = *(uint16_t *)((uint64_t)offset_array + i * 2);
		data_region = (void *)((uint64_t)this + (uint64_t)off);
		record_size = get_record_size(data_region);
		stored_key = get_key(data_region);
		stored_val = get_val((void *)stored_key);
		printf("==========================================================\n");
		printf("| data_sz:%u | key: %s | val :%lu |\n", record_size, (char *)stored_key, stored_val, strlen((char *)stored_key));
	}
}
