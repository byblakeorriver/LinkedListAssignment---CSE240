//Blake Willoughby
//VS 2017

// READ BEFORE YOU START:
// Please read the given Word document for the project description with an illustrartive diagram.
// You are given a partially completed program that creates a list of dogs for an adoption shelter.
// Each dog has the corresponding information: name, breed, and a linked list of checkups.
// Please read the instructions above each required function and follow the directions carefully.
// If you modify any of the given code, return types, or parameters, you risk failing test cases.
//
// Note, Textbook Section 2.10 gives a case study on complex linked list operations. 
// This project is based on that case study. Make sure you read the code in section 2.10.

// The following will be accepted as input in the following format: "name:breed"
// Example Input:	"Spot:Terrier" or "Daisy:Poodle"
// Valid name:		String containing alphabetical letters beginning with a capital letter
// Valid breed:		String containing alphabetical letters beginning with a capital letter
// Valid date:		String in the following format: "MM/DD/YYYY" ex: "01/01/2010"
// All input will be a valid length and no more than the allowed number of dogs will be added to the linked list.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// included to check for memory leaks
#define CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#pragma warning(disable: 4996)

// used to create a linked list of containers, each contaning a "dog"
struct container {
	struct dog *dog;
	struct container *next;
} *listHead = NULL;

// used to hold dog information and linked list of "checkups"
struct dog {
	char name[30];
	char breed[30];
	struct checkup *checkups;
};

// used to create a linked list of checkups containing "dates"
struct checkup {
	char date[30];
	struct checkup *next;
};

// forward declaration of functions that have already been implemented
void flush();
void branching(char);
void helper(char);
void remove_all(struct container*);
void display(struct container*);

// the following forward declarations are for functions that require implementation

// return type		// name and parameters				// points
void				add_dog(char*, char*);				//  5
struct dog*			search_dog(char*);					//  5
void				add_checkup(char*, char*);			// 10
char*				last_checkup(char*);				// 15
void				remove_one(char*);					// 15
														//													Total: 50 points for hw07

struct container*	list_of_breed(char*);				// 25
struct container*	list_by_name();						// 25
														//													Total: 50 points for hw08

int main()
{
	char ch = 'i';

	printf("Dog Adoption Center\n\n");

	do
	{
		printf("Please enter your selection:\n");
		printf("\ta: add a new dog to the list\n");
		printf("\ts: search for a dog on the list\n");
		printf("\tr: remove a dog from the list\n");
		printf("\tc: add a checkup date for dog\n");
		printf("\tl: display last checkup for a dog\n");
		printf("\tn: display list of dogs by name\n");
		printf("\tb: display list of dogs of breed\n");
		printf("\tq: quit\n");
		ch = tolower(getchar());
		flush();
		branching(ch);
	} while (ch != 'q');

	remove_all(listHead);
	listHead = NULL;

	_CrtDumpMemoryLeaks();// check for memory leaks (VS will let you know in output if they exist)

	return 0;
}

// consume leftover '\n' characters
void flush()
{
	int c;
	do c = getchar(); while (c != '\n' && c != EOF);
}

// branch to different tasks
void branching(char c)
{
	switch (c)
	{
	case 'a':
	case 's':
	case 'r':
	case 'c':
	case 'l':
	case 'b':
	case 'n': helper(c); break;
	case 'q': break;
	default: printf("Invalid input!\n");
	}
}

