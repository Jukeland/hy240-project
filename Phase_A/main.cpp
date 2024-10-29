#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#include "streaming_service.h"

// using std::string, std::cout, std::endl, std::getline, std::ifstream;
using namespace std;

struct user* user_list;
struct movie* Horror_head;
struct movie* SciFi_head;
struct movie* Drama_head;
struct movie* Romance_head;
struct movie* Documentary_head;
struct movie* Comedy_head;
struct movie* category_table[6] = {};
struct new_movie* new_releases;

/*
 * this function adds a new node to user_list
 */
void pushUser(user** head, int uid){
	user* newNode = new user;
	newNode->next = *head;
	newNode->uid = uid;
    newNode->suggestedHead = NULL;
    newNode->suggestedTail = NULL;
    newNode->watchHistory = NULL;
	*head = newNode;
}

/*
 * this function prints the user_list
 */
void printUserList(user* head){
	while(head != NULL){
		cout << head->uid;
		if(head->next != NULL)
			cout << ", ";
		head = head->next;
	}
}

/*
 * this function prints the new_releases list
 */
void printNewReleases(new_movie* head){
	while(head != NULL){
		cout << "<";
		cout << head->info.mid << ", ";
		cout << head->category << ", ";
		cout << head->info.year << ">";
		if(head->next != NULL)
			cout << ", ";
		head = head->next;
	}
}

/*
 * this function prints the movies lists
 */
void printMovies(movie* head){
	while(head != NULL){
		cout << head->info.mid;
		if(head->next != NULL)
			cout << ", ";
		head = head->next;
	}
}

/*
 * this function creates a new node for new_movie list
 */
new_movie* pushNewMovie(unsigned mid, movieCategory_t category, unsigned year){
	new_movie* new_node = new new_movie;
	new_node->info.mid = mid;
	new_node->category = category;
	new_node->info.year = year;
	new_node->next = NULL;
	return new_node;
}

/*
 * this function creates a new node for movie list
 */
void pushMovie(movie** head, unsigned mid, unsigned year){
	movie* new_node = new movie;
	new_node->info.mid = mid;
	new_node->info.year = year;
	new_node->next = *head;
	*head = new_node;
}

/*
 * this function prints the suggested list 
 */
void printSuggestedList(suggested_movie* head){
	while(head != NULL){
		cout << head->info.mid;
		if(head->next != NULL)
			cout << ", ";
		head = head->next;
	}
}

void pushSuggested(suggested_movie** head, unsigned mid, unsigned year){

	suggested_movie* newNode = new suggested_movie;
	suggested_movie* last = *head;
	
	newNode->info.mid = mid;
	newNode->info.year = year;
	newNode->next = NULL;
	if(*head == NULL){
		newNode->prev = NULL;
		*head = newNode;
		return;
	}
	while(last->next != NULL){
		last = last->next;
	}
	last->next = newNode;
	newNode->prev = last;

}

/*
 * this function adds a new node after the suggested head 
 * of the current user
 */
void pushAfterHead(suggested_movie** head, suggested_movie* first, unsigned mid, unsigned year){
	
	if(first == NULL){

		suggested_movie* newNode = new suggested_movie;
		suggested_movie* last = *head;

		newNode->info.mid = mid;
		newNode->info.year = year;
		newNode->next = NULL;
		if(*head == NULL){
			newNode->prev = NULL;
			*head = newNode;
			return;
		}
		while(last->next != NULL){
			last = last->next;
		}
		last->next = newNode;
		newNode->prev = last;

	}else{

		suggested_movie* newNode = new suggested_movie;
		suggested_movie* afterHead = *head;

		while(afterHead->next != first)
			afterHead = afterHead->next;
		newNode->info.mid = mid;
		newNode->info.year = year;
		newNode->next = first;
		newNode->prev = afterHead;
		first->prev = newNode;
		afterHead->next = newNode;

	}
	
}

/* 
 * this function adds a new node before the suggested tail 
 * of the current user
 */
