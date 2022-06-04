#include "page.hpp"

class btree{
	private: 
		page *root;
		uint64_t height;
	
	public:
		btree();
		void insert(char*, uint64_t);
		uint64_t lookup(char *);
};