// This function will determine what info is needed and which function to send that info to.
// It uses values that are returned from some functions to produce the correct ouput.
// There is no implementation needed here, but you should trace the code and know how it works.
// It is always helpful to understand how the code works before implementing new features.
// Do not change anything in this function or you risk failing the automated test cases.
void helper(char c)
{
	if (c == 'a')
	{
		char input[100];

		printf("\nPlease enter the dog's info in the following format:\n");
		printf("name:breed\n");
		fgets(input, sizeof(input), stdin);

		// discard '\n' chars attached to input
		input[strlen(input) - 1] = '\0';

		char* name = strtok(input, ":"); // strtok used to parse string
		char* breed = strtok(NULL, ":");

		struct dog* result = search_dog(name);

		if (result == NULL)
		{
			add_dog(name, breed);
			printf("\nDog added to list successfully\n\n");
		}
		else
			printf("\nThat dog is already on the list\n\n");
	}
	else if (c == 's' || c == 'r' || c == 'c' || c == 'l')
	{
		char name[30];

		printf("\nPlease enter the dog's name:\n");
		fgets(name, sizeof(name), stdin);

		// discard '\n' chars attached to input
		name[strlen(name) - 1] = '\0';

		struct dog* result = search_dog(name);
		
		if (result == NULL)
			printf("\nThat dog is not on the list\n\n");
		else if (c == 's')
			printf("\nBreed: %s\n\n", result->breed);
		else if (c == 'r')
		{
			remove_one(name);
			printf("\nDog removed from the list\n\n");
		}
		else if (c == 'c')
		{
			char date[30];

			printf("\nPlease enter the date of the checkup:\n");
			fgets(date, sizeof(date), stdin);

			// discard '\n' chars attached to input
			date[strlen(date) - 1] = '\0';

			add_checkup(name, date);
			printf("\nCheckup added\n\n");
		}
		else if (c == 'l')
		{
			char* result = last_checkup(name);
			if (result == NULL)
				printf("\nNo checkups documented.\n\n");
			else
				printf("\nLast checkup: %s\n\n", result);
		}
	}
	else if (c == 'b')
	{
		char breed[30];

		printf("\nPlease enter the breed:\n");
		fgets(breed, sizeof(breed), stdin);

		// discard '\n' chars attached to input
		breed[strlen(breed) - 1] = '\0';

		struct container* result = list_of_breed(breed);

		printf("\nList of dogs with breed type %s:\n\n", breed);

		display(result);
		
		remove_all(result);printf("made it");
		result = NULL;
	}
	else // c = 'n'
	{
		struct container* result = list_by_name();

		printf("\nList of dogs sorted by name:\n\n");

		display(result);
		remove_all(result);
		result = NULL;
	}
}

// This function recursively removes all dogs from the linked list of containers
// Notice that all of the checkups for all of the dogs must be removed as well
void remove_all(struct container* dogs)
{
	struct checkup* temp;
	if (dogs != NULL)
	{
		remove_all(dogs->next);
		while (dogs->dog->checkups != NULL)
		{
			temp = dogs->dog->checkups;
			dogs->dog->checkups = dogs->dog->checkups->next;
			free(temp);
		}
		free(dogs->dog);
		free(dogs);
	}
}

// This function prints the list of dogs in an organized format
// It may be useful to trace this code before you get started
void display(struct container* dogs)
{
	struct container* container_traverser = dogs;

	if (container_traverser == NULL)
	{
		printf("\nThere are no dogs on this list!\n\n");
		return;
	}

	while (container_traverser != NULL) // traverse list of dogs
	{
		printf("Name: %s\n", container_traverser->dog->name);
		printf("Breed: %s\n", container_traverser->dog->breed);
		printf("Checkups on file: ");

		struct checkup* ptr = container_traverser->dog->checkups;
		if (ptr == NULL)
		{
			printf("No checkups documented.");
		}
		else
		{
			while (ptr != NULL) // traverse list of checkups
			{
				printf("\n%s", ptr->date);
				ptr = ptr->next;
			}
		}

		printf("\n\n"); // formatting
		container_traverser = container_traverser->next;
	}
}

// hw07 Q1 : add (5 points)
// This function should add dog to the head of the list of containers.
// The function search_dog() is called before calling this function, 
// therefore you can assume that the dog is not already on the list.
void add_dog(char* name, char* breed)
{
	//declaration
	struct container *newCon=NULL;
	struct dog *newDog=NULL;
	struct checkup *newCheckups = NULL;
	//memory allocation
	newCon = (struct container *)malloc(sizeof(struct container));
	//if no memory left
	if (newCon == NULL) {
		printf("Fatal Error: Out of Memory. Exiting now.");
		return;
	}//if
	else {
		//If there is room to add allocate memory for new checkup and dog structures
		newDog = (struct dog *)malloc(sizeof(struct dog));
		newCheckups = (struct checkup *)malloc(sizeof(struct checkup));
		//if no memory left
		if (newDog == NULL) {
			printf("Fatal Error: Out of Memory. Exiting now.");
			return;
		}//if
		else {
			//Create the new container node
			strcpy(newDog->name, name);
			strcpy(newDog->breed, breed);
			newDog->checkups= NULL;//Make the checkups empty
			newCon->dog = newDog;
			newCon->next = listHead;//Put it at the beginning of the container list
			listHead = newCon;
		}//else
	}//else
}//add_dog