void pushBeforeTail(suggested_movie** tail, suggested_movie* last, unsigned mid, unsigned year){

	if(last == NULL){

		suggested_movie* newNode = new suggested_movie;
		suggested_movie* first = *tail;

		newNode->info.mid = mid;
		newNode->info.year = year;
		newNode->next = NULL;
		if(*tail == NULL){
			newNode->next = NULL;
			*tail = newNode;
			return;
		}
		
		while(first->prev != NULL){
			first = first->prev;
		}
		first->prev = newNode;
		newNode->next = first;

	}else{

		suggested_movie* newNode = new suggested_movie;
	 	suggested_movie* beforeTail = *tail;

	 	while(beforeTail->prev != last)
	 		beforeTail = beforeTail->prev;
	 	newNode->info.mid = mid;
	 	newNode->info.year = year;
	 	newNode->prev = last;
	 	newNode->next = beforeTail;
	 	last->next = newNode;
		beforeTail->prev = newNode;	

	}

}

/* 
 * this function puts all movies from new_releases 
 * to the corresponding category list 
 */
void makeCategoryList(movie** categoryHead, movieCategory_t category){

	new_movie* curr = new_releases;
	
	while(curr != NULL){
		if(curr->category == category)
			pushMovie(categoryHead, curr->info.mid, curr->info.year);
		curr = curr->next;
	}

}

/* 
 * this function takes an int category code as an aegument 
 * and returns the corresponding movieCategory_t
 */
movieCategory_t findCategory(int catCode){

	movieCategory_t cat;

	switch(catCode){
        case 0:
            cat = HORROR;
            break;
        case 1:
            cat = SCIFI;
            break;
        case 2:
            cat = DRAMA;
            break;
        case 3:
            cat = ROMANCE;
            break;
        case 4:
            cat = DOCUMENTARY;
            break;
        case 5:
            cat = COMEDY;
            break;
    }
	return cat;

}

/* 
 * this function reverses a singly linked list 
 */
void reverseMovieList(movie** head){

	movie* current = *head;
	movie* next = NULL;
	movie* prev = NULL;

	while(current != NULL){
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
	}
	*head = prev;

}

/* 
 * this function reverses a doubly linked list
 */
void reverseSuggestedMovieList(suggested_movie** head){

	suggested_movie* temp = NULL;
	suggested_movie* current = *head;

	while(current != NULL){
		temp = current->prev;
		current->prev = current->next;
		current->next = temp;
		current = current->prev;
	}
	if(temp != NULL)
		*head = temp->prev;

}

/*
 * this function deletes the 
 * first node of the stack
 */
void pop(user* thisUser){

	movie* temp;

	temp = thisUser->watchHistory;
	thisUser->watchHistory = thisUser->watchHistory->next;
	delete temp;

}

/* 
 * this function initializes the global structures
 */
void init_structures(){
    
	/* 
	 * push the sentinel node into user_list
	 */
	pushUser(&user_list, -1);

	/*
	 * initialize the category lists to NULL
	 * and put them into category_table
	 */
	Horror_head = NULL;
	SciFi_head = NULL;
	Drama_head = NULL;
	Romance_head = NULL;
	Documentary_head = NULL;
	Comedy_head = NULL;
	category_table[0] = Horror_head;
	category_table[1] = SciFi_head;
	category_table[2] = Drama_head;
	category_table[3] = Romance_head;
	category_table[4] = Documentary_head;
	category_table[5] = Comedy_head;

	/*
	 * initialize new_releases list to NULL
	 */
	new_releases = NULL;
}

/*
 * this function destroys all the structures
 * used in this streaming service and frees
 * all memory associated with them
 */
