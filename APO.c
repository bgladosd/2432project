#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// give string output First letter UP
char *upFirstLetter(char *str)
{
    char *newStr = malloc(sizeof(char) * (strlen(str) + 1));
    newStr[0] = toupper(str[0]);
    strcpy(newStr + 1, str + 1);
    return newStr;
}

// check if string is only space
int isOnlySpace(char *str)
{
    while (*str)
    {
        if (!isspace(*str))
        {
            return 0;
        }
        str++;
    }
    return 1;
}

float utilizationCal(char slot[][5][5][15], int dayNum)
{
    float percentage;
    int usedSlot = 0;
    int i, j, k;

    for (i = 0; i <= dayNum; i++)
    {
        for (j = 0; j < 5; j++)
        {
            if (strcmp(slot[i][j][0], "empty"))
                usedSlot++;
        }
    }

    float a, b;
    a = usedSlot;
    b = (dayNum + 1) * 5;

    printf("debug: utilization dayNum: %f %f\n", a, b);

    percentage = a / b;
    return percentage;
}

// pull element in the index 0 of command array, then all move forward 1 index, the last will become empty
void pullCommandArray(char command[][15][20], int *commandIndex, int pullIndex)
{
    int i, j;
    if ((*commandIndex) > 0)
    { // index = 0 -> only one element inside
        for (i = pullIndex; i <= *commandIndex; i++)
        {
            for (j = 0; j < 15; j++)
            {
                strcpy(command[i][j], command[i + 1][j]);
            }
        }
    }

    // last to null // zero element will only run this
    for (j = 0; j < 15; j++)
    {
        strcpy(command[(*commandIndex)][j], "");
    }
    // printf("pull, index 0 is now: %s %s\n", command[0][0], command[0][1]);
    (*commandIndex)--;
    // printf("index is: %d\n", *commandIndex);
}

// accept batch file to input command into command array
int inputFileCommand(char command[200][15][20], int *commandIndex)
{
    int i, j;
    char line[300];
    char fileName[20];
    int fileLine = 0; // count amount of line loaded
    // printf("index: %d\n", *commandIndex);
    strcpy(fileName, command[*commandIndex][1]);
    fileName[strcspn(fileName, "\r\n")] = '\0';
    // printf("index 9: '%d'\n", fileName[9]);
    printf("opening file %s...\n", fileName);
    // printf("filename length = %d\n", strlen(fileName));
    //  after reading command, pull array
    pullCommandArray(command, commandIndex, *commandIndex); // index = -1 here
    // printf("index after file pull: %d\n", commandIndex);
    //  open file
    FILE *ifp = fopen(fileName, "r");
    // printf("ifp = %d\n",ifp);
    if (ifp == NULL)
    { // open file fail
        printf("Error opening file.\n");
    }
    else
    { // open file success
        while (fgets(line, 300, ifp) != NULL)
        { // read every line in file
            fileLine++;
            (*commandIndex)++;                  // store in starting from index 0
            line[strcspn(line, "\r\n")] = '\0'; // assign \0, if it is \r\n
            // printf("read: %s\n", line);
            //  split to command
            char *token;
            token = strtok(line, " ");
            i = 0;
            while (token != NULL) // read each character in line
            {
                strcpy(command[*commandIndex][i], token);
                // printf("read <----- %s\n",command[commandIndex][i]);
                //  printf("%s \n",command[i]); //debug what is input
                i++;
                token = strtok(NULL, " ");
            }
            // if file include a command to input the file now opening, delete that line, otherwise will loop
            if ((strcmp(command[*commandIndex][0], "inputFile") == 0) && (strcmp(command[*commandIndex][1], fileName) == 0))
            {
                printf("command[%d]: %s, %s is now deleted due to having the same file name as the opening file. \n", *commandIndex, command[*commandIndex][0], command[*commandIndex][1]);
                for (j = 0; j < 15; j++)
                {
                    strcpy(command[*commandIndex][j], "");
                }
                fileLine--;
                *commandIndex--;
            }
            // printf("previous command[%d]: %s, %s\n", (*commandIndex)-1 ,command[(*commandIndex)-1][0], command[(*commandIndex)-1][1]);
            // printf("loading command[%d]: %s, %s\n", *commandIndex ,command[*commandIndex][0], command[*commandIndex][1]);
            if ((strcmp(command[*commandIndex][0], "inputFile") == 0))
            {
                inputFileCommand(command, commandIndex);
            }

            // fprintf(file, line);
        }
        printf("Input file %s finished. Loaded %d line(s).\n", fileName, fileLine);
    }
    // close file
    fclose(ifp);
}

void cloneEvent(char allmyEvents[][5][15], char clone_myEvents[][5][15], int eventCount)
{
    int arrayLength = eventCount;
    int i, j;
    for (i = 0; i < arrayLength; i++)
    {
        for (j = 0; j < 5; j++)
        {
            strcpy(clone_myEvents[i][j], allmyEvents[i][j]);
        }
    }

    for (i = 0; i < eventCount; i++)
    {
        printf("debug before: %s %s %s %s %s \n", allmyEvents[i][0], allmyEvents[i][1], allmyEvents[i][2], allmyEvents[i][3], allmyEvents[i][4]);
    }

    for (i = 0; i < eventCount; i++)
    {
        printf("debug cloneEvents: %s %s %s %s %s \n", clone_myEvents[i][0], clone_myEvents[i][1], clone_myEvents[i][2], clone_myEvents[i][3], clone_myEvents[i][4]);
    }
}

void sortEventByPriority(char myEvents[][5][15], int eventCount)
{
    int arrayLength = eventCount;
    int i, j;

    for (i = 0; i < eventCount; i++)
    {
        printf("debug before sort: %s %s %s %s %s \n", myEvents[i][0], myEvents[i][1], myEvents[i][2], myEvents[i][3], myEvents[i][4]);
    }

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

    for (i = 0; i < eventCount; i++)
    {
        printf("debug sortEvents: %s %s %s %s %s \n", myEvents[i][0], myEvents[i][1], myEvents[i][2], myEvents[i][3], myEvents[i][4]);
    }
}

void capitalizeNames(char name[][20], char nameWithCap[][20], int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        // Copy the original name to nameWithCap
        strcpy(nameWithCap[i], name[i]);

        // If the first character is lowercase, convert to uppercase
        if (name[i][0] >= 'a' && name[i][0] <= 'z')
        {
            nameWithCap[i][0] = name[i][0] - 32;
        }
    }
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
            if (!(inputTime + (int)(duration * 100) <= existingTime || inputTime >= existingTime + (int)(existingDuration * 100)))
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
//, char privateTime[50][6][15], char projectMeeting[50][6][15], char groupStudy[50][6][15], char gathering[50][6][15]
void addEvent(char myEvents[200][5][15], int *eventCount, const char *eventType, const char *date, const char *time, const char *duration, const char *id)
{
    strcpy(myEvents[*eventCount][0], eventType);
    strcpy(myEvents[*eventCount][1], date);
    strcpy(myEvents[*eventCount][2], time);
    strcpy(myEvents[*eventCount][3], duration);
    strcpy(myEvents[*eventCount][4], id);

    (*eventCount)++;
}

