/***************************************
*
*  Author: Caden Johnsen
*  Date: 2/9/20
*  File: buildrooms.c
*  Description: This program creates a
*  series of files with descriptions
*  of the in-game rooms and connections
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

// creating a room struct
struct room {
  char name[8];
  char type[10];
  char* connections[6];
};

// function to remove previous rooms directories
void removeDirectory() {
  system("rm -r -f dir.rooms.*");  // removes directory recursively with files inside
}

// function to make the rooms directory
void makeDirectory(char *fileName, char *prefix, int pid) {
  sprintf(fileName, "%s%d\0", prefix, pid); // combines fileName with prefix and pid
  mkdir(fileName, 0755);  // creates the directory with the fileName
}

// function to make the room files
void makeRoomFiles(char *fileName, struct room* rtemp) {
  char tempString[40];
  int i = 0;
  int counter = 1;
  FILE* myFile;
  strcpy(tempString, fileName); // adding the file location so it can switch directories
  strcat(tempString, rtemp->name);  //adding the desired room name
  int fileDescriptor = open(tempString, O_RDWR | O_CREAT, 0600);  // makes file in directory
  myFile = fopen(tempString, "w+"); // opens file for writing
  fprintf(myFile, "ROOM NAME: %s\n", rtemp->name, rtemp->type);  // writes in name
  for(i = 0; i < 6; i++) {
    if(rtemp->connections[i] != "\0") {
      fprintf(myFile,"CONNECTION %d: %s\n", counter, rtemp->connections[i]);  // checks if a connection is there and writes it
      counter++;
    }
  }
  fprintf(myFile, "ROOM TYPE: %s\n", rtemp->type);  // writes in type
  fclose(myFile); // closes the file
}

// function to assign room names
void assignRoomNames(char **roomNames) {
  roomNames[0] = "Living";
  roomNames[1] = "Family";
  roomNames[2] = "Kitchen";
  roomNames[3] = "Bed";
  roomNames[4] = "Bath";
  roomNames[5] = "Master";
  roomNames[6] = "Office";
  roomNames[7] = "Dining";
  roomNames[8] = "Laundry";
  roomNames[9] = "Secret";
}

// function to assign room types
void assignRoomTypes(char **roomTypes) {
  roomTypes[0] = "START_ROOM";
  roomTypes[1] = "MID_ROOM";
  roomTypes[2] = "END_ROOM";
}

// checks if room name has already been used
void roomNameCheck(int *randomName, int *randomName2, int *roomNamesTracker, int *index2) {
  int i = 0;

  (*randomName) = rand() % 10;  // pick a random index to get a name from
  while (roomNamesTracker[(*randomName)] == 1) {  // loop to check if that name is taken
    (*randomName) = rand() % 10;  // pick a random index to get a name from
  }
  roomNamesTracker[(*randomName)] = 1;  // tell the tracking var which name is taken
}

// checks if room type has already been used
void roomTypeCheck(int *randomType, int *roomTypesTracker, int *index) {
  (*index)++;

  // checks if only END flag has been set
  if(roomTypesTracker[0] == 0 && roomTypesTracker[2] == 1) {  // checks if end room is created
    if((*index) == 7) {
      (*randomType) = 0;  // assigns a random value for type
      roomTypesTracker[0] = 1;  // sets START flag
    }
    else {
      (*randomType) = rand() % 2;  // assigns a random value for type
      if((*randomType) == 0) {
        roomTypesTracker[0] = 1;  // sets START flag
      }
    }
  }

  // checks if only START flag has been set
  else if(roomTypesTracker[0] == 1 && roomTypesTracker[2] == 0) { // checks if start room is created
    if((*index) == 7) {
      (*randomType) = 2;  // assigns a random value for type
      roomTypesTracker[2] = 1;  // sets END flag
    }
    else {
      (*randomType) = (rand() % 2) + 1;  // assigns a random value for type
      if((*randomType) == 2) {
        roomTypesTracker[2] = 1;  // sets END flag
      }
    }
  }

  // checks if both START and END flags have been set
  else if(roomTypesTracker[0] == 1 && roomTypesTracker[2] == 1) { // checks if start and end are created
    (*randomType) = (rand() % 1) + 1;  // assigns a random value for type
  }

  // checks if neither START and END flags have been set
  else if(roomTypesTracker[0] == 0 && roomTypesTracker[2] == 0) {
    if((*index) == 6) {
      (*randomType) = rand() % 1;  // assigns a random value for type
    }
    if((*index) == 7) {
      (*randomType) = (rand() % 1) + 2;  // assigns a random value for type
    }
    else {
      (*randomType) = rand() % 3;  // assigns a random value for type
      if((*randomType) == 0) {
        roomTypesTracker[0] = 1;  // sets START flag
      }
      if((*randomType) == 2) {
        roomTypesTracker[2] = 1;  // sets END flag
      }
    }
  }
}

// checks if room connections have already been made
void roomConnectionCheck(int *i, int roomConnectionsTracker[7][7]) {
  int j = 0;
  int temp = 0;
  int counter = 0;

    for(j = (*i); j < 7; j++) {
      if(j == (*i)) {
          roomConnectionsTracker[j][j] = 0; // makes sure room can not connect to itself
      }
      else {
        roomConnectionsTracker[(*i)][j] = (rand() % 2); // pick if it should connect or not
      }
      counter++;
    }
}

// checks if room connections have already been made
void roomConnectionCheck2(int *i, int roomConnectionsTracker[7][7]) {
  int j = 0;
  int temp = 0;
  int counter = 0;

    for(j = 0; j < (*i); j++) {
      roomConnectionsTracker[(*i)][j] = roomConnectionsTracker[j][(*i)];  // mirror the connection to both rooms
    }

}

// function to create a new room
void makeRoom(char **roomNames, char **roomTypes,
  struct room* r1, struct room* r2, struct room* r3, struct room* r4,
  struct room* r5, struct room* r6, struct room* r7) {
    int temp = 0, i = 0, j = 0, index = 0, index2 = 0;
    int r1c = 0, r2c = 0, r3c = 0, r4c = 0, r5c = 0, r6c = 0, r7c = 0;
    int randomName = 10;  // picks a random room name
    int randomConnection = 10;  // pick a random room to connect to
    int randomConnectionNum = 0; // pick number of connections from 3 to 6
    int randomType = 10;  // picks a random room type
    int roomNamesTracker[10]; // keeps track of which room names have been selected
    int roomTypesTracker[3];  // keeps track of which room types have been selected
    int roomConnectionChecker[7];  // keeps track of which room types have been selected
    int roomConnectionsTracker[7][7];  // keeps track of which connections have been made
    char tempString[8];

    for(j = 0; j < 10; j++) {
      roomNamesTracker[j] = 0;  // zero out all elements in room name tracker
    }
    for(j = 0; j < 3; j++) {
      roomTypesTracker[j] = 0;  // zero out all elements in room type tracker
    }
    for(j = 0; j < 7; j++) {
      roomConnectionChecker[j] = 0;  // zero out all elements in room name tracker
    }
    for(j = 0; j < 6; j++) {
      r1->connections[j] = "\0";
      r2->connections[j] = "\0";
      r3->connections[j] = "\0";
      r4->connections[j] = "\0";
      r5->connections[j] = "\0";
      r6->connections[j] = "\0";
      r7->connections[j] = "\0";
    }

    // assign room a random Type from roomTypes
    roomTypeCheck(&randomType, roomTypesTracker, &index);
    strcpy(r1->type, roomTypes[randomType]);
    roomTypeCheck(&randomType, roomTypesTracker, &index);
    strcpy(r2->type, roomTypes[randomType]);
    strcpy(r2->type, roomTypes[randomType]);
    roomTypeCheck(&randomType, roomTypesTracker, &index);
    strcpy(r3->type, roomTypes[randomType]);
    roomTypeCheck(&randomType, roomTypesTracker, &index);
    strcpy(r4->type, roomTypes[randomType]);
    roomTypeCheck(&randomType, roomTypesTracker, &index);
    roomTypeCheck(&randomType, roomTypesTracker, &index);
    strcpy(r5->type, roomTypes[randomType]);
    roomTypeCheck(&randomType, roomTypesTracker, &index);
    strcpy(r6->type, roomTypes[randomType]);
    roomTypeCheck(&randomType, roomTypesTracker, &index);
    strcpy(r7->type, roomTypes[randomType]);

    // assign room a random name from roomNames
    roomNameCheck(&randomName, &randomName, roomNamesTracker, &index2);
    strcpy(r1->name, roomNames[randomName]);
    roomNameCheck(&randomName, &randomName, roomNamesTracker, &index2);
    strcpy(r2->name, roomNames[randomName]);
    roomNameCheck(&randomName, &randomName, roomNamesTracker, &index2);
    strcpy(r3->name, roomNames[randomName]);
    roomNameCheck(&randomName, &randomName, roomNamesTracker, &index2);
    strcpy(r4->name, roomNames[randomName]);
    roomNameCheck(&randomName, &randomName, roomNamesTracker, &index2);
    strcpy(r5->name, roomNames[randomName]);
    roomNameCheck(&randomName, &randomName, roomNamesTracker, &index2);
    strcpy(r6->name, roomNames[randomName]);
    roomNameCheck(&randomName, &randomName, roomNamesTracker, &index2);
    strcpy(r7->name, roomNames[randomName]);

    // loop makes sure each room has at least 3 connections
    while(temp < 7) {
      temp = 0;
      for(j = 0; j < 7; j++) {
        roomConnectionChecker[j] = 0;  // zero out all elements in room name tracker
      }
      for(i = 0; i < 7; i++) {
      roomConnectionCheck(&i, roomConnectionsTracker);  // create each room connections
      roomConnectionCheck2(&i, roomConnectionsTracker); // mirror connections onto other rooms
      }
      for(i = 0; i < 7; i++){
        for(j = 0; j < 7; j++) {
          roomConnectionChecker[i] += roomConnectionsTracker[i][j]; // add up each row for at least 3 connections
        }
        if(roomConnectionChecker[i] >= 3) {
          temp++; // if row has more than 3 connections mark it as done
        }
      }
    }
    index = 0;

    // loop through each row and column and assign each connection to the other sides room name
    for(i = 0; i < 7; i++) {
      for(j = 0; j < 7; j++) {
        if(roomConnectionsTracker[i][j] == 1) { // check if connection exists
          if(i == 0) {  // check which row it appears in
            if(j == 1) {  //check which column it appears in
              r2->connections[r2c] = r1->name;
              r2c++;
            }
            else if(j == 2) {
              r3->connections[r3c] = r1->name;
              r3c++;
            }
            else if(j == 3) {
              r4->connections[r4c] = r1->name;
              r4c++;
            }
            else if(j == 4) {
              r5->connections[r5c] = r1->name;
              r5c++;
            }
            else if(j == 5) {
              r6->connections[r6c] = r1->name;
              r6c++;
            }
            else if(j == 6) {
              r7->connections[r7c] = r1->name;
              r7c++;
            }
          }
          else if(i == 1) { // checks which row it allears in
            if(j == 0) {  // checks which column it appears in
              r1->connections[r1c] = r2->name;
              r1c++;
            }
            else if(j == 2) {
              r3->connections[r3c] = r2->name;
              r3c++;
            }
            else if(j == 3) {
              r4->connections[r4c] = r2->name;
              r4c++;
            }
            else if(j == 4) {
              r5->connections[r5c] = r2->name;
              r5c++;
            }
            else if(j == 5) {
              r6->connections[r6c] = r2->name;
              r6c++;
            }
            else if(j == 6) {
              r7->connections[r7c] = r2->name;
              r7c++;
            }
          }
          else if(i == 2) { // check which row it appears in
            if(j == 0) {
              r1->connections[r1c] = r3->name;
              r1c++;
            }
            else if(j == 1) {
              r2->connections[r2c] = r3->name;
              r2c++;
            }
            else if(j == 3) {
              r4->connections[r4c] = r3->name;
              r4c++;
            }
            else if(j == 4) {
              r5->connections[r5c] = r3->name;
              r5c++;
            }
            else if(j == 5) {
              r6->connections[r6c] = r3->name;
              r6c++;
            }
            else if(j == 6) {
              r7->connections[r7c] = r3->name;
              r7c++;
            }
          }
          else if(i == 3) { // check which row it appears in
            if(j == 0) {
              r1->connections[r1c] = r4->name;
              r1c++;
            }
            else if(j == 1) {
              r2->connections[r2c] = r4->name;
              r2c++;
            }
            else if(j == 2) {
              r3->connections[r3c] = r4->name;
              r3c++;
            }
            else if(j == 4) {
              r5->connections[r5c] = r4->name;
              r5c++;
            }
            else if(j == 5) {
              r6->connections[r6c] = r4->name;
              r6c++;
            }
            else if(j == 6) {
              r7->connections[r7c] = r4->name;
              r7c++;
            }
          }
          else if(i == 4) { // check which row it appears in
            if(j == 0) {  // check which row it appears in
              r1->connections[r1c] = r5->name;
              r1c++;
            }
            else if(j == 1) {
              r2->connections[r2c] = r5->name;
              r2c++;
            }
            else if(j == 2) {
              r3->connections[r3c] = r5->name;
              r3c++;
            }
            else if(j == 3) {
              r4->connections[r4c] = r5->name;
              r4c++;
            }
            else if(j == 5) {
              r6->connections[r6c] = r5->name;
              r6c++;
            }
            else if(j == 6) {
              r7->connections[r7c] = r5->name;
              r7c++;
            }
          }
          else if(i == 5) { // check which row it appears in
            if(j == 0) {  // check which row it appears in
              r1->connections[r1c] = r6->name;
              r1c++;
            }
            else if(j == 1) {
              r2->connections[r2c] = r6->name;
              r2c++;
            }
            else if(j == 2) {
              r3->connections[r3c] = r6->name;
              r3c++;
            }
            else if(j == 3) {
              r4->connections[r4c] = r6->name;
              r4c++;
            }
            else if(j == 4) {
              r5->connections[r5c] = r6->name;
              r5c++;
            }
            else if(j == 6) {
              r7->connections[r7c] = r6->name;
              r7c++;
            }
          }
          else if(i == 6) { // check which row it appears in
            if(j == 0) {  // check which row it appears in
              r1->connections[r1c] = r7->name;
              r1c++;
            }
            else if(j == 1) {
              r2->connections[r2c] = r7->name;
              r2c++;
            }
            else if(j == 2) {
              r3->connections[r3c] = r7->name;
              r3c++;
            }
            else if(j == 3) {
              r4->connections[r4c] = r7->name;
              r4c++;
            }
            else if(j == 4) {
              r5->connections[r5c] = r7->name;
              r5c++;
            }
            else if(j == 5) {
              r6->connections[r6c] = r7->name;
              r6c++;
            }
          }
        }
      }
    }
}

int main(int argc, char const *argv[]) {
  int i = 0, j = 0;
  char fileName[40];
  char fileCreated[40];
  char* roomNames[10];
  char* roomTypes[10];
  int pid = getpid(); // gets PID number
  char prefix[] = "dir.rooms.";  // folder prefix
  char tempString[8];
  struct room r1, r2, r3, r4, r5, r6, r7; // create seven rooms

  srand(time(NULL));  // reset so that rand() function properly

  removeDirectory();  // remove previously used directories recursively
  makeDirectory(fileName, prefix, pid); // create the directory that will store the files
  assignRoomNames(roomNames); // specify room "name" titles
  assignRoomTypes(roomTypes); // specify room "type" titles
  makeRoom(roomNames, roomTypes, &r1, &r2, &r3, &r4, &r5, &r6, &r7);  // assigns all room names, types, and connections

  strcat(fileName, "/");  // add this to make sure it creates the file in the directory
  makeRoomFiles(fileName, &r1); // make room file for each room
  makeRoomFiles(fileName, &r2); // make room file for each room
  makeRoomFiles(fileName, &r3); // make room file for each room
  makeRoomFiles(fileName, &r4); // make room file for each room
  makeRoomFiles(fileName, &r5); // make room file for each room
  makeRoomFiles(fileName, &r6); // make room file for each room
  makeRoomFiles(fileName, &r7); // make room file for each room

  return 0; // end program
}