void destroy_structures(){
	/*
	 * TODO: For a bonus
	 * empty all lists and stacks
	 * and free all memory associated
	 * with list/stack nodes here
	 */

	user* currentUser = user_list;
	user* temp;
	new_movie* currentNewMovie = new_releases;
	new_movie* tempNM;
	movie* currentMovie;
	movie* tempM;

	/*
	 * delete every user from user_list
	 * along with the memory allocated
	 * for the user
	 */
	while(currentUser != NULL){
		temp = currentUser;
		currentUser = currentUser->next;
		delete temp;
	}

	/*
	 * delete every new_movie from new_releases list
	 * along with the memory allocated for it
	 */
	while(currentNewMovie != NULL){
		tempNM = currentNewMovie;
		currentNewMovie = currentNewMovie->next;
		delete temp;
	}

	/*
	 * for every category list, destroy every movie
	 * along with the memory allocated for it
	 */
	for(int i = 0; i < 6; i++){
		currentMovie = category_table[i];
		while(currentMovie != NULL){
			tempM = currentMovie;
			currentMovie = currentMovie->next;
			delete tempM;
		}
	}

	cout << " ===== Structures Destroyed successfully =====\n";

}

/* 
 * this function adds a user to the user_list 
 * -R <uid> 
 */
int register_user(int uid){

	user* current = user_list;
	bool alreadyUser = false;

	/*
	 * find if the user already exists
	 */ 
	while(current != NULL){	
		if(current->uid == uid){
			alreadyUser = true;
			break;
		}
		current = current->next;
	}

	/* 
	 * if user does not already exist 
	 * push into user_list else 
	 * print error message and return -1 
	 */
	if(!alreadyUser){	
		pushUser(&user_list, uid);
	}else{
		cout << "R " << uid;
		cout << "\nUser already exists\n";
		return -1;
	}

	cout << "R " << uid;
	cout << "\nUsers = ";
	printUserList(user_list);
	cout << "\nDONE\n";
	return 0;
}

/* 
 * this functions removes a user from the user_list
 * -U <uid> 
 */
void unregister_user(int uid){
	
	user* ref = user_list;
	suggested_movie* current_sm;
	suggested_movie* temp;
	movie* current_wh;
	movie* temporary;
	user* current = user_list->next;
	user* before_del = user_list;

	/*
	 * find the user's info
	 */
	while(ref != NULL){
		if(ref->uid == uid){
			current_sm = ref->suggestedHead;
			current_wh = ref->watchHistory;
			break;
		}
		ref = ref->next;
	}

	/*
	 * delete the suggested movie list
	 */
	while(current_sm != NULL){
		temp = current_sm;
		current_sm = current_sm->next;
		delete temp;
	}

	/*
	 * delete the watch history stack
	 */
	while(current_wh != NULL){
		temporary = current_wh;
		current_wh = current_wh->next;
		delete temporary;
	}

	/*
	 * delete the user from the user list
	 */
	while(current != NULL){
		if(before_del->uid == uid){
			user_list = current;
			delete before_del;
			break;
		}
		if(current->uid == uid){
			before_del->next = current->next;
			delete current;
			break;
		}
		current = current->next;
		before_del = before_del->next;
	}

	cout << "U " << uid;
	cout << "\nUsers = ";
	printUserList(user_list);
	cout << "\nDONE\n" ;
}

/* 
 * this function adds a new node to the new_releases list 
 * -A <mid> <category> <year> 
 */
int add_new_movie(unsigned mid, movieCategory_t category, unsigned year){

	new_movie* newnode = pushNewMovie(mid, category, year);

	/* if the new movie has to go in the front of new_releases 
	 * then simply link the newnode to the current head 
	 * and appoint newnode as the head of new_releases 
	 * else insert the new node in a sorted way to new_releases 
	 */
	if(new_releases == NULL || new_releases->info.mid >= newnode->info.mid){
		newnode->next = new_releases;
		new_releases = newnode;
	}else{
		new_movie* current = new_releases;
		while(current->next != NULL && current->next->info.mid < newnode->info.mid){
			current = current->next;
		}
		newnode->next = current->next;
		current->next = newnode;
	}

	cout << "A " << mid << " " << category << " " << year;
	cout << "\nNew movies = ";
	printNewReleases(new_releases);
	cout << "\nDONE\n";
	return 0;
}

/* 
 * this function distributes the movies from new_releases 
 * to the category table 
 * -D 
 */
