#include <iostream>
#include <string.h>

#define TRIE_MAX 26
#define QUEUE_MAX 10000

typedef struct Trie
{
  struct Trie* next[TRIE_MAX];  //指针数组
  int pass; // 标记该节点有多少个相同前缀的节点
  int end;  //标记该节点加入过多少次，或者说有多少个以该节点结尾的单词
}Trie;

void init(struct Trie** p)  // 注意这里得用二级指针
{
  *p = (struct Trie* )malloc(sizeof(struct Trie));
  (*p)->pass = 0;
  (*p)->end = 0;
  memset((*p)->next, NULL, sizeof((*p)->next));  //这里的sizeof大小为26*4
}

// 正常插入，生成节点
void insert(struct Trie* cur, const char *s)
{
  if(cur == NULL || s == NULL)
  {
    return;
  }
  cur->pass++;
  int len = strlen(s);
  for(int i = 0; i < len; i++)
  {
    int t = s[i] - 'a';
    if(cur->next[t] == NULL)
    {
      init(&cur->next[t]);
    }
    cur = cur->next[t];
    cur->pass++;
  }
  cur->end++; // 以该节点为结尾的end
}

// 搜索这个单词之前加入过几次
int search(struct Trie* cur,const char *s)
{
  if(s == NULL)
  {
    return 0;
  }
  int len = strlen(s);
  for(int i = 0; i < len; i++)
  {
    int t = s[i] - 'a';
    if(cur->next[t] == NULL)  // 说明这个单词不完整
    {
      return 0;
    }
    cur = cur->next[t];
  }
  return cur->end;
}

void bfs_free(struct Trie* cur)
{
  // 广度优先遍历
  Trie *queue[QUEUE_MAX];
  int head = 0, tail = 0;       // 队列头尾指针
  memset(queue, NULL, sizeof(queue));
  Trie *node = cur;

  queue[tail++] = node;         // 头节点入队
  while(head != tail)
  {
    node = queue[head++];     // 节点出队
    for (int i = 0; i < TRIE_MAX; i++)
    {
      if (tail < QUEUE_MAX && node->next[i] != NULL)
      {
        queue[tail++] = node->next[i];
      }
    }
  }
  int i = 0;
  while(i < tail && queue[i] != NULL)
  {
    free(queue[i++]);
  }
}

void del(struct Trie* cur, const char *s)
{
  if(search(cur, s) != 0)
  {
    cur->pass--;
    int len = strlen(s);
    for(int i = 0; i < len; i++)
    {
      int t = s[i] - 'a';
      if(--cur->next[t]->pass == 0) // 直接删除后续所有节点
      {
        bfs_free(cur->next[t]);
        cur->next[t] = NULL;
        return;
      }
      cur = cur->next[t];
    }
    cur->end--;  // 删除这个单词
  }
}

// 所有加入的字符串中，有几个是以s这个字符串作为前缀的
int prefixNumber(struct Trie* cur, char* s) {
  if(s == NULL)
  {
    return 0;
  }
  int len = strlen(s);
  for(int i = 0; i < len; i++)
  {
    int t = s[i] - 'a';
    if(cur->next[t] == NULL)  // 说明这个单词不完整
    {
      return 0;
    }
    cur = cur->next[t];
  }
  return cur->pass;
}
