/***************************************
*
*  Author: Caden Johnsen
*  Date: 2/9/20
*  File: adventure.c
*  Description: This program provides
*  an interface to play the game
*
****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

// creating a room struct
struct room {
  char name[20];
  char type[12];
  char connections[6][30];
  int connectionCounter;
};

// finds the directory name by searching through the current directory
void findDirectory(char *directoryName, DIR *dir, struct dirent *dp) {
  char* tempString;
  char* directoryPrefix = "dir.rooms";
  dir = opendir("."); // open the current directory

  // loop until no more lines to read
  while((dp = readdir(dir)) != NULL) {
    tempString = dp->d_name;  //save string names into tempString
    if(strstr(tempString, directoryPrefix) != NULL) { // compare if taken in file matches director specifications
      strcpy(directoryName, tempString);  // copies name of directory
    }
  }
  closedir(dir);  // closes directory
}

// opens the needed directory and reads in file names
void openDirectory(char *directoryName, char **fileNames, FILE* myFile, DIR *dir, struct dirent *dp) {
  int i = 0, counter = 0;
  char* tempString;

  dir = opendir(directoryName); // open the directory needed

  // read in every line from directory to get all file names
  while((dp = readdir(dir)) != NULL) {
    tempString = dp->d_name;  // assigns a temp variable with the string name of a file

    // stores tempString into a matrix that holds all the room file names
    if(counter >= 1) {
      fileNames[i] = tempString;  // build matrix to save file names
      i++;
    }
    counter++;
  }
  closedir(dir);
}

// opens the files and gets their input to store into room structs
void openFiles(char *directoryName, char *fileNames[7], int *index, struct room* rtemp) {
  int i = 0, temp2 = 0;
  int temp;
  char tempString[40];
  char tempType[40];
  char fileContents[100];
  char buf[200];
  FILE* myFile;

  strcpy(tempString, directoryName); // adding the file location so it can switch directories
  strcat(tempString, fileNames[(*index)]);  //adding the desired file name
  myFile = fopen(tempString, "r"); // opens file for writing

  fgets(buf, 255, myFile);  // get next line from file
  strcpy(rtemp->name, buf + 11);  // shift past constant information to room name

  fgets(buf,255,myFile);  // get next line from file
  strcpy(rtemp->connections[i], buf + 14);  // shift past constant information to connection name

  i++;
  fgets(buf,255,myFile);  // get next line from file
  strcpy(rtemp->connections[i], buf + 14);  // shift past constant information to connection name

  i++;
  fgets(buf,255,myFile);  // get next line from file
  strcpy(rtemp->connections[i], buf + 14);  // shift past constant information to connection name

  // loop until there are no more lines in the file
  while(temp2 == 0) {
    fgets(buf, 255, myFile);
    // checks if line that was just obtained begins with a C for CONNECTION
    if(buf[0] == 'C') {
      i++;
      strcpy(rtemp->connections[i], buf + 14);  // shift past constant information to connection name
      rtemp->connectionCounter++;  // increment number of connections
    }
    // checks if line that was just obtained begins with a R for ROOM TYPE
    else if(buf[0] == 'R') {
      strcpy(rtemp->type, buf + 11);  // shift past constant information to type
      temp2 = 1;
    }
  }
  fclose(myFile); // closes the file
}

// prints the actual room details
void printRoom(struct room* rtemp) {
  int i = 0;

  printf("CURRENT LOCATION: %s", rtemp->name);  // opening statement to print name of current room
  printf("POSSIBLE CONNECTIONS: ");

  // for loop prints each possible connection for that room
  for(i = 0; i < rtemp->connectionCounter; i++) {
    printf("%s", rtemp->connections[i]);
    // check if this is the last connection and end the sentence otherwise use a comma
    if(i + 1 < rtemp->connectionCounter) {
      printf(", ");
    }
    else {
      printf(".\n");
    }
  }
  printf("WHERE TO? >");
}

// check if the type of the room sent in is END_ROOM and exit while loop in main
void endGame(int *end, struct room* rtemp) {
  if(rtemp->type[0] == 'E') {
    (*end) = 1;
  }
}

// determines what to print for current room or exit out of loop
void displayCurrentRoom(char *userInput, int *end, char pathTaken[20][20], int counter, struct room* r1, struct room* r2, struct room* r3, struct room* r4,
struct room* r5, struct room* r6, struct room* r7) {
  int temp = 0;
  int i = 0;
  char tempString[20];

  strcpy(tempString, userInput);
  // see if user input is the same as the endgame room
  if(strstr(r1->name, userInput) != NULL) {
    endGame(end, r1);
  }
  else if(strstr(r2->name, userInput) != NULL) {
    endGame(end, r2);
}
  else if(strstr(r3->name, userInput) != NULL) {
    endGame(end, r3);
  }
  else if(strstr(r4->name, userInput) != NULL) {
    endGame(end, r4);
  }
  else if(strstr(r5->name, userInput) != NULL) {
    endGame(end, r5);
  }
  else if(strstr(r6->name, userInput) != NULL) {
    endGame(end, r6);
  }
  else if(strstr(r7->name, userInput) != NULL) {
    endGame(end, r7);
  }

  // if end has not been reached, go to next room
  if((*end) != 1) {
    if(strstr(r1->name, userInput) != NULL) {
      printRoom(r1);
      strcpy(pathTaken[counter], userInput);
    }
    else if(strstr(r2->name, userInput) != NULL) {
      printRoom(r2);
      strcpy(pathTaken[counter], userInput);
  }
    else if(strstr(r3->name, userInput) != NULL) {
      printRoom(r3);
      strcpy(pathTaken[counter], userInput);
    }
    else if(strstr(r4->name, userInput) != NULL) {
      printRoom(r4);
      strcpy(pathTaken[counter], userInput);
    }
    else if(strstr(r5->name, userInput) != NULL) {
      printRoom(r5);
      strcpy(pathTaken[counter], userInput);
    }
    else if(strstr(r6->name, userInput) != NULL) {
      printRoom(r6);
      strcpy(pathTaken[counter], userInput);
    }
    else if(strstr(r7->name, userInput) != NULL) {
      printRoom(r7);
      strcpy(pathTaken[counter], userInput);
    }
    else {
      printf("HUH I DON'T UNDERSTAND THAT ROOM. TRY AGAIN\n");
      strcpy(userInput, tempString);
    }
  }
}

// function to set beginning game conditions
void startGame(char *userInput, struct room* r1, struct room* r2, struct room* r3, struct room* r4,
struct room* r5, struct room* r6, struct room* r7) {
  int temp = 0;
  int i = 0;
  char* tempString;

  // check if first character of type is an S for START_ROOM for each room
  if(r1->type[0] == 'S') {
    strcpy(userInput, r1->name);
  }
  else if(r2->type[0] == 'S') {
    strcpy(userInput, r2->name);
  }
  else if(r3->type[0] == 'S') {
    strcpy(userInput, r3->name);
  }
  else if(r4->type[0] == 'S') {
    strcpy(userInput, r4->name);
  }
  else if(r5->type[0] == 'S') {
    strcpy(userInput, r5->name);
  }
  else if(r6->type[0] == 'S') {
    strcpy(userInput, r6->name);
  }
  else if(r7->type[0] == 'S') {
    strcpy(userInput, r7->name);
  }
}

// function to end the game and give the final statistics
void gameOver(int counter,  char pathTaken[20][20]) {
  int i = 0;
  printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
  printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", counter - 1);
  for(i = 0; i <counter - 1; i++) {
    printf("%s", pathTaken[i]);
  }
}

int main(int argc, char const *argv[]) {
  int i = 0, j = 1;
  int counter = 0;
  int end = 0;
  char directoryName[20];
  char userInput[20];
  char* fileNames[7];
  char* fileNamesTemp;
  char pathTaken[20][20];
  struct room r1, r2, r3, r4, r5, r6, r7; // create seven rooms
  DIR *dir;
  struct dirent *dp;
  FILE* myFile;

  r1.connectionCounter = 3; // initialize the connections to the min amount of 3
  r2.connectionCounter = 3;
  r3.connectionCounter = 3;
  r4.connectionCounter = 3;
  r5.connectionCounter = 3;
  r6.connectionCounter = 3;
  r7.connectionCounter = 3;

  findDirectory(directoryName, dir, dp);  // determine name of directory
  openDirectory(directoryName, fileNames, myFile, dir, dp); // open that directory
  strcat(directoryName, "/");  //adding the desired room name

  // open each file and get their contents stored into the room structs
  openFiles(directoryName, fileNames, &j, &r1);
  j++;
  openDirectory(directoryName, fileNames, myFile, dir, dp);
  openFiles(directoryName, fileNames, &j, &r2);
  j++;
  openDirectory(directoryName, fileNames, myFile, dir, dp);
  openFiles(directoryName, fileNames, &j, &r3);
  j++;
  openDirectory(directoryName, fileNames, myFile, dir, dp);
  openFiles(directoryName, fileNames, &j, &r4);
  j++;
  openDirectory(directoryName, fileNames, myFile, dir, dp);
  openFiles(directoryName, fileNames, &j, &r5);
  j++;
  openDirectory(directoryName, fileNames, myFile, dir, dp);
  openFiles(directoryName, fileNames, &j, &r6);
  j++;
  openDirectory(directoryName, fileNames, myFile, dir, dp);
  openFiles(directoryName, fileNames, &j, &r7);
  j++;
  openDirectory(directoryName, fileNames, myFile, dir, dp);

  // remove any trailing new line characters on connections
  for(i = 0; i < r1.connectionCounter; i++) {
    strtok(r1.connections[i], "\n");
  }
  for(i = 0; i < r2.connectionCounter; i++) {
    strtok(r2.connections[i], "\n");
  }
  for(i = 0; i < r3.connectionCounter; i++) {
    strtok(r3.connections[i], "\n");
  }
  for(i = 0; i < r4.connectionCounter; i++) {
    strtok(r4.connections[i], "\n");
  }
  for(i = 0; i < r5.connectionCounter; i++) {
    strtok(r5.connections[i], "\n");
  }
  for(i = 0; i < r6.connectionCounter; i++) {
    strtok(r6.connections[i], "\n");
  }
  for(i = 0; i < r7.connectionCounter; i++) {
    strtok(r7.connections[i], "\n");
  }

  // set start conditions for game
  startGame(userInput, &r1, &r2, &r3, &r4, &r5, &r6, &r7);

  // while end room has not been reached, keep going
  while(end == 0) {
      displayCurrentRoom(userInput, &end, pathTaken, counter, &r1, &r2, &r3, &r4, &r5, &r6, &r7);
      if(end == 0) {
        fgets(userInput, sizeof(userInput), stdin); // get user input
        // strcpy(pathTaken[counter], userInput);
      }
    counter++;
  }
  gameOver(counter, pathTaken);

  return 0;
}
