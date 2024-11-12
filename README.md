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

5. 主程序演示：
```c
int main() {
    // 创建一个度为3的B树
    // 插入26个字母作为测试数据
    // 然后逐个删除这些字母
    // 每次操作后打印树的结构
}
```

