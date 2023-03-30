#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
    char type[15];
    char name[10][20];
    char date[9];
    int time;
    float duration;
} Event;

void sortEventByPriority(char myEvents[][5][15], int eventCount)
{
    int arrayLength = eventCount;
    int i, j;
    for (i = 0; i < arrayLength - 1; i++)
    {
        for (j = 0; j < arrayLength - i - 1; j++)
        {
            if (checkPriority(myEvents[j][0]) > checkPriority(myEvents[j + 1][0]))
            {
                char temp[15];
                strcpy(temp, myEvents[j][0]);
                strcpy(myEvents[j][0], myEvents[j + 1][0]);
                strcpy(myEvents[j + 1][0], temp);

                strcpy(temp, myEvents[j][1]);
                strcpy(myEvents[j][1], myEvents[j + 1][1]);
                strcpy(myEvents[j + 1][1], temp);

                strcpy(temp, myEvents[j][2]);
                strcpy(myEvents[j][2], myEvents[j + 1][2]);
                strcpy(myEvents[j + 1][2], temp);

                strcpy(temp, myEvents[j][3]);
                strcpy(myEvents[j][0], myEvents[j + 1][3]);
                strcpy(myEvents[j + 1][3], temp);

                strcpy(temp, myEvents[j][4]);
                strcpy(myEvents[j][4], myEvents[j + 1][4]);
                strcpy(myEvents[j + 1][4], temp);
            }
        }
    }
}

char *upFirstLetter(char *str)
{
    char *newStr = malloc(sizeof(char) * (strlen(str) + 1));
    newStr[0] = toupper(str[0]);
    strcpy(newStr + 1, str + 1);
    return newStr;
}

int checkName(char command[20], char name[][20], int userNum)
{
    int i = 0;
    int child_index = -1;
    for (i = 0; i < userNum; i++)
    {
        // Find the index of the child
        if (strcmp(command, name[i]) == 0)
        {
            child_index = i;
            break; // name not exist
        }
    }
    return child_index;
}

int timeSlotFree(char myEvents[][5][15], int numEvents, const char *date, const char *time, float duration)
{
    int i;
    for (i = 0; i < numEvents; i++)
    {
        if (strcmp(myEvents[i][1], date) == 0)
        {
            int existingTime = atoi(myEvents[i][2]);
            float existingDuration = atof(myEvents[i][3]);
            int inputTime = atoi(time);
            if (inputTime >= existingTime && inputTime < existingTime + (int)(existingDuration * 100))
            {
                return 0;
            }
            if (inputTime + (int)(duration * 100) > existingTime && inputTime + (int)(duration * 100) <= existingTime + (int)(existingDuration * 100))
            {
                return 0;
            }
            // not (input end <= existing start or input start >= existing end)
            // this may better
            /*
            if (!(inputTime + (int)(duration * 100) <= existingTime || inputTime >= existingTime + (int)(existing Duration * 100))) {
                return 0;
            }
            */
        }
    }
    return 1;
}

void addEvent(char myEvents[][5][15], int *eventCount, const char *eventType, const char *date, const char *time, const char *duration, const char *id)
{
    strcpy(myEvents[*eventCount][0], eventType);
    strcpy(myEvents[*eventCount][1], date);
    strcpy(myEvents[*eventCount][2], time);
    strcpy(myEvents[*eventCount][3], duration);
    strcpy(myEvents[*eventCount][4], id);
    (*eventCount)++;
}

int checkPriority(char event[])
{
    if (strcmp(event, "privateTime") == 0)
    {
        return 4;
    }
    else if (strcmp(event, "projectMeeting") == 0)
    {
        return 3;
    }
    else if (strcmp(event, "groupStudy") == 0)
    {
        return 2;
    }
    else
        return 1;
}