void distribute_new_movies(void){
	
	new_movie* temp;

	/*
	 * put all the movies from new_releases to
	 * the corresponding category list
	 * this sorts the lists into descending order
	 */
	makeCategoryList(&Horror_head, HORROR);
	makeCategoryList(&SciFi_head, SCIFI);
	makeCategoryList(&Drama_head, DRAMA);
	makeCategoryList(&Romance_head, ROMANCE);
	makeCategoryList(&Documentary_head, DOCUMENTARY);
	makeCategoryList(&Comedy_head, COMEDY);

	/*
	 * delete the new_releases list
	 */
	while(new_releases != NULL){
		temp = new_releases;
		new_releases = new_releases->next;
		delete(temp);
	}
	
	/*
	 * put the new category lists into the category table
	 */
	category_table[0] = Horror_head;
	category_table[1] = SciFi_head;
	category_table[2] = Drama_head;
	category_table[3] = Romance_head;
	category_table[4] = Documentary_head;
	category_table[5] = Comedy_head;

	/*
	 * reverse the category lists to sort them in 
	 * ascending order as they should be
	 */
	reverseMovieList(&Horror_head);
	reverseMovieList(&SciFi_head);
	reverseMovieList(&Drama_head);
	reverseMovieList(&Romance_head);
	reverseMovieList(&Documentary_head);
	reverseMovieList(&Comedy_head);

	/*
	 * put the new category lists into the category table
	 */
	category_table[0] = Horror_head;
	category_table[1] = SciFi_head;
	category_table[2] = Drama_head;
	category_table[3] = Romance_head;
	category_table[4] = Documentary_head;
	category_table[5] = Comedy_head;

	cout << "D" << endl;
	cout << "Categorized Movies:\n";
	cout << "\"0\" Horror: ";
	printMovies(Horror_head);
	cout << "\n\"1\" Sci-Fi: ";
	printMovies(SciFi_head);
	cout << "\n\"2\" Drama: ";
	printMovies(Drama_head);
	cout << "\n\"3\" Romance: ";
	printMovies(Romance_head);
	cout << "\n\"4\" Documentary: ";
	printMovies(Documentary_head);
	cout << "\n\"5\" Comedy: ";
	printMovies(Comedy_head);
	cout << "\nDONE\n";
}

/*
 * this function adds the movie with movieID = mid 
 * to the watch history stack of the user with userID = uid 
 * -W <uid> <mid>
 */
int watch_movie(int uid, unsigned mid){

	movie* currentMovie;
	user* currentUser = user_list;
	bool movieFound = false, userFound = false;

	/*
	 * find the user
	 */
	while(currentUser != NULL){
		if(currentUser->uid == uid){
			userFound = true;
			break;
		}
		currentUser = currentUser->next;
	}

	/*
	 * if user is not found print error message
	 * and return -1
	 */
	if(!userFound){
		cout << "User not found\n";
		return -1;
	}

	/*
	 * find the movie by searching in
	 * all of the category lists
	 * if the movie is found break
	 * out of the loops
	 */
	for(int i = 0; i < 6; i++){
		currentMovie = category_table[i];
		while(currentMovie != NULL){
			if(currentMovie->info.mid == mid){
				movieFound = true;
				pushMovie(&currentUser->watchHistory, currentMovie->info.mid, currentMovie->info.year);
				break;
			}
			currentMovie = currentMovie->next;
		}
		if(movieFound)
			break;
	}

	/*
	 * if the movie is not found 
	 * print error message
	 * and return -1
	 */
	if(!movieFound){
		cout << "Movie not found\n";
		return -1;
	}

	cout << "W " << uid << " " << mid;
	cout << "\nUser " << uid << " Watch History = ";
	printMovies(currentUser->watchHistory);
	cout << "\nDONE\n";

	return 0;

}

/*
 * this function adds suggested movies to the user's 
 * suggested movie list with userID = uid from the 
 * watch history stack of the other users 
 * -S <uid>
 */
