#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 30

/*
@brief struct for graph
*/
struct Node {
    char data;
    struct Node* neighbors[MAX_NODES];
    int neighborCount;
};

/*
@brief struct for queue
*/
struct Queue {
    int front, rear, size;
    unsigned capacity;
    int *array;
};

/*
@brief struct for return value from calculateCommunityNumber()
*/
struct Community{
    int communityNumber;
    int* visited;
    int isEnd;
};

/*
@brief creates a queue
@param capacity capacity of queue
@return created queue
*/
struct Queue* createQueue(unsigned capacity) {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    queue->array = (int*)malloc(queue->capacity * sizeof(int));
    return queue;
}

/*
@brief checks if queue is full
@param queue queue to check
@return 1 if queue is full, 0 otherwise
*/
int isFull(struct Queue* queue) {
    return (queue->size == queue->capacity);
}

/*
@brief checks if queue is empty
@param queue queue to check
@return 1 if queue is empty, 0 otherwise
*/
int isEmpty(struct Queue* queue) {
    return (queue->size == 0);
}

/*
@brief adds an item to queue
@param queue queue to add item
@param item item to add
*/
void enqueue(struct Queue* queue, int item) {
    if (isFull(queue)) {
        return;
    }

    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

/*
@brief removes an item from queue
@param queue queue to remove item
@return removed item
*/
int dequeue(struct Queue* queue) {
    if (isEmpty(queue)) {
        return -1;
    }

    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

/*
@brief reads graph from file
@param graph graph to read
@param nodeCount node count of graph
@param filename file name to read
*/
void readGraph(struct Node* graph[], int* nodeCount, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("File error!");
        exit(EXIT_FAILURE);
    }

    char line[100];  // Max size of line

    while (fgets(line, sizeof(line), file)) {
        char nodeData;
        sscanf(line, "%c:", &nodeData);

        //Create node
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->data = nodeData;
        newNode->neighborCount = 0;

        //Read neighbors
        char* neighborStr = strchr(line, ':') + 1;
        char* token = strtok(neighborStr, ",");
        while (token != NULL) {
            struct Node* neighbor = (struct Node*)malloc(sizeof(struct Node));
            neighbor->data = token[0];
            neighbor->neighborCount = 0;

            //Add neighbor to node
            newNode->neighbors[newNode->neighborCount++] = neighbor;

            token = strtok(NULL, ",");
        }

        //Add node to graph
        graph[(*nodeCount)++] = newNode;
    }

    fclose(file);
}

/*
@brief prints graph
@param graph graph to print
@param nodeCount node count of graph
*/
void printGraph(struct Node* graph[], int nodeCount) {
    printf("\nGraph:\n");
    int i,j;
    for (i = 0; i < nodeCount; ++i) {
        printf("%c:", graph[i]->data);
        for (j = 0; j < graph[i]->neighborCount; ++j) {
            if(graph[i]->neighbors[j]!=NULL){
                printf("%c,", graph[i]->neighbors[j]->data);
            }
        }
        printf("\n");
    }
    printf("\n");
}

/*
@brief finds shortest paths and calculates edge betweenness then removes the edges that have highest edge betweenness
@param graph graph to find shortest path and calculate edge betweenness
@param nodeCount node count of graph
*/
void findShortestPathAndCalculateEdgeBetweenness(struct Node* graph[],int nodeCount) {

    int edgeBetweenness[nodeCount][nodeCount];
    int visited[nodeCount];
    int parent[nodeCount];
    int source,destination,isFound,current;
    int i,j,k;
    for(i=0;i<nodeCount;i++){
        for(j=0;j<nodeCount;j++){
            edgeBetweenness[i][j]=0;
        }
    }
    struct Queue* queue = createQueue(nodeCount);
    //For all nodes find shortest path
    for(i = 0; i < nodeCount; i++){
        for(j = 0; j < nodeCount; j++){
            //Find shortest path between i and j
            if(i != j){
                //BFS start from node i, until find node j
                enqueue(queue, i);
                for(k=0;k<nodeCount;k++){
                    visited[k]=0;
                }
                for(k=0;k<nodeCount;k++){
                    parent[k]=-1;
                }
                source = i;
                destination = j;
                visited[source]=1;
                isFound = 0;
                while(!isEmpty(queue) && isFound == 0){
                    current=dequeue(queue);
                    for(k=0;k<graph[current]->neighborCount;k++){
                        if(graph[current]->neighbors[k] != NULL){
                            if(visited[graph[current]->neighbors[k]->data-'A']==0){
                                visited[graph[current]->neighbors[k]->data-'A']=1;
                                parent[graph[current]->neighbors[k]->data-'A']=current;
                                enqueue(queue,graph[current]->neighbors[k]->data-'A');
                            }
                        }
                    }
                    if (current == j){
                        //If destination node found, stop the bfs and clear the queue
                        isFound = 1;
                        while(!isEmpty(queue)){
                            dequeue(queue);
                        }
                    }   
                }
                if(isFound == 1){
                    //If shortest path found, increase edge betweenness of the edges traversed
                    //Print path
                    source = i;
                    destination = j;
                    printf("Path %c -> %c\n",graph[i]->data,graph[j]->data);
                    while(parent[destination] != source){
                        printf("%c - ",graph[destination]->data);
                        destination = parent[destination];
                        edgeBetweenness[parent[destination]][destination]+=1;
                        edgeBetweenness[destination][parent[destination]]+=1;
                    }
                    printf("%c - %c\n",graph[destination]->data,graph[source]->data);
                    edgeBetweenness[source][destination]+=1;
                    edgeBetweenness[destination][source]+=1;
                }
            }
        }
    }

    //Print edge betweenness
    for(i=0;i<nodeCount;i++){
        for(j=0;j<i;j++){
            if(edgeBetweenness[i][j] != 0){
                printf("\nEdge (%c , %c) : %d times",graph[i]->data,graph[j]->data,edgeBetweenness[i][j]/2);
            }
        }
    }

    //Find the edges with the highest betweenness, there can be multiple edges that with the highest betweenness
    int max = 0;
    int maxEdgeBetweenness[nodeCount][nodeCount];
    //Find max value
    //Set maxEdgeBetweenness 0 simultaneously
    for(i=0;i<nodeCount;i++){
        for(j=0;j<nodeCount;j++){
            maxEdgeBetweenness[i][j]=0;
            if(edgeBetweenness[i][j] > max){
                max = edgeBetweenness[i][j];
            }
        }
    }
    
    //Find edges with the highest betweenness
    for(i=0;i<nodeCount;i++){
        for(j=0;j<nodeCount;j++){
            if(edgeBetweenness[i][j] == max){
                maxEdgeBetweenness[i][j] = 1;
            }
        }
    }
    
    //Remove edges with the highest betweenness and print them
    printf("\n");
    for(i=0;i<nodeCount;i++){
        for(j=0;j<i;j++){
            if(maxEdgeBetweenness[i][j] == 1){
                printf("\nRemoving edge %c --- %c",graph[i]->data,graph[j]->data);
                for(k=0;k<graph[i]->neighborCount;k++){
                    if(graph[i]->neighbors[k] != NULL){
                        if(graph[i]->neighbors[k]->data==graph[j]->data){
                            graph[i]->neighbors[k]=NULL;
                        }
                    }

                }
                for(k=0;k<graph[j]->neighborCount;k++){
                    if(graph[j]->neighbors[k] != NULL){
                        if(graph[j]->neighbors[k]->data==graph[i]->data){
                            graph[j]->neighbors[k]=NULL;
                        }
                    }
                }
            }
        }
    }

    printf("\n");
    printGraph(graph,nodeCount);
}

/*
@brief calculates community number, finds which node belongs to which community and 
        checks if there is a community that has less than or equal to t nodes
@param graph graph to calculate community number
@param nodeCount node count of graph
@param t t value
@return struct Community that has the information about community number, community nodes and
        if there is a community that has less than or equal to t nodes
*/
struct Community* calculateCommunityNumber(struct Node* graph[],int nodeCount, int t){

    int communityNumber = 0;
    int *visited = (int*)malloc(sizeof(int)*nodeCount);
    int i,j;
    int current;
    for(i=0;i<nodeCount;i++){
        visited[i]=-1;
    }
    //BFS for all nodes except not visited
    struct Queue* queue = createQueue(nodeCount);
    for(i = 0; i< nodeCount; i++){
        if(visited[i] == -1){
            enqueue(queue, i);
            //Mark BFS's first node and all other nodes accessible from the node with the community number
            visited[i] = communityNumber;
            while(!isEmpty(queue)){
                current=dequeue(queue);
                for(j=0;j<graph[current]->neighborCount;j++){
                    if(graph[current]->neighbors[j] != NULL){
                        if(visited[graph[current]->neighbors[j]->data-'A'] == -1){
                            visited[graph[current]->neighbors[j]->data -'A']= communityNumber;
                            enqueue(queue,graph[current]->neighbors[j]->data-'A');
                        }
                    }
                }
                
            }
            communityNumber++;
        }
    }
    
    struct Community* com = (struct Community*)malloc(sizeof(struct Community));
    //The number of communities is equal to the number of bfs required to visit all nodes.
    com->communityNumber = communityNumber;
    com->visited = visited;
    int communitySize = 0;
    //Calculate how many nodes are in each community
    for(i=0;i<communityNumber;i++){
        communitySize = 0;
        for(j=0; j<nodeCount; j++){
            if(visited[j] == i){
                communitySize++;
            }
        }
        if(communitySize <= t){
            //If there are less than or equal to t nodes in a community than stop the algorithm
            com->isEnd = 1;
            return com;
        }
    }
    com->isEnd = 0;
    return com;
}

int main() {
    struct Node* graph[MAX_NODES];
    int nodeCount = 0;

    readGraph(graph, &nodeCount, "input.txt");
    printGraph(graph, nodeCount);

    int i,j,kValue,tValue;
    printf("\nEnter k value: ");
    scanf("%d",&kValue);
    printf("Enter t value: ");
    scanf("%d",&tValue);
    printf("\n");

    struct Community* com = NULL;
    int lastCommunityNumber = 0;
    int repeatedCommunityNumberCounter = 1;
    int flag = 0;
    //Calculate community number of given graph
    com = calculateCommunityNumber(graph,nodeCount,tValue);
    if(com->isEnd == 1){
        flag = 1;
        printf("\nProgram terminated due to t value!\n");
    }
    lastCommunityNumber = com->communityNumber;
    int iteration = 1;

    while(flag == 0){
        printf("\n------------------------------ ITERATION %d ------------------------------\n",iteration);
        free(com);
        findShortestPathAndCalculateEdgeBetweenness(graph,nodeCount);
        com = calculateCommunityNumber(graph,nodeCount,tValue);
        if(com->isEnd == 1){
            flag = 1;
            printf("\nProgram terminated due to t value!\n");
        }
        if(com->communityNumber == lastCommunityNumber){
            //If the number of communities is same previous one
            repeatedCommunityNumberCounter++;
        }else{
            //If it's not the same as before reset counter
            repeatedCommunityNumberCounter = 1;
        }
        if(repeatedCommunityNumberCounter == kValue){
            //If the number of communities is same k times than stop the algorithm
            flag = 1;
            printf("\nProgram terminated due to k value!\n");
        }
        lastCommunityNumber = com->communityNumber;

        //Print number of communities and community nodes
        printf("\nNumber of communities: %d\n",lastCommunityNumber);
        for(i=0;i<lastCommunityNumber;i++){
            printf("Community %d: ",i+1);
            for(j=0; j<nodeCount; j++){
                if(com->visited[j] == i){
                    printf("%c ",graph[j]->data);
                }
            }
            printf("\n");
        }
        printf("\n");
        iteration++;
    }
    

    // Belleği serbest bırak
    for (i = 0; i < nodeCount; i++) {
        free(graph[i]);
    }
    free(com);

    return 0;
}
