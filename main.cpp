#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 19

typedef struct Customer {
    char name[MAXLEN + 1];
    int points;
} Customer;

typedef struct TreeNode {
    Customer* customerPtr;
    int size;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// Function declarations
TreeNode* insert(TreeNode* root, Customer* customer);
TreeNode* search(TreeNode* root, const char* name, int* depth);
TreeNode* deleteNode(TreeNode* root, const char* key);
TreeNode* findMax(TreeNode* node);
int countSmaller(TreeNode* root, const char* name);
void deleteTree(TreeNode* root);
void fillArrayInOrder(TreeNode* root, Customer** customerArray, int* index);

int main() {
    int n;
    scanf("%d", &n);

    TreeNode* root = NULL;
    Customer** customerArray = (Customer**)malloc(n * sizeof(Customer*));

    int commandCount = 0;

    for (int i = 0; i < n; i++) {
        // printf("Iteration %d\n", commandCount + 1);

        char command[10];
        scanf("%s", command);

        if (strcmp(command, "add") == 0) {
            char name[MAXLEN + 1];
            int points;
            scanf("%s %d", name, &points);
            Customer* customer = (Customer*)malloc(sizeof(Customer));
            strcpy(customer->name, name);
            customer->points = points;
            root = insert(root, customer);
            printf("%s %d\n", customer->name, customer->points);
            customerArray[i] = customer;
        } else if (strcmp(command, "sub") == 0) {
            char name[MAXLEN + 1];
            int points;
            scanf("%s %d", name, &points);
            int depth = 0;
            TreeNode* result = search(root, name, &depth);
            if (result != NULL) {
                int remainingPoints = (result->customerPtr->points > points) ? result->customerPtr->points - points : 0;
                result->customerPtr->points = remainingPoints;
                printf("%s %d\n", result->customerPtr->name, result->customerPtr->points);
            } else {
                printf("%s not found\n", name);
            }
        } else if (strcmp(command, "del") == 0) {
            char name[MAXLEN + 1];
            scanf("%s", name);
            int depth = 0;
            TreeNode* result = search(root, name, &depth);
            if (result != NULL) {
                printf("%s deleted\n", result->customerPtr->name);
                root = deleteNode(root, result->customerPtr->name);
            } else {
                printf("%s not found\n", name);
            }
        } else if (strcmp(command, "search") == 0) {
            char name[MAXLEN + 1];
            scanf("%s", name);
            int depth = 0;
            TreeNode* result = search(root, name, &depth);
            if (result != NULL) {
                printf("%s %d %d\n", result->customerPtr->name, result->customerPtr->points, depth);
            } else {
                printf("%s not found\n", name);
            }
        } else if (strcmp(command, "count_smaller") == 0) {
            char name[MAXLEN + 1];
            scanf("%s", name);
            int count = countSmaller(root, name);
            printf("%d\n", count);
        }

        commandCount++;
        if (commandCount >= n) {
            break; // Exit the loop if the limit is reached
        }
    }

    int sortedIndex = 0;
    Customer** sortedArray = (Customer**)malloc(n * sizeof(Customer*));
    fillArrayInOrder(root, sortedArray, &sortedIndex);

    // Print the sorted array
    for (int i = 0; i < n; ++i) {
        printf("%s %d\n", sortedArray[i]->name, sortedArray[i]->points);
    }

    // Clean up
    deleteTree(root);
    free(customerArray);
    free(sortedArray);

    return 0;
}

// Inserting into the binary tree where it belongs
TreeNode* insert(TreeNode* root, Customer* customer) {
    if (root == NULL) {
        TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
        newNode->customerPtr = customer;
        newNode->size = 1;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    int cmp = strcmp(customer->name, root->customerPtr->name);

    if (cmp < 0) {
        root->left = insert(root->left, customer);
    } else if (cmp > 0) {
        root->right = insert(root->right, customer);
    } else {
        // Customer already exists, update points
        root->customerPtr->points += customer->points;
    }

    // Update size of the current node
    root->size = 1 + (root->left ? root->left->size : 0) + (root->right ? root->right->size : 0);

    return root;
}

// Function to find the node with the maximum value in a given tree
TreeNode* findMax(TreeNode* node) {
    while (node->right != NULL) {
        node = node->right;
    }
    return node;
}

// Function to delete a node with a given key from the BST
TreeNode* deleteNode(TreeNode* root, const char* key) {
    if (root == NULL) {
        return root;
    }

    int cmp = strcmp(key, root->customerPtr->name);

    if (cmp < 0) {
        root->left = deleteNode(root->left, key);
    } else if (cmp > 0) {
        root->right = deleteNode(root->right, key);
    } else {
        // Node with only one child or no child
        if (root->left == NULL) {
            TreeNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            TreeNode* temp = root->left;
            free(root);
            return temp;
        }

        // Node with 2 children
        TreeNode* temp = findMax(root->left);

        // Copy in order succession
        strcpy(root->customerPtr->name, temp->customerPtr->name);
        root->customerPtr->points = temp->customerPtr->points;

        // Delete the successor
        root->left = deleteNode(root->left, temp->customerPtr->name);
    }

    // Update the size of the current node
    root->size = 1 + (root->left ? root->left->size : 0) + (root->right ? root->right->size : 0);

    return root;
}


TreeNode* search(TreeNode* root, const char* name, int* depth) {
    if (root == NULL) {
        return NULL;
    }

    int cmp = strcmp(name, root->customerPtr->name);

    if (cmp == 0) {
        return root;
    } else if (cmp < 0) {
        (*depth)++;
        return search(root->left, name, depth);
    } else {
        (*depth)++;
        return search(root->right, name, depth);
    }
}

int countSmaller(TreeNode* root, const char* name) {
    if (root == NULL) {
        return 0;
    }

    int cmp = strcmp(name, root->customerPtr->name);

    if (cmp <= 0) {
        return countSmaller(root->left, name);
    } else {
        return 1 + (root->left ? root->left->size : 0) + countSmaller(root->right, name);
    }
}

void deleteTree(TreeNode* root) {
    if (root != NULL) {
        deleteTree(root->left);
        deleteTree(root->right);
        free(root->customerPtr);
        free(root);
    }
}

void fillArrayInOrder(TreeNode* root, Customer** customerArray, int* index) {
    if (root == NULL) {
        return;
    }

    // Traverse the tree in descending order
    fillArrayInOrder(root->right, customerArray, index);

    // Fill the array with the current customer
    customerArray[(*index)++] = root->customerPtr;

    // Continue the traversal
    fillArrayInOrder(root->left, customerArray, index);
}