int suggest_movies(int uid){

	user* thisUser = user_list;
	movie* toAdd;
	user* currentUser = user_list;
	unsigned midToAdd, yearToAdd;
	bool userFound = false, isEmpty = true;
	int counter = 1;

	/*
	 * find the user
	 */
	while(thisUser != NULL){
		if(thisUser->uid == uid){
			userFound = true;
			break;
		}
		thisUser = thisUser->next;
	}
	
	/*
	 * if user is not found
	 * print error message
	 * and return -1
	 */
	if(!userFound){
		cout << "User not found\n";
		return -1;
	}

	suggested_movie* middleFirst = NULL;
	suggested_movie* middleLast = NULL;
	
	if(thisUser->suggestedHead != NULL && thisUser->suggestedTail != NULL){
		isEmpty = false;
		middleFirst = thisUser->suggestedHead->next;
		middleLast = thisUser->suggestedTail->prev;
		
	}

	/*
	* traverse through the users list
	* take the first node off the currentUser watchHistory stack
	* and push it in a zigzag way to the
	* suggested movies list of thisUser
	*/
	while(currentUser != NULL){
		if(currentUser->uid != uid && currentUser->uid != -1 && currentUser->watchHistory != NULL){
			toAdd = currentUser->watchHistory;
			midToAdd = toAdd->info.mid;
			yearToAdd = toAdd->info.year;
			pop(currentUser);
			if(counter % 2 != 0){
				pushAfterHead(&thisUser->suggestedHead, middleFirst, midToAdd, yearToAdd);
			}else{
				pushBeforeTail(&thisUser->suggestedTail, middleLast, midToAdd, yearToAdd);
			}
			counter++;
		}
		currentUser = currentUser->next;
	}


	if(isEmpty){

		suggested_movie* last = thisUser->suggestedHead;
		suggested_movie* first = thisUser->suggestedTail;

		/*
		* link the head list and the tail list
		* into one doubly linked list if necessary
		*/
		if(thisUser->suggestedHead != NULL){
			while(last->next != NULL){
				last = last->next;
			}
		}
		if(thisUser->suggestedTail != NULL){
			while(first->prev != NULL){
				first = first->prev;
			}
		}
		if(first != NULL)
			last->next = first;
		if(last != NULL)
			first->prev = last;

	}

	cout << "S " << uid;
	cout << "\nUser " << uid << " Suggested Movies = ";
	printSuggestedList(thisUser->suggestedHead);
	cout << "\nDONE\n";

	return 0;

}

/*
 * this function finds all movies that
 * are in category1 or category2 and has
 * a release year greater than the given year
 * and puts them into the suggested movies
 * list of the user with userID = uid
 * -F <uid> <category1> <category2> <year>
 */
