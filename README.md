# btree

B树是一种平衡的多路搜索树，常用于数据库和文件系统。以下是B树的主要特点：

1. 基本性质：
- 每个节点最多有m个子节点（m称为B树的阶）
- 除根节点和叶节点外，每个节点至少有⌈m/2⌉个子节点
- 所有叶节点都在同一层
- 每个节点包含k个关键字，其中k的范围是⌈m/2⌉-1 ≤ k ≤ m-1

2. 关键特征：
- 自平衡
- 每个节点存储多个关键字
- 适合磁盘存储和大量数据的查询

3. 主要操作时间复杂度：
- 查找：O(log n)
- 插入：O(log n)
- 删除：O(log n)

4. 应用场景：
- 数据库索引
- 文件系统（如Linux ext4）
- 需要频繁读写大量数据的系统


## c 实现


1. 基本结构定义：
```c
#define DEGREE 3  // B树的度为3
typedef int KEY_VALUE;

// B树节点结构
typedef struct _btree_node {
    KEY_VALUE *keys;         // 键值数组
    struct _btree_node **childrens;  // 子节点指针数组
    int num;                 // 当前节点中键值的数量
    int leaf;               // 是否为叶子节点
} btree_node;

// B树结构
typedef struct _btree {
    btree_node *root;       // 根节点指针
    int t;                  // 最小度数(minimum degree)
} btree;
```

2. 核心操作实现：

a. 插入操作：
```c
void btree_insert(btree *T, KEY_VALUE key) {
    btree_node *r = T->root;
    // 如果根节点已满，需要分裂
    if (r->num == 2 * T->t - 1) {
        btree_node *node = btree_create_node(T->t, 0);
        T->root = node;
        node->childrens[0] = r;
        btree_split_child(T, node, 0);
        
        int i = 0;
        if (node->keys[0] < key) i++;
        btree_insert_nonfull(T, node->childrens[i], key);
    } else {
        btree_insert_nonfull(T, r, key);
    }
}
```

b. 删除操作：
```c
void btree_delete_key(btree *T, btree_node *node, KEY_VALUE key) {
    // 找到key的位置
    int idx = 0;
    while (idx < node->num && key > node->keys[idx]) {
        idx++;
    }

    // 如果找到key
    if (idx < node->num && key == node->keys[idx]) {
        // 如果是叶子节点，直接删除
        if (node->leaf) {
            // 删除逻辑...
        } else {
            // 非叶子节点的删除逻辑...
        }
    } else {
        // 递归到子节点删除
        // 处理节点最小度数问题...
    }
}
```

3. 辅助功能：

a. 节点分裂：
```c
void btree_split_child(btree *T, btree_node *x, int i) {
    // 将一个满节点分裂成两个节点
    btree_node *y = x->childrens[i];
    btree_node *z = btree_create_node(t, y->leaf);
    // 分裂逻辑...
}
```

b. 节点合并：
```c
void btree_merge(btree *T, btree_node *node, int idx) {
    // 合并两个子节点
    btree_node *left = node->childrens[idx];
    btree_node *right = node->childrens[idx+1];
    // 合并逻辑...
}
```

4. 特点：

- 维护了B树的所有性质：
  - 节点的键值个数限制
  - 除根节点外所有节点至少半满
  - 所有叶子节点在同一层

- 包含完整的插入、删除、遍历操作
- 实现了节点分裂和合并操作来维护B树平衡
- 使用了二分查找来优化节点内的查找操作



## cpp 实现


模板化的B树(B-Tree)实现，支持基本的插入、删除、搜索操作。B树是一种自平衡的搜索树，广泛应用于数据库和文件系统中。

### 特性
- 模板化实现，支持任意可比较类型
- 自平衡特性
- 支持插入、删除、搜索操作
- 异常安全
- 使用智能指针管理内存

### B树的基本概念
B树是一种多路搜索树，具有以下性质：
1. 每个节点最多有`2t-1`个键值（t为最小度数）
2. 除根节点外，每个节点至少有`t-1`个键值
3. 所有叶子节点都在同一层
4. 节点内的键值按升序排列

#### 可视化示例
```
假设 t (最小度数) = 3：

         [30,60]                  根节点
        /   |   \
   [10,20] [40,50] [70,80,90]    子节点
```

### 主要操作

#### 1. 插入操作
```cpp
void insert(const T& key);
```
插入过程：
1. 检查根节点是否已满
2. 如果已满，分裂根节点
3. 递归插入新键值

示例：
```
插入40到节点 [10,20,30,50]
结果: [10,20,30,40,50]
```

#### 2. 删除操作
```cpp
void remove(const T& key);
```
删除过程包括以下情况：
- 从叶子节点删除
- 从内部节点删除
- 需要合并节点的情况

#### 3. 搜索操作
```cpp
std::optional<T> search(const T& key) const;
```
返回一个optional，包含找到的值或空值。

### 代码示例

```cpp
// 创建一个整数B树，最小度数为3
BTree<int> tree(3);

// 插入一些值
tree.insert(10);
tree.insert(20);
tree.insert(30);

// 搜索值
auto result = tree.search(20);
if (result) {
    std::cout << "Found: " << *result << std::endl;
}

// 删除值
tree.remove(20);
```

### 性能特性
- 搜索时间复杂度: O(log n)
- 插入时间复杂度: O(log n)
- 删除时间复杂度: O(log n)

其中n为树中的节点总数。

### 节点结构
每个节点包含：
- 键值数组 (`std::vector<T> keys`)
- 子节点指针数组 (`std::vector<std::shared_ptr<Node>> children`)
- 叶子节点标志 (`bool leaf`)

### 实现细节

#### 分裂操作
当节点满时（包含2t-1个键），分裂为两个节点：
```
分裂前:  [A B C D E]  (假设t=3)
分裂后:  [C]
       /     \
    [A B]   [D E]
```

#### 合并操作
当节点键值数量过少时，可能需要与兄弟节点合并：
```
合并前:  [B]
       /     \
    [A]     [C]
    
合并后:  [A B C]
```