void doFCFS(char myEvents[][5][15], int *eventCount, char FCFS[][5][15], int *FCFSCount, int childID, int *rejectCount, char rejectID[][4])
{
    int i, j, k, p;
    int day = 9; // Lazy to calcuate difference of date ,use 9 first

    printf("debug: doFCFS \n");

    // clear FCFS storage first, empty can be used to check output or not in future
    for (i = 0; i <= *eventCount; i++)
    {
        strcpy(FCFS[i][0], "empty");
        strcpy(FCFS[i][4], "000");
    }

    if (*eventCount == 0)
    {
        printf("this child no events \n");
        return;
    }
    for (i = 0; i < day; i++)
    {
        j = 0;
        int dayEventCount = 0;
        // int timeSlotsCount=0;
        char date[9];
        char dayEvents[5][5][15];
        int timeSlotsSpace[5] = {0, 0, 0, 0, 0};

        strcpy(date, "");     // clear date
        strcat(date, "2023"); // year
        strcat(date, "04");   // month
        if (i < 10)
        {
            strcat(date, "0"); // day
        }
        char int_to_char;
        int_to_char = (i + '0'); // need to fix in future
        // strcat(date, int_to_char); // day
        date[7] = int_to_char;
        date[8] = '\0';

        char time[5][5] = {"1800", "1900", "2000", "2100", "2200"};
        printf("Event of %s: \n", date);

        while (j < *eventCount)
        { // searching events
            if (strcmp(myEvents[j][1], date) == 0)
            {
                int success = 1;
                int pos = 0;

                for (k = 0; k <= 4; k++)
                { // check timeslots free or not
                    // printf("debug: check each timeslot\n");
                    if (strcmp(myEvents[j][2], time[k]) == 0)
                    {
                        int dur;
                        dur = atoi(myEvents[j][3]);
                        pos = k;
                        // printf("debug: check sametimeslot %d\n",dur);
                        for (p = 0; p < dur; p++)
                        {
                            // printf("debug: timeslot %d :%d\n",18+k+j,timeSlotsSpace[k+p]);
                            if (timeSlotsSpace[k + p] != 0)
                            {
                                // timeSlotsSpace[k+p]=checkPriority(myEvents[j][0]);
                                success = 0;
                                break;
                            }
                        }
                    }
                    if (success == 0)
                        break;
                }

                // if time slots is free, add to dayEvents
                if (success == 1)
                {
                    for (p = 0; p < atoi(myEvents[j][3]); p++)
                    {

                        timeSlotsSpace[pos + p] = checkPriority(myEvents[j][0]);
                    }
                    strcpy(dayEvents[dayEventCount][0], myEvents[j][0]);
                    strcpy(dayEvents[dayEventCount][1], myEvents[j][1]);
                    strcpy(dayEvents[dayEventCount][2], myEvents[j][2]);
                    strcpy(dayEvents[dayEventCount][3], myEvents[j][3]);
                    strcpy(dayEvents[dayEventCount][4], myEvents[j][4]);
                    dayEventCount++;
                    printf("Time slot is free: %s %s %s %s %s\n", myEvents[j][0], myEvents[j][1], myEvents[j][2], myEvents[j][3], myEvents[j][4]);
                }
                else
                {
                    strcpy(rejectID[*rejectCount], myEvents[j][4]);
                    (*rejectCount)++;
                    printf("Time slot is NOT free: %s %s %s %s %s\n", myEvents[j][0], myEvents[j][1], myEvents[j][2], myEvents[j][3], myEvents[j][4]);
                }
            }
            j++;
        }

        // for dayCount,add event to FCFS
        for (j = 0; j < dayEventCount; j++)
        {
            strcpy(FCFS[*FCFSCount][0], dayEvents[j][0]);
            strcpy(FCFS[*FCFSCount][1], dayEvents[j][1]);
            strcpy(FCFS[*FCFSCount][2], dayEvents[j][2]);
            strcpy(FCFS[*FCFSCount][3], dayEvents[j][3]);
            strcpy(FCFS[*FCFSCount][4], dayEvents[j][4]);
            (*FCFSCount)++;
        }
    }

    printf("debug: FCFS schedule of child %d: \n", childID);
    for (i = 0; i < *FCFSCount; i++)
    {

        printf("%s %s %s %s %s\n", FCFS[i][0], FCFS[i][1], FCFS[i][2], FCFS[i][3], FCFS[i][4]);
    }

    printf("debug: FCFS rejected of child %d: \n", childID);
    for (i = 0; i < *rejectCount; i++)
    {

        printf("%s \n", rejectID[i]);
    }
}

