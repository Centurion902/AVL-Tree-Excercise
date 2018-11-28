/*****************************
Name: Eli-Henry Dykhne
ID: 0996524
Date 26/11/2018
Assigment name: Assigment 4
Program: A4
******************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

#ifndef A4_H
#define A4_H

typedef struct node{
    char *key;
    int balance;
    int frequency;
    struct node *left;
    struct node *right;
} node;

/*
Function: Traverses tree inorder and prints all keys with frequency higher than given freq.
*/
void printAbove(node *root, int freq);

/*
Function: Finds specific key in tree.
*/
node * findKey(node *root, char *key);

/*
Function: decrements frequency of key. if cannot find it prints no_such_key. 
If frequency is zero, removes from avl tree and rebalances. Then returns new root.
*/
node * removeKey(node *root, char *key);

/*
Function: finds balance factor of a node
*/
int balanceFactor(node *root);

/*
Function: Balances the tree and returns new root of subtree
*/
node * balance(node *root);

/*
Function: returns sum of all the frequencies of nodes in the tree
*/
int totalFrequency(node *root);

/*
Function: returns number of nodes in tree
*/
int size(node *root);

/*
Function: returns the height of a node
*/
int height(node *root);

/*
Function; Inserts key and balances tree
*/
node* insert(node *root, char *key);

/*
Function: prints tree
*/
void display(node *root, int spacing);


/*
Function: frees tree recursively
*/
void freeTree(node *root);


#endif
