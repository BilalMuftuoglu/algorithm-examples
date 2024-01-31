#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*
@brief Struct for Hash Table items
*/
typedef struct HASH_ITEM{
    char* username;
    int isDeleted;
}HASH_ITEM;

/*
@brief Finds the numerical value of a given name using Horner's rule
@param username The name whose numerical value will be calculated
@return Numerical value of the given name
*/
int calculateKeyWithHorner(char* username){
    int key = 0;
    int R = 31;
    int L = strlen(username);
    int i;
    for(i=0;i<L;i++){
        key += (int)username[i] * (int)pow(R,L-1-i);
    }
    return abs(key);
}

/*
@brief Converts a given numerical value into a hash index using double hashing and division methods
@param key Numeric value to be converted to hash index
@param i Value starting from 0 and increasing to get the next hash index of the key that can be placed
@param M Size of the Hash Table
@param mode Mode in which the program is run (1 = Normal Mode, 2 = Debug Mode)
@return Calculated hash index of the key
*/
int hashFunction(int key,int i,int M,int mode){
    int h1 = key % M;//Result of first hash function
    int h2 = 1 + (key % (M-2));//Result of second hash function
    int hashIndex = (h1 + i*h2) % M;//Result of double hashing
    if(mode == 2){
        printf("H1 = %d -- H2 = %d -- i = %d -- HashIndex = %d\n",h1,h2,i,hashIndex);
    }
    return hashIndex;
}

/*
@brief Prints the given HashTable
@param hashTable Hash Table to be printed
@param M Size of the Hash Table
@return 
*/
void printHashTable(HASH_ITEM* hashTable, int M){
    printf("\n");
    int i;
    for(i=0;i<M;i++){
        //if(hashTable[i].username != NULL){
            printf("%d: %s (%d)\n",i,hashTable[i].username,hashTable[i].isDeleted);
        //}
    }
}

/*
@brief Prints the given two Hash Tables side by side
@param hashTable1 First Hash Table to be printed
@param hashTable2 Second Hash Table to be printed
@param M Size of the Hash Tables
@return
*/
void printTwoHashTable(HASH_ITEM* hashTable1, HASH_ITEM* hashTable2, int M){
    printf("\n");
    int i;
    for(i=0;i<M;i++){
        printf("%d: %s (%d) || %d: %s (%d)\n",i,hashTable1[i].username,hashTable1[i].isDeleted,i,hashTable2[i].username,hashTable2[i].isDeleted);
    }
}

/*
@brief Removes the given name from the Hash Table
@param hashTable Hash Table to be removed from
@param M Size of the Hash Table
@param username Name to be removed
@param mode Mode in which the program is run (1 = Normal Mode, 2 = Debug Mode)
@return
*/
void removeFromHashTable(HASH_ITEM* hashTable, int M, char* username, int mode, int* counter){
    if(mode == 2){
        printf("\nRemoving %s\n",username);
    }
    int key = calculateKeyWithHorner(username);
    int hashIndex = hashFunction(key,0,M,mode);
    int i = 1;
    while(hashTable[hashIndex].username != NULL){
        if(strcmp(hashTable[hashIndex].username,username) == 0 && hashTable[hashIndex].isDeleted == 0){
            hashTable[hashIndex].isDeleted = 1;
            *counter = *counter - 1;
            if(mode == 2){
                printf("%s was removed from [%d] after %d attempts\n",username,hashIndex,i);
            }else{
                printf("%s was removed from [%d]\n",username,hashIndex);
            }
            return;
        }
        hashIndex = hashFunction(key,i,M,mode);
        i++;
    }
    if(mode == 2){
        printf("%s was not found after %d attempts!\n",username,i);
    }else{
        printf("%s was not found in the table!\n",username);
    }
}

