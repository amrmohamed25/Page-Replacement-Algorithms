#include <stdio.h>
#include <stdlib.h>


char inputAlgo[10];
int itemCount = 0, SIZE = 0;
int rear = 0;
int *queue;

void writeHeader() {//function used to print header
    printf("Replacement Policy = %s\n-------------------------------------\nPage   Content of Frames\n----   -----------------\n",
           inputAlgo);
}

void writeEnd(int pageFaults) {//function used to print page faults
    printf("-------------------------------------\nNumber of page faults = %d\n", pageFaults);
}


int checkIfExist(int num) {//function used to check if page exists
    for (int i = 0; i < SIZE; i++) {
        if (queue[i] == num)
            return 0;
    }
    return 1;
}

void predictNextCLOCK(
        int *helpingQueue) {//function used to predict which element we are going to remove in this function we search for 0 and as we move we set any 1 to 0
    while (1) {
        if (helpingQueue[rear] == 0) {
            break;
        } else {
            helpingQueue[rear] = 0;//setting from 1 to 0
        }
        rear++;
        rear = rear % SIZE;
    }
}

void printQueue() {//function used to print queue
    for (int i = 0; i < SIZE; i++) {
        if (queue[i] != 0)
            printf(" %02d", queue[i]);//%02d to add extra zero for zero digits
    }
    printf(" \n");
}

void CLOCK(int *input, int numberOfElements, int *helpingQueue) {
    writeHeader();
    int pageFaults = 0;
    for (int i = 0; i < numberOfElements; i++) {
        if (itemCount < SIZE) {//if it is smaller than frame size then we write it without page faults
            if (checkIfExist(input[i])) {//if it isnt in queue
                helpingQueue[rear] = 1;//then we will insert it and set it to 1
                queue[rear++] = input[i];
                rear = rear % SIZE;
                itemCount++;
            }
            printf("%02d    ", input[i]);
        } else {//else means we reached size of frame
            if (checkIfExist(input[i])) {
                predictNextCLOCK(
                        helpingQueue);//if it doesn't exist then predict nextClock which will set rear to correct location of next page
                queue[rear] = input[i];
                helpingQueue[rear++] = 1;
                rear = rear % SIZE;
                printf("%02d F  ", input[i]);
                pageFaults++;
            } else {//means that it exists then we will set it to 1
                for (int k = 0; k < SIZE; k++) {
                    if (queue[k] == input[i]) {
                        helpingQueue[k] = 1;
                        break;
                    }
                }
                printf("%02d    ", input[i]);
            }
        }
        printQueue();
    }
    writeEnd(pageFaults);
}


void LRU(int *input, int numberOfElements) {
    writeHeader();
    int pageFaults = 0;
    for (int i = 0; i < numberOfElements; i++) {
        if (itemCount < SIZE) {
            if (checkIfExist(input[i])) {
                queue[rear++] = input[i];
                itemCount++;
            }
            printf("%02d    ", input[i]);
        } else {
            if (checkIfExist(input[i])) {
                int farIndex = i;
                for (int k = 0; k < SIZE; k++) {//finding the most far page that hasn't been used
                    int j;
                    for (j = i; j >= 0; j--) {
                        if (queue[k] == input[j]) {//each time check if it is equal to page
                            if (j < farIndex) {//if it is equal then check if is far from farIndex
                                rear = k;//then this page can be removed
                                farIndex = j;
                            }
                            break;
                        }
                    }
                }
                queue[rear] = input[i];
                printf("%02d F  ", input[i]);
                pageFaults++;
            } else {
                printf("%02d    ", input[i]);
            }
        }
        printQueue();
    }
    writeEnd(pageFaults);
}