int main(int argc, char *argv[])
{
    if (argc > 13 || argc < 6)
    {
        printf("The range of user should be 3 to 10, make sure no space in user name! \n");
        return 0;
    }

    printf("-----Welcome to APO-----\n");

    int userNum = argc - 3;
    char name[userNum][20];

    // get date of begin and end--------------------------------------------------------------
    int i, j, k;
    int startYear, endYear, startMonth, endMonth, startDay, endDay;
    // get manage request time
    for (i = 1; i < 3; i++)
    {
        char temp[9];
        char tempY[5];
        char tempM[3];
        char tempD[3];
        for (j = 0; j <= 8; j++)
            temp[j] = argv[i][j];
        for (j = 0; j <= 3; j++)
            tempY[j] = temp[j];
        for (j = 0; j <= 1; j++)
            tempM[j] = temp[j + 4];
        for (j = 0; j <= 1; j++)
            tempD[j] = temp[j + 6];

        if (i == 1)
        {
            startYear = atoi(tempY);
            startMonth = atoi(tempM);
            startDay = atoi(tempD);
        }
        else
        {
            endYear = atoi(tempY);
            endMonth = atoi(tempM);
            endDay = atoi(tempD);
        }
    }

    // Debug: print date
    printf("Start of manage: %d - %d - %d \n", startYear, startMonth, startDay);
    printf("End of manage: %d - %d - %d \n", endYear, endMonth, endDay);
    printf("Number of user: %d \n", userNum);
    // get name of user
    for (i = 0; i < userNum; i++)
    {
        strcpy(name[i], argv[i + 3]);
        printf("%s \n", name[i]);
    }

    // Modification start from here
    int processEnd = 0;
    int pid;
    char buf[100];
    int buf_n;
    int n;

    int fd[userNum][2][2]; // [child][pipe][0: read, 1:write]
    // Therefore, fd[child][1][0] is for parent read from child
    // Therefore, fd[child][0][1] is for parent write to child
    // Therefore, fd[child][1][1] is for child write to parent
    // Therefore, fd[child][0][0] is for child read from parent
    for (i = 0; i < userNum; i++)
    {
        if (pipe(fd[i][0]) == -1 || pipe(fd[i][1]) == -1)
        {
            printf("Something went wrong when creating pipe\n");
            exit(1);
        }
    }

    // Create child processes
    for (i = 0; i < userNum; i++)
    {
        pid = fork();

        if (pid == -1)
        {
            printf("Something went wrong when creating child\n");
            exit(1);
        }

        if (pid == 0)
        { // Child below
            int j;
            char command[15][20];
            pid = getpid();
            char message[100];

            // Close the write end of parent to child
            close(fd[i][0][1]);
            // Close the read end of child to parent
            close(fd[i][1][0]);

            // Store the number of event that a child participated in
            int eventCount = 0;
            int FCFSCount = 0;
            int rejectCount = 0;

            // Array to store the events that the child participated
            char myEvents[200][5][15];
            char FCFS[200][5][15];
            char rejectID[200][4];
            // myEvents[idOfEvent][0: Event Type, 1: Date, 2: Time, 3: Duration][]
            while (1)
            {
                memset(message, 0, sizeof(message));
                n = read(fd[i][0][0], message, sizeof(message));
                if (n <= 0)
                    break; // EOF or error
                message[n] = '\0';

                printf("DEBUG: Child %d: Received--> %s \n", i, message);

                if (strcmp(message, "endProgram") == 0)
                {
                    printf("Child %d pid %d: engProgram, message: %s\n", i + 1, pid, message);
                    break; // End the game
                }

                else
                {
                    // spilt by space and store into command

                    char *token;
                    token = strtok(message, " ");
                    j = 0;
                    while (token != NULL)
                    {
                        strcpy(command[j], token);
                        // printf("%s \n",command[i]); //debug what is input
                        j++;
                        token = strtok(NULL, " ");
                    }

                    // printf("debug: check token \n");

                    if (strcmp(command[0], "privateTime") == 0 || strcmp(command[0], "projectMeeting") == 0 || strcmp(command[0], "groupStudy") == 0 || strcmp(command[0], "gathering") == 0 || strcmp(command[0], "printEvent") == 0 || strcmp(command[0], "printSchd") == 0)
                    {
                        // Check the availibility
                        // if (timeSlotFree(myEvents, eventCount, command[1], command[2], atof(command[3])))
                        //{
                        // printf("time slot is free: adding event!\n");

                        // printf("debug: check command \n");

                        if (strcmp(command[0], "printEvent") == 0)
                        { // printing recorded events
                            printf("Recorded events of child %d, %s \n", i, name[i]);
                            for (k = 0; k <= eventCount; k++)
                            {
                                printf("%s %s %s %s %s\n", myEvents[k][0], myEvents[k][1], myEvents[k][2], myEvents[k][3], myEvents[k][4]);
                            }
                            strcpy(message, "-> [printEvent done] \n");
                            write(fd[i][1][1], message, sizeof(message));
                        }
                        else if (strcmp(command[0], "printSchd") == 0)
                        { // printing recorded events
                            // printf("debug: check FCFS \n");
                            if (strcmp(command[1], "FCFS") == 0)
                            {
                                doFCFS(myEvents, &eventCount, FCFS, &FCFSCount, i, &rejectCount, rejectID);
                                strcpy(message, "-> [printSchd FCFS done] \n");
                                write(fd[i][1][1], message, sizeof(message));
                            }
                        }

                        // bgladosd alternative try on the flow
                        else if (strcmp(command[0], "printSchdTemp") == 0)
                        { // printing recorded events
                            // printf("debug: check FCFS \n");
                            // if (strcmp(command[1], "FCFS") == 0)
                            // {
                            //     // doFCFS(myEvents, &eventCount, FCFS, &FCFSCount, i, &rejectCount, rejectID);
                            //     strcpy(message, "-> [printSchd FCFS done] \n");
                            //     write(fd[i][1][1], message, sizeof(message));
                            // }
                            strcpy(message, "-> [printSchd FCFS done] \n");
                            write(fd[i][1][1], message, sizeof(message));
                        }

                        else
                        {
                            addEvent(myEvents, &eventCount, command[0], command[1], command[2], command[3], command[4]);
                            strcpy(message, "-> [Recorded] \n");
                            write(fd[i][1][1], message, sizeof(message));
                        }

                        //}
                        /*
                        else
                        {
                            addEvent(myEvents, &eventCount, command[0], command[1], command[2], command[3]);
                            strcpy(message, "Time slot is NOT free!!\n");
                            write(fd[i][1][1], message, sizeof(message));
                        }
                        */
                    }

                    else
                    {
                        strcpy(message, "Child Ready\n");
                        write(fd[i][1][1], message, sizeof(message));
                    }
                }
            }
            exit(0);
        }
        else
        {
            // Close the read end of parent to child
            close(fd[i][0][0]);
            // Close the write end of child to parent
            close(fd[i][1][1]);
        }
    }

    for (i = 0; i < userNum; i++)
    {
        strcpy(buf, "fork");
        write(fd[i][0][1], buf, strlen(buf));
        buf_n = read(fd[i][1][0], buf, 100);
        buf[buf_n] = '\0';
        printf("parent: Reading from child %d: %s \n", i, buf);
    }

    char command[15][20];
    char input[100];

    char allEvents[200][5][15];
    char nameinvolved[200][10][20]; // store who involved the event
    int eventIndex = 0;
    // ID: for reject multiple people events
    char appointmentID[200][4];
    int appointmentID_A = 0;
    int appointmentID_B = 0;
    int appointmentID_C = 0;

    // // start communication with child
    FILE *fp;
    // Opening file in writing mode --> will overwrite the file if it exists
    fp = fopen("All_Requests.dat", "w");
    while (processEnd != 1)
    {
        // generate ID
        char id[4];
        id[0] = appointmentID_A + '0';
        id[1] = appointmentID_B + '0';
        id[2] = appointmentID_C + '0';
        id[4] = '\0';

        if (appointmentID_C < 10)
        {
            appointmentID_C++;
        }
        else if (appointmentID_B < 10)
        {
            appointmentID_B++;
            appointmentID_C = 0;
        }
        else
        {
            appointmentID_C = 0;
            appointmentID_B = 0;
            appointmentID_A++;
        }

        // clear command before input
        for (i = 0; i <= 14; i++)
        {
            strcpy(command[i], "");
        }

        // read input from keyboard
        printf("Please enter appointment: \n");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

        // write user input into file
        fprintf(fp, "%s\n", input);

        // spilt by space and store into token
        char *token;
        token = strtok(input, " ");
        i = 0;
        while (token != NULL)
        {
            strcpy(command[i], token);
            // printf("%s \n",command[i]); //debug what is input
            i++;
            token = strtok(NULL, " ");
        }

        // printf("Repeat: your command is %s \n", command); // debug

        // command: endProgram
        if (strcmp(command[0], "endProgram") == 0)
        {

            for (i = 0; i < userNum; i++)
            {
                strcpy(buf, "endProgram");
                write(fd[i][0][1], buf, strlen(buf));
                printf("msg sent to child %d \n", i);
                buf_n = read(fd[i][1][0], buf, 100);
                buf[buf_n] = '\0';
                printf("msg receive from child %d \n", i);
            }
            processEnd = 1;
        }
        // ----------------new implement
        // add event
        else if (strcmp(command[0], "privateTime") == 0 || strcmp(command[0], "projectMeeting") == 0 || strcmp(command[0], "groupStudy") == 0 || strcmp(command[0], "gathering") == 0)
        {
            // char involved[10][20]; // store who involved the event

            // check data valid or not
            int isValid = 1;
            // check name exist and name index
            int child_index = checkName(command[1], name, userNum);
            if (child_index == -1)
            {
                isValid = 0;
                printf("No such name %s!\n", command[1]);
            }
            // store involved to array
            j = 5;
            while (strcmp(command[j], "") != 0)
            {
                child_index = checkName(command[1], name, userNum);
                if (child_index > -1)
                {
                    strcpy(nameinvolved[eventIndex][j - 4], command[j]);
                }
                else
                {
                    isValid = 0;
                    printf("No such name %s!\n", command[j]);
                }
                j++;
            }
            // check date out of range?
            char tempY[5];
            char tempM[3];
            char tempD[3];
            for (j = 0; j <= 3; j++)
                tempY[j] = command[2][j];
            for (j = 4; j <= 5; j++)
                tempM[j] = command[2][j + 4];
            for (j = 6; j <= 7; j++)
                tempD[j] = command[2][j + 6];
            int tempYear = atoi(tempY);
            int tempMonth = atoi(tempM);
            int tempDay = atoi(tempD);
            if (!(tempYear >= startYear && tempYear <= endYear && tempMonth >= startMonth && tempMonth <= endMonth && tempDay >= startDay && tempDay <= endDay))
            {
                isValid = 0;
                printf("Date %d is out of the range.\n", command[2]);
            }
            // check start time
            if (!(strcmp(command[3], "1800") == 0 || strcmp(command[3], "1900") == 0 || strcmp(command[3], "2000") == 0 || strcmp(command[3], "2100") == 0 || strcmp(command[3], "2200") == 0))
            {
                isValid = 0;
                printf("Time %d is out of the range.\n", command[3]);
            }
            float tempDuration = atof(command[4]);
            if (tempDuration > 5.0 || tempDuration < 0.0)
            {
                isValid = 0;
                printf("Duration %d is not in the range.\n", command[4]);
            }

            if (isValid == 1)
            {
                char idString[4];
                sprintf(idString, "%d", eventIndex);
                // (char myEvents[][5][15], int *eventCount, const char *eventType, const char *date, const char *time, const char *duration, const char *id)
                addEvent(allEvents, &eventIndex, command[0], command[2], command[3], command[4], idString);

                // add to name involved array
                strcpy(nameinvolved[eventIndex][0], command[1]);
                j = 5;
                while (strcmp(command[j], "") != 0)
                {
                    strcpy(nameinvolved[eventIndex][j - 4], command[j]);
                    j++;
                }
                /*
                strcpy(event[eventIndex].type, command[0]);
                for (j = 0, j < 10; j++) {
                    strcpy(event[eventIndex].name[i], nameinvolved[i]);
                }
                strcpy(event[eventIndex].data, command[2]);
                event[eventIndex].time = atoi(command[3]);
                event[eventIndex].duration = atof(command[4]);
                eventIndex++;
                */
            }
        }

        // command: privateTime
        else if (strcmp(command[0], "privateTime") == 0 || strcmp(command[0], "printEvent") == 0)
        {

            int child_index = -1;
            for (i = 0; i < userNum; i++)
            {
                // Find the index of the child
                if (strcmp(command[1], name[i]) == 0)
                {
                    child_index = i;
                    break;
                }
            }
            if (child_index > -1)
            {
                char cat_string[100];
                if (strcmp(command[0], "privateTime") == 0)
                {
                    strcpy(cat_string, "");         // clear cat_string
                    strcat(cat_string, command[0]); // command type
                    strcat(cat_string, " ");
                    strcat(cat_string, command[2]); // date
                    strcat(cat_string, " ");
                    strcat(cat_string, command[3]); // start time
                    strcat(cat_string, " ");
                    strcat(cat_string, command[4]); // duartion
                    strcat(cat_string, " ");
                    strcat(cat_string, id); // id
                }
                else
                    strcpy(cat_string, "printEvent");

                strcpy(buf, cat_string); // copy to buf

                write(fd[child_index][0][1], buf, strlen(buf));
                printf("msg sent to child %d \n", i);

                // Reading info from child
                buf_n = read(fd[i][1][0], buf, 100);
                buf[buf_n] = '\0';
                printf("Reading by parent --> child %d: %s \n", i, buf);
            }
        }

        else if (strcmp(command[0], "projectMeeting") == 0 || strcmp(command[0], "groupStudy") == 0 || strcmp(command[0], "gathering") == 0)
        {

            int child_index;

            char involved[10][20]; // store who involved the event
            char cat_string[100];
            strcpy(cat_string, "");         // clear buf
            strcat(cat_string, command[0]); // command type
            strcat(cat_string, " ");
            strcat(cat_string, command[2]); // date
            strcat(cat_string, " ");
            strcat(cat_string, command[3]); // start time
            strcat(cat_string, " ");
            strcat(cat_string, command[4]); // duartion
            strcat(cat_string, " ");
            strcat(cat_string, id); // id

            strcpy(involved[0], command[1]);
            j = 5;
            while (strcmp(command[j], "") != 0)
            {
                strcpy(involved[j - 4], command[j]);
                j++;
            }

            for (i = 0; i < j; i++)
            {
                for (child_index = 0; child_index < userNum; child_index++)
                {
                    if (strcmp(involved[i], name[child_index]) == 0)
                    {
                        strcpy(buf, cat_string); // copy to buf
                        write(fd[child_index][0][1], buf, strlen(buf));
                        printf("msg sent to child %d \n", i);

                        // Reading info from child
                        buf_n = read(fd[child_index][1][0], buf, 100);
                        buf[buf_n] = '\0';
                        printf("Reading by parent --> child %d: %s \n", i, buf);
                    }
                }
            }
        }
        else if (strcmp(command[0], "printSchd") == 0)
        {
            // printf("debug: send printSchd to child\n");
            for (i = 0; i < userNum; i++)
            {
                if (strcmp(command[1], "FCFS") == 0)
                {
                    strcpy(buf, "printSchd FCFS");
                }

                write(fd[i][0][1], buf, strlen(buf));
                buf_n = read(fd[i][1][0], buf, 100);
                buf[buf_n] = '\0';

                // buf read from child should be id of reject event
                // strcat the id
            }
            // after all child successfully make their schedule and write reject id to parent,
            // strtok the string of id to many id
            // while (!=the end of token)
            //  for each child
            //     send id to child
            //     read buf from child (child will write sth to parent after search and reject the id)
        }

        // bgladosd alternative try on the flow
        else if (strcmp(command[0], "printSchdTemp") == 0)
        {
            // printf("debug: send printSchd to child\n");
            for (i = 0; i < userNum; i++)
            {
                if (strcmp(command[1], "FCFS") == 0)
                {
                    strcpy(buf, "printSchdTemp FCFS");
                }

                write(fd[i][0][1], buf, strlen(buf));
                buf_n = read(fd[i][1][0], buf, 100);
                buf[buf_n] = '\0';

                // buf read from child should be id of reject event
                // strcat the id
            }
            // after all child successfully make their schedule and write reject id to parent,
            // strtok the string of id to many id
            // while (!=the end of token)
            //  for each child
            //     send id to child
            //     read buf from child (child will write sth to parent after search and reject the id)
        }
    }

    // parent end process--------------------------------------------------------------------------------

    // wait for child process
    for (i = 1; i < userNum; i++)
    {
        wait(NULL);
        close(fd[i][1][0]);
    }

    // Close the file
    fclose(fp);
    printf("Parent pid %d: ByeBye!\n", getpid());

    exit(0);

    // cc apo.c -o apo
}