/*
@brief Searches the given name in the Hash Table. If the name is in the table, its index is only found if it has not been deleted.
@param hashTable Hash Table to be searched
@param M Size of the Hash Table
@param username Name to be searched
@param mode Mode in which the program is run (1 = Normal Mode, 2 = Debug Mode)
@return Index of the name in the Hash Table (-1 = Not Found)
*/
int searchInHashTable(HASH_ITEM* hashTable, int M, char* username,int mode){
    if(mode == 2){
        printf("\nSearching %s\n",username);
    }

    int key = calculateKeyWithHorner(username);
    int hashIndex = hashFunction(key,0,M,mode);
    int i = 1;
    while(hashTable[hashIndex].username != NULL){
        if(strcmp(hashTable[hashIndex].username,username) == 0 && hashTable[hashIndex].isDeleted == 0){
            if(mode == 2){
                printf("%s was found in [%d] after %d attempts\n",username,hashIndex,i);
            }
            return hashIndex;
        }
        hashIndex = hashFunction(key,i,M,mode);
        i++;
    }
    if(mode == 2){
        printf("%s was not found after %d attempts!\n",username,i);
    }
    return -1;
}

/*
@brief Searches the given name in the Hash Table. If the name is in the table, its index is returned whether it is deleted or not.
@param hashTable Hash Table to be searched
@param M Size of the Hash Table
@param username Name to be searched
@return Index of the name in the Hash Table (-1 = Not Found)
*/
int searchInHashTableAll(HASH_ITEM* hashTable, int M, char* username){
    int key = calculateKeyWithHorner(username);
    int hashIndex = hashFunction(key,0,M,1);
    int i = 1;
    while(hashTable[hashIndex].username != NULL){
        if(strcmp(hashTable[hashIndex].username,username) == 0){
            //If the name is in the table, its index is returned whether it is deleted or not.
            return hashIndex;
        }
        hashIndex = hashFunction(key,i,M,1);
        i++;
    }
    return -1;
}

/*
@brief Inserts the given name to the Hash Table
@param hashTable Hash Table to be inserted
@param M Size of the Hash Table
@param username Name to be inserted
@param mode Mode in which the program is run (1 = Normal Mode, 2 = Debug Mode)
@return
*/
void insertToHashTable(HASH_ITEM* hashTable, int M, char* username,int mode, int* counter, int N){
	
    if(*counter == N){
        printf("Maximum number of elements reached!\n");
        return;
    }

    if(mode == 2){
        printf("\nInserting %s\n",username);
    }
    
    //The search is performed before the name is inserted to the hash table
    int hashIndex = searchInHashTableAll(hashTable,M,username);
    if (hashIndex != -1 && hashTable[hashIndex].isDeleted == 0){
        //If the name exists in the table and has not been deleted, it will not be inserted.
        printf("%s is already in the table!\n",username);
        return;
    }else if(hashIndex != -1 && hashTable[hashIndex].isDeleted == 1){
        //If the name exists in the table but has been deleted, it will be inserted to the same index.
        hashTable[hashIndex].isDeleted = 0;
        if(mode == 1){
            printf("%s was inserted to %d\n",username,hashIndex);
        }else{
            printf("The previously deleted %s was re-inserted to %d!\n",username,hashIndex);    
        }
        return;
    }
	
    //If the name does not exist in the table, it will be inserted to the first empty index.
    int key = calculateKeyWithHorner(username);
    hashIndex = hashFunction(key,0,M,mode);
    int i = 1;
    while(hashTable[hashIndex].username != NULL && hashTable[hashIndex].isDeleted == 0){
        hashIndex = hashFunction(key,i,M,mode);
        i++;
    }
    
    if(hashTable[hashIndex].isDeleted == 0){
        //A suitable index with no element inserted before was found
        hashTable[hashIndex].username = (char*) malloc(sizeof(char)*(strlen(username) + 1));
        if(hashTable[hashIndex].username == NULL){
            printf("Memory allocation error!");
            exit(1);
        }
        *counter = *counter + 1;
        strcpy(hashTable[hashIndex].username,username);
    }else{
        //A suitable index with a deleted element was found
        strcpy(hashTable[hashIndex].username,username);
        hashTable[hashIndex].isDeleted = 0;
        *counter = *counter + 1;
    }
    if(mode == 1){
        printf("%s was inserted to [%d]\n",username,hashIndex);
    }else{
        printf("%s was inserted to [%d] after %d attempts\n",username,hashIndex,i);
    }
    
}

