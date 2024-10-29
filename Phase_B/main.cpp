#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>

#include "Movie.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ifstream;

new_movie_t *new_releases;
bool is_first_time = true;
bool isFirstTime = true;
int previous_user = -1;
int num_of_movies = 0;
movieCategory_t *categoryArray[6];
// int debug_array[50];
// bool debug_isFound = false;

int hashtable_size = 5; 	/** The size of the users hashtable (>0) */
int max_users;         /** The maximum number of registrations (users) */
int max_id;            /** The maximum user ID */

const int primes_g[170] = 		{  5,   7,  11,  13,  17,  19,  23,  29,  31,  37,
                               41,  43,  47,  53,  59,  61,  67,  71,  73,  79,
                               83,  89,  97, 101, 103, 107, 109, 113, 127, 131,
                              137, 139, 149, 151, 157, 163, 167, 173, 179, 181,
                              191, 193, 197, 199, 211, 223, 227, 229, 233, 239,
                              241, 251, 257, 263, 269, 271, 277, 281, 283, 293,
                              307, 311, 313, 317, 331, 337, 347, 349, 353, 359,
                              367, 373, 379, 383, 389, 397, 401, 409, 419, 421,
                              431, 433, 439, 443, 449, 457, 461, 463, 467, 479,
                              487, 491, 499, 503, 509, 521, 523, 541, 547, 557,
                              563, 569, 571, 577, 587, 593, 599, 601, 607, 613,
                              617, 619, 631, 641, 643, 647, 653, 659, 661, 673,
                              677, 683, 691, 701, 709, 719, 727, 733, 739, 743,
                              751, 757, 761, 769, 773, 787, 797, 809, 811, 821,
                              823, 827, 829, 839, 853, 857, 859, 863, 877, 881,
                              883, 887, 907, 911, 919, 929, 937, 941, 947, 953,
                          	  967, 971, 977, 983, 991, 997, 1009, 1013, 1019, 1021 };

class hashtable{
	public:
		int number_of_elements;
		int capacity;
		user_t** arr;

		hashtable(){
			this->capacity = 1;
			this->number_of_elements = 0;
			this->arr = new user_t*[this->capacity];
			this->arr[0] = NULL;
		}

		int hash_function(int key){
            return key % this->capacity;
        }

		float get_load_factor(){ 
	        return (float)(this->number_of_elements + 1) / (float)(this->capacity);
        }

		void rehashing(){
            int old_capacity = this->capacity;
			user_t** temp = this->arr;

            cout << "\n####################";
            cout << "\n# Rehashing...     #\n";
            cout << "#                  #";
            cout << "\n# Old Capacity: " << old_capacity << " #\n";

			for(int i = 0; i < 170; i++){
				if(primes_g[i] > old_capacity * 1.3){
					this->capacity = primes_g[i];
					break;
				}
			}

            cout << "# New Capacity: " << this->capacity << " #";
            cout << "\n####################\n\n";

			this->arr = new user_t*[this->capacity];

			for(int i = 0; i < this->capacity; i++){
				this->arr[i] = NULL;
			}

			for(int i = 0; i < old_capacity; i++){
				user_t* current_head = temp[i];
				while(current_head != NULL){
					this->insert(current_head->userID, current_head->history);
					current_head = current_head->next;
				}
			}

			delete[] temp;
        }

		void insert(int key, userMovie_t* history){
            if(this->get_load_factor() >= 10.0f){
				this->rehashing();
			}

			int index = this->hash_function(key);

			if(this->arr[index] == NULL){

				user_t* newNode = new user_t;
				newNode->userID = key;
				newNode->history = history;
				newNode->next = NULL;
				arr[index] = newNode;

			}else{

				user_t* newNode = new user_t;
				newNode->userID = key;
				newNode->history = history;
				newNode->next = this->arr[index];
				this->arr[index] = newNode;

			}

			this->number_of_elements = this->number_of_elements + 1;
        }

		user_t* search(int key){
            int index = this->hash_function(key);
			user_t* head = this->arr[index];

			while(head != NULL){
				if(head->userID == key){
					return head;
				}
				head = head->next;
			}

			cout << "Key not found." << endl;
			return NULL;
        }
};

hashtable* user_hashtable_p = new hashtable();

