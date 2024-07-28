#include "main.h"
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <ctime>
#include <memory>
#include <math.h>
#include <cstdlib>

#ifndef PREFIX_TRIE_H
#define PREFIX_TRIE_H

using namespace std;

class Prefix_Trie{
    
    private:
        char*           subjectData            = nullptr;
        long            allocatedSubjectSize,
                        subjectSize,
                        scaffoldCount,
                        numNodes;
             
        const long      MAX_SCAFFOLDS          = 607;
        const int       FRAGMENT_SIZE          = 36;
        
        character_Node* root                   = nullptr;
    
    public:
    
    
    
    Prefix_Trie();
    ~Prefix_Trie();
    character_Node* build_Trie();
    void search_Trie();
    long random_Index(int max, int stringLength);
    string copy_String(string fromString, int low, int high);
    bool file_reader(const string& file_Name);
    void genome_Constructor(const string toAdd, long catIndex);
    void genome_Deconstructor(char* subjectPtr);
    character_Node* new_Node();
    string mutate_Fragment(string fragment);
    char random_Character();
    void create_Trie(int nmers, bool subProblemB);
    void Add_Node_To_Trie(string fragment);
    void destroy_Trie(character_Node* tempRoot);
    int get_Nodes();
    string copy_From_Subject( int low, int high);
    
};


#endif 