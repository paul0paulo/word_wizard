#include "hashtable.h"
#include <fstream>
#include <sstream>
#include <cctype>
#include <string>
#include <stdexcept>
using namespace std;

HashTable::HashTable(int capacity) // hash table constructor with the initialized values
    : capacity(capacity), 
      collisions(0), 
      unique_words(0), 
      total_words(0), 
      hash_code_function(2)
{
    buckets = new LinkedList[capacity]; // creating a new array of type with size capacity of the hashtable
}


HashTable::~HashTable() // destructor
{
    delete[] buckets; // delete the array
}

// Hash functions
unsigned long HashTable::hashCode(string key)
{
    unsigned long hash = 0;
    if (hash_code_function == 1) // linear ascii sum function
    {
        for (size_t i = 0; i < key.size(); i++)
        {
            hash += key[i]; // add the ASCII value of each character
        }
    }
    else if (hash_code_function == 2) // polynomial rolling hash
    {
        unsigned int p = 31; // polynomial hash function base
        unsigned int m = capacity; // use table capacity for modulus operator
        for (unsigned int i = 0; i < key.length(); i++)
        {
            hash = (hash * p + key[i]) % m; // hash function
        }
    }
    else if (hash_code_function == 3) // cycle shift Hash
    {
        hash = 0; // initialize hash
        for (size_t i = 0; i < key.size(); i++)
        {
            hash = (hash << 5) | (hash >> (27)); // left rotate by 5 bits
            hash ^= key[i]; // XOR with the current character
        }
    }
    else if (hash_code_function == 4) // linear modified hashing
    {
        hash = 1; // initialize hash to 1 to avoid multiplication by 0
        for (size_t i = 0; i < key.size(); i++)
        {
            hash *= key[i]; // multiply by ASCII value of each character
        }
    }
    else if (hash_code_function == 5) // alternating sum/polynomial hashfunction
    {
        bool alternator = true; // boolean for alternations
        for (size_t i = 0; i < key.size(); i++)
        {
            if (alternator)
            {
                hash += key[i];  // add the ASCII value of the character
            }
            else
            {
                unsigned int p = 31; // polynomial hash function base
                unsigned int m = capacity; // use table capacity for modulus operator
                hash = (hash * p + key[i]) % m; //polynomial hashing
            }
            alternator = !alternator;  // alternate between addition and polynomial rolling
        }
    }
    else if (hash_code_function == 6) // alternating polynomial and cycle shift hash functions
    {
        bool alternator = true; // boolean for alternations
        for (size_t i = 0; i < key.size(); i++)
        {
            if (alternator)
            {
                unsigned int p = 31; // polynomial hash function base
                unsigned int m = capacity; // use table capacity for modulus operator
                hash = (hash * p + key[i]) % m; //polynomial hashing
            }
            else
            {
                hash = (hash << 5) | (hash >> (27)); // left rotate by 5 bits
                hash ^= key[i]; // XOR with the current character
            }
            alternator = !alternator;  // alternate between cycle shift and polynomial rolling
        }
    }
    return hash % capacity; // ensure the hashcode is within the range of the hash table
}

// insert a word into the hash table
void HashTable::insert(string word)
{
    // changing the word into lowercase
    for (size_t i = 0; i < word.length(); i++)
    {
        word[i] = tolower(word[i]); // change each character into lower case
    }

    unsigned int index = hashCode(word); // get hashcode for hash table index

    // check if the word already exists
    Node *existing_word = buckets[index].find(word); // if it exists return a pointer to the word or otherwie
    if (existing_word != nullptr)
    {
        existing_word -> setValue(existing_word -> getValue() + 1); // update value(frequency) of the word
    }
    else if (existing_word == nullptr)
    {
        // check if a collision occurs if inserted in a non empty linked list (bucket) 
        if (!buckets[index].empty()) // if the bucket is not empty
        {
            collisions++; // increment collisions if inserting into a non-empty bucket
        }
        buckets[index].insert(word, 1); // if no collisions add a new word with count 1
        unique_words++; // increment unique words if a new key was added
    }

    total_words++; // increment total word count
}

// find the frequency of a word
int HashTable::find_freq(string word)
{
    //  do // to make sure a proper string is being found
    // {
    //     if (word.empty()) // check if the input is empty
    //     {
    //         cout << "Please enter a valid (non-empty) string" << endl; // print message
    //     }
    // } while (word.empty()); // enter a while loop if the word is empty

    // changing the word into lowercase
    for (size_t i = 0; i < word.length(); i++) 
    {
        word[i] = tolower(word[i]); // change each character into lower case
    }
    unsigned int index = hashCode(word); // get hash table index
    Node* node = buckets[index].find(word); // find it in the basket with the same hashcode which returns a pointer
    int frequency = 0; // initatilze frequency of the word
    if (node != nullptr) // if it is the hashtable
    {
        frequency = node -> getValue(); // update the frequency to the value of its occurences
    }
    return frequency; // return frequency or 0 which 0 implies it is not found
}

// get the total number of collisions
unsigned int HashTable::getCollisions()
{
    return collisions;
}

// get the number of unique words
unsigned int HashTable::getUniqueWords()
{
    return unique_words;
}

// get the total number of words
unsigned int HashTable::getTotalWords()
{
    return total_words;
}

// Import words from a file and populate the hash table
void HashTable::import(string path)
{
    ifstream fein(path); // open the file at the given path as fein
    if (!fein.is_open()) // if it is succesdfully opened
    {
        throw runtime_error("could not open file: " + path); // throw an errosh if file couldnt be opened
    }

    const string punctuations = ".,!?;\"“”‘’"; // a string of punctuations to ignore

    string line;
    while (getline(fein, line)) // read the file line by line
    {
        istringstream ss(line); // create a stream for the line
        string word;
        while (ss >> word) // extract each word from the line
        {
           
            size_t start = 0; // start index
            while (start < word.length() && punctuations.find(word[start]) != string::npos) // check for the punctuations marks at the begining of the word
            {
                ++start; // increment the starting index
            }

            
            size_t end = word.length(); // end indez
            while (end > start && punctuations.find(word[end - 1]) != string::npos) // check for the punctuations marks at the end of the word
            {
                --end; // decrement the end index
            }

            if (start < end) // if it is not an empty  word (not everything cleared)
            {
                word = word.substr(start, end - start); // slice it from the start to end index to get the cleaned word
                insert(word); // insert the word into the hashhtable
            }
        }
    }

    fein.close();
    //sucess messages to show the user
    cout << "Done !!" << endl << endl;
    // float coll_per_word = float(getCollisions())/ float(getTotalWords());
    cout << "The number of collisions is: " << getCollisions() << endl;
    cout << "The number of words is: " << getTotalWords() << endl;
    cout << "The number of unique_words is: " << getUniqueWords() << endl;
    // cout << "The number of collisions per word is: " << coll_per_word << endl;

}