int filtered_movie_search(int uid, movieCategory_t category1, movieCategory_t category2, unsigned year){
	user* thisUser = user_list;
	suggested_movie* newList = NULL;
	suggested_movie* listToAppend = NULL;
	movie* currentMovie = category_table[category1];
	suggested_movie* currMovie1;
	suggested_movie* currMovie2;
	suggested_movie* first;
	suggested_movie* last;
	bool isGood = false;
	int counter = 0;
	
	/*
	 * find the user
	 */
	while(thisUser != NULL){
		if(thisUser->uid == uid)
			break;
		thisUser = thisUser->next;
	}

	/*
	 * if user does not exist print
	 * error message and return -1
	 */
	if(thisUser == NULL){
		cout << "User not found\n";
		return -1;
	}
		
	/*
	 * find all the movies of category1 that 
	 * have year >= given year and push them
	 * into the newList doubly linked list
	 * at this moment the list is sorted in ascending order
	 */
	while(currentMovie != NULL){	// O(n)
		if(currentMovie->info.year >= year){
			pushSuggested(&newList, currentMovie->info.mid, currentMovie->info.year);
			//pushAfterHead(&newList, currentMovie->info.mid, currentMovie->info.year);
		}
		currentMovie = currentMovie->next;
	}
	 
	/*
	 * find all the movies of category2 that 
	 * have year >= given year and push them
	 * into the newList doubly linked list
	 * at this moment the list can be split into
	 * two sub lists that either of them are
	 * sorted in ascending order
	 */
	currentMovie = category_table[category2];
	while(currentMovie != NULL){	// O(m)
		if(currentMovie->info.year >= year){
			pushSuggested(&newList, currentMovie->info.mid, currentMovie->info.year);
			//pushAfterHead(&newList, currentMovie->info.mid, currentMovie->info.year);
		}
		currentMovie = currentMovie->next;
	}

	
	/* tests */
	// cout << "New List: ";
	// printSuggestedList(newList);
	// cout << endl;


	/*
	 * if newList is only 2 nodes long and
	 * it is sorted in descending order
	 * then reverse it and say that the list is good to go
	 */
	currMovie1 = newList;
	if(currMovie1->info.mid > currMovie1->next->info.mid){
		reverseSuggestedMovieList(&newList);
		currMovie1 = newList;
		currMovie2 = currMovie1->next;
		isGood = true;
	}

	
	/* tests */
	// printSuggestedList(newList);
	// cout << "\ncurrMovie1: " << currMovie1->info.mid << endl;
	// cout << "currMovie2: " << currMovie2->info.mid << endl;

 
	/*
	 * traverse through the newList to find 
	 * the start of the second sublist (currMovie2)
	 */
	currMovie1 = newList;
	while(currMovie1->info.mid < currMovie1->next->info.mid && !isGood){	// O(n + m)
		currMovie2 = currMovie1->next;
		currMovie1 = currMovie1->next;
		if(currMovie1->next == NULL)
			break;
	}
	if(currMovie2->next != NULL)
		currMovie2 = currMovie2->next;
	
	
	currMovie1 = newList;
	first = currMovie2;


	/* tests */
	// cout << "currMovie1 = " << currMovie1->info.mid << endl;
	// cout << "currMovie2 = " << currMovie2->info.mid << endl;
	// cout << "First = " << first->info.mid << endl;
	// cout << "Last = " << last->info.mid << endl;


	/*
	 * if first node of second sublist has mid > than its previous node
	 * then the list is good
	 */
	if(currMovie2 != NULL && currMovie2->prev->info.mid < currMovie2->info.mid){
		isGood = true;
		listToAppend = newList;
	}
 
	/*
	 * traverse through the newList until currMovie1 reaches the
	 * first node of the second sublist or currMovie2 reaches the
	 * end of newList, and push to listToAppend all the nodes 
	 * in a sorted way
	 */
	while(currMovie1 != first && currMovie2 != NULL && !isGood){	// O(n + m)
		counter++;
		if(currMovie1->info.mid < currMovie2->info.mid){
			pushSuggested(&listToAppend, currMovie1->info.mid, currMovie1->info.year);
			//pushAfterHead(&listToAppend, currMovie1->info.mid, currMovie1->info.year);
			currMovie1 = currMovie1->next;
		}else{
			pushSuggested(&listToAppend, currMovie2->info.mid, currMovie2->info.year);
			//pushAfterHead(&listToAppend, currMovie2->info.mid, currMovie2->info.year);
			currMovie2 = currMovie2->next;
		}
	}
 
	/*
	 * if currMovie1 ran out first push all the remaining nodes
	 * of the second sublist to listToAppend, else if
	 * currMovie2 ran out first push all the remaining nodes
	 * of the first sublist to listToAppend
	 * at this time the listToApped is sorted in ascending order
	 */
	if(currMovie1 == first && !isGood && counter != 0){
		while(currMovie2 != NULL){ // O(m)
			pushSuggested(&listToAppend, currMovie2->info.mid, currMovie2->info.year);
			//pushAfterHead(&listToAppend, currMovie2->info.mid, currMovie2->info.year);
			currMovie2 = currMovie2->next;
		}
	}else if(currMovie2 == NULL && !isGood && counter != 0){
		while(currMovie1 != first){ // O(n)
			pushSuggested(&listToAppend, currMovie1->info.mid, currMovie1->info.year);
			//pushAfterHead(&listToAppend, currMovie1->info.mid, currMovie1->info.year);
			currMovie1 = currMovie1->next;
		}
	}

	/*
	 * find the last node of the listToAppend
	 * we use that for the user's suggested tail
	 */
	last = listToAppend;
	while(last->next != NULL){ // O(n + m)
		last = last->next;
	}

	/*
	 * finally append the sorted listToAppend
	 * into the user's suggested
	 */
	thisUser->suggestedHead = listToAppend;
	thisUser->suggestedHead->prev = NULL;
	thisUser->suggestedTail = last;
	thisUser->suggestedTail->next = NULL;

	cout << "F " << uid << " " << category1 << " " << category2 << " " << year << endl;
	cout << "User " << uid << " Suggested Movies = ";
	printSuggestedList(thisUser->suggestedHead);
	cout << "\nDONE" << endl;

	return 0;

}

