#include "Prefix_Trie.h"

using namespace std;
    
#ifndef PREFIX_TRIE_CPP
#define PREFIX_TRIE_CPP


    
Prefix_Trie::Prefix_Trie(){
    
    cout << "Constructor called\n";
    
    subjectData            = nullptr;
    allocatedSubjectSize   = 3000000000;
    subjectSize            = 0;
    scaffoldCount          = 0;
    numNodes               = 0;
    root                   = nullptr;
         
    
}


Prefix_Trie::~Prefix_Trie(){
    
    cout << "Deconstructor called\n";
    
    genome_Deconstructor(subjectData);
    destroy_Trie(root);
}


character_Node* Prefix_Trie::build_Trie(){
    return nullptr;
}


void Prefix_Trie::search_Trie(){
    
}


long Prefix_Trie::random_Index(int max, int stringLength ){

    // return a random number from 0 to the size of the desired dataset
    return rand() % (stringLength - max);
}


string Prefix_Trie::copy_String(string fromString, int low, int high){
    string output ="";

    for(int index=low; index<high; index++){
        output += fromString[index];
    }

    return output;
}

string Prefix_Trie::copy_From_Subject( int low, int high){
    string output ="";

    for(int index=low; index<high; index++){
        output += subjectData[index];
    }

    return output;
}

bool Prefix_Trie::file_reader(const string& file_Name) {
    // variables
    ifstream file(file_Name);
    string currentScaffoldName;
    string currentLine;
    string tempGenome = "";
    long prevSize;
    unsigned long index;
    //time_t stop_Watch = 0;

    // check if the file can be opened
    if (!file.is_open()) {
        // display error and exit
        cerr << "Error: Unable to open file " << file_Name << "\n";
        return false;
    }

    
    // initialize starting array, set to 3 billion to save on runtime complexity
    allocatedSubjectSize = 3000000000;
    subjectData = new char[allocatedSubjectSize];
    
    // iterate through file contents line by line
    while ( scaffoldCount < MAX_SCAFFOLDS && getline(file, currentLine)) {
        // check if we hit a header
        if(currentLine[0] == '>' && !tempGenome.empty()){
            
            // increment scaffold count by 1
            scaffoldCount++;
            
            // calculate new size for genome array
            prevSize = subjectSize;
        
            // resize the genome array to fit last scaffold and copy data into new array
            genome_Constructor(tempGenome, prevSize);

            // reset the temp genome string to empty
            tempGenome = "";
        }

        else if (!currentLine.empty() && currentLine[0] != '>') {
            // Iterate through current line and append characters to temp_Genome
            for (index = 0; index < currentLine.length() && index < 80; index++) {
                if(  currentLine[index] == 'A' 
                    || currentLine[index] == 'C' 
                    || currentLine[index] == 'G' 
                    || currentLine[index] == 'T' 
                    || currentLine[index] == 'N'){
                
                
                    // Append character to temp_Genome
                    tempGenome += currentLine[index];
                }
            }
        }
    }

    // post append information of last scaffold
    if (!tempGenome.empty()) {
        // calculate new genome array size
        prevSize = subjectSize;
        
        // resize the genome array to fit last scaffold and copy data into new array
        genome_Constructor(tempGenome, prevSize);
    }

    // close the file
    file.close();

    // return sucess
    return true;
}// end of file reader function


// helper function for resizing and adding a new scaffold into the subject array
void Prefix_Trie::genome_Constructor(const string toAdd, long catIndex) {
    long index;
    long length = toAdd.length();
    
    // check if the array needs to be resized
    if(subjectSize + length >= allocatedSubjectSize){
        // calculate the new size of the genome
        long newSize = subjectSize + length;
        
        // allocate new array size
        char* newSubjectArr = new char[newSize];
        
        // copy data from old array to new one
        for(index = 0; index < subjectSize; index++){
            newSubjectArr[index] = subjectData[index];
        }
        
        // deallocate memory from old array
        delete[] subjectData;
        
        // set genome pointer to new array
        subjectData = newSubjectArr;
        
        // set the new allocated size for genome array
        allocatedSubjectSize = newSize;
    }
    
        
    for(index = 0; index < length; index++){
        //add in the new data 
        subjectData[catIndex + index] = toAdd[index];
    }
    
    // set the new genome size
    subjectSize = catIndex + toAdd.length();
}


