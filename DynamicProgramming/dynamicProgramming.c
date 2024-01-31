#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

/*
@brief Checks whether a found sequence has been found before
@param longestSequences Matrix where the longest sequences found are kept as strings
@param sequence The sequence to be checked to see if it has been found before
@param count Number of longest sequences found
@return Returns 1 if the same sequence has been found before, 0 otherwise.
*/
int isExist(char** longestSequences, char* sequence, int count){
    int i;
    for(i = 0; i < count; i++){
        if(strcmp(longestSequences[i], sequence) == 0){
            return 1;
        }
    }
    return 0;
}

/*
@brief Prints the longest sequences found
@param longestSequences Matrix where the longest sequences found are kept as strings
@param count Number of longest sequences found
@return
*/
void printLongestSequences(char** longestSequences, int count){
    int i;
    printf("\nLongest Sequences:\n");
    for (i = 0; i < count; i++) {
        printf("%s\n", longestSequences[i]);
    }
}

/*
@brief Prints a matrix
@param matrix Matrix to be printed
@param row Number of rows of the matrix
@param column Number of columns of the matrix
@return
*/
void printMatrix(int** matrix, int row, int column){
    int i, j;
    for(i = 0; i < row; i++){
        for(j = 0; j < column; j++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

/*
@brief Recursively finds the longest common sequence of two strings
@param str1 First string
@param str2 Second string
@param i Index of the first string
@param j Index of the second string
@param longestLength Length of the longest sequence
@param dp Matrix that holds the longest sequence lengths
@param sequence String to which the sequence will be added letter by letter
@param count Number of longest sequences found
@param selection Auxiliary matrix that holds information about which letters should be added to the sequence
@param longestSequences Matrix where the longest sequences found are kept as strings
@return
*/
void findLongestCommonSequenceHelper(char *str1, char *str2, int i, int j, int longestLength, int **dp, char sequence[longestLength], int *count, int **selection, char** longestSequences) {
    while (i > 0 && j > 0 ) {
        if (selection[i][j] == 1) {
            //Matching value
            sequence[--longestLength] = str1[i - 1];
            i--;
            j--;
        } else if (selection[i][j] == 2) {
            //Skip first
            i--;
        } else if (selection[i][j] == 3) {
            //Skip second
            j--;
        } else if(selection[i][j] == 4){
            //If the left and top values are equal, two different sequences can exist
            findLongestCommonSequenceHelper(str1, str2, i - 1, j, longestLength,dp,sequence, count , selection,longestSequences);
            findLongestCommonSequenceHelper(str1, str2, i, j - 1, longestLength,dp,sequence, count, selection,longestSequences);
            return;
        }

        if (longestLength == 0) {
            //A sequence is completed
            sequence[longestLength - 1] = '\0';

            //Check if the same sequence has been found before
            if(isExist(longestSequences, sequence, *count)){
                return;
            }
            //Add sequence to the matrix that holds all longest sequences
            longestSequences[*count] = (char*) malloc(longestLength * sizeof(char));
            strcpy(longestSequences[*count], sequence);
            
            (*count)++;

            return;
        }
    }
}

/*
@brief Finds the longest common sequences of two strings
@param str1 First string
@param str2 Second string
@return
*/
void findLongestCommonSequences(char *str1, char *str2) {
    int i,j;
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    //Initialization of matrices
    int **dp = (int **)calloc((len1 + 1) , sizeof(int *));
    int **selection = (int **)calloc((len1 + 1) , sizeof(int *));
    for (i = 0; i <= len1; i++) {
        dp[i] = (int *)calloc((len2 + 1) , sizeof(int));
        selection[i] = (int *)calloc((len2 + 1) , sizeof(int));
    }

    
    for (i = 0; i <= len1; i++) {
        for (j = 0; j <= len2; j++) {
            if (i == 0 || j == 0) {
                //First row and column are 0
                dp[i][j] = 0;
                selection[i][j] = 0;
            } else if (str1[i - 1] == str2[j - 1]) {
                //Matching value
                dp[i][j] = dp[i - 1][j - 1] + 1;
                selection[i][j] = 1;
            } else {
                //Mismatched value
                if(dp[i - 1][j] > dp[i][j - 1]){
                    //skip first string
                    dp[i][j] = dp[i - 1][j];
                    selection[i][j] = 2;
                }else if(dp[i - 1][j] < dp[i][j - 1]){
                    //skip second string
                    dp[i][j] = dp[i][j - 1];
                    selection[i][j] = 3;
                }else{
                    //left and top value are equal, there can be 2 sub-sequence
                    dp[i][j] = dp[i - 1][j];
                    selection[i][j] = 4;
                }
            }
        }

        printf("\nRow - %d\n-------------------",i);
        printf("\nDynamic programming matrix:\n\n");
        printMatrix(dp,len1+1,len2+1);
        
        printf("\nSelection matrix:\n\n");
        printMatrix(selection,len1+1,len2+1);
        
    }


    // Length of longest common sequence
    int longestLength = dp[len1][len2];
    printf("\nLength of longest common sequence: %d\n", longestLength);

    
    char sequence[longestLength];
    int count = 0;
    
    char **longestSequences = (char **)malloc(MAX * sizeof(char *));

    //Finds Strings
    findLongestCommonSequenceHelper(str1, str2, len1, len2, longestLength,dp,sequence, &count, selection,longestSequences);
    
    //Print longest sequences
    printLongestSequences(longestSequences, count);

    //Memory clearing
    for (i = 0; i <= len1; i++) {
        free(dp[i]);
        free(selection[i]);
    }
    free(dp);
    free(selection);

    for (i = 0; i < count; i++) {
        free(longestSequences[i]);
    }
    free(longestSequences);
}

int main() {

    char str1[MAX], str2[MAX];

    printf("String 1: ");
    scanf("%s", str1);
    printf("String 2: ");
    scanf("%s", str2);

    findLongestCommonSequences(str1, str2);
    
    return 0;
}
