#include <iostream>
#include <fstream>
#include <cstring>
#include <conio.h>
#include "Word.hpp"
using namespace std;
void menu();
void FileReader();
void FileWriter();
string getlineEng();
string getlineFars();
node* Search(bool isInserting = false    , string eng  = getlineEng() );
bool  Insert(string fars = getlineFars() , string eng  = getlineEng() );
bool  Delete(node* NodeToBeDeleted = Search());
void  PrintTraversals(node* startpoint = root);
void  PrintPre       (node* startpoint = root , ostream& printer = cout );
void  PrintIn        (node* startpoint = root);
void  PrintPost      (node* startpoint = root);
int main()
{
    FileReader();
    menu();
    FileWriter();
    return 0;
}

void menu(){
    cout << "Welcome to your dictionary mister!" << endl;
    cout << "BST Dictionary Management menu:"    << endl;
    while(true){
        cout << "1)Insert a word" << endl;
        cout << "2)Delete a word" << endl;
        cout << "3)Search a word" << endl;
        cout << "4)Traversals"    << endl;
        cout << "5)Exit"          << endl;
        cout << "enter a number:";
        char choice;
        {
        string MenuInput;
        cin >> MenuInput;
        choice = MenuInput[0];
        }
        system("CLS");
        switch(choice){
            case '1' :Insert();                 continue;
            case '2' :Delete();                 continue;
            case '3' :Search()->data.printall();continue;
            case '4' :PrintTraversals();        continue;
            case '5' :return;//exit(0);//but if you use exit() function some destructors are not called
        }
        if(!('0' <= choice && choice <= '9')) 
            cerr << "Error\a\n" << "invalid character\nyou must enter a number not " << choice << endl;    
        else 
            cerr << "Error\a\n" << "invalid character\nyou must enter a number in the given range(1-5)!" << endl;
        system("pause");        
    }
}
void FileReader(){
    ifstream fin("./BST.txt" , ios::in);
    if(!fin)cerr << "Error\a\n" << "File not open!\n" << "BST was not restored";
    else{
        string word , eng , fars;
        while(getline(fin , word)){
            int commalocation = word.find(',');
            eng.assign(word.c_str() , commalocation);
            fars.assign(word , commalocation + 1 , word.size() - eng.size() - 1);
            Insert(fars , eng);
        }
    }
    fin.close();
}
void FileWriter(){
    ofstream fout("./BST.txt" , ios::out | ios::trunc);
    if(!fout)cerr << "Error\a\n" << "File not open!\n" << "BST was not stored";
    else     PrintPre(root , fout);
    fout.close();
}
string getlineEng(){
    string eng;
    cout << "english word:";cin.ignore();
    getline(cin , eng);
    return eng;
}
string getlineFars(){
    string fars;
    cout << "kalame farsi:";
    getline(cin , fars);
    return fars;
}
node* Search(bool isInserting , string eng){ 
    node* sercher = root;
    while(sercher != nullptr){
        if     ( 0 == strcasecmp(sercher->data.getEnglish().c_str() , eng.c_str() ) ) return sercher;                                     
        else if( 0 <  strcasecmp(sercher->data.getEnglish().c_str() , eng.c_str() ) ) {parent = sercher; sercher = sercher->left;}
        else   {parent = sercher; sercher = sercher->right;}
    }
    if (!isInserting){
        system("CLS");
        if(root == nullptr)cerr << "tree is empty\a\n";
        else               cerr << "Error\a\n" << "not found!\n"; 
        system("pause");
    }   
    return nullptr;
}
bool Insert(string fars , string eng){
    node* oldword = Search(true , eng);
    if (oldword != nullptr){
        system("CLS");
        cerr << "Error\a\n" << "this word already exists in the dictionary!\n";
        system("pause");
        return false;
    }
    if (root == nullptr){
        root = new node;
        root->data.setall(eng , fars);
        return true;
    }
    node* neword = new node;
    neword->data.setall(eng , fars);
    if ( 0 < strcasecmp(parent->data.getEnglish().c_str() , eng.c_str() )) parent->left  = neword;
    else parent->right = neword;
    return true;
}
bool Delete(node* del){//why as parameter why not in first line: to can be used by coder for deletion of special node* because does not want search()
    if (del == nullptr)return false;
    if (del->left == nullptr && del->right == nullptr){//if was leaf
        if (del == root){root = nullptr; return true;}//root has not parent hence throws exception
        if (parent->left == del)parent->left  = nullptr;
        else                    parent->right = nullptr;
    }
    else if (del->left == nullptr || del->right == nullptr){//if was node with degree 1
        if (del == root && root->left  != nullptr){root = root->left;  delete root->left;  return true;}
        if (del == root && root->right != nullptr){root = root->right; delete root->right; return true;}
        if      (parent->left  == del && del->left  != nullptr) parent->left  = del->left;
        else if (parent->left  == del && del->right != nullptr) parent->left  = del->right;
        else if (parent->right == del && del->left  != nullptr) parent->right = del->left;
        else                                                    parent->right = del->right;
    }
    else{//if was node with degree 2//keys' subsitution with inorder successor
        node* delcopy = del;
        del = del->right;
        while(del->left != nullptr) {parent = del; del = del->left;}
        delcopy->data = del->data;
        if(delcopy->right->left != nullptr)parent->left   = del->right;
        else                               delcopy->right = del->right;
    }
    delete del;
    return true;
}
void PrintTraversals(node* root){
    if(root == nullptr){cerr << "tree is empty\a\n";return;}
    cout << "Preorder:  ";PrintPre(root); cout << endl;
    cout << "Inorder:   ";PrintIn(root);  cout << endl;
    cout << "Postorder: ";PrintPost(root);cout << endl;
}
void PrintPre(node *root , ostream& fcout)
{
    if(root == nullptr)return;
    fcout << root->data.getEnglish() << ',' << root->data.getFarsi();
    (&fcout == &cout)?cout << "---":fcout << endl;
    PrintPre(root->left , fcout);
    PrintPre(root->right, fcout);
}
void PrintIn(node* root){
    if(root == nullptr)return;
    PrintIn(root->left);
    cout << root->data.getEnglish() << ',' << root->data.getFarsi() << "---";
    PrintIn(root->right);
}
void PrintPost(node* root){
    if(root == nullptr)return;
    PrintPost(root->left);
    PrintPost(root->right);
    cout << root->data.getEnglish() << ',' << root->data.getFarsi() << "---"; 
}







// char* strTochr(string& s){//convertion of string to char*
//     char* chr = s.data();//c++17 or newer : best option to access a non-const char* of std::string
//     //char* chr = &s[0];//another efficient way(most simplest one)
//     return chr;
// } 
// s = chr;//char* to string: just use assignment operator(if the string hasn't been constructed you can use string constructor)//s(chr);