// deconstructor for deallocating the genome character array
void Prefix_Trie::genome_Deconstructor(char* subjectPtr){
    delete[] subjectPtr;
    subjectPtr=nullptr;
}


// function that will drive the trie creation
void Prefix_Trie::create_Trie(int nmers, bool subProblemB){
    
    // cerr << "entered create_Trie\n";
    
    // create root node
    root = new_Node();
    int segmentLength = 50000;
    long randIndex = random_Index(segmentLength, subjectSize);

    // grab a random 50k long segment from the subject data
    string segment = copy_From_Subject(randIndex, randIndex + segmentLength);
    string fragment = "";
    int index = 0;
    
    // cerr << "Created segment\n";
    //cerr << segment << endl;
    
    // loop for number of 36-mers (5k, 50k, 100k, and 1M)
    for(index=0; index < nmers; index++){
        //get a radnom index in the 50k long segment ( 36 size fragment)
        randIndex = random_Index(segmentLength, FRAGMENT_SIZE);    
        
        // copy the fragment in from that random index
        fragment = copy_String(segment, randIndex, randIndex +FRAGMENT_SIZE);
        
        // cerr << "Fragment: " << fragment << endl;
        
        // if on subpart B do the 5% perbase error rate
        if(subProblemB){
            // update fragment with mutations
            fragment = mutate_Fragment(fragment);
        }
        
        // pass to function to add to trie
        Add_Node_To_Trie(fragment);
            
    }
            
}
        

// function to add a node into the trie
void Prefix_Trie::Add_Node_To_Trie(string fragment){
    
    int index = 0;
    character_Node* tempPtr = root;
    bool run = true;
    
    
    // cerr << "Entered Add_Node\n";
    
    // iterate through the input parameter string
    for(index=0; index< FRAGMENT_SIZE; index++){
        
        run = true;
        
        // cerr << "On index: " << index << endl;
        // cerr << "Letter  : " << fragment[index] << endl;
        
        // loop through trie starting at root
        while(tempPtr != nullptr && run){
            
            // cerr << "Top of loop\n";
            
            // check if letter does not exist ( if pointer to that character_Node is nullptr)
            if(fragment[index] == 'A' && tempPtr->A == nullptr){
                
                // cerr << "Adding A to the tree\n";
                // create node for letter
                // add node
                tempPtr->A = new_Node();
                
                // go to that node
                tempPtr = tempPtr->A;
            }
            
            else if(fragment[index] == 'C' && tempPtr->C == nullptr){
                
                // cerr << "Adding C to the tree\n";
                
                // create node for letter
                // add node
                tempPtr->C = new_Node();
                
                // go to that node
                tempPtr = tempPtr->C;
            }
            
            else if(fragment[index] == 'G' && tempPtr->G == nullptr){
                
                // cerr << "Adding G to the tree\n";
                
                // create node for letter
                // add node
                tempPtr->G = new_Node();
                
                // go to that node
                tempPtr = tempPtr->G;
            }
            
            else if(fragment[index] == 'T' && tempPtr->T == nullptr){
                
                // cerr << "Adding T to the tree\n";
                
                
                // create node for letter
                // add node
                tempPtr->T = new_Node();
                
                // go to that node
                tempPtr = tempPtr->T;
            }
            
            else if(fragment[index] == 'N' && tempPtr->N == nullptr){
                
                // cerr << "Adding N to the tree\n";
                
                // create node for letter
                // add node
                tempPtr->N = new_Node();
                
                // go to that node
                tempPtr = tempPtr->N;
            }
        
            // check if letter already exists (if pointer to that character_Node is NOT nullptr)
            if(fragment[index] == 'A' && tempPtr->A != nullptr){
                
                // cerr << "going to node A\n";
                
                // go to that node
                tempPtr = tempPtr->A;
            }
            else if(fragment[index] == 'C' && tempPtr->C != nullptr){
                
                // cerr << "going to node C\n";
                
                // go to that node
                tempPtr = tempPtr->C;
            }
            else if(fragment[index] == 'G' && tempPtr->G != nullptr){
                
                // cerr << "going to node G\n";
                
                // go to that node
                tempPtr = tempPtr->G;
            }
            else if(fragment[index] == 'T' && tempPtr->T != nullptr){
                
                // cerr << "going to node T\n";
                
                // go to that node
                tempPtr = tempPtr->T;
            }
            else if(fragment[index] == 'N' && tempPtr->N != nullptr){
                
                // cerr << "going to node N\n";
                
                // go to that node
                tempPtr = tempPtr->N;
            }
                
            run = false;
        }
            
        // iterate to next character 
    }
}