void OPTIMAL(int *input, int numberOfElements) {
    writeHeader();
    int pageFaults = 0;
    for (int i = 0; i < numberOfElements; i++) {
        if (itemCount < SIZE) {
            if (checkIfExist(input[i])) {
                queue[rear++] = input[i];
                itemCount++;
            }
            printf("%02d    ", input[i]);
        } else {
            if (checkIfExist(input[i])) {
                int farIndex = i;//farIndex used to know which is the far page that was referenced
                for (int k = 0; k < SIZE; k++) {
                    int j;
                    for (j = i; j < numberOfElements; j++) {
                        if (queue[k] == input[j]) {//if page = frame then check if j>farIndex means it is far then current farIndex
                            if (j > farIndex) {
                                farIndex = j;//set the new far
                                rear=k;//set the new rear
                            }
                            break;
                        }
                    }
                    if (j ==
                        numberOfElements)//If we reached end of inner loop means this page wasn't referenced so replace it
                    {
                        rear = k;
                        break;
                    }
                }
                queue[rear] = input[i];
                printf("%02d F  ", input[i]);
                pageFaults++;
            } else {
                printf("%02d    ", input[i]);
            }
        }
        printQueue();
    }
    writeEnd(pageFaults);
}


void FIFO(int *input, int numberOfElements) {
    writeHeader();
    int pageFaults = 0;
    for (int i = 0; i < numberOfElements; i++) {
        if (itemCount < SIZE) {
            if (checkIfExist(input[i])) {
                queue[rear++] = input[i];
                rear = rear % SIZE;
                itemCount++;
            }
            printf("%02d    ", input[i]);
        } else {
            if (checkIfExist(input[i])) {
                queue[rear++] = input[i];
                rear = rear % SIZE;
                printf("%02d F  ", input[i]);
                pageFaults++;
            } else {
                printf("%02d    ", input[i]);
            }
        }
        printQueue();
    }
    writeEnd(pageFaults);
}

int *allocateArray(int elements) {//alocating array and setting it equal to zero
    int *arr = (int *) malloc(elements * sizeof(int));
    for (int i = 0; i < elements; i++) {
        arr[i] = 0;
    }
    return arr;
}


int *scanInput(int *numberOfElements) {
    int *input = malloc(10 * sizeof(int));
    int i = 0;
    int elements = 10;//making array of size 10 but it will be resized
    fscanf(stdin, "%d", &SIZE);//scanning page size
    fscanf(stdin, "%s", inputAlgo);//scanning algo name
    int temp;
    while (fscanf(stdin, "%d", &temp) != EOF) {//scanning integer
        input[i] = temp;
        i++;

        if (i % 10 == 0 && i != 0) {
            input = (int *) realloc(input, (i + 10) * sizeof(int));//reallocating if array is full
            elements += 10;
        }

    }
    input = (int *) realloc(input, (i - 1) * sizeof(int));//removing -1
    *numberOfElements = i - 1;//removing -1
    return input;
}

int strcmp(char *str1, char *str2) {//strcmp function without string.h library
    int i = 0;
    int j = 0;
    while (str1[i] != '\0' && str1[j] != '\0') {
        if (str1[i] != str2[j]) {
            return 0;
        }
        i++;
        j++;
    }
    if (str1[i] != '\0' || str2[j] != '\0') {
        return 0;
    }
    return 1;
}

int main() {
    int numberOfElements;
    int *input = scanInput(&numberOfElements);
    queue = allocateArray(SIZE);
    if (strcmp(inputAlgo, "FIFO")) {
        FIFO(input, numberOfElements);
    } else if (strcmp(inputAlgo, "OPTIMAL")) {
        OPTIMAL(input, numberOfElements);
    } else if (strcmp(inputAlgo, "LRU")) {
        LRU(input, numberOfElements);
    } else if (strcmp(inputAlgo, "CLOCK")) {
        int *helpingQueue = malloc(sizeof(int) * SIZE);
        for (int i = 0; i < SIZE; i++) {
            helpingQueue[i] = 0;
        }
        CLOCK(input, numberOfElements, helpingQueue);
    }
    return 0;
}
