#include "extendible_hash.h"

/*
 * constructor
 * array_size: fixed array size for each bucket
 */
template <typename K, typename V>
ExtendibleHash<K, V>::ExtendibleHash(size_t size) : globalDepth_(0), bucketMaxSize_(size), numBuckets_(0)
{
  bucketTable_.push_back(std::make_shared<Bucket>(0));
}

/*
 * helper function to calculate the hashing address of input key
 */
template <typename K, typename V>
size_t ExtendibleHash<K, V>::HashKey(const K &key) const
{
  return std::hash<K>{}(key);
}

/*
 * helper function to return global depth of hash table
 * NOTE: you must implement this function in order to pass test
 */
template <typename K, typename V>
int ExtendibleHash<K, V>::GetGlobalDepth() const
{
  return globalDepth_;
}

/*
 * helper function to return local depth of one specific bucket
 * NOTE: you must implement this function in order to pass test
 */
template <typename K, typename V>
int ExtendibleHash<K, V>::GetLocalDepth(int bucket_id) const
{
  return bucketTable_[bucket_id]->localDepth_;
}

/*
 * helper function to return current number of bucket in hash table
 */
template <typename K, typename V>
int ExtendibleHash<K, V>::GetNumBuckets() const
{
  return numBuckets_;
}

/*
 * lookup function to find value associate with input key
 */
template <typename K, typename V>
bool ExtendibleHash<K, V>::Find(const K &key, V &value)
{
  // https://en.cppreference.com/w/cpp/thread/mutex
  std::lock_guard<std::mutex> guard(mutex_);

  auto index = getBucketIndex(key);
  std::shared_ptr<Bucket> bucket = bucketTable_[index];
  if (bucket != nullptr && bucket->items.find(key) != bucket->items.end())
  {
    value = bucket->items[key];
    return true;
  }
  return false;
}

/*
 * delete <key,value> entry in hash table
 */
template <typename K, typename V>
bool ExtendibleHash<K, V>::Remove(const K &key)
{
  std::lock_guard<std::mutex> guard(mutex_);

  auto index = getBucketIndex(key);
  std::shared_ptr<Bucket> bucket = bucketTable_[index];

  if (bucket == nullptr || bucket->items.find(key) == bucket->items.end())
  {
    return false;
  }
  // TODO: Shrink & Combination
  bucket->items.erase(key);
  return true;
}

template <typename K, typename V>
int ExtendibleHash<K, V>::getBucketIndex(const K &key) const
{
  // mask = (1 << globalDepth_) - 1

  // egg: globalDepth_ = 1
  // hash code      110100000001
  // mask           000000000001
  //                      &
  // index          000000000001

  return HashKey(key) & ((1 << globalDepth_) - 1);
}

/*
 * insert <key,value> entry in hash table
 * Split & Redistribute bucket when there is overflow and if necessary increase
 * global depth
 */
template <typename K, typename V>
void ExtendibleHash<K, V>::Insert(const K &key, const V &value)
{
  std::lock_guard<std::mutex> guard(mutex_);

  auto index = getBucketIndex(key);
  std::shared_ptr<Bucket> targetBucket = bucketTable_[index];

  // 设globalDepth_为 n，可以理解为虚拟扩容了 n 次，每次扩容都是原来的2倍
  // 最多可能存在 2 ^ n 个bucket address指向同一块bucket

  // 设localDepth_为 m， 可以理解为实际扩容了 m 次，每次扩容都是原来的2倍
  // 说明在 2 ^ n 个的 bucket address内，实际上有 2 ^ m 个bucket address指向同一块bucket
  // 剩余 2 ^ ( n - m )个bucket address指向的是同一块bucket

  // 当 m == n 时，说明每个bucket address都指向唯一的一块bucket
  while (targetBucket->items.size() == bucketMaxSize_)
  {
    if (targetBucket->localDepth == globalDepth_)
    {
      size_t length = bucketTable_.size();
      // bucketTable_扩大一倍，由于bucketTable_存的是智能指针，所以有多个不同指针指向相同的bucket
      for (size_t i = 0; i < length; i++)
      {
        bucketTable_.push_back(bucketTable_[i]);
      }
      // 全局深度增加
      globalDepth_++;
    }

    // 注意这里用的是localDepth_
    int mask = 1 << targetBucket->localDepth_;

    auto zeroBucket = std::make_shared<Bucket>(targetBucket->localDepth_ + 1);
    auto oneBucket = std::make_shared<Bucket>(targetBucket->localDepth_ + 1);
    // 重新把targetBucket分配到两个新增的bucket
    for (auto item : targetBucket->items)
    {
      size_t hashkey = HashKey(item.first);
      if (hashkey & mask)
      {
        oneBucket->items.insert(item);
      }
      else
      {
        zeroBucket->items.insert(item);
      }
    }

    for (size_t i = 0; i < bucketTable_.size(); i++)
    {
      if (bucketTable_[i] == targetBucket)
      {
        // 原本i & mask 为
        // 根据localDepth_的mask，释放原有的targetBucket
        if (i & mask)
        {
          bucketTable_[i] = oneBucket;
        }
        else
        {
          bucketTable_[i] = zeroBucket;
        }
      }
    }

    index = getBucketIndex(key);
    targetBucket = bucketTable_[index];
  }  // end while

  targetBucket->items[key] = value;
}