// hw07 Q2 : search (5 points)
// In this function, you are passed the name of a dog to find its breed.
// If the dog exists on the list, return a pointer to the requested dog. If not, return NULL.
// (You must return a pointer to a node in your list. Do not create a pointer that just includes the breed)
// (Remember that it is enough to search for a dog by only their name since no 2 dogs will have the same name)
struct dog* search_dog(char* name)
{
	//declare and initialize a finder and an iterator
	struct dog *find;
	struct container *iterator;
	iterator = (struct container *)malloc(sizeof(struct container));
	find = (struct dog *) malloc(sizeof(struct dog));

	iterator = listHead;//point iterator at the list
	while (iterator != NULL) {
		//If the iterators name matches the parameter name point the finder at the iterator and return the node
		if (stricmp(name, iterator->dog->name) == 0) {
			find = iterator->dog;
			return find;
		}//if
		else {
			iterator = iterator->next;//If not found iterate
		}//else
	}//while
	return NULL;//not found
}//search_dog

// hw07 Q3: add_checkup (10)
// In this function, you are passed the name of a dog and a date of a checkup.
// You should add the date to the tail of the linked list of the dogs "checkups".
// You can assume that all checkups will be added in chronological order.
// The function search_dog() is called before calling this function, 
// therefore you can assume that the dog is already on the dog list and 
// the new checkup date is not on the checkups list.
void add_checkup(char* name, char* date)
{	
	//declare structure pointers
	struct container *conIter;
	struct checkup *newCheckup;
	struct checkup *checkIter;
	//memory allocation	
	newCheckup = (struct checkup *)malloc(sizeof(struct checkup));
	//Point the iterator at the listHead
	conIter = listHead;
	//Put the new checkup date into the newCheckup node
	strcpy(newCheckup->date, date);
	newCheckup->next = NULL;
	//Put it at the end of the checkup linked list
	while (conIter != NULL) {
		//Find the right container and dog
		if (strcmp(conIter->dog->name, name) == 0) {
			if (conIter->dog->checkups == NULL) {
				//if checkup list is empty
				conIter->dog->checkups = newCheckup;
				return;//added
			}//if
			//point the checkup iterator at the beginning of the checkup list for a specific container
			checkIter = conIter->dog->checkups;
			//Get to the end of the checkup list
			while (checkIter->next != NULL) {
				checkIter = checkIter->next;
			}//while
				checkIter->next = newCheckup;//add to the end
				return;//added
		}//if
		else {
			//iterate the containers
			conIter = conIter->next;
		}//else
	}//while
}//add_checkup

// hw07 Q4: last_checkup (15)
// In this function, you are passed the name of a dog to find the date of its last checkup.
// Remember that checkups are stored in chronological order,
// therefore the last checkup will be at the tail of the linked list of checkups.
// If the dog has not yet had a checkup added to its list of checkups, return NULL.
// The function search_dog() is called before calling this function, 
// therefore you can assume that the dog is not already on the list.
char* last_checkup(char* name)
{
	//Declare pointer structures
	struct container *conIter;
	struct checkup *checkIter;
	//Point Container iterator at the beginning of the linked list
	conIter = listHead;
	//Find last checkup
	while (conIter != NULL) {
		//Found the right dog
		if (strcmp(conIter->dog->name, name) == 0) {
			checkIter = conIter->dog->checkups;//Point the checkIterator at the appropriate linked list of checkups
			if (checkIter == NULL) { return NULL; }//If no checkups
			while (checkIter->next != NULL) {
				//Iterate to last checkup
				checkIter = checkIter->next;
			}//while
			//return last checkup
			return checkIter->date;
		}//if
		else {
			//iterate containers
			conIter = conIter->next;
		}//else
	}//while
	return NULL;//Dog doesn't exist
}//last_checkup

