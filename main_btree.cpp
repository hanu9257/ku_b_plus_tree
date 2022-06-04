#include "btree.hpp"
#include <iostream> 
#define STRING_LEN 20

int main(){
	btree *tree = new btree();

	char key[STRING_LEN];
	char i;
	int val = 100;
	int cnt = 0;

	for(i='a'; i<='z'; i+=1){
		for(int j=0; j<STRING_LEN-1; j++){
			key[j] = i;
		}
		key[STRING_LEN-1]='\0';
		cnt++;
		val*=cnt;
		tree->insert(key, val);
	}

	val = 100;
	cnt = 0;
	for(i='a'; i<='z'; i+=1){
		for(int j=0; j<STRING_LEN-1; j++){
			key[j] = i;
		}
		key[STRING_LEN-1]='\0';
		cnt++;
		val*=cnt;
		if(val== tree->lookup(key)){
			printf("key :%s founds\n",key);		
		}
		else{
			printf("key :%s Something wrong\n",key);		
		}

	}

	return 0;

}
