//
// Created by Christoph Greger on 07.11.25.
//

#include "SplayTree.h"
#include <gtest/gtest.h>

TEST(SplayTreeTest, EmptyTreeOperations) {

    SplayTree tree;

    // Empty tree should not contain any element
    EXPECT_FALSE(tree.contains(10, 1));

    // findSmallestByKey on empty tree returns nullptr
    EXPECT_EQ(tree.findSmallestByKey(10), nullptr);

    // Removing any element from empty tree should return false
    EXPECT_FALSE(tree.remove(10, 1));
    EXPECT_FALSE(tree.removeSmallest(10));
}

TEST(SplayTreeTest, SingleInsertAndRemove) {

    SplayTree tree;

    // Insert a single node
    tree.insert(5, 100);

    // Now tree should contain that node
    EXPECT_TRUE(tree.contains(5, 100));

    // Inserting the same key and id again should work
    tree.insert(5, 100);
    EXPECT_TRUE(tree.contains(5, 100));

    // findSmallestByKey should return the node for the exact key
    Node* result = tree.findSmallestByKey(5);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->key, 5);
    EXPECT_EQ(result->id, 100);

    // Remove the node
    EXPECT_TRUE(tree.remove(5, 100));

    // After removal, tree should be empty
    EXPECT_FALSE(tree.contains(5, 100));
    EXPECT_EQ(tree.findSmallestByKey(5), nullptr);
}

TEST(SplayTreeTest, MultipleInsertUniqueKeys) {

    SplayTree tree;

    // Insert multiple nodes with unique keys
    tree.insert(10, 1);
    tree.insert(20, 1);
    tree.insert(5, 1);

    // All inserted nodes should be found
    EXPECT_TRUE(tree.contains(10, 1));
    EXPECT_TRUE(tree.contains(20, 1));
    EXPECT_TRUE(tree.contains(5, 1));

    // findSmallestByKey for each existing key returns correct node
    Node* n5 = tree.findSmallestByKey(5);
    ASSERT_NE(n5, nullptr);
    EXPECT_EQ(n5->key, 5);
    EXPECT_EQ(n5->id, 1);

    Node* n10 = tree.findSmallestByKey(10);
    ASSERT_NE(n10, nullptr);
    EXPECT_EQ(n10->key, 10);
    EXPECT_EQ(n10->id, 1);

    Node* n20 = tree.findSmallestByKey(20);
    ASSERT_NE(n20, nullptr);
    EXPECT_EQ(n20->key, 20);
    EXPECT_EQ(n20->id, 1);

    // Remove one key and check others remain
    EXPECT_TRUE(tree.remove(10, 1));
    EXPECT_FALSE(tree.contains(10, 1));
    EXPECT_TRUE(tree.contains(5, 1));
    EXPECT_TRUE(tree.contains(20, 1));

    // Removing a non-existent key should not affect existing keys
    EXPECT_FALSE(tree.remove(999, 999));
    EXPECT_TRUE(tree.contains(5, 1));
    EXPECT_TRUE(tree.contains(20, 1));
}

TEST(SplayTreeTest, DuplicateKeysHandling) {

    SplayTree tree;

    // Insert multiple nodes with the same key but different ids
    tree.insert(10, 5);
    tree.insert(10, 2);
    tree.insert(10, 8);

    // Tree should contain all inserted unique pairs
    EXPECT_TRUE(tree.contains(10, 5));
    EXPECT_TRUE(tree.contains(10, 2));
    EXPECT_TRUE(tree.contains(10, 8));

    // findSmallestByKey should return the node with the smallest id for key=10
    Node* smallest = tree.findSmallestByKey(10);
    ASSERT_NE(smallest, nullptr);
    EXPECT_EQ(smallest->key, 10);
    EXPECT_EQ(smallest->id, 2);

    // removeSmallest should remove the (10,2) node
    EXPECT_TRUE(tree.removeSmallest(10));
    EXPECT_FALSE(tree.contains(10, 2));

    // Now the smallest id for key=10 should be 5
    Node* newSmallest = tree.findSmallestByKey(10);
    ASSERT_NE(newSmallest, nullptr);
    EXPECT_EQ(newSmallest->id, 5);

    // Remove a specific key-id pair (not the smallest)
    EXPECT_TRUE(tree.remove(10, 8));
    EXPECT_FALSE(tree.contains(10, 8));

    // The remaining node with key=10 should be (10,5)
    EXPECT_TRUE(tree.contains(10, 5));

    // Remove the last remaining node with key=10
    EXPECT_TRUE(tree.removeSmallest(10));
    EXPECT_FALSE(tree.contains(10, 5));

    // Tree should be empty now for key=10
    EXPECT_EQ(tree.findSmallestByKey(10), nullptr);
}