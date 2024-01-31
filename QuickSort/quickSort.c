#include <stdio.h> 
#include <time.h>
#include <stdlib.h>

/*
@brief swaps two numbers in an array

@param num1 first number in swap
@param num2 second number in swap

@return
*/
void swap(int *num1, int *num2){ 
    int temp = *num1; 
    *num1 = *num2; 
    *num2 = temp; 
    return;
}

/*
@brief a function that prints the given array

@param arr array to be printed
@param N size of the array

@return
*/
void printArray(int arr[],int N) { 
    int i;
    for(i = 0; i < N; i++) { 
        printf("%d ", arr[i]); 
    } 
    printf("\n"); 
    return;
} 
  
/*
@brief a function that partitions the array. Unlike standard partition, the pivot is given beforehand

@param arr array to be partitioned
@param left lower index of arr
@param right higher index of arr
@param pivot pivot element

@return index where the pivot is placed
*/
int partition(int arr[], int left, int right, int pivot){ 
    int i = left; 
    int j; 
      
    for(j = left; j < right; j++){
        //Place elements smaller than the pivot on the left side and larger elements on the right side
        if (arr[j] < pivot) { 
            swap(&arr[i],&arr[j]);
            i++;
        }else if(arr[j] == pivot){ 
            swap(&arr[j],&arr[right]);//place the pivot at the end of the array
            j--; 
        }
    }  
    
    swap(&arr[i],&arr[right]);//Put the rightmost pivot element where it should be

    return i;// index of pivot
} 
  
/*
@brief a function that matches the pairs of locks and keys

@param locks array of locks
@param keys array of keys
@param left lower index
@param right higher index

@return
*/
void matchPairs(int locks[], int keys[], int left, int right){ 

    if (left < right){ 
        //select the pivot from the end of the keys and sort the locks according to this pivot
        int pivot = partition(locks, left, right, keys[right]); 
        //sort the keys according to the pivot that is placed in the index returned from the previous partition
        partition(keys, left, right, locks[pivot]); 
  
        //Recursively sort the rest of the array
        matchPairs(locks, keys, left, pivot - 1); 
        matchPairs(locks, keys, pivot + 1, right); 
    }
    return;
}

int main() { 
    srand(time(NULL));

    int N,i,left,right,option;

    printf("Enter N number: ");
    scanf("%d", &N);//size of arrays
    
    int locks[N],keys[N];

    printf("\n1- Enter the arrays yourself");
    printf("\n2- Generate random arrays");
    printf("\nChoose an option (1/2): ");
    scanf("%d",&option);//array creation option

    switch(option){
        case 1:
            printf("\nEnter locks array: ");
            for(i = 0; i < N; i++){
                scanf("%d",&locks[i]);
            }
            printf("Enter keys array: ");
            for(i = 0; i < N; i++){
                scanf("%d",&keys[i]);
            }
            break;

        case 2:
            //filling lock and key arrays with numbers from 0 to N-1
            for(i = 0; i < N; i++) { 
                locks[i] = i; 
                keys[i] = i; 
            }
            
            //Shuffling the locks
            for(i = 0; i < N; i++) { 
                int randomIndex = rand() % N;
                swap(&locks[i],&locks[randomIndex]);
            }
            //Shuffling the keys
            for(i = 0; i < N; i++){
                int randomIndex = rand() % N;
                swap(&keys[i],&keys[randomIndex]);
            }
            // Printing unsorted arrays
            printf("\nLocks array: ");
            printArray(locks,N);
            printf("Keys array: ");
            printArray(keys,N);
            break;

        default:
            printf("Invalid option!");
            return 0;
    }

    // Sort the arrays using QuickSort algorithm.
    left = 0;
    right = N-1;
    matchPairs(locks, keys, left, right); 
  
    //Printing sorted arrays
    printf("\nMatched locks and keys are : \n");
    printArray(locks,N); 
    printArray(keys,N); 

    return 0;
} 
