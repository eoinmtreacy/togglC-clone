#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

struct Task
    {
    char user_input[100]; // Assuming a maximum input length of 99 characters
    int time;
    bool active;
    };

struct Task taskArray[100];
int taskCount = 0;

void readTasks(const char* directory, struct Task array[]);
void writeTasksToFiles(const char* directory, struct Task array[]);
void getTask(void);

int main()
{
    const char *directory = "./tasks";
    while (1)
    {
        readTasks(directory, taskArray);
        getTask();
        writeTasksToFiles(directory, taskArray);
    }
    
    
    
    return 0;
}

    void readTasks(const char* directory, struct Task array[])
    {
    DIR *d;
    struct dirent *dir;
    d = opendir(directory);

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            // Check if the file has a .txt extension
            if (strstr(dir->d_name, ".txt") != NULL) {
                // Construct the full path to the file
                char filepath[256]; // Adjust the size as needed
                snprintf(filepath, sizeof(filepath), "%s/%s", directory, dir->d_name);

                // Open the file for reading
                FILE *file = fopen(filepath, "r");
                if (file != NULL) {
                    struct Task task;

                    // Read task description (first line)
                    if (fgets(task.user_input, sizeof(task.user_input), file) != NULL) {
                        // Remove the newline character from the input
                        if (task.user_input[strlen(task.user_input) - 1] == '\n') {
                            task.user_input[strlen(task.user_input) - 1] = '\0';
                        }

                        // Read time (second line)
                        char timeStr[100];
                        if (fgets(timeStr, sizeof(timeStr), file) != NULL) {
                            task.time = atoi(timeStr);

                            // Read active status (third line)
                            char activeStr[100];
                            if (fgets(activeStr, sizeof(activeStr), file) != NULL) {
                                task.active = (strcmp(activeStr, "true\n") == 0) ? true : false;

                                taskArray[taskCount] = task;
                                taskCount++;


                                // Print the task details
                                printf("Task: %s\n", task.user_input);
                                printf("Time: %d\n", task.time);
                                printf("Active: %s\n", task.active ? "true" : "false");
                                printf("-------------\n");
                            }
                        }
                    }

                    // Close the file
                    fclose(file);
                }
            }
        }
        closedir(d);
    }
}

void writeTasksToFiles(const char* directory, struct Task array[]) {
    for (int i = 0; i < taskCount; i++) {
        char filename[20]; // Adjust the size as needed
        snprintf(filename, sizeof(filename), "%s/%d.txt", directory, i);
        printf("%s\n",filename);

        // Open the file for writing
        FILE *file = fopen(filename, "w");
        if (file != NULL) {
            // Write task description (first line)
            fprintf(file, "%s\n", taskArray[i].user_input);

            // Write time (second line)
            fprintf(file, "%d\n", taskArray[i].time);

            // Write active status (third line)
            fprintf(file, "%s\n", taskArray[i].active ? "true" : "false");

            // Close the file
            fclose(file);
        }
    }
}

void getTask(void)
{
    char input[100]; // Assuming a maximum input length of 99 characters
    printf("enter letters to create new tasks, single numbers to activate existing, or exit:\n");
    fgets(input, sizeof(input), stdin); // Read a line of text from the user

    //if (strcmp(input, "q") == 0) {
    //        break; // Exit the loop if the user types 'exit'
    //    }
//
    struct Task newTask;
    strncpy(newTask.user_input, input, sizeof(newTask.user_input) - 1);
    newTask.user_input[sizeof(newTask.user_input) - 1] = '\0'; // Ensure null-terminated string
    newTask.time = 0;
    newTask.active = false;

    // Append the new task to the taskArray
    taskArray[taskCount] = newTask;
    taskCount++;
}