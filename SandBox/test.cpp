// #include <iostream>
// using namespace std;

// int main(){
//     cout <<"hello world" << endl;
//     cout <<"hello world2" << endl;

//     return 0;
// }


// #include <iostream>
// using namespace std;

// const double SALES_TAX = 0.075;
// void compareTax(double subTotal, double &tax , double &total ,double taxRate = SALES_TAX);


// int main(){
//     double sub = 10.0;
//     double tax = 0;
//     double total = 0;
//     compareTax(sub,tax,total);
//     cout<< "total : " << total <<endl;
//     cout<< "tax : " << tax <<endl;
//     return 0;
// }

// void compareTax(double subTotal, double &tax, double &total, double taxRate){
//     tax = subTotal * taxRate;
//     total = subTotal + tax;
// }


#include <iostream>
using namespace std;

void addItems(int newItem,int *&scores, int &numItems, int &capacity);

int main(){
    int capacity = 10;
    int numItems = 5;

    int *scores = new int[capacity]{1,2,3,4,5};
    for (size_t i = 0; i < 100; i++)
    {
        addItems(i*2 + 3,scores,numItems,capacity);
    }
    
    cout<< "capacity: " <<capacity<<endl;
    cout<<"num items: "<<numItems<<endl;

    for (size_t i = 0; i < numItems; i++)
    {
        cout<<scores[i]<< " ";
    }   

    return 0;
}

void addItems(int newItem,int *&scores, int &numItems, int &capacity){
    if(numItems == capacity){
        capacity = capacity * 2;
        int *temp = new int[capacity];
        
        for(int i = 0; i< numItems; ++i){
            temp[i] = scores[i];
        }
        delete [] scores;
        scores = temp;
    }
    scores[numItems] = newItem;
    ++numItems;
}




