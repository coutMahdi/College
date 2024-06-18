
class Word
{
    private:
        std::string english = "";
        std::string farsi = "";
    public :
        void setEnglish(std::string english){
            this->english = english;
        }
        std::string getEnglish(){
            return english;
        }
        void setFarsi(std::string farsi){
            this->farsi = farsi;
        }
        std::string getFarsi(){
            return farsi;
        }
        void setall(std::string english , std::string farsi){
            this->english = english;
            this->farsi = farsi;
        }
        void printall(){
            if(this == nullptr)return;//if Search() function return nullptr
            // if(english != "\0" && farsi != "\0")
            std::cout << "english word:" << english << '\n'
                 << "farsi:"        << farsi   << '\n';
        }    
};
struct node
{
    Word data;
    node* left  = nullptr , *right = nullptr;
};
node* root = nullptr;
node* parent = nullptr;
/*note:
deleting an node* does not change address had been stored in another node*
double delete       leads to runtime error that ends the program
nullptr = new node; leads to runtime error that ends the program
*/