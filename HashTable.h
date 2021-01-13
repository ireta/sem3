#ifndef __HASHTABLE__
#define __HASHTABLE__

#include "HashUtils.h"
#include "ItemNotFoundException.h"
/* Do not add new libraries or files */

#define BUCKET_SIZE 2

// Do not modify the public interface of this class.
// Otherwise, your code will note compile!
template <class K, class T>
class HashTable {
    struct Entry {
        K Key;             // the key of the entry
        T Value;   // the value of the entry
        bool Deleted;        // flag indicating whether this entry is deleted
        bool Active;         // flag indicating whether this item is currently used

        Entry() : Key(), Value(), Deleted(false), Active(false) {}
    };

    struct Bucket {
        Entry entries[BUCKET_SIZE];
    };

    int _capacity; // INDICATES THE TOTAL CAPACITY OF THE TABLE
    int _size; // INDICATES THE NUMBER OF ITEMS IN THE TABLE

    Bucket* _table; // THE HASH TABLE

    // == DEFINE HELPER METHODS & VARIABLES BELOW ==


public:
    // TODO: IMPLEMENT THESE FUNCTIONS.
    // CONSTRUCTORS, ASSIGNMENT OPERATOR, AND THE DESTRUCTOR
    HashTable();
    // COPY THE WHOLE CONTENT OF RHS INCLUDING THE KEYS THAT WERE SET AS DELETED
    HashTable(const HashTable<K, T>& rhs);
    HashTable<K, T>& operator=(const HashTable<K, T>& rhs);
    ~HashTable();

    // TODO: IMPLEMENT THIS FUNCTION.
    // INSERT THE ENTRY IN THE HASH TABLE WITH THE GIVEN KEY & VALUE
    // IF THE GIVEN KEY ALREADY EXISTS, THE NEW VALUE OVERWRITES
    // THE ALREADY EXISTING ONE. IF THE LOAD FACTOR OF THE TABLE IS GREATER THAN 0.6,
    // RESIZE THE TABLE WITH THE NEXT PRIME NUMBER.
    void Insert(const K& key, const T& value);

    // TODO: IMPLEMENT THIS FUNCTION.
    // DELETE THE ENTRY WITH THE GIVEN KEY FROM THE TABLE
    // IF THE GIVEN KEY DOES NOT EXIST IN THE TABLE, THROW ItemNotFoundException()
    void Delete(const K& key);

    // TODO: IMPLEMENT THIS FUNCTION.
    // IT SHOULD RETURN THE VALUE THAT CORRESPONDS TO THE GIVEN KEY.
    // IF THE KEY DOES NOT EXIST, THROW ItemNotFoundException()
    T& Get(const K& key) const;

    // TODO: IMPLEMENT THIS FUNCTION.
    // AFTER THIS FUNCTION IS EXECUTED THE TABLE CAPACITY MUST BE
    // EQUAL TO newCapacity AND ALL THE EXISTING ITEMS MUST BE REHASHED
    // ACCORDING TO THIS NEW CAPACITY.
    // WHEN CHANGING THE SIZE, YOU MUST REHASH ALL OF THE ENTRIES
    void Resize(int newCapacity);

    // THE IMPLEMENTATION OF THESE FUNCTIONS ARE GIVEN TO YOU
    // DO NOT MODIFY!
    int Capacity() const; // RETURN THE TOTAL CAPACITY OF THE TABLE
    int Size() const; // RETURN THE NUMBER OF ACTIVE ITEMS
    void getKeys(K* keys); // PUT THE ACTIVE KEYS TO THE GIVEN INPUT PARAMETER
};


template <class K, class T>
HashTable<K, T>::HashTable() {
    int prime = NextCapacity(0);
    _table = new Bucket[prime];
    _size = 0;
    _capacity = prime;
}

template <class K, class T>
HashTable<K, T>::HashTable(const HashTable<K, T>& rhs) {
    _size = rhs._size;
    _capacity = rhs._capacity;
    _table = new Bucket[_capacity];
    for(int i=0 ; i<BUCKET_SIZE ; ++i){
        for(int j=0 ; j<_capacity ; ++j){
            _table[j].entries[i].Active = rhs._table[j].entries[i].Active;
            _table[j].entries[i].Deleted = rhs._table[j].entries[i].Deleted;
            _table[j].entries[i].Key = rhs._table[j].entries[i].Key;
            _table[j].entries[i].Value = rhs._table[j].entries[i].Value;
        }
    }
}