// hw07 Q5: remove_one (15)
// In this function, you are passed the name of a dog to remove the corresponding dog from the list.
// The search function is called before this function so you can assume that the dog is on the list.
// You will need to find the dog and remove it using proper memory management to ensure no memory leaks.
void remove_one(char* name)
{
	//Declare an initialize structure pointers
	struct container *finder;//to find the one to remove
	struct container *temp;//for restructuring the linked list
	struct container *oneBefore;//the one before the finder
	struct dog *dogMatch;//To match the dogs
	//Memory allocation
	finder = (struct container *)malloc(sizeof(struct container));
	temp = (struct container *)malloc(sizeof(struct container));
	oneBefore = (struct container *)malloc(sizeof(struct container));
	//get the dog structure via the search_dog() function
	dogMatch = search_dog(name);
	//Point the finder at the beginning of the linked list
	finder = listHead;
	//remove the first one
	if (finder->dog == dogMatch) {
		temp = listHead;
		listHead = listHead->next;
		//Take care of memory leaks
		free(temp->dog->checkups);
		free(temp->dog);
		free(temp);
		return;//removed
	}//if
	//remove one in the middle and last positions
	while (finder->next != NULL) {
		oneBefore = listHead;//Point the oneBefore at the head node
		while (oneBefore->dog != dogMatch) {
			//If the one the finder is pointing at is the match stop before it
			if (oneBefore->next->dog == dogMatch) {
				temp = oneBefore->next;
				oneBefore->next = temp->next;//fix list structure
				//Take care of memory leaks
				free(temp->dog->checkups);
				free(temp->dog);
				free(temp);
				return;//removed
			}//if
			else {
				//iterate one before
				oneBefore = oneBefore->next;
			}//else
		}//while
		finder = finder->next;//iterate the finder
	}//while
}//remove_one

// hw08 Q1: list_of_breed (25)
// This function is used to construct a new linked list of containers from the global list of containers.
// The returned list should only contain dogs which are of the breed type parameter (container->dog->breed).
// No sorting is required for this list.
// The list that you return will be cleaned up for you by the remove_all() function (see helper() function),
// however you will need to make sure that you leave no dangling references (those cause memory leaks too).
// Notice that the returned list will need to contain all dog and checkup information to be displayed.
// This function should NOT modify the global linked list.
struct container* list_of_breed(char* breed)
{
	//declare structure pointers
	struct container *breedList;//new list
	struct container *iterator;//iterator for the original list
	struct container *temp;//new node
	struct container *breedIterator;//iterator for the breedList
	struct checkup *checkIterator;//To Iterator the checkList
	struct checkup *checkList;//New checkList
	struct checkup *aCheck;
	struct dog *tempDog = NULL;//temporary dog structure
	struct checkup *tempCheck = NULL;//temporary checkList
	
	//memory allocation
	checkIterator = (struct checkup *)malloc(sizeof(struct checkup));
	breedList = (struct container *)malloc(sizeof(struct container));
	iterator = (struct container *)malloc(sizeof(struct container));
	breedIterator = (struct container *)malloc(sizeof(struct container));
	checkList = (struct checkup *)malloc(sizeof(struct checkup));
	//NULL initializations
	checkList = NULL;
	breedList = NULL;
	//Point iterator at the beginning of the list
	iterator = listHead;
	
	while (iterator != NULL) {
		//Look for containers in the original linked list whose breed match the parameters and then put that container in the new list
		if (stricmp(iterator->dog->breed , breed)==0) {
			//allocate memory to transfer to the new List
			temp = (struct container *)malloc(sizeof(struct container));
			tempDog = (struct dog *)malloc(sizeof(struct dog));
			//Put the dog in the new container
			temp->dog = tempDog;
			strcpy(temp->dog->name, iterator->dog->name);
			strcpy(temp->dog->breed, iterator->dog->breed);
			temp->dog->checkups = NULL;
			temp->next = NULL;
			checkList = NULL;
			checkIterator = iterator->dog->checkups;
			//copy checkup list
			while (checkIterator != NULL) {
				//memory allocation for transfer temps
				tempCheck = (struct checkup *)malloc(sizeof(struct checkup));
				//copy from original list
				strcpy(tempCheck->date, checkIterator->date);
				tempCheck->next = NULL;
				//Put each checkup into the new list
				if (checkList == NULL) {
					//if the new list is empty
					checkList = tempCheck;
				}//if
				else {
					struct checkup *aCheck = checkList;
					while (aCheck->next != NULL) {
						aCheck = aCheck->next;
					}
					aCheck->next = tempCheck;
				}//else
				checkIterator = checkIterator->next;//iterate checkIterator
			}//while
			 //Put the new checkup list into the new dog structure
			temp->dog->checkups = checkList;
			checkList = NULL;
			//Add new Container to the new list
			if (breedList == NULL) {
				//if empty make first node
				breedList = temp;
			}//if
			else {
				//else put at the end of the list
				if (breedList->next == NULL) {
					breedList->next = temp;
				}//if
				else {
					//iterate to the end of the new list
					breedIterator = breedList;
					while (breedIterator->next != NULL) {
						breedIterator = breedIterator->next;
					}//while
					//add at the end
					breedIterator->next = temp;
				}//else
			}//else
		}//if
		//iterate the original list
		iterator = iterator->next;
	}//while
	
	
	return breedList;//return newList
}//list_by_breed

