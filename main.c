#include <stdio.h>
#include <stdlib.h>

#define MAX 2  // Maximum keys in a node
#define MIN 1  // Minimum keys in a node

typedef struct BTreeNode {
    int val[MAX + 1]; // Maksimal 3 elemen sebelum split
    struct BTreeNode *link[MAX + 2]; // Maksimal 4 children
    int count;
} BTreeNode;

BTreeNode *root;

// Buat node baru
BTreeNode *createNode(int val, BTreeNode *child) {
    BTreeNode *newNode = (BTreeNode *)malloc(sizeof(BTreeNode));
    newNode->val[1] = val;
    newNode->count = 1;
    newNode->link[0] = root;
    newNode->link[1] = child;
    return newNode;
}

// Sisipkan nilai ke dalam node
void addValToNode(int val, int pos, BTreeNode *node, BTreeNode *child) {
    for (int i = node->count; i > pos; i--) {
        node->val[i + 1] = node->val[i];
        node->link[i + 1] = node->link[i];
    }
    node->val[pos + 1] = val;
    node->link[pos + 1] = child;
    node->count++;
}

// Bagi node saat penuh
void splitNode(int val, int *pval, int pos, BTreeNode *node,
               BTreeNode *child, BTreeNode **newNode) {
    int median, i;

    if (pos > MIN)
        median = MIN + 1;
    else
        median = MIN;

    *newNode = (BTreeNode *)malloc(sizeof(BTreeNode));
    for (i = median + 1; i <= MAX; i++) {
        (*newNode)->val[i - median] = node->val[i];
        (*newNode)->link[i - median] = node->link[i];
    }

    (*newNode)->count = MAX - median;
    node->count = median;

    if (pos <= MIN)
        addValToNode(val, pos, node, child);
    else
        addValToNode(val, pos - median, *newNode, child);

    *pval = node->val[node->count];
    (*newNode)->link[0] = node->link[node->count];
    node->count--;
}

// Sisipkan nilai ke node bukan akar
int setValueInNode(int val, int *pval, BTreeNode *node,
                   BTreeNode **child) {
    int pos;
    if (!node) {
        *pval = val;
        *child = NULL;
        return 1;
    }

    if (val < node->val[1])
        pos = 0;
    else {
        for (pos = node->count; (val < node->val[pos] && pos > 1); pos--);
        if (val == node->val[pos]) {
            printf("Duplikat nilai tidak diperbolehkan\n");
            return 0;
        }
    }

    if (setValueInNode(val, pval, node->link[pos], child)) {
        if (node->count < MAX) {
            addValToNode(*pval, pos, node, *child);
        } else {
            splitNode(*pval, pval, pos, node, *child, child);
            return 1;
        }
    }
    return 0;
}

// Fungsi utama untuk insert
void insert(int val) {
    int flag, i;
    BTreeNode *child;

    flag = setValueInNode(val, &i, root, &child);
    if (flag)
        root = createNode(i, child);
}

// Tampilkan B-Tree
void traversal(BTreeNode *node) {
    if (node) {
        for (int i = 0; i < node->count; i++) {
            traversal(node->link[i]);
            printf("%d ", node->val[i + 1]);
        }
        traversal(node->link[node->count]);
    }
}

// Program utama
int main() {
    int values[] = {9, 0, 8, 1, 7, 2, 6, 3, 5, 4};
    int n = sizeof(values) / sizeof(values[0]);

    for (int i = 0; i < n; i++)
        insert(values[i]);

    printf("Traversal hasil akhir B-Tree:\n");
    traversal(root);
    printf("\n");

    return 0;
}