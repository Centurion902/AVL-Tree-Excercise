/*****************************
Name: Eli-Henry Dykhne
ID: 0996524
Date 26/11/2018
Assigment name: Assigment 4
Program: A4
******************************/

#include "A4.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
const int TEST = 0;//if 1, we are in testing mode
const int KEY_SIZE = 20;

int main(){
  int command, freq;
  FILE *file;
  char *key, piece;
  int init = 0;
  node *temp = NULL;
  node *root = NULL;
  //int count = 0;

  if(TEST){
    printf("!!Testing Mode!!\n");
    file = fopen("A4_data_test.txt", "r+");/*opens file to read in testing mode*/
  }else{
    file = fopen("A4_data_f18.txt", "r+");/*opens file to read*/
  }

	do{
		printf("1. Initialization\n"
	         "2. Find\n"
	         "3. Insert\n"
	         "4. Remove\n"
	         "5. Check Height and Size\n"
	         "6. Find All (above a given frequency)\n"
	         "7. Exit\n"
	         "Enter a code (1-7) and hit Return\n"
           "avl/> ");
	scanf("%d[^\n]", &command);//gets user input while protecting program from infinite loops in case of bad user input
	getchar();
		
		switch (command){

			case 1:
        init = 1;
        int i = 0;
        key = malloc(sizeof(char)*KEY_SIZE);//TODO ask about max size of keys
        while(!feof(file)){//this piece of code reads in the keys from the files char by char.
          fread(&piece, sizeof(char), 1, file);
          
          if(!isspace(piece)){
            key[i] = piece;
            i++;
          }else{
            key[i] = '\0';
            if(i != 0){
              //count++;
              root = insert(root, key);
              key = malloc(sizeof(char)*KEY_SIZE);//TODO ask about max size of keys
            }
            i = 0;
          }

        }
        free(key);
        fclose(file);
        //printf("%d\n", count);
				break;

			case 2:
        if(!init){
          printf("Error: program not initalized yet\n");
          break;
        }

				printf("find key: ");
        key = malloc(sizeof(char)*KEY_SIZE);//TODO ask about max size of keys
        scanf("%s", key);
        
        temp = findKey(root, key);

       
        if(temp){
          printf("key: %s, frequency: %d\n", temp->key, temp->frequency);
        }else{
          printf("no_such_key\n");
        }
        
        
        free(key);
				break;

			case 3:
        if(!init){
          printf("Error: program not initalized yet\n");
          break;
        }

				printf("insert key: ");
        key = malloc(sizeof(char)*KEY_SIZE);//TODO ask about max size of keys
        scanf("%s", key);
        root = insert(root, key);
				break;

			case 4:
        if(!init){
          printf("Error: program not initalized yet\n");
          break;
        }

        printf("remove key: ");
        key = malloc(sizeof(char)*KEY_SIZE);//TODO ask about max size of keys
        scanf("%s", key);

        root = removeKey(root, key);
        
        free(key);
				break;

			case 5:
        if(!init){
          printf("Error: program not initalized yet\n");
          break;
        }
        printf("height: %d, size: %d, total count: %d\n", height(root), size(root), totalFrequency(root));
				break;
			case 6:
        if(!init){
          printf("Error: program not initalized yet\n");
          break;
        }

        printf("frequency: ");
        scanf("%d", &freq);
        
        printAbove(root, freq);
				break;
      case 7:
        
        break;
			default:
				printf("Please enter a valid command.\n");
				break;
		}
    //for visualizing tree while debugging
    if(TEST){
      display(root,0);
    }
	}while(command != 7);
  freeTree(root);
}

node* insert(node *root, char *key){
  if(root){
    if(!strcmp(root->key,key)){
      root->frequency++;
      free(key);
    }else if(strcmp(root->key,key)>0){
      root->right = insert(root->right, key);

    }else if(strcmp(root->key,key)<0){
      root->left = insert(root->left, key);
      
    }
  }else{
    root = malloc(sizeof(node));
    root->key = key;
    root->frequency = 1;
    root->left = NULL;
    root->right = NULL;
  }
  //rebalancing if nescesary
  root->balance = balanceFactor(root);
  if(root->balance > 1 || root->balance < -1){
    root = balance(root);
  }

  return root;
}