void addEvent_privateTime(char privateTime[50][6][15], int *privateTimeCount, const char *eventType, const char *date, const char *time, const char *duration, const char *id)
{
    strcpy(privateTime[*privateTimeCount][0], eventType);
    strcpy(privateTime[*privateTimeCount][1], date);
    strcpy(privateTime[*privateTimeCount][2], time);
    strcpy(privateTime[*privateTimeCount][3], duration);
    strcpy(privateTime[*privateTimeCount][4], id);

    (*privateTimeCount)++;
}

void addEvent_projectMeeting(char projectMeeting[50][6][15], int *projectMeetingCount, const char *eventType, const char *date, const char *time, const char *duration, const char *id)
{
    strcpy(projectMeeting[*projectMeetingCount][0], eventType);
    strcpy(projectMeeting[*projectMeetingCount][1], date);
    strcpy(projectMeeting[*projectMeetingCount][2], time);
    strcpy(projectMeeting[*projectMeetingCount][3], duration);
    strcpy(projectMeeting[*projectMeetingCount][4], id);

    (*projectMeetingCount)++;
}

void addEvent_groupStudy(char groupStudy[50][6][15], int *groupStudyCount, const char *eventType, const char *date, const char *time, const char *duration, const char *id)
{
    strcpy(groupStudy[*groupStudyCount][0], eventType);
    strcpy(groupStudy[*groupStudyCount][1], date);
    strcpy(groupStudy[*groupStudyCount][2], time);
    strcpy(groupStudy[*groupStudyCount][3], duration);
    strcpy(groupStudy[*groupStudyCount][4], id);

    (*groupStudyCount)++;
}

void addEvent_gathering(char gathering[50][6][15], int *gatheringCount, const char *eventType, const char *date, const char *time, const char *duration, const char *id)
{
    strcpy(gathering[*gatheringCount][0], eventType);
    strcpy(gathering[*gatheringCount][1], date);
    strcpy(gathering[*gatheringCount][2], time);
    strcpy(gathering[*gatheringCount][3], duration);
    strcpy(gathering[*gatheringCount][4], id);

    (*gatheringCount)++;
}

void combine_eventArray(char privateTime[50][6][15], char projectMeeting[50][6][15], char groupStudy[50][6][15], char gathering[50][6][15], int privateTimeCount, int projectMeetingCount, int groupStudyCount, int gatheringCount, char clone_myEvents[200][5][15])
{
    int i, j, pos;
    i = 0;
    pos = 0;
    int total = privateTimeCount + projectMeetingCount + groupStudyCount + gatheringCount;

    printf("debug before combine_eventArray\n");
    while (i < privateTimeCount)
    {
        printf("event %d, %s %s %s %s %s \n", i, privateTime[i][0], privateTime[i][1], privateTime[i][2], privateTime[i][3], privateTime[i][4]);
        i++;
    }
    i = 0;
    while (i < projectMeetingCount)
    {
        printf("event %d, %s %s %s %s %s \n", i, projectMeeting[i][0], projectMeeting[i][1], projectMeeting[i][2], projectMeeting[i][3], projectMeeting[i][4]);
        i++;
    }
    i = 0;
    while (i < groupStudyCount)
    {
        printf("event %d, %s %s %s %s %s \n", i, groupStudy[i][0], groupStudy[i][1], groupStudy[i][2], groupStudy[i][3], groupStudy[i][4]);
        i++;
    }
    i = 0;
    while (i < gatheringCount)
    {
        printf("event %d, %s %s %s %s %s \n", i, gathering[i][0], gathering[i][1], gathering[i][2], gathering[i][3], gathering[i][4]);
        i++;
    }
    i = 0;

    printf("debug combine_eventArray\n");
    while (i < privateTimeCount)
    {
        for (j = 0; j < 5; j++)
        {
            strcpy(clone_myEvents[i][j], privateTime[i][j]);
        }
        i++;
    }
    pos = i;
    i = 0;
    while (i < projectMeetingCount)
    {
        for (j = 0; j < 5; j++)
        {
            strcpy(clone_myEvents[i + privateTimeCount][j], projectMeeting[i][j]);
        }
        i++;
    }
    pos = i + pos;
    i = 0;
    while (i < groupStudyCount)
    {
        for (j = 0; j < 5; j++)
        {
            strcpy(clone_myEvents[i + privateTimeCount + projectMeetingCount][j], groupStudy[i][j]);
        }
        i++;
    }
    pos = i + pos;
    i = 0;
    while (i < gatheringCount)
    {
        for (j = 0; j < 5; j++)
        {
            strcpy(clone_myEvents[i + privateTimeCount + projectMeetingCount + groupStudyCount][j], gathering[i][j]);
        }
        i++;
    }

    // debug
    for (i = 0; i < total; i++)
    {
        printf("event %d, %s %s %s %s %s \n", i, clone_myEvents[i][0], clone_myEvents[i][1], clone_myEvents[i][2], clone_myEvents[i][3], clone_myEvents[i][4]);
    }
}

// bgladosd alternative of add event
//  void addEventAlt(char myEvents[200][5][15], int *eventCount, const char *eventType, const char *date, const char *time, const char *duration, const char *id)
//  {
//      strcpy(myEvents[*eventCount][0], eventType);
//      strcpy(myEvents[*eventCount][1], date);
//      strcpy(myEvents[*eventCount][2], time);
//      strcpy(myEvents[*eventCount][3], duration);
//      strcpy(myEvents[*eventCount][4], id);
//      (*eventCount)++;
//  }

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

int getDayNum(char start[], char end[], int startYear, int startMonth, int startDay)
{
    int today = atoi(start);
    int last = atoi(end);
    int year = startYear;
    int month = startMonth;
    int day = startDay;
    int leap = 0;
    int num_of_day = 0;

    while (last >= today)
    {
        // debug: printf("%d: %d %d \n",num_of_day,today,last);
        day++;
        num_of_day++;

        if ((year % 4 == 0 && year % 100 != 0) && (year % 400 == 0))
            leap = 1;

        if ((day == 31 && (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)) || (day == 30 && (month == 4 || month == 6 || month == 9 || month == 11)))
        {
            day = 1;
            month++;
        }
        else if (day == 28 && leap == 0 && month == 2)
        {
            day = 1;
            month++;
        }
        else if (day == 29 && leap == 1 && month == 2)
        {
            day = 1;
            month++;
        }

        if (month == 13)
        {
            month = 1;
            year++;
        }

        today = year * 10000 + month * 100 + day;
    }
    return num_of_day;
}

