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
void getTask(char *directory);

int main()
{
    char *directory = "./tasks";
    while (1)
    {
        readTasks(directory, taskArray);
        getTask(directory);
        writeTasksToFiles(directory, taskArray);
    }
    return 0;
}

void readTasks(const char* directory, struct Task array[])
{
taskCount = 0;
DIR *d;
struct dirent *dir;
d = opendir(directory);
if (d) {
    while ((dir = readdir(d)) != NULL) {
        // Check if the file has a .txt extension
        if (strstr(dir->d_name, ".txt") != NULL) 
        {
            // Construct the full path to the file
            char filepath[256]; // Adjust the size as needed
            snprintf(filepath, sizeof(filepath), "%s/%s", directory, dir->d_name);
            // Open the file for reading
            FILE *file = fopen(filepath, "r");
            if (file != NULL) 
            {
                struct Task task;
                // Read task description (first line)
                if (fgets(task.user_input, sizeof(task.user_input), file) != NULL) 
                {
                    // Remove the newline character from the input
                    if (task.user_input[strlen(task.user_input) - 1] == '\n') 
                    {
                        task.user_input[strlen(task.user_input) - 1] = '\0';
                    }
                    // Read time (second line)
                    char timeStr[100];
                    if (fgets(timeStr, sizeof(timeStr), file) != NULL) 
                    {
                        task.time = atoi(timeStr);
                        // Read active status (third line)
                        char activeStr[100];
                        if (fgets(activeStr, sizeof(activeStr), file) != NULL) 
                        {
                            task.active = (strcmp(activeStr, "true\n") == 0) ? true : false;
                            taskArray[taskCount] = task;
                            printf("%i: ", taskCount);
                            printf("%s, ", task.user_input);
                            printf("%d, ", task.time);
                            printf("%s\n", task.active ? "active" : "inactive");
                            taskCount++;
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

void writeTasksToFiles(const char* directory, struct Task array[]) 
{
    for (int i = 0; i < taskCount; i++) 
    {
        char filename[20];
        snprintf(filename, sizeof(filename), "%s/%d.txt", directory, i);

        FILE *file = fopen(filename, "w");
        if (file != NULL) 
        {

            fprintf(file, "%s\n", taskArray[i].user_input);
            fprintf(file, "%d\n", taskArray[i].time);
            fprintf(file, "%s\n", taskArray[i].active ? "true" : "false");

            fclose(file);
        }
    }
}

void getTask(char *directory)
{
    char input[100];
    printf("enter letters to create new tasks, single numbers to activate existing, or exit:\n");
    fgets(input, sizeof(input), stdin);

    //handle single letter commands for delete, refresh, activate and edit
    if ((input[1]) == '\n')
    {
        if (strcmp(input, "d\n") == 0)
        {
            char deleteBuffer[20];
            int delete;
            printf("enter task number to be deleted: ");
            scanf("%d", &delete);
            sprintf(deleteBuffer, "%s%d", directory, delete);
            if(remove(deleteBuffer == 0))
            {
                printf("file deleted\n");
                taskCount--;
                int j = 0;
                for (int i = 0; i < taskCount; i++)
                {
                    if (i != delete)
                    {
                        taskArray[i] = taskArray[i + j];
                    } else 
                    {
                        j++;
                    }
                }
            }
        }
    } else
        {
            struct Task newTask;
            strncpy(newTask.user_input, input, sizeof(newTask.user_input) - 1);
            newTask.user_input[sizeof(newTask.user_input) - 1] = '\0';
            newTask.time = 0;
            newTask.active = false;
            taskArray[taskCount] = newTask;
            taskCount++;
        }
}