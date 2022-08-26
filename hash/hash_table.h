#ifndef HASH_TABLE_H
#define HASH_TABLE_H

template <typename K, typename V>
class HashTable
{
 public:
  HashTable() {}
  virtual ~HashTable() {}
  // lookup and modifier
  virtual bool Find(const K &key, V &value) = 0;
  virtual bool Remove(const K &key) = 0;
  virtual void Insert(const K &key, const V &value) = 0;
};

#endif
