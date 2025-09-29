
#ifndef _HASHTABLE_H
#define _HASHTABLE_H
#include <string>
#include "linkedlist.h"
using std::string;

class HashTable
{
	private:
		LinkedList *buckets;		        		// Array of Buckets (chains)
		unsigned int capacity;				    	// Total Capacity of HashTable
		unsigned int collisions; 					// Total Number of Collisions
		unsigned int unique_words;					// Number of unique words in the hashtable
		unsigned int total_words;					// Total number of words in the hashtable
		int hash_code_function;						// Selected Hash_code function


	
	public:
		HashTable(int capacity);
		unsigned long hashCode(string key);
		unsigned int getCollisions();
		unsigned int getUniqueWords();
		unsigned int getTotalWords();
		void import(string path);
		void insert(string word);
		int find_freq(string word);						//return the frequency of a word
		~HashTable();
};
#endif