void setEmptySlots(char Slot[][5][5][15], int num_of_day)
{
    int i, j;
    for (i = 0; i < num_of_day; i++)
    {
        for (j = 0; j <= 4; j++)
        {
            strcpy(Slot[i][j][0], "empty");
        }
    }
}

bool tryTimeSlot(char event[5][15], char Slot[][5][5][15], char start[], int startYear, int startMonth, int startDay)
{
    printf("---->Child %d try: %s %s %s %s %s \n", getpid() - getppid() - 1, event[0], event[1], event[2], event[3], event[4]);
    int i, j, k, p;
    int dif_of_day = getDayNum(start, event[1], startYear, startMonth, startDay);

    char time[5][5] = {"1800", "1900", "2000", "2100", "2200"};

    int pos;
    int success = 1;

    for (k = 0; k <= 4; k++)
    { // check timeslots free or not
        // printf("debug: check each timeslot\n");
        if (strcmp(event[2], time[k]) == 0)
        {
            int dur;
            dur = atoi(event[3]);
            // printf("debug: check sametimeslot %d\n",dur);
            for (p = 0; p < dur; p++)
            {
                // printf("debug: timeslot %d :%d\n",18+k+j,timeSlotsSpace[k+p]);
                if (strcmp(Slot[dif_of_day][k + p][0], "empty") != 0)
                {
                    // timeSlotsSpace[k+p]=checkPriority(myEvents[j][0]);
                    success = 0;
                    printf("debug Not free: %s %s %s %s %s \n", event[0], event[1], event[2], event[3], event[4]);
                    break;
                }
            }
        }

        if (success == 0)
            break;
    }
    if (success == 0)
        return false;
    else
    {
        printf("debug free: %s %s %s %s %s \n", event[0], event[1], event[2], event[3], event[4]);
        return true;
    }
}

void addSlot(char event[5][15], char Slot[][5][5][15], char start[], int startYear, int startMonth, int startDay)
{
    printf("---->Child %d add: %s %s %s %s %s \n", getpid() - getppid() - 1, event[0], event[1], event[2], event[3], event[4]);
    int i, j, k, p;
    int dif_of_day = getDayNum(start, event[1], startYear, startMonth, startDay);

    char time[5][5] = {"1800", "1900", "2000", "2100", "2200"};
    int found = -1;

    for (k = 0; k <= 4; k++)
    { // check timeslots free or not
        // printf("debug: check each timeslot\n");
        if (strcmp(event[2], time[k]) == 0)
        {
            found = 1;
            int dur;
            dur = atoi(event[3]);
            // printf("debug: check sametimeslot %d\n",dur);
            for (p = 0; p < dur; p++)
            {
                // printf("debug: timeslot %d :%d\n",18+k+j,timeSlotsSpace[k+p]);
                for (i = 0; i <= 4; i++)
                {
                    strcpy(Slot[dif_of_day][k + p][i], event[i]);
                }
            }
        }

        if (found == 1)
            break;
    }

    printf("---->Child %d end: %s %s %s %s %s \n", getpid() - getppid() - 1, event[0], event[1], event[2], event[3], event[4]);
}