/*
@brief Rehashes the given Hash Table using undeleted elements
@param hashTable Hash Table to be rehashed
@param M Size of the Hash Table
@param mode Mode in which the program is run (1 = Normal Mode, 2 = Debug Mode)
@return New Hash Table
*/
HASH_ITEM* rearrange(HASH_ITEM* hashTable, int M, int mode, int* counter, int N){
    
    HASH_ITEM* newHashTable = (HASH_ITEM*) malloc(sizeof(HASH_ITEM)*M);
    if(newHashTable == NULL){
        printf("Memory allocation error!");
        exit(1);
    }

	int i;
	for(i=0;i<M;i++){
    	newHashTable[i].username = NULL;
        newHashTable[i].isDeleted = 0;
    }
    
    for(i=0;i<M;i++){
        //Names that were not deleted in the old table are inserted to the new table
        if(hashTable[i].isDeleted == 0 && hashTable[i].username != NULL){
            insertToHashTable(newHashTable,M,hashTable[i].username,mode, counter, N);
        }
    }
    if(mode == 2){
        printTwoHashTable(hashTable,newHashTable,M);
    }
    
    free(hashTable);
    return newHashTable;
}

int main(){
    int N;//Maximum number of elements
    int M;//Size of hash table
    int choice;//transaction number to be selected
    float loadFactor;//Load Factor of hash table
    char *username;
    int mode;//Program mode
    int counter = 0;//Number of names in the table that have not been deleted

    printf("Enter 1 to run in normal mode and 2 for debug mode(1/2): ");
    scanf("%d",&mode);
    printf("Enter the maximum number of elements(N): ");
    scanf("%d",&N);
    printf("Enter the Load Factor(alpha): ");
    scanf("%f",&loadFactor);
    printf("Enter the table size(M): ");
    scanf("%d",&M);

    //Hash Table Initialization
    HASH_ITEM* hashTable = (HASH_ITEM*) malloc(sizeof(HASH_ITEM)*M);
    if(hashTable == NULL){
        printf("Memory allocation error!");
        exit(1);
    }

    int i;
    for(i=0;i<M;i++){
    	hashTable[i].username = NULL;
        hashTable[i].isDeleted = 0;
    }
    
    //Fill the table with some names
    insertToHashTable(hashTable,M,"bilal",mode,&counter,N);
    insertToHashTable(hashTable,M,"mustafa",mode,&counter,N);
    insertToHashTable(hashTable,M,"ali",mode,&counter,N);
    insertToHashTable(hashTable,M,"mehmet",mode,&counter,N);
    insertToHashTable(hashTable,M,"veli",mode,&counter,N);
    insertToHashTable(hashTable,M,"ayse",mode,&counter,N);
    insertToHashTable(hashTable,M,"fatma",mode,&counter,N);
    
    printHashTable(hashTable,M);

    while(1){
        printf("\n\n1-Insert\n2-Search\n3-Remove\n4-Print\n5-Rearrange\n6-Exit\n\n");
        scanf("%d",&choice);
		username = (char*) malloc(sizeof(char)*30);
		
        switch(choice){
            case 1:
                printf("Enter the user name: ");
                scanf("%s",username);
                insertToHashTable(hashTable,M,username,mode,&counter,N);
                break;

            case 2:
                printf("Enter the user name: ");
                scanf("%s",username);
                int index = searchInHashTable(hashTable,M,username,mode);
                if(mode == 1){
                    if(index != -1){
                        printf("%s was found in %d\n",username,index);
                    }else{
                        printf("%s was not found in the table!\n",username);
                    }
                }else{
                    printHashTable(hashTable, M);
                }
                break;

            case 3:
                printf("Enter the user name: ");
                scanf("%s",username);
                removeFromHashTable(hashTable,M,username,mode,&counter);
                break;

            case 4:
                printHashTable(hashTable,M);
                break;
            
            case 5:
                counter = 0;
                hashTable = rearrange(hashTable,M,mode,&counter,N);
                break;
            
            default:
                return 0;
                break;
            }
    }
}