// function to create a new node
character_Node* Prefix_Trie::new_Node(){
    // allocate a memory block for the character_Node
    character_Node* node = new character_Node;
    
    node->A = nullptr;
    node->C = nullptr;
    node->T = nullptr;
    node->G = nullptr;
    node->N = nullptr;
    
    // increment node counter
    numNodes++;
    
    // return ptr to the new node
    return node;
}

    
    


// function to mutate string for part B
string Prefix_Trie::mutate_Fragment(string fragment){
    string mutatedStr = "";
    int randomNum = 0;
    
    // iterate through string
    for(int index=0; index < fragment.length(); index++){
        // get random number 1 to 100
        randomNum = 1 + (rand() % 100);
        
        // if percentage is 1-5
        if( randomNum < 6 ){
            // get random character
            // put random character at current index
            mutatedStr += random_Character();
        }
            
        // otherwise 
        else{
            // use current character
            mutatedStr += fragment[index];
        }
    }
            
    // return updated string
    return mutatedStr;
}
    
    
// function to return a random character (A,C,G,T,N)
char Prefix_Trie::random_Character(){
    // array [A,C,G,T,N]
    char alphabet[5] = {'A','C','G','T','N'};
    
    // get a random number 0 - 4
    
    // return character at that index
    return alphabet[rand() % 5];
}
    

// function to destroy the trie
void Prefix_Trie::destroy_Trie(character_Node* tempRoot){
    
    if(tempRoot->A != nullptr){
        destroy_Trie(tempRoot->A);
    }
    if(tempRoot->C != nullptr){
        destroy_Trie(tempRoot->C);
    }
    if(tempRoot->G != nullptr){
        destroy_Trie(tempRoot->G);
    }
    if(tempRoot->T != nullptr){
        destroy_Trie(tempRoot->T);
    }
    if(tempRoot->N != nullptr){
        destroy_Trie(tempRoot->N);
    }
    
    delete tempRoot;
    
}
    

// function to search through subject(main traverse driver)
    // use same 50k long segment from trie creation
    
    // iterate thorugh the segment ( index < 50k - 36) 
        // create fragmnet at index
        
        // call to search trie function
        
    
        
        
            
        
// function that will search through trie ( GOTTA USE A STACK???)
    // use string as stack???
    // mismatch score var
    // mismatch index var
    

    // iterate through fragment passed in AND while mismatch score < 2
    
        // if exists
        
            // go to that node and continue
            
            // add 1 to mismatch, explore children ( possibly use mismatch index to come back and try another path)
                // try A
                
                // try C
                
                // try G
                
                // try T
                
                // try N
            
            
        // if character doesnt exist
            // add 1 to mismatch counter and traverse children ( possibly use mismatch index to come back and try another path, which could classify as the stack)
                // try A
                
                // try C
                
                // try G
                
                // try T
                
                // try N
        
        // if at leaf node ( or at depth 36) 
            // fragment found
            // increment hit count
            // return success
            
            
    // return failure

int Prefix_Trie::get_Nodes(){
    return numNodes;
}

#endif