void doFCFS(char myEvents[][5][15], int *eventCount, char FCFS[][5][15], int *FCFSCount, int childID, int *rejectCount, char rejectID[][4])
{
    int i, j, k, p;
    int day = 9; // Lazy to calcuate difference of date ,use 9 first

    printf("debug: doFCFS \n");
    /*
        // clear FCFS storage first, empty can be used to check output or not in future
        for (i = 0; i <= *eventCount; i++)
        {
            strcpy(FCFS[i][0], "empty");
            strcpy(FCFS[i][4], "000");
        }
    */
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

void convertEventInfoForPrint(char arr[][15], char result[][15])
{
    // Convert date format from "YYYYMMDDHHMM" to "YYYY-MM-DD"
    char date[11];
    strncpy(date, arr[1], 4);
    date[4] = '-';
    strncpy(date + 5, arr[1] + 4, 2);
    date[7] = '-';
    strncpy(date + 8, arr[1] + 6, 2);
    date[10] = '\0';

    // Convert time format from "HH.MM" to "HH:MM"
    char startTime[6];
    strncpy(startTime, arr[2], 2);
    startTime[2] = ':';
    strncpy(startTime + 3, arr[2] + 2, 2);
    startTime[5] = '\0';

    // Calculate end time based on start time and duration
    int duration = atoi(arr[3]);
    char startHrStr[3];
    strncpy(startHrStr, arr[2], 2);
    startHrStr[2] = '\0';
    int endHour = (atoi(startHrStr) + duration) % 24;
    char endTime[6];
    sprintf(endTime, "%02d:%s", endHour, "00");

    // Convmert event type
    if (strcmp(arr[0], "privateTime") == 0)
    {
        strcpy(result[3], "Private Time");
    }
    else if (strcmp(arr[0], "projectMeeting") == 0)
    {
        strcpy(result[3], "Project Meeting");
    }
    else if (strcmp(arr[0], "groupStudy") == 0)
    {
        strcpy(result[3], "Group Study");
    }
    else
    {
        strcpy(result[3], "Gathering");
    }

    // Fill in result array
    strcpy(result[0], date);
    strcpy(result[1], startTime);
    strcpy(result[2], endTime);
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
    char nameWithCap[userNum][20];
    // events list for child and Parent
    //  allEvents[idOfEvent][0: Event Type, 1: Date, 2: Time, 3: Duration, 4:id]
    char allEvents[200][5][15] = {{{0}}};
    int rejectedList[200] = {0};
    int rejectedCount = 0;
    char clone_allEvents[200][5][15]; // for priority schedualing
    int schdMode = -1;                // use for print schd 1 = FCFS / 2 = Priority
    char privateTime[50][6][15];
    char projectMeeting[50][6][15];
    char groupStudy[50][6][15];
    char gathering[50][6][15];

    int privateTimeCount = 0;
    int projectMeetingCount = 0;
    int groupStudyCount = 0;
    int gatheringCount = 0;

    // get date of begin and end--------------------------------------------------------------
    int i, j, k;
    int startYear, endYear, startMonth, endMonth, startDay, endDay;
    char startDayStr[12];
    char endDayStr[12];
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
            strcpy(startDayStr, tempY);
            strcat(startDayStr, "-");
            strcat(startDayStr, tempM);
            strcat(startDayStr, "-");
            strcat(startDayStr, tempD);
        }
        else
        {
            endYear = atoi(tempY);
            endMonth = atoi(tempM);
            endDay = atoi(tempD);
            strcpy(endDayStr, tempY);
            strcat(endDayStr, "-");
            strcat(endDayStr, tempM);
            strcat(endDayStr, "-");
            strcat(endDayStr, tempD);
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
    capitalizeNames(name, nameWithCap, userNum);
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
        ////////////////////////////////////////////CHILD Start////////////////////////////////////////////////////////////////////////////////////////
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
            int childRealEventCount = 0;
            // Array to store the events that the child participated
            char FCFS[200][5][15];
            char rejectID[200][4];
            char FCFS_Slot[getDayNum(argv[1], argv[2], startYear, startMonth, startDay) + 1][5][5][15];
            char Priority_Slot[getDayNum(argv[1], argv[2], startYear, startMonth, startDay) + 1][5][5][15];

            // performance

            // myEvents[idOfEvent][0: Event Type, 1: Date, 2: Time, 3: Duration, 4:id][]
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
                    // printSchd
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
                                printf("%s %s %s %s %s\n", allEvents[k][0], allEvents[k][1], allEvents[k][2], allEvents[k][3], allEvents[k][4]);
                            }
                            strcpy(message, "-> [printEvent done] \n");
                            write(fd[i][1][1], message, sizeof(message));
                        }
                        // delete later
                        else if (strcmp(command[0], "printSchdasdasdasd") == 0)
                        { // printing recorded events
                            // printf("debug: check FCFS \n");
                            if (strcmp(command[1], "FCFS") == 0)
                            {
                                doFCFS(allEvents, &eventCount, FCFS, &FCFSCount, i, &rejectCount, rejectID);
                                strcpy(message, "-> [printSchd FCFS done] \n");
                                write(fd[i][1][1], message, sizeof(message));
                            }
                        }

                        // bgladosd alternative try on the flow CHILD
                        else if (strcmp(command[0], "printSchd") == 0)
                        { // printing recorded events
                            // printf("debug: check FCFS \n");
                            // if (strcmp(command[1], "FCFS") == 0)
                            // {
                            //     // doFCFS(myEvents, &eventCount, FCFS, &FCFSCount, i, &rejectCount, rejectID);
                            //     strcpy(message, "-> [printSchd FCFS done] \n");
                            //     write(fd[i][1][1], message, sizeof(message));
                            // }
                            schdMode = -1;
                            if (strcmp(command[1], "FCFS") == 0)
                            {
                                strcpy(message, "Starting PrintSchdTemp FCFS\n");
                                schdMode = 1;
                                write(fd[i][1][1], message, sizeof(message));
                                // clear slots before use
                                setEmptySlots(FCFS_Slot, getDayNum(argv[1], argv[2], startYear, startMonth, startDay) + 1);
                            }
                            else if (strcmp(command[1], "Priority") == 0)
                            {
                                strcpy(message, "Starting PrintSchdTemp Priority\n");
                                combine_eventArray(privateTime, projectMeeting, groupStudy, gathering, privateTimeCount, projectMeetingCount, groupStudyCount, gatheringCount, clone_allEvents);
                                schdMode = 2;
                                write(fd[i][1][1], message, sizeof(message));
                                // clear slots before use
                                setEmptySlots(Priority_Slot, getDayNum(argv[1], argv[2], startYear, startMonth, startDay) + 1);
                            }

                            int EventPointer = 0;

                            while (1)
                            {

                                bool childHaveEvent = false;
                                // child printSCHD confirmed and START

                                // read once/////////////////////////////////////////////
                                memset(message, 0, sizeof(message));
                                n = read(fd[i][0][0], message, sizeof(message));
                                if (n <= 0)
                                    break; // EOF or error
                                message[n] = '\0';

                                printf("DEBUG printSCHD 1: Child %d: Received--> %s \n", i, message);
                                if (strcmp("end", message) == 0)
                                {
                                    memset(message, 0, sizeof(message));
                                    n = read(fd[i][0][0], message, sizeof(message));
                                    if (n <= 0)
                                        break; // EOF or error
                                    message[n] = '\0';
                                    // Handle parent request of getting reil event ID
                                    if (strcmp("getRealEventId", message) == 0)
                                    {
                                        char childRealEventCountStr[5];
                                        sprintf(childRealEventCountStr, "%d", childRealEventCount);
                                        strcpy(message, childRealEventCountStr);
                                        write(fd[i][1][1], message, sizeof(message));

                                        // After sending the real event count, send the real event ID
                                        // Resst message
                                        memset(message, 0, sizeof(message));
                                        int numOfDaysIndex, timeSlotIndex;

                                        if (schdMode == 1)
                                        {
                                            for (numOfDaysIndex = 0; numOfDaysIndex < sizeof(FCFS_Slot) / sizeof(FCFS_Slot[0]); numOfDaysIndex++)
                                            {
                                                for (timeSlotIndex = 0; timeSlotIndex < 5; timeSlotIndex++)
                                                {

                                                    // Check if the time slot is empty
                                                    if (strcmp(FCFS_Slot[numOfDaysIndex][timeSlotIndex][0], "empty"))
                                                    {
                                                        // printf("Child %d: Event %d: %s, %s, %s, %s, %s\n", i, j, FCFS_Slot[numOfDaysIndex][timeSlotIndex][0], FCFS_Slot[numOfDaysIndex][timeSlotIndex][1], FCFS_Slot[numOfDaysIndex][timeSlotIndex][2], FCFS_Slot[numOfDaysIndex][timeSlotIndex][3], FCFS_Slot[numOfDaysIndex][timeSlotIndex][4]);
                                                        if (strcmp(message, FCFS_Slot[numOfDaysIndex][timeSlotIndex][4]) == 0)
                                                        {
                                                            // Check if duration > 1, and already send to parent
                                                            continue;
                                                        }
                                                        strcpy(message, FCFS_Slot[numOfDaysIndex][timeSlotIndex][4]); // Event ID
                                                        write(fd[i][1][1], message, sizeof(message));
                                                    }
                                                }
                                            }

                                            // After printing all the events in the child, send the endRealEventId message
                                            strcpy(message, "endRealEventId");
                                            write(fd[i][1][1], message, sizeof(message));

                                            float util = utilizationCal(FCFS_Slot, getDayNum(argv[1], argv[2], startYear, startMonth, startDay)) * 100;
                                            char utilStr[50];
                                            sprintf(utilStr, "%g", util);
                                            strcpy(message, utilStr);
                                            write(fd[i][1][1], message, sizeof(message));
                                        }
                                        else if (schdMode == 2)
                                        {
                                            for (numOfDaysIndex = 0; numOfDaysIndex < sizeof(Priority_Slot) / sizeof(Priority_Slot[0]); numOfDaysIndex++)
                                            {
                                                for (timeSlotIndex = 0; timeSlotIndex < 5; timeSlotIndex++)
                                                {

                                                    // Check if the time slot is empty
                                                    if (strcmp(Priority_Slot[numOfDaysIndex][timeSlotIndex][0], "empty"))
                                                    {
                                                        // printf("Child %d: Event %d: %s, %s, %s, %s, %s\n", i, j, Priority_Slot[numOfDaysIndex][timeSlotIndex][0], Priority_Slot[numOfDaysIndex][timeSlotIndex][1], Priority_Slot[numOfDaysIndex][timeSlotIndex][2], Priority_Slot[numOfDaysIndex][timeSlotIndex][3], Priority_Slot[numOfDaysIndex][timeSlotIndex][4]);
                                                        if (strcmp(message, Priority_Slot[numOfDaysIndex][timeSlotIndex][4]) == 0)
                                                        {
                                                            // Check if duration > 1, and already send to parent
                                                            continue;
                                                        }
                                                        strcpy(message, Priority_Slot[numOfDaysIndex][timeSlotIndex][4]); // Event ID
                                                        write(fd[i][1][1], message, sizeof(message));
                                                    }
                                                }
                                            }

                                            // After printing all the events in the child, send the endRealEventId message
                                            strcpy(message, "endRealEventId");
                                            write(fd[i][1][1], message, sizeof(message));

                                            float util = utilizationCal(Priority_Slot, getDayNum(argv[1], argv[2], startYear, startMonth, startDay)) * 100;
                                            char utilStr[50];
                                            sprintf(utilStr, "%g", util);
                                            strcpy(message, utilStr);
                                            write(fd[i][1][1], message, sizeof(message));
                                        }
                                    }
                                    // end if first conversation is End
                                    break;
                                }

                                // should received an ID
                                // if the processing event round is larger than size of child event list
                                //
                                // Seems like it is ok to overflow so I don't do anything first
                                //
                                // if (atoi(message) > eventCount)
                                // {
                                //     printf("event overflow \n");
                                //     EventPointer = 0;
                                // }
                                // need check later if it is correct

                                // check if it is avaiable
                                if (schdMode == 1)
                                    printf("--->kk debug: Asking ID %d : Child Checking Event ID: %s : User is %s\n", atoi(message), allEvents[EventPointer][4], name[i]);
                                else if (schdMode == 2)
                                    printf("--->kk debug: Asking ID %d : Child Checking Event ID: %s : User is %s\n", atoi(message), clone_allEvents[EventPointer][4], name[i]);
                                if ((schdMode == 1 && strcmp(allEvents[EventPointer][4], message) == 0) || (schdMode == 2 && strcmp(clone_allEvents[EventPointer][4], message) == 0))
                                {
                                    // have that event, check if it is available
                                    // tryTimeSlot
                                    childHaveEvent = true;
                                    if (schdMode == 1)
                                    { // if not ok say no
                                        if (tryTimeSlot(allEvents[EventPointer], FCFS_Slot, argv[1], startYear, startMonth, startDay))
                                            strcpy(message, "ok");
                                        else
                                            strcpy(message, "no");
                                    }
                                    else if (schdMode == 2)
                                    {
                                        if (tryTimeSlot(clone_allEvents[EventPointer], Priority_Slot, argv[1], startYear, startMonth, startDay))
                                            strcpy(message, "ok");
                                        else
                                            strcpy(message, "no");
                                    }
                                }
                                else
                                {
                                    // don't have that event say ok
                                    strcpy(message, "ok");
                                }
                                // write once////////////////////////////////////////////////////
                                write(fd[i][1][1], message, sizeof(message));

                                // read once/////////////////////////////////////////////////////
                                memset(message, 0, sizeof(message));
                                n = read(fd[i][0][0], message, sizeof(message));
                                if (n <= 0)
                                    break; // EOF or error
                                message[n] = '\0';
                                printf("Child pass fail part: child %d received %s \n", i, message);

                                // should receive pass or fail
                                if (strcmp("pass", message) == 0)
                                {
                                    if (childHaveEvent)
                                    {
                                        // passed, log it to Calender
                                        if (schdMode == 1)
                                        {
                                            addSlot(allEvents[EventPointer], FCFS_Slot, argv[1], startYear, startMonth, startDay);
                                        }
                                        else if (schdMode == 2)
                                        {
                                            addSlot(clone_allEvents[EventPointer], Priority_Slot, argv[1], startYear, startMonth, startDay);
                                        }
                                        childRealEventCount++;
                                    }
                                }
                                else if (strcmp("fail", message) == 0)
                                {
                                    // failed
                                    if (childHaveEvent)
                                    {
                                        // add this to rejected list
                                        strcpy(rejectID[rejectCount++], allEvents[EventPointer][4]);
                                    }
                                }
                                else
                                {
                                    // if the respond not pass or faill it is really weird, we treate it as fail
                                    printf("super bug not pass or fail!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
                                    strcpy(rejectID[rejectCount++], allEvents[EventPointer][4]);
                                }
                                // tell parent this round is finished
                                // write once////////////////////////////////////////////////////
                                strcpy(message, name[i]);
                                write(fd[i][1][1], message, sizeof(message));
                                if (childHaveEvent)
                                {
                                    EventPointer++;
                                }
                                // finished one event on parent list,
                                // start listening
                            }
                        }
                        // child add event////////////////////////////////////////////////////
                        // it is an add event prompt, like privatetime
                        else
                        {
                            addEvent(allEvents, &eventCount, command[0], command[1], command[2], command[3], command[4]);
                            if (strcmp(command[0], "privateTime") == 0)
                                addEvent_privateTime(privateTime, &privateTimeCount, command[0], command[1], command[2], command[3], command[4]);
                            else if (strcmp(command[0], "gathering") == 0)
                                addEvent_gathering(gathering, &gatheringCount, command[0], command[1], command[2], command[3], command[4]);
                            else if (strcmp(command[0], "groupStudy") == 0)
                                addEvent_groupStudy(groupStudy, &groupStudyCount, command[0], command[1], command[2], command[3], command[4]);
                            else if (strcmp(command[0], "projectMeeting") == 0)
                                addEvent_projectMeeting(projectMeeting, &projectMeetingCount, command[0], command[1], command[2], command[3], command[4]);

                            strcpy(message, "-> [Recorded] \n");
                            write(fd[i][1][1], message, sizeof(message));
                            printf("child print event\n");
                            printf("1. %s\n", allEvents[0]);
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
            ////////////////////////////////////////////CHILD END////////////////////////////////////////////////////////////////////////////////////////
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
    ////////////////////////////////////////////Parent start////////////////////////////////////////////////////////////////////////////////////////
    int reportIndex = 0;
    char command[200][15][20];
    int commandIndex = -1;
    char input[100];

    // char allEvents[200][5][15]; moved up make all child also use this
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
        if (appointmentID_A == 0 && appointmentID_B == 0)
        {
            id[0] = appointmentID_C + '0';
            id[1] = '\0';
        }
        else if (appointmentID_A == 0 && appointmentID_B != 0)
        {
            id[0] = appointmentID_B + '0';
            id[1] = appointmentID_C + '0';
            id[2] = '\0';
        }
        else
        {
            id[0] = appointmentID_A + '0';
            id[1] = appointmentID_B + '0';
            id[2] = appointmentID_C + '0';
            id[4] = '\0';
        }

        if (appointmentID_C < 10)
        {
            appointmentID_C++;
            if (appointmentID_C == 10)
            {
                appointmentID_C = 0;
                appointmentID_B++;
            }
            if (appointmentID_B == 10)
            {
                appointmentID_B = 0;
                appointmentID_A++;
            }
        }
        /*
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
        */
        // clear command before input
        /*should be cleared in pullCommandArray
        for (i = 0; i <= 14; i++)
        {
            strcpy(command[i], "");
        }
        */

        // if command index = -1 , read input, else run command
        if (commandIndex == -1)
        {
            commandIndex++;
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
                strcpy(command[0][i], token);
                // printf("%s \n",command[i]); //debug what is input
                i++;
                token = strtok(NULL, " ");
            }
        }

        // command index = 0 after read user input
        // input file
        if (strcmp(command[0][0], "inputFile") == 0)
        {
            inputFileCommand(command, &commandIndex);
        }

        //        printf("Repeat: your command is %s \n", command); // debug

        // command: endProgram
        if (strcmp(command[0][0], "endProgram") == 0)
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
        // parent add event
        else if (strcmp(command[0][0], "privateTime") == 0 || strcmp(command[0][0], "projectMeeting") == 0 || strcmp(command[0][0], "groupStudy") == 0 || strcmp(command[0][0], "gathering") == 0)
        {
            // char involved[10][20]; // store who involved the event

            // check data valid or not
            int isValid = 1;
            // check name exist and name index
            int child_index = checkName(command[0][1], name, userNum);
            if (child_index == -1)
            {
                isValid = 0;
                printf("No such name %s!\n", command[0][1]);
            }
            // store involved to array
            j = 5;

            while (strcmp(command[0][j], "") != 0)
            {
                child_index = checkName(command[0][1], name, userNum);
                if (child_index > -1)
                {
                    strcpy(nameinvolved[eventIndex][j - 4], command[0][j]);
                }
                else
                {
                    isValid = 0;
                    printf("No such name %s!\n", command[0][j]);
                }
                j++;
            }
            // check date out of range?
            char tempY[5];
            char tempM[3];
            char tempD[3];
            for (j = 0; j <= 3; j++)
                tempY[j] = command[0][2][j];
            for (j = 4; j <= 5; j++)
                tempM[j - 4] = command[0][2][j];
            for (j = 6; j <= 7; j++)
                tempD[j - 6] = command[0][2][j];
            int tempYear = atoi(tempY);
            int tempMonth = atoi(tempM);
            int tempDay = atoi(tempD);
            if (!(tempYear >= startYear && tempYear <= endYear && tempMonth >= startMonth && tempMonth <= endMonth && tempDay >= startDay && tempDay <= endDay))
            {
                isValid = 0;
                printf("Date %s is out of the range.\n", command[0][2]);
            }
            // check start time
            if (!(strcmp(command[0][3], "1800") == 0 || strcmp(command[0][3], "1900") == 0 || strcmp(command[0][3], "2000") == 0 || strcmp(command[0][3], "2100") == 0 || strcmp(command[0][3], "2200") == 0))
            {
                isValid = 0;
                printf("Time %d is out of the range.\n", command[0][3]);
            }
            float tempDuration = atof(command[0][4]);
            if (tempDuration > 5.0 || tempDuration < 0.0)
            {
                isValid = 0;
                printf("Duration %d is not in the range.\n", command[0][4]);
            }

            if (isValid == 1)
            {
                printf("haha\n");
                char idString[4];
                sprintf(idString, "%d", eventIndex);
                // (char myEvents[][5][15], int *eventCount, const char *eventType, const char *date, const char *time, const char *duration, const char *id)

                // add to name involved array
                strcpy(nameinvolved[eventIndex][0], command[0][1]);
                j = 5;
                while (strcmp(command[0][j], "") != 0)
                {
                    strcpy(nameinvolved[eventIndex][j - 4], command[0][j]);
                    j++;
                }
                addEvent(allEvents, &eventIndex, command[0][0], command[0][2], command[0][3], command[0][4], idString);
                printf("parent add\n");
                int child_index;

                char involved[10][20]; // store who involved the event
                char cat_string[100];
                strcpy(cat_string, "");            // clear buf
                strcat(cat_string, command[0][0]); // command type
                strcat(cat_string, " ");
                strcat(cat_string, command[0][2]); // date
                strcat(cat_string, " ");
                strcat(cat_string, command[0][3]); // start time
                strcat(cat_string, " ");
                strcat(cat_string, command[0][4]); // duartion
                strcat(cat_string, " ");
                strcat(cat_string, id); // id

                strcpy(involved[0], command[0][1]);
                j = 5;
                while (strcmp(command[0][j], "") != 0)
                {
                    strcpy(involved[j - 4], command[0][j]);
                    j++;
                }

                if (strcmp(command[0][0], "privateTime") == 0)
                    addEvent_privateTime(privateTime, &privateTimeCount, command[0][0], command[0][2], command[0][3], command[0][4], id);
                else if (strcmp(command[0][0], "gathering") == 0)
                    addEvent_gathering(gathering, &gatheringCount, command[0][0], command[0][2], command[0][3], command[0][4], id);
                else if (strcmp(command[0][0], "groupStudy") == 0)
                    addEvent_groupStudy(groupStudy, &groupStudyCount, command[0][0], command[0][2], command[0][3], command[0][4], id);
                else if (strcmp(command[0][0], "projectMeeting") == 0)
                    addEvent_projectMeeting(projectMeeting, &projectMeetingCount, command[0][0], command[0][2], command[0][3], command[0][4], id);

                for (i = 0; i < j - 4; i++)
                {
                    // debug
                    printf("number of j: %d", j);
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
        }

        // command: printEvent For debug
        else if (strcmp(command[0][0], "printEvent") == 0)
        {

            int child_index = -1;
            for (i = 0; i < userNum; i++)
            {
                // Find the index of the child
                if (strcmp(command[0][1], name[i]) == 0)
                {
                    child_index = i;
                    break;
                }
            }
            // moveded to with projectmeeting should delete later
            //  if (child_index > -1)
            //  {
            //      char cat_string[100];
            //      if (strcmp(command[0], "privateTime") == 0)
            //      {
            //          strcpy(cat_string, "");         // clear cat_string
            //          strcat(cat_string, command[0]); // command type
            //          strcat(cat_string, " ");
            //          strcat(cat_string, command[2]); // date
            //          strcat(cat_string, " ");
            //          strcat(cat_string, command[3]); // start time
            //          strcat(cat_string, " ");
            //          strcat(cat_string, command[4]); // duartion
            //          strcat(cat_string, " ");
            //          strcat(cat_string, id); // id
            //      }
            //      else
            //          strcpy(cat_string, "printEvent");

            //     strcpy(buf, cat_string); // copy to buf

            //     write(fd[child_index][0][1], buf, strlen(buf));
            //     printf("msg sent to child %d \n", i);

            //     // Reading info from child
            //     buf_n = read(fd[i][1][0], buf, 100);
            //     buf[buf_n] = '\0';
            //     printf("Reading by parent --> child %d: %s \n", i, buf);
            // }
        }
        // delete later old
        else if (strcmp(command[0][0], "printSchdzxc") == 0)
        {
            // printf("debug: send printSchd to child\n");
            for (i = 0; i < userNum; i++)
            {
                if (strcmp(command[0][1], "FCFS") == 0)
                {
                    strcpy(buf, "printSchd FCFS");
                }
                else if (strcmp(command[0][1], "Priority") == 0)
                {
                    strcpy(buf, "printSchd Priority");
                    schdMode = 2;
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

        // Handle printSchd
        else if (strcmp(command[0][0], "printSchd") == 0)
        {

            printf("debug: In here!!!\n");
            // printf("debug: send printSchd to child\n");
            for (i = 0; i < userNum; i++)
            {
                strcpy(buf, "printSchd");
                if (strcmp(command[0][1], "FCFS") == 0)
                {
                    schdMode = 1;
                    strcpy(buf, "printSchd FCFS");
                }
                else if (strcmp(command[0][1], "Priority") == 0)
                {
                    schdMode = 2;
                    combine_eventArray(privateTime, projectMeeting, groupStudy, gathering, privateTimeCount, projectMeetingCount, groupStudyCount, gatheringCount, clone_allEvents);
                    strcpy(buf, "printSchd Priority");
                }

                write(fd[i][0][1], buf, strlen(buf));
                buf_n = read(fd[i][1][0], buf, 100);
                buf[buf_n] = '\0';
                if (schdMode == 1)
                    printf("printFCFS START Reading by parent --> child %s \n", buf);
                else if (schdMode == 2)
                    printf("printPriority START Reading by parent --> child %s \n", buf);

                // buf read from child should be id of reject event
                // strcat the id
            }

            int processingEvent, askingChild;
            char eventNumStr[10];
            int childOkForCurrentEvent[userNum]; // 0 = not ok, 1 = ok
            for (processingEvent = 0; processingEvent < eventIndex; processingEvent++)
            {
                int askingEvent = processingEvent;
                if (schdMode == 2)
                    askingEvent = atoi(clone_allEvents[processingEvent][4]);
                printf("Parrent asking EVENT : ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ %d \n", askingEvent);

                for (askingChild = 0; askingChild < userNum; askingChild++)
                {
                    // set childOkForCurrentEvent to 0
                    childOkForCurrentEvent[askingChild] = 0;
                    sprintf(eventNumStr, "%d", askingEvent);
                    memset(buf, 0, sizeof(buf));
                    strcpy(buf, eventNumStr);
                    write(fd[askingChild][0][1], buf, strlen(buf));
                    n = read(fd[askingChild][1][0], buf, 100);
                    buf[n] = '\0';
                    printf("Reading by parent --> child %d: %s \n", askingChild, buf);
                    if (strcmp(buf, "ok") == 0)
                    {
                        childOkForCurrentEvent[askingChild] = 1;
                    }
                    else
                    {
                        childOkForCurrentEvent[askingChild] = 0;
                        printf("Parent: Child %d cannot join. Event %d Fail !\n", askingChild, processingEvent);
                    }
                }
                // check if all child ok for current event
                int allChildOk = 1;
                for (askingChild = 0; askingChild < userNum; askingChild++)
                {
                    if (childOkForCurrentEvent[askingChild] == 0)
                    {
                        allChildOk = 0;
                    }
                }

                memset(buf, 0, sizeof(buf));

                printf("parent start write pass or fail \n");
                for (i = 0; i < userNum; i++)
                {
                    if (allChildOk == 1)
                    {
                        // Case of all child ok for current event
                        strcpy(buf, "pass");
                    }
                    else
                    {
                        // Case of any child fail to join current event
                        strcpy(buf, "fail");
                    }
                    write(fd[i][0][1], buf, strlen(buf));
                    buf_n = read(fd[i][1][0], buf, 100);
                    buf[buf_n] = '\0';
                    printf("printFCFS one turn end Reading by parent --> child %s \n", buf);
                }

                // Handle add to reject list
                if (allChildOk == 0)
                {
                    // Case of any child fail to join current event
                    // add to reject list
                    rejectedList[rejectedCount] = processingEvent;
                    rejectedCount++;
                }
            }
            // all process checked send End to child
            strcpy(buf, "end");
            for (i = 0; i < userNum; i++)
            {
                write(fd[i][0][1], buf, strlen(buf));
            }

            // Print the schedule to file
            FILE *fpFCFS;
            // reportIndex
            char reportFileName[40] = "";
            strcpy(reportFileName, "G30_");
            char reportNameId[10] = "";
            sprintf(reportNameId, "%02d", reportIndex);
            strcat(reportFileName, reportNameId);
            strcat(reportFileName, "_");
            strcat(reportFileName, command[0][1]);
            strcat(reportFileName, ".txt");
            fpFCFS = fopen(reportFileName, "w");

            fprintf(fpFCFS, "%s", "Period: ");
            fprintf(fpFCFS, "%s to %s\n", startDayStr, endDayStr);
            if (schdMode == 1)
                fprintf(fpFCFS, "%s\n", "Algorithm used: FCFS:");
            else if (schdMode == 2)
                fprintf(fpFCFS, "%s\n", "Algorithm used: Priority:");
            fprintf(fpFCFS, "\n%s\n", "***Appointment Schedule***");

            int numOfEventUserAccepted[userNum];
            memset(numOfEventUserAccepted, 0, sizeof(numOfEventUserAccepted));
            char userUtil[userNum][50];

            for (i = 0; i < userNum; i++)
            {
                strcpy(buf, "getRealEventId");
                write(fd[i][0][1], buf, strlen(buf));
                // Parent will keep reading until child send "endRealEventId"
                // First time read will be the number of events child has
                buf_n = read(fd[i][1][0], buf, 100);
                buf[buf_n] = '\0';
                //  allEvents[idOfEvent][0: Event Type, 1: Date, 2: Time, 3: Duration, 4:id]
                fprintf(fpFCFS, "\n  %s, you have %s̲ appointments.\n", nameWithCap[i], buf);

                fprintf(fpFCFS, "%-13s%-8s%-8s%-18s%-20s\n", "Date", "Start", "End", "Type", "People");
                fprintf(fpFCFS, "=================================================================\n");

                while (1)
                {
                    buf_n = read(fd[i][1][0], buf, 100);
                    buf[buf_n] = '\0';
                    if (strcmp(buf, "endRealEventId") == 0)
                    {
                        buf_n = read(fd[i][1][0], buf, 100);
                        buf[buf_n] = '\0';
                        strcpy(userUtil[i], buf);
                        break;
                    }
                    // After getting each event number, retrieve the event from allEvents
                    int allEventInedx, parti;
                    char curEventParticipants[20 * userNum + 10];

                    for (allEventInedx = 0; allEventInedx < sizeof(allEvents) / sizeof(allEvents[0]); allEventInedx++)
                    {
                        strcpy(curEventParticipants, "");
                        if (strcmp(allEvents[allEventInedx][4], buf) == 0)
                        {
                            numOfEventUserAccepted[i]++;
                            char eventInfo[4][15];
                            convertEventInfoForPrint(allEvents[allEventInedx], eventInfo);
                            for (parti = 0; parti < userNum; parti++)
                            {
                                if (strlen(allEvents[allEventInedx][parti]) > 1)
                                {
                                    if (strcmp(nameinvolved[allEventInedx][parti], name[i]) == 0)
                                        continue;
                                    strcat(curEventParticipants, upFirstLetter(nameinvolved[allEventInedx][parti]));
                                    strcat(curEventParticipants, " ");
                                }
                            }
                            if (isOnlySpace(curEventParticipants))
                                strcpy(curEventParticipants, "-");
                            fprintf(fpFCFS, "%-13s%-8s%-8s%-18s%-20s\n", eventInfo[0], eventInfo[1], eventInfo[2], eventInfo[3], curEventParticipants);
                            break;
                        }
                    }
                }
            }
            fprintf(fpFCFS, "\n");
            fprintf(fpFCFS, "%*s", (int)((50 + strlen(nameWithCap[i])) / 2), "- End of ");
            fprintf(fpFCFS, "%s's Schedule -\n", nameWithCap[i]);
            fprintf(fpFCFS, "=================================================================\n");
            fprintf(fpFCFS, "\n");
            fprintf(fpFCFS, "%*s", (int)((50 + strlen(nameWithCap[i])) / 2), "- End of ");
            fprintf(fpFCFS, "%s's Schedule -\n", nameWithCap[i]);
            fprintf(fpFCFS, "=================================================================\n");
            fprintf(fpFCFS, "\n\n%s\n", "***  Performance ***\n");
            // fprintf(fpFCFS, "Total Number of Requests Received: %d\n", eventIndex);
            // fprintf(fpFCFS, "Total Number of Requests Accepted: %d\n", 5);
            // fprintf(fpFCFS, "Total Number of Requests Rejected: %d\n", 5);
            // fprintf(fpFCFS, "\n\n");
            fprintf(fpFCFS, "Number of Requests Accepted by Individual:\n");
            for (i = 0; i < userNum; i++)
            {
                fprintf(fpFCFS, "%s: %d\n", nameWithCap[i], numOfEventUserAccepted[i]);
            }
            // do something
            fprintf(fpFCFS, "\n\n");
            fprintf(fpFCFS, "Utilization of Time Slot:\n");
            for (i = 0; i < userNum; i++)
            {
                fprintf(fpFCFS, "%s: %s%%\n", nameWithCap[i], userUtil[i]);
            }
            // Close the file
            fclose(fpFCFS);

            // printed one file index of report plus
            reportIndex++;

            printf("[Exported file: schedule.txt]\n");

            // Handle print rejected list
            FILE *fpReject;
            fpReject = fopen("rejected.dat", "w");

            fprintf(fpReject, "***Rejected List***\n\n");
            fprintf(fpReject, "Altogether there are %d̲ appointments rejected.\n", rejectedCount);
            fprintf(fpFCFS, "=================================================================\n");
            char curEventParticipants[20 * userNum + 10];
            int parti;
            for (i = 0; i < rejectedCount; i++)
            {
                strcpy(curEventParticipants, "");
                for (parti = 0; parti < userNum; parti++)
                {
                    if (strlen(allEvents[rejectedList[i]][parti]) > 0)
                    {
                        strcat(curEventParticipants, nameinvolved[rejectedList[i]][parti]);
                        strcat(curEventParticipants, " ");
                    }
                }
                fprintf(fpReject, "%d. %s %s %s %s %s\n", i + 1, allEvents[rejectedList[i]][0], allEvents[rejectedList[i]][1], allEvents[rejectedList[i]][2], allEvents[rejectedList[i]][3], curEventParticipants);
            }
            fprintf(fpFCFS, "\n");
            fprintf(fpFCFS, "%*s", (int)((85 + strlen(nameWithCap[i])) / 2), "- End of Rejected List -\n");
            fprintf(fpFCFS, "=================================================================\n");
            // Close the file
            fclose(fpReject);

            // Clear the variable for rejected list and count
            rejectedCount = 0;
            memset(rejectedList, 0, sizeof(rejectedList));
        }
        else if (strcmp(command[0][0], "inputFile") != 0)
        {
            printf("no such command.\n");
        }

        // printf("index: %d\n", commandIndex);
        //  if = inputFile, have one more file command to load. if != -1, array not empty
        if ((strcmp(command[0][0], "inputFile") != 0) && commandIndex != -1)
        {
            // printf("pull, index: %d\n", commandIndex);
            pullCommandArray(command, &commandIndex, 0); // pull out index 0, and index--
            // printf("Repeat: your command is %s \n", command); // debug
        }

        // if (commandIndex<-1) break;
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