/*
 * this function takes off the movie with movieID = mid 
 * from the streaming service 
 * (deletes the movie from every structure) 
 * -T <mid> 
 */
void take_off_movie(unsigned mid){
	user* currentUser = user_list;
	suggested_movie* currentMovie;
	movie* currM;
	movie* previous;
	bool isFound = false;

	/*
	 * 
	 * 
	 */

	category_table[0] = Horror_head;
	category_table[1] = SciFi_head;
	category_table[2] = Drama_head;
	category_table[3] = Romance_head;
	category_table[4] = Documentary_head;
	category_table[5] = Comedy_head;

	/*
	 * 
	 * 
	 */
	cout << "T " << mid << "\n";
	while(currentUser != NULL){
		currentMovie = currentUser->suggestedHead;	
		while(currentMovie != NULL){
			if(currentMovie->info.mid == mid){
				if(currentMovie->prev == NULL){	//if the node to be deleted is the head
					currentMovie->next->prev = NULL;
					currentUser->suggestedHead = currentMovie->next;
				}else if(currentMovie->next == NULL){ //if the node to be deleted is the tail
					currentMovie->prev->next = NULL;
					currentUser->suggestedTail = currentMovie->prev;
				}else{
					currentMovie->next->prev = currentMovie->prev;
					currentMovie->prev->next = currentMovie->next;
				}
				delete currentMovie;
				cout << "\n ===== Movie " << mid << " removed from user " << currentUser->uid << " suggested list =====\n" << endl;
				cout << "User " << currentUser->uid << " suggested list: ";
				printSuggestedList(currentUser->suggestedHead);
				cout << "\n";
				break;
			}
			currentMovie = currentMovie->next;
		}
		currentUser = currentUser->next;
	}

	/*
	 * for every category list find the movie for removal
	 * link the previous and next nodes, remove the node
	 * and print the outcome
	 */
	for(int i = 0; i < 6; i++){

		/*
	 	 * if this category list is empty
	 	 * go to next category list
	 	 */
		if(category_table[i] == NULL)
			continue;

		/*
	 	 * if the node we are searching for is the head
	 	 * of the category list then 
	 	 */
		previous = category_table[i];
		if(previous->info.mid == mid){
			isFound = true;
			/*
	 		 * if the node is not the only one in the list
	 		 * move the head to the next node
			 * and delete the current node
	 		 */
			if(previous->next != NULL)
				category_table[i] = category_table[i]->next;
			delete previous;

			cout << "\n ===== Movie " << mid << " removed from \"" << findCategory(i) << "\" category list =====\n" << endl;
			cout << "\"" << i << "\"" << " Category List = ";
			printMovies(category_table[i]);
			cout << "\n";
			break;
		}

		/*
	 	 * if the node we are searching for is not the head
		 * of the category list then traverse through
	 	 * the category list until we find the node
	 	 */
		currM = previous->next;
		while(currM != NULL && !isFound){
			/*
			 * if we find the node we are searching for
			 */
			if(currM->info.mid == mid){
				isFound = true;
				/*
	 			 * if the node is the last node of the list
	 			 * link previous node to NULL
				 * else link previous node to the next node
				 * and delete the current node
	 			 */
				if(currM->next == NULL)
					previous->next = NULL;
				else
					previous->next = currM->next;
				delete currM; 

				cout << "\n ===== Movie " << mid << " removed from \"" << findCategory(i) << "\" category list =====\n" << endl;
				cout << "\"" << i << "\"" << " Category List = ";
				printMovies(category_table[i]);
				cout << "\n";
				break;
			}
			currM = currM->next;
			previous = previous->next;
		}

		/*
	 	 * if the movie is found then
	 	 * break out of the loop
		 * (there's no duplicate movies)
	 	 */
		if(isFound)
			break;

	}

	/*
	 * if the movie is not found
	 * print error message
	 */
	if(!isFound)
		cout << "Movie not found\n";

	/*
	 * put the new category lists
	 * into the category table
	 */
	Horror_head = category_table[0];
	SciFi_head = category_table[1];
	Drama_head = category_table[2];
	Romance_head = category_table[3];
	Documentary_head = category_table[4];
	Comedy_head = category_table[5];	
	
	cout << "\nDONE\n";

}

