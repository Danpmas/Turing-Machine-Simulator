#include <stdio.h>
#include <stdlib.h>

struct TapeCell {
    char value;
    struct TapeCell* prev;
    struct TapeCell* next;
};

struct Instruction {
    char write_value;
    char move_direction; // 'L' for left, 'R' for right
    int new_state;
};

struct TapeCell* createCell(char value) {
    struct TapeCell* newCell = (struct TapeCell*)malloc(sizeof(struct TapeCell));
    if (newCell == NULL) {
        printf("No space for new cell");
        exit(1);
    }
    newCell->value = value;
    newCell->prev = NULL;
    newCell->next = NULL;
    return newCell;
}

void insertEnd(struct TapeCell** head, char value) {
    struct TapeCell* newCell = createCell(value);
    if (*head == NULL) {
        *head = newCell;
    } else {
        struct TapeCell* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newCell;
        newCell->prev = current;
    }
}

// Function to display the linked list
void displayList(struct TapeCell* head) {
    struct TapeCell* current = head;
    while (current != NULL) {
        printf("%c ", current->value);
        current = current->next;
    }
    //# of states was overlapping without the new line
    printf("\n");
}

int main() {

    char filename[] = "/Users/danmas/Desktop/TM.txt";
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("File not found");
        return 1;
    }

    // Create a doubly linked list for the Turing machine tape
    struct TapeCell* tape = NULL;

    // Set the first value to 'A'
    insertEnd(&tape, 'A');

    // Read the rest of the values from line one in the text file
    char firstline;
    while ((firstline = fgetc(file)) != '\n' && firstline != -1) {
        //Doesn't include states, just values
        insertEnd(&tape, firstline);
    }

    // Display the linked list
    printf("Initial Tape contents: ");
    displayList(tape);

    int states, start_state;
    int end_state;
    char info_lines[100]; // Assuming a line won't exceed 1000 characters
    char instr_lines[100];
    if (fgets(info_lines, sizeof(info_lines), file) != NULL) {
        sscanf(info_lines, "%d", &states);
        printf("Number of states: %d\n", states);
    }
    if (fgets(info_lines, sizeof(info_lines), file) != NULL) {
        sscanf(info_lines, "%d", &start_state);
        printf("Start state: %d\n", start_state);
    }
    if (fgets(info_lines, sizeof(info_lines), file) != NULL) {
        sscanf(info_lines, "%d", &end_state);
        printf("End state: %d\n", end_state);
    }

    // Initialize the instruction array (calloc was hard to implement :3)
    struct Instruction instructionss[states][128];
    for (int i = 0; i < states; i++) {
        for (int j = 0; j < 128; j++) {
            instructionss[i][j].write_value = ' '; // Initialize with a space
            instructionss[i][j].move_direction = ' ';
            instructionss[i][j].new_state = -1; // Initialize with an invalid state
        }
    }

    int current_state, new_st;
    char read_val, write_val, move_dir;
    while (fgets(instr_lines, sizeof(instr_lines), file)) {
        sscanf(instr_lines, "(%d,%c)->(%c,%c,%d)", &current_state, &read_val, &write_val, &move_dir, &new_st);
        //printf("Instructions: (%d,%c)->(%c,%c,%d)\n", current_state, read_val, write_val, move_dir, new_st);

        // Populate the instructions array based on the input
        instructionss[current_state][read_val].write_value = write_val;
        instructionss[current_state][read_val].move_direction = move_dir;
        instructionss[current_state][read_val].new_state = new_st;
    }
    // Close the file
    fclose(file);
    //State was at 5 after reading file
    current_state = 0;
    struct TapeCell* current_head = tape;
    while (current_state != end_state) {
        char read_values = current_head->value;
        struct Instruction current_instruction = instructionss[current_state][read_values];

        // Update tape
        current_head->value = current_instruction.write_value;

        // Move head
        if (current_instruction.move_direction == 'L') {
            if (current_head->prev == NULL) {
                // Create a new cell on the left if needed
                struct TapeCell* new_Cell = (struct TapeCell*)malloc(sizeof(struct TapeCell));
                if (new_Cell == NULL) {
                    printf("No space for new cell");
                    return 1;
                }
                new_Cell->value = 'B';
                new_Cell->prev = NULL;
                new_Cell->next = current_head;
                current_head->prev = new_Cell;
                tape = new_Cell; // Update tape start
            } else {
                current_head = current_head->prev;
            }
        } else if (current_instruction.move_direction == 'R') {
            if (current_head->next == NULL) {
                // Create a new cell on the right if needed
                struct TapeCell* new_Cell = (struct TapeCell*)malloc(sizeof(struct TapeCell));
                if (new_Cell == NULL) {
                    printf("No space for new cell");
                    return 1;
                }
                new_Cell->value = 'B';
                new_Cell->prev = current_head;
                new_Cell->next = NULL;
                current_head->next = new_Cell;
            } else {
                current_head = current_head->next;
            }
        } else {
            printf("Invalid move direction: %c\n", current_instruction.move_direction);
            return 1;
        }

        // Update current state after R/L
        current_state = current_instruction.new_state;
    }

    printf("Final Tape Contents: ");
    displayList(tape);

    // Free memory for the linked list TapeCells
    struct TapeCell* current = tape;
    while (current != NULL) {
        struct TapeCell* temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}