void push_user(user_t** head, int uid){
    user_t* new_user = new user_t;
    new_user->userID = uid;
    new_user->history = NULL;
    new_user->next = *head;
    *head = new_user;
}

void print_users(user_t* head){
    user_t* current = head;
    while(current != NULL){
        cout << "   User: " << current->userID << "\n";
        current = current->next;
    }
}

/*
 * a utility function used to create and
 * initialize a new node in new_releases tree
 */
new_movie_t* create_new_movie(int mid, int cat, int year){
    new_movie_t* newNode = new new_movie_t;
    newNode->movieID = mid;
    newNode->category = cat;
    newNode->year = year;
    newNode->watchedCounter = 0;
    newNode->sumScore = 0;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

/*
 * a utility function used to traverse and 
 * print the new_releases tree in ascending order
 * (inorder traversal of binary search tree)
 */
void print_new_movies(new_movie_t* root){
    if(root != NULL){
        print_new_movies(root->left);
        cout << root->movieID << " ";
        print_new_movies(root->right);
    }
}

void delete_new_movies(new_movie_t* root){
    if(root == NULL)
        return;

    delete_new_movies(root->left);
    delete_new_movies(root->right);

    delete root;
    
}

/*
 * a function used to insert a new node in
 * the correct position in new_releases
 * tree (binary search tree)
 */
new_movie_t* insert_new_movie(new_movie_t* root, int mid, int cat, int year){
    
    if(root == NULL)
        return create_new_movie(mid, cat, year);

    if(mid <= root->movieID){
        new_movie_t* lc = insert_new_movie(root->left, mid, cat, year);
        root->left = lc;
    }else if(mid > root->movieID){
        new_movie_t* rc = insert_new_movie(root->right, mid, cat, year);
        root->right = rc;
    }

    return root;
} 

/*
 * a utility function used to traverse and 
 * print one category tree in ascending order
 * (inorder traversal of binary search tree)
 */
void print_movies(movie_t* root){
    if(root != NULL){
        print_movies(root->left);
        cout << root->movieID << " ";
        print_movies(root->right);
    }
}

/*
 * a function used to print print all
 * the category tree is ascending order
 * (inorder traversal of binary search tree)
 */
void print_category_array(){
    for(int i = 0; i < 6; i++){
        cout << "\nCategory " << i << ": ";
        print_movies(categoryArray[i]->movie);
    }
}

list_node* cat_0;
list_node* cat_1;
list_node* cat_2;
list_node* cat_3;
list_node* cat_4;
list_node* cat_5;

list_node* cat_table[6];
bool flag = true;

void init_table(){
    flag = false;
    cat_table[0] = cat_0;
    cat_table[1] = cat_1;
    cat_table[2] = cat_2;
    cat_table[3] = cat_3;
    cat_table[4] = cat_4;
    cat_table[5] = cat_5;
}

void push_to_list(list_node** head, int mid, int year, int watchedCounter, int sumScore){
    list_node* newNode = new list_node;
    newNode->next = *head;
    newNode->movieID = mid;
    newNode->year = year;
    newNode->watchedCounter = watchedCounter;
    newNode->sumScore = sumScore;
    *head = newNode;
}

void traverse_new_movies(new_movie_t* root){
    if(flag)
        init_table();
    if(root != NULL){
        traverse_new_movies(root->left);
        for(int i = 0; i < 6; i++){
            if(root->category == i){
                push_to_list(&cat_table[i], root->movieID, root->year, root->watchedCounter, root->sumScore);
            }
        }
        traverse_new_movies(root->right);
    }
}

void print_list(list_node* head){
    while(head != NULL){
        cout << head->movieID;
        if(head->next != NULL)
            cout << ", ";
        head = head->next;
    }
}

void reverse_list(list_node** head){

    list_node* current = *head;
    list_node* next = NULL;
    list_node* prev = NULL;

    while(current != NULL){
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head = prev;

}

int count_nodes(list_node* head){
    list_node* current = head;
    int counter = 0;
    while(current != NULL){
        counter++;
        current = current->next;
    }
    return counter;
}

/*
 * a utility function used to create and
 * initialize a new node in new_releases tree
 */
movie_t* create_movie(int mid, int year){
    movie_t* newNode = new movie_t;
    newNode->movieID = mid;
    newNode->year = year;
    newNode->watchedCounter = 0;
    newNode->sumScore = 0;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

movie_t* insert_movie(movie_t* root, int mid, int year){
    
    if(root == NULL)
        return create_movie(mid, year);

    if(mid <= root->movieID){
        movie_t* lc = insert_movie(root->left, mid, year);
        root->left = lc;
    }else if(mid > root->movieID){
        movie_t* rc = insert_movie(root->right, mid, year);
        root->right = rc;
    }

    return root;
} 

movie_t* list_to_bst_recur(list_node** head, int n){

    if(n <= 0)
        return nullptr;

    movie_t* left = list_to_bst_recur(head, n/2);

    movie_t* root = create_movie( (*head)->movieID, (*head)->year );
    root->left = left;

    *head = (*head)->next;

    root->right = list_to_bst_recur(head, n - n/2 - 1);

    return root;

}

movie_t* make_bst_from_list(list_node* head){

    int n = count_nodes(head);

    return list_to_bst_recur(&head, n);

}

void delete_movies(movie_t* root){
    if(root == NULL)
        return;

    delete_movies(root->left);
    delete_movies(root->right);

    delete root;
    
}

void print_cat_trees(movie_t* root){
    if(root != NULL){
        print_cat_trees(root->left);
        cout << root->movieID << " ";
        print_cat_trees(root->right);
    }
}

movie_t* search_bst(movie_t* root, int key){

    if(root == NULL || root->movieID == key)
        return root;

    if(key <= root->movieID)
        return search_bst(root->left, key);
    else    
        return search_bst(root->right, key);

}

void count_nodes_bst(movie_t* root, int score){
    if(root == NULL)
        return;

    count_nodes_bst(root->left, score);
    // cout << "\n";
    // cout << "movieID: " << root->movieID << endl;
    // cout << "sumScore: " << root->sumScore << endl;
    // cout << "watchedCounter: " << root->watchedCounter << endl;
    // cout << "median: " << (float)root->sumScore / (float)root->watchedCounter << endl;
    // cout << "score: " << score << endl;
    // cout << "\n";
    if(root->watchedCounter != 0){
        float median = (float)root->sumScore / (float)root->watchedCounter;
        if(median > (float)score)
            num_of_movies++;
    }
    
    count_nodes_bst(root->right, score);
}

void make_array_from_bst(movie_t* root, int score, movie_t* arr[]){
    if(root == NULL)
        return;

    make_array_from_bst(root->left, score, arr);

    if(root->watchedCounter != 0){
        float median = (float)root->sumScore / (float)root->watchedCounter;
        if(median > (float)score){
            arr[num_of_movies] = root;
            num_of_movies++;
        }
    }
        
    make_array_from_bst(root->right, score, arr);
}

void delete_wh_tree(userMovie_t* root){
    if(root == NULL)
        return;
    
    delete_wh_tree(root->left);
    delete_wh_tree(root->right);

    delete root;
}

bool is_leaf(userMovie_t* node){
    if(node->left == NULL && node->right == NULL)
        return true;
    else
        return false;
}

userMovie_t* create_userMovie(int mid, int cat, int score){

    userMovie_t* newNode = new userMovie_t;
    newNode->movieID = mid;
    newNode->category = cat;
    newNode->score = score;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;

    return newNode;

}

userMovie_t* insert_leaf(userMovie_t* root, int mid, int cat, int score){

    if(root == NULL)
        return create_userMovie(mid, cat, score);

    userMovie_t* current = root;
    userMovie_t* leaf;
    userMovie_t* parent;

    while(!is_leaf(current)){
        if(mid <= current->movieID)
            current = current->left;
        else
            current = current->right;
    }    
    leaf = current;
    parent = current->parent;

    userMovie_t* internalNode = new userMovie_t;
    userMovie_t* newLeaf = create_userMovie(mid, cat, score);
    userMovie_t* replacementLeaf = create_userMovie(leaf->movieID, leaf->category, leaf->score);
    
    internalNode->parent = parent;
    if(mid <= leaf->movieID){
        internalNode->movieID = mid;
        internalNode->category = cat;
        internalNode->score = score;
        internalNode->left = newLeaf;
        internalNode->right = replacementLeaf;
    }else{
        internalNode->movieID = leaf->movieID;
        internalNode->category = leaf->category;
        internalNode->score = leaf->score;
        internalNode->left = replacementLeaf;
        internalNode->right = newLeaf;
    }

    newLeaf->parent = internalNode;
    replacementLeaf->parent = internalNode;

    if(parent != NULL){
        if(parent->left == leaf)
            parent->left = internalNode;
        else if(parent->right == leaf)
            parent->right = internalNode;
    }
    
    return internalNode;

}

void print_history_tree(userMovie_t* root, char opCode){
    if(root == NULL)
        return;

    if(opCode == 'W'){

        print_history_tree(root->left, opCode);
        if(is_leaf(root))
            cout << "   Movie: " << root->movieID << ", Score: " << root->score << "\n";
        print_history_tree(root->right, opCode);

    }else if(opCode == 'P'){

        print_history_tree(root->left, opCode);
        if(is_leaf(root))
            cout << "         Movie: " << root->movieID << ", Score: " << root->score << "\n";
        print_history_tree(root->right, opCode);

    }
}

void traverse_history_tree(userMovie_t* root, int sum, int counter){
    if(root == NULL)
        return;

    traverse_history_tree(root->left, sum, counter);

    if(is_leaf(root)){
        sum += root->score;
        counter++;
    }

    traverse_history_tree(root->right, sum, counter);
}

userMovie_t* find_next_leaf(userMovie_t* leaf){

    userMovie_t* parent = leaf->parent;
    userMovie_t* current = leaf;

    /*
     * find the root of the subtree 
     * that includes the successor
     */
    while(parent != NULL && current == parent->right){  // O(h)
        current = parent;
        parent = current->parent;
    }

    /*
     * if parent == NULL the leaf has
     * no successor (is the right most leaf)
     */
    if(parent == NULL){
        return parent;
    }

    /*
     * go one to the right
     * since the successor
     * is sure to be in the
     * right subtree
     */
    parent = parent->right;

    /*
     * find the left most
     * leaf of the right
     * subtree since its
     * sure to be the successor
     */
    while(!is_leaf(parent)){        // O(h)
        if(parent->left != NULL)
            parent = parent->left;
        else    
            parent = parent->right;
    }

    //successor = parent;
    return parent;
}

void init_structures(){

    for(int i = 0; i < 6; i++){
        categoryArray[i] = new movieCategory_t;
    }

    // for(int i = 0; i < 50; i++){
    //     debug_array[i] = -1;
    // }

}

void destroy_structures(){

    /* traverse and delete user hashtable */

    user_t* current;
    for(int i  = 0; i < user_hashtable_p->capacity; i++){
        current = user_hashtable_p->arr[i];
        while(current != NULL){
            user_t* temp = current;
            current = current->next;
            delete_wh_tree(temp->history);
            delete temp;
        }
    }
    delete user_hashtable_p;

    /* traverse and delete list_node helping structure */

    list_node* node;
    for(int i = 0; i < 6; i++){
        node = cat_table[i];
        while(node != NULL){
            list_node* temp = node;
            node = node->next;
            delete temp;
        }
    }

    /* traverse and delete new_releases */

    delete_new_movies(new_releases);

    /* traverse and delete categoryArray */

    movie_t* movie;
    for(int i = 0; i < 6; i++){
        movie = categoryArray[i]->movie;
        delete_movies(movie);
    }

    cout << " ===== Structures Destroyed successfully =====\n" << endl;

}

int register_user(int userID){

    int index = user_hashtable_p->hash_function(userID);
    user_hashtable_p->insert(userID, NULL);
    cout << "Chain <" << index << "> of Users:\n";
    user_t* head = user_hashtable_p->arr[index];
    print_users(head);
    cout << "\nDONE\n";

    return 1;
}

int unregister_user(int userID){

    int index = user_hashtable_p->hash_function(userID);
    user_t* current = user_hashtable_p->arr[index];
    user_t* previous = NULL;
    userMovie_t* current_wh;

    /* find the user's info */
    while(current != NULL){
        if(current->userID == userID){
            current_wh = current->history;
            break;
        }
        previous = current;
        current = current->next;
    }

    /* if user was not found */
    if(current == NULL){
        cout << "User " << userID << " not found.\n";
        return -1;
    }

    /* delete the watch history tree */
    delete_wh_tree(current_wh);

    /* 
     * if user to be deleted is the head of the list 
     * then put the next user of the chain into the
     * corresponding hashtable slot and delete current
     * else link the previous to the next of current
     * and delete current
     */
    if(current == user_hashtable_p->arr[index]){
        user_hashtable_p->arr[index] = current->next;
        delete current;
    }else{
        previous->next = current->next;
        delete current;
    }

    return 1;
}

int add_new_movie(int movieID, int category, int year){

    if(is_first_time){
        new_releases = insert_new_movie(new_releases, movieID, category, year);
        is_first_time = false;
    }else
        insert_new_movie(new_releases, movieID, category, year);


    return 1;
}

int distribute_movies(void){

    traverse_new_movies(new_releases);

    return 1;
}

int watch_movie(int userID, int category, int movieID, int score){

    if(userID != previous_user){
        previous_user = userID;
        isFirstTime = true;
    }
        

    user_t* user = user_hashtable_p->search(userID);

    if(user == NULL){
        cout << "User " << userID << " not found.\n";
        return -1;
    }

    movie_t* movie = search_bst(categoryArray[category]->movie, movieID);

    if(movie == NULL){
        cout << "Movie " << movieID << " not found.\n";
        return -1;
    }

    // for(int i = 0; i < 50; i++){
    //     if(movie->movieID == debug_array[i])
    //         debug_isFound = true;
    // }

    // if(!debug_isFound){
    //     for(int i = 0; i < 50; i++){
    //         if(debug_array[i] == -1){
    //             debug_array[i] = movie->movieID;
    //             break;
    //         }
    //     }
    // }

    movie->watchedCounter++;
    movie->sumScore += score;

    if(isFirstTime || is_leaf(user->history)){
        user->history = insert_leaf(user->history, movieID, category, score);
        isFirstTime = false;
    }else{
        insert_leaf(user->history, movieID, category, score);
    }

    return 1;
}

int filter_movies(int userID, int score){

    // int N = 0;
    // for(int i = 0; i < 50; i++){
    //     if(debug_array[i] != -1)
    //         N++;
    // }
    // cout << "debug_array size: " << N << endl;

    for(int i = 0; i < 6; i++){
        count_nodes_bst(categoryArray[i]->movie, score);
    }

    const int size = num_of_movies;
    movie_t* arr[size];
    num_of_movies = 0;

    cout << "arr size: " << size << endl;

    for(int i = 0; i < 6; i++){
        make_array_from_bst(categoryArray[i]->movie, score, arr);
    }

    // TODO sort the array using heapsort

    for(int i = 0; i < size; i++){
        cout << "arr[" << i << "] = " << arr[i]->movieID << endl;
    }

    num_of_movies = 0;
    return 1;
}

int user_stats(int userID){

    user_t* user = user_hashtable_p->search(userID);
    int score_sum = 0;
    int counter = 0;

    //traverse_history_tree(user->history, score_sum, counter);

    /*
     * find left most leaf of the tree
     */
    userMovie_t* leaf = user->history;
    while(leaf->left != NULL){
        leaf = leaf->left;
    }

    score_sum += leaf->score;
    counter++;

    /*
     * repetitively find next leaf in
     * user's history tree. add the
     * scores and increment counter
     * at each leaf
     */
    while(find_next_leaf(leaf) != NULL){
        leaf = find_next_leaf(leaf);
        if(leaf != NULL){
            score_sum += leaf->score;
            counter++;
        }
    }

    float median = (float)score_sum / (float)counter;
    cout << "Q " << userID << " " << (float)median << endl;
    cout << "\nDONE\n";

    return 1;
}

int search_movie(int movieID, int category){

    movie_t* returned;

    for(int i = 0; i < 6; i++)
        if(category == i)
            returned = search_bst(categoryArray[i]->movie, movieID);

    if(returned != NULL){
        cout << "I " << movieID << " " << category << " " << returned->year;
        cout << "\n\nDONE\n";
    }else{
        cout << "I " << movieID << " " << category << "\n\n";
        cout << "There's no movie " << movieID << " in category " << category;
        cout << "\n\nDONE\n";
    }
    

    return 1;
}

int print_movies(void){

    print_category_array();

    return 1;
}

int print_users(void){

    user_t* current;

    cout << "P\n";

    for(int i = 0; i < user_hashtable_p->capacity; i++){

        cout << "\n> Chain <" << i << "> of Users:\n";

        current = user_hashtable_p->arr[i];
        while(current != NULL){
            cout << "     User: " << current->userID << endl; 
            print_history_tree(current->history, 'P');
            current = current->next;
        }

    }

    cout << "\nDONE\n";

    return 1;
}

int main(int argc, char* argv[]){

    string line, key = " ", opCode;
    int mid, cat, year, uid, score;
    int return_status;
    //Category_t cat;
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
        if(line.length() > 2){
			pos = line.find(key);
        	opCode = line.substr(0, pos);
        	line.erase(0, pos + 1);
		}else{
			if(line[0] == 'D')
				opCode = "D";
			else if(line[0] == 'M')
				opCode = "M";
			else if(line[0] == 'P')
				opCode = "P";
            else if(line == ""){
                cout << "Blank Line. Continuing...\n";
                cout << "\n========================================================\n\n";
                continue;
            }else{
                opCode = "noop";
            }
		}

        if(opCode == "0"){

            max_users = stoi(line);
            cout << "Max Users: " << max_users << endl;

        }else if(opCode == "1"){

            max_id = stoi(line);
            cout << "Max ID: " << max_id << endl;

        }else if(opCode == "R"){

            uid = stoi(line);
            cout << "R " << uid << "\n";
            register_user(uid);

        }else if(opCode == "U"){

            uid = stoi(line);
            cout << "U " << uid << "\n";
            unregister_user(uid);

        }else if(opCode == "A"){

            /* find the movie id */
            pos = line.find(key);
            mid = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);

            /* find the category */
            pos = line.find(key);
            cat = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);

            /* find the movie year */
            year = stoi(line);

            return_status = add_new_movie(mid, cat, year);
            
            if(return_status){
                
                cout << opCode << " " << mid << " " << cat << " " << year << "\n\n";
                cout << "New Releases Tree: ";
                print_new_movies(new_releases);
                cout << "\n\nDONE" << endl;

            }else
                cout << opCode << " " << mid << " " << cat << " " << year << " Failed\n";

        }else if(opCode == "D"){

            return_status = distribute_movies();

            if(return_status){

                for(int i = 0; i < 6; i++){
                    reverse_list(&cat_table[i]);
                    categoryArray[i]->movie = make_bst_from_list(cat_table[i]);
                }
                
                cout << opCode << "\n\n";
                cout << "Movie Category Array:";
                print_category_array();
                cout << "\n\nDONE\n";

            }else
                cout << opCode << " " << "Failed\n";

        }else if(opCode == "I"){

            /* find the movie id */
            pos = line.find(key);
            mid = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);

            /* find the category */
            pos = line.find(key);
            cat = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);

            if(stoi(line) != cat){
                cout << "I " << mid << " " << cat << " " << line << "\n\n";
                cout << "Too many arguments. Continuning...\n";
                cout << "\n========================================================\n\n";
                continue;
            }

            return_status = search_movie(mid, cat);

        }else if(opCode == "W"){

            pos = line.find(key);
            uid = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);

            pos = line.find(key);
            cat = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);

            pos = line.find(key);
            mid = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);         

            pos = line.find(key);
            score = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);   

            watch_movie(uid, cat, mid, score);

            cout << "W " << uid << " " << cat << " " << mid << " " << score << "\n";
            cout << "\nHistory Tree of User " << uid << "\n";
            print_history_tree(user_hashtable_p->search(uid)->history, 'W');
            cout << "\n\nDONE\n";

        }else if(opCode == "F"){

            pos = line.find(key);
            uid = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);

            pos = line.find(key);
            score = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);

            filter_movies(uid, score);

            cout << "F " << uid << " " << score << "\n";


        }else if(opCode == "Q"){

            pos = line.find(key);
            uid = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);

            user_stats(uid);

        }else if(opCode == "M"){

            cout << opCode << "\n\n";
            cout << "Movie Category Array:";
            print_movies();
            cout << "\n\nDONE\n";

        }else if(opCode == "P"){

            print_users();

        }else{

            cout << "WARNING: Unrecognized opCode. Continuing...\n";

        }

        cout << "\n========================================================\n\n";

    }

    inFile.close();
    destroy_structures();
    cout << "========================================================\n";

    return 0;
}