// hw08 Q2: list_by_name (25)
// This function is used to construct a new linked list of containers from the global list of containers.
// The returned list should be sorted alphabetically by each container's dog's name (container->dog->name).
// The list that you return will be cleaned up for you by the remove_all() function (see helper() function),
// however you will need to make sure that you leave no dangling references (those cause memory leaks too).
// Notice that the returned list will need to contain all dog and checkup information to be displayed.
// You can again assume that for this assignment, no 2 dogs on the list will have the same name.
// You may want to use the function that you have written above as a blueprint for this function.
// This function should NOT modify the global linked list.
struct container* list_by_name()
{
	//Declare structure pointers
	struct container *alphaList;//newList
	struct container *iterator;//to iterate the original list
	struct container *follower;//One to follow the iterator
	struct container *temp;// Used for transferring containers from originalList to alphaList
	struct container *alphaIterator;//For iterating the alphabetical list
	struct checkup *checkIterator;//To iterate the checkList of a node in the original list
	struct checkup *checkList;//new checkList for the new list that will recieve the checkups from the existing list
	struct checkup *tempCheck = NULL;//For transferring datat between lists
	struct dog *tempDog = NULL;//For transferring data between lists
	//memory allocation
	checkIterator = (struct checkup *)malloc(sizeof(struct checkup));
	alphaList = (struct container *)malloc(sizeof(struct container));
	iterator = (struct container *)malloc(sizeof(struct container));
	alphaIterator = (struct container *)malloc(sizeof(struct container));
	checkList = (struct checkup *)malloc(sizeof(struct checkup));
	checkList = NULL;
	alphaList = NULL;
	//point iterator at the beginning of the original list and the follower at the iterator
	iterator = listHead;
	follower = iterator;
	//go to every container in the list and place it in the new list in alphabetical order
	while (iterator != NULL) {
		
		//memory allocation for transfer temps
		temp = (struct container *)malloc(sizeof(struct container));
		tempDog = (struct dog *)malloc(sizeof(struct dog));
		//copy container of the original list
		temp->dog = tempDog;
		strcpy(temp->dog->name, iterator->dog->name);
		strcpy(temp->dog->breed, iterator->dog->breed);
		temp->dog->checkups = NULL;
		temp->next = NULL;
		//point the checkIterator at the original checkup list to be transferred.
		checkList = NULL;
		checkIterator = iterator->dog->checkups;
		//copy checkup list
		while (checkIterator != NULL) {
			//memory allocation for transfer temps
			tempCheck = (struct checkup *)malloc(sizeof(struct checkup));
			//copy from original list
			strcpy(tempCheck->date, checkIterator->date);
			tempCheck->next = NULL;
			//Put each checkup into the new list
			if (checkList == NULL) {
				//if the new list is empty
				checkList = tempCheck;
			}//if
			else {
				struct checkup *aCheck = checkList;
				while (aCheck->next != NULL) {
					aCheck = aCheck->next;
				}
				aCheck->next = tempCheck;
			}//else
			checkIterator = checkIterator->next;
			//iterate checkIterator
		}//while
		 //Put the new checkup list into the new dog structure
		temp->dog->checkups = checkList;
		checkList = NULL;
		//Put the new Container node into the alphabetical list
		struct container *follower = alphaList;
		if (alphaList == NULL) {
			//if list is empty make first node
			alphaList = temp;
			alphaList->next = NULL;
			
		}//if
		else {
			
			//Check where it goes in the list alphabetically
			if (stricmp(temp->dog->name, alphaList->dog->name) < 0) {
				//if it goes at the beginning of the list
				temp->next = alphaList;
				alphaList = temp;
				

			}//if
			else{
				//if it goes somewhere else on the list
				//Point alphaIterator at the beginning of the alphaList and the follower at the alphaIterator
				struct container *alphaIterator = alphaList->next;
				
				while (alphaIterator != NULL) {
					
					//Find out what one it goes before
					if (stricmp(temp->dog->name, alphaIterator->dog->name) < 0) {
						//Place it between two nodes
						temp->next = alphaIterator;
						follower->next = temp;
						
						break;//exit loop
					}
					else {
						
						//repoint follower and iterate alphaIterator
						follower = alphaIterator;
						alphaIterator = alphaIterator->next;
					}
				}//while
				follower->next = temp;
				
				
			}
		}//else
		iterator = iterator->next;//iterate original list
	}//while
	return alphaList;//return list in alphabetical order
}//list_by_name