template <class K, class T>
HashTable<K, T>& HashTable<K, T>::operator=(const HashTable<K, T>& rhs) {
    delete[] _table;
    _size = rhs._size;
    _capacity = rhs._capacity;
    _table = new Bucket[_capacity];
    for(int i=0 ; i<BUCKET_SIZE ; ++i){
        for(int j=0 ; j<_capacity ; ++j){
            _table[j].entries[i].Active = rhs._table[j].entries[i].Active;
            _table[j].entries[i].Deleted = rhs._table[j].entries[i].Deleted;
            _table[j].entries[i].Key = rhs._table[j].entries[i].Key;
            _table[j].entries[i].Value = rhs._table[j].entries[i].Value;
        }
    }
    return *this;
}

template <class K, class T>
HashTable<K, T>::~HashTable() {
    delete[] _table;
}

template <class K, class T>
void HashTable<K, T>::Insert(const K& key, const T& value) {
    if(_size/((double)BUCKET_SIZE*(double)_capacity) > 0.6){
        Resize(NextCapacity(_capacity));
    }
    int current = Hash(key)%_capacity;
    for(int i=1 ; i<=_capacity ; i++){
        for(int j=0 ; j<2 ; ++j){
            if(_table[current].entries[j].Active && _table[current].entries[j].Key == key && !_table[current].entries[j].Deleted){
                _table[current].entries[j].Value = value;
                return;
            }
            if(!_table[current].entries[j].Active || _table[current].entries[j].Deleted){
                _size++;
                _table[current].entries[j].Active = true;
                _table[current].entries[j].Deleted = false;
                _table[current].entries[j].Key = key;
                _table[current].entries[j].Value = value;
                return;
            }
        }
        current = (current + i*i) % _capacity;
    }
}

template <class K, class T>
void HashTable<K, T>::Delete(const K& key) {
    if(_size == 0){
        throw ItemNotFoundException();
    }
    int current = Hash(key)%_capacity;
    for(int i=1 ; i<=_capacity ; i++){
        for(int j=0 ; j<2 ; ++j){
            if(_table[current].entries[j].Active && _table[current].entries[j].Key == key && !_table[current].entries[j].Deleted){
                _table[current].entries[j].Deleted = true;
                _size--;
                return;
            }
            if(!_table[current].entries[j].Active){
                throw ItemNotFoundException();
                return;
            }
        }
        current = (current + i*i) % _capacity;
    }
    throw ItemNotFoundException();
}

template <class K, class T>
T& HashTable<K, T>::Get(const K& key) const {
    if(_size == 0){
        throw ItemNotFoundException();
    }
    int current = Hash(key)%_capacity;
    for(int i=1 ; i<=_capacity ; i++){
        for(int j=0 ; j<2 ; ++j){
            if(_table[current].entries[j].Active && _table[current].entries[j].Key == key && !_table[current].entries[j].Deleted){
                return _table[current].entries[j].Value;
            }
            if(!_table[current].entries[j].Active){
                throw ItemNotFoundException();
            }
        }
        current = (current + i*i) % _capacity;
    }
    throw ItemNotFoundException();
}


template <class K, class T>
void HashTable<K, T>::Resize(int newCapacity) {
    Bucket* old = _table;
    int tmp = _capacity;
    _capacity = newCapacity;
    _size = 0;
    _table = new Bucket[_capacity];
    for(int i=0 ; i<2 ; ++i){
        for(int j=0 ; j<tmp ; ++j){
            if(old[j].entries[i].Active && !old[j].entries[i].Deleted){
                Insert(old[j].entries[i].Key,old[j].entries[i].Value);
            }
        }
    }
    delete[] old;
}



template <class K, class T>
int HashTable<K, T>::Capacity() const {
    return _capacity;
}

template <class K, class T>
int HashTable<K, T>::Size() const {
    return _size;
}


template <class K, class T>
void HashTable<K, T>::getKeys(K* keys) {
    int index = 0;

    for (int i = 0; i < _capacity; i++) {
        Bucket& bucket = _table[i];
        for (int j = 0; j < BUCKET_SIZE; j++) {
            if (bucket.entries[j].Active && !bucket.entries[j].Deleted) {
                keys[index++] = bucket.entries[j].Key;
            }
        }
    }
}

#endif