/*
 * this function prints the movies of all category lists 
 * -M
 */
void print_movies(void){

	cout << "M\n";
	cout << "Categorized Movies:\n";
	cout << "\"0\" Horror: ";
	printMovies(Horror_head);
	cout << "\n\"1\" Sci-Fi: ";
	printMovies(SciFi_head);
	cout << "\n\"2\" Drama: ";
	printMovies(Drama_head);
	cout << "\n\"3\" Romance: ";
	printMovies(Romance_head);
	cout << "\n\"4\" Documentary: ";
	printMovies(Documentary_head);
	cout << "\n\"5\" Comedy: ";
	printMovies(Comedy_head);
	cout << "\nDONE\n";

}

/*
 * this function prints all the users 
 * along with their info 
 * -P
 */
void print_users(void){

	user* currentUser = user_list;

	cout << "P\n";
	cout << "Users:\n" ;
	while(currentUser != NULL){
		cout << currentUser->uid << ":";
		cout << "\nSuggested: ";
		printSuggestedList(currentUser->suggestedHead); 
		cout << "\nWatch History: ";
		printMovies(currentUser->watchHistory); 
		cout << "\n";
		currentUser = currentUser->next;
	}
	cout << "\nDONE\n";

}

int main(int argc, char* argv[]){

	string line, key = " ", opCode;
    unsigned mid, cat, year;
	int uid;
    movieCategory_t category1, category2;
    size_t pos = 0;

	if(argc != 2){
		cout << "Usage: " << argv[0] << " <input_file>\n";
		exit(EXIT_FAILURE);
	}

	ifstream inFile(argv[1]);
	if(!inFile){
		perror("ifstream error for opCode file open");
		exit(EXIT_FAILURE);
	}

	init_structures();

    while(!inFile.eof()){

        getline(inFile, line);
        pos = line.find(key);
        opCode = line.substr(0, pos);
        line.erase(0, pos + 1);

        if(opCode == "R"){

			uid = stoi(line);
            register_user(uid);

        }else if(opCode == "U"){

			uid = stoi(line);
            unregister_user(uid);

        }else if(opCode == "A"){

            pos = line.find(key);
            mid = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);
            pos = line.find(key);
            cat = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);
            year = stoi(line);
            category1 = findCategory(cat);
            add_new_movie(mid, category1, year);

        }else if(opCode == "D"){

			distribute_new_movies();

		}else if(opCode == "W"){

			pos = line.find(key);
			uid = stoi(line.substr(0, pos));
			line.erase(0, pos + 1);
			mid = stoi(line);
			watch_movie(uid, mid);

		}else if(opCode == "S"){

			uid = stoi(line);
			suggest_movies(uid);

		}else if(opCode == "F"){

			pos = line.find(key);
			uid = stoi(line.substr(0, pos));
			line.erase(0, pos + 1);
			pos = line.find(key);
			cat = stoi(line.substr(0, pos));
			category1 = findCategory(cat);
			line.erase(0, pos + 1);
			pos = line.find(key);
			cat = stoi(line.substr(0, pos));
			category2 = findCategory(cat);
			line.erase(0, pos + 1);
			year = stoi(line);
			filtered_movie_search(uid, category1, category2, year);

		}else if(opCode == "T"){

			take_off_movie(stoi(line));

		}else if(opCode == "M"){

			print_movies();

		}else if(opCode == "P"){

			print_users();

		}else{

			cout << "WARNING: Unrecognized opCode. Continuing...\n";

		}

		cout << "\n===========================================\n" << endl;

    }
	inFile.close();
	destroy_structures();
    return 0;

}