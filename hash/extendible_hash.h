#ifndef EXTENDIBLE_HASH_H
#define EXTENDIBLE_HASH_H

#include <cstdlib>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "hash_table.h"

template <typename K, typename V>
class ExtendibleHash : public HashTable<K, V>
{
  class Bucket
  {
    explicit Bucket(int depth) : localDepth_(depth){};
    int localDepth_;
    std::map<K, V> items_;
  };

 public:
  // constructor
  ExtendibleHash(size_t size);
  // helper function to generate hash addressing
  size_t HashKey(const K &key) const;
  // helper function to get global & local depth
  int GetGlobalDepth() const;
  int GetLocalDepth(int bucket_id) const;
  int GetNumBuckets() const;
  // lookup and modifier
  bool Find(const K &key, V &value) override;
  bool Remove(const K &key) override;
  void Insert(const K &key, const V &value) override;

 private:
  int getBucketIndex(const K &key) const;
  int globalDepth_;
  size_t bucketMaxSize_;
  int numBuckets_;
  std::vector<std::shared_ptr<Bucket>> bucketTable_;
  std::mutex mutex_;
};

#endif