/*
This remove key function is a modified version of a standard
delete function from a binary tree to facilitate balancing afterwards.
It was created with referance to examples provided by the website below.
www.geeksforgeeks.org/binary-search-tree-set-2-delete/amp/
*/
node * removeKey(node *root, char *key){
  node *temp, *par;
  if(root == NULL){
    printf("no_such_key\n");//base case for not finding key
    return root;
  }

  if(strcmp(root->key,key)>0){
    root->right = removeKey(root->right, key);
  }else if(strcmp(root->key,key)<0){
    root->left = removeKey(root->left, key);
  }else{//if we have found the key
    printf("key: %s, frequency: %d\n", root->key, --(root->frequency));//doesnt remove key unless frequency is zero
    if(root->frequency != 0){
      return root;
    }

    if(root->left == NULL){
      temp = root->right;
      free(root->key);//frees key to remove
      free(root);
      return temp;
    }else if(root->right == NULL){
      temp = root->left;
      free(root->key);
      free(root);
      return temp;
    }else{
      temp = root->right;
      par = root;
      while(temp->left != NULL){//finds succesor
        par = temp;
        temp = temp->left;
      }
      

      strcpy(root->key,temp->key);//copies values of succesor into root
      root->frequency = temp->frequency;

      
      if(temp == par->left){
        par->left = NULL;
      }else{
        par->right = NULL;
      }
      free(temp->key);//deletes old node of succesor
      free(temp);
      

      
    }

    root->balance = balanceFactor(root);
    if(root->balance > 1 || root->balance < -1){
      root = balance(root);
    }
  }
  return root;
}

node * findKey(node *root, char *key){
  if(root){
    if(strcmp(root->key,key)>0){
      return findKey(root->right, key);
    }else if(strcmp(root->key,key)<0){
      return findKey(root->left, key);  
    }
  }
  return root;
}

int balanceFactor(node *root){
  return height(root->left)-height(root->right);
}

node * balance(node *root){
  node *nodes[7];
  node *x = root, *y, *z;
  if(x->balance > 0){
    y = x->left;

    if(y->balance > 0){//finds what configuration we are in and assigns x,y,z and subtrees acordingly
      z = y->left;

      nodes[1] = z;
      nodes[3] = y;
      nodes[5] = x;

      nodes[0] = z->left;
      nodes[2] = z->right;
      nodes[4] = y->right;
      nodes[6] = x->right;
    }else{
      z = y->right;

      nodes[1] = y;
      nodes[3] = z;
      nodes[5] = x;

      nodes[0] = y->left;
      nodes[2] = z->left;
      nodes[4] = z->right;
      nodes[6] = x->right;
    }
  }else{
    y = x->right;

    if(y->balance > 0){
      z = y->left;

      nodes[1] = x;
      nodes[3] = z;
      nodes[5] = y;

      nodes[0] = x->left;
      nodes[2] = z->left;
      nodes[4] = z->right;
      nodes[6] = y->right;
    }else{
      z = y->right;

      nodes[1] = x;
      nodes[3] = y;
      nodes[5] = z;

      nodes[0] = x->left;
      nodes[2] = y->left;
      nodes[4] = z->left;
      nodes[6] = z->right;
    }
  }
  
  nodes[3]->left = nodes[1];//restructures
  nodes[3]->right = nodes[5];
  nodes[1]->left = nodes[0];
  nodes[1]->right = nodes[2];
  nodes[5]->left = nodes[4];
  nodes[5]->right = nodes[6];

  nodes[1]->balance = balanceFactor(nodes[1]);//updates balance factor
  nodes[3]->balance = balanceFactor(nodes[3]);
  nodes[5]->balance = balanceFactor(nodes[5]);

  return nodes[3];
}

int totalFrequency(node *root){
  if(root){
    return root->frequency + totalFrequency(root->left) + totalFrequency(root->right);
  }
  return 0;
}

int size(node *root){
  if(root){
    return size(root->left) + size(root->right) + 1;
  }
  return 0;
}

int height(node *root){
  int left = 0;//takes the height of the heighest subtree and adds one
  int right = 0;
  if(root == NULL){
    return 0;
  }

  left = height(root->left)+1;
  right = height(root->right)+1;

  if(left>right){
    return left;
  }else{
    return right;
  }

}



void freeTree(node *root){
  if(root != NULL){
    freeTree(root->right);
    freeTree(root->left);
    free(root->key);
    free(root);
  }
}


void display(node *root, int spacing){
  if(root){
   
    display(root->right, spacing+5);//recursive call with push to move lower nodes right

    printf("\n");
    for(int i = 0; i < spacing; i++){//prints horizontal spacing and pushes lower nodes right
      printf(" ");
    }
    printf("%s %d\n", root->key, root->frequency);

    display(root->left, spacing+5);
  }

}

void printAbove(node *root, int freq){
  if(root){//traverses inorder
    if(root->left){
      printAbove(root->left, freq);
    }

    if(root->frequency > freq){
      printf("key: %s, frequency: %d\n", root->key, root->frequency);
    }

    if(root->right){
      printAbove(root->right, freq);
    }
  }
}