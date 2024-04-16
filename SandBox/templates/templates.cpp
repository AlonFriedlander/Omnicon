

// // template<typename T, int length>
// // class Array{
// //     public:
// //         T array[10];

// //         void fill(T value){
// //             for (int i = 0; i < length; i++)
// //             {
// //                 array[i] = value;
// //             }
// //         }

// //         T& at(int index){
// //             return array[index];
// //         }
// // };

// // int main(){
// //     Array<int,7> intarr;
// //     intarr.fill(5);
// //     cout<<"int array at 4 is: "<<intarr.at(4)<<endl;

// //     Array<string,9> stringarr;
// //     stringarr.fill("3");
// //     cout<<"string array at 4 is: "<<stringarr.at(4)<<endl;

// //     return 0;
// // }


// // #include <iostream>
// // #include <algorithm>
// // #include <vector>

// // int main() {
// //     std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

// //     // Partition the vector based on even numbers
// //     auto it = std::partition(numbers.begin(), numbers.end(), [](int n) {
// //         return n % 2 == 0; // Predicate: returns true for even numbers
// //     });

// //     // Print the partitioned vector
// //     std::cout << "Partitioned vector: ";
// //     for (auto iter = numbers.begin(); iter != it; ++iter) {
// //         std::cout << *iter << " "; // Print elements satisfying the predicate
// //     }
// //     std::cout << "| ";
// //     for (auto iter = it; iter != numbers.end(); ++iter) {
// //         std::cout << *iter << " "; // Print elements not satisfying the predicate
// //     }
// //     std::cout << std::endl;

// //     return 0;
// // }
// // C++ code to demonstrate the working of 
// // partition() and is_partitioned()
// // #include<iostream>
// // #include<algorithm> // for partition algorithm
// // #include<vector> // for vector
// // using namespace std;
// // int main()
// // {
// // 	// Initializing vector
// // 	vector<int> vect = { 2, 1, 5, 6, 8, 7 };
	
// // 	// Checking if vector is partitioned 
// // 	// using is_partitioned()
// // 	is_partitioned(vect.begin(), vect.end(), [](int x)
// // 	{
// // 		return x%2==0;
		
// // 	})?
	
// // 	cout << "Vector is partitioned":
// // 	cout << "Vector is not partitioned";
// // 	cout << endl;
	
// // 	// partitioning vector using partition()
// // 	partition(vect.begin(), vect.end(), [](int x)
// // 	{
// // 		return x%2==0;
		
// // 	});
	
// // 	// Checking if vector is partitioned 
// // 	// using is_partitioned()
// // 	is_partitioned(vect.begin(), vect.end(), [](int x)
// // 	{
// // 		return x%2==0;
		
// // 	})?
	
// // 	cout << "Now, vector is partitioned after partition operation":
// // 	cout << "Vector is still not partitioned after partition operation";
// // 	cout << endl;
	
// // 	// Displaying partitioned Vector
// // 	cout << "The partitioned vector is : ";
// // 	for (int &x : vect) cout << x << " ";
	
// // 	return 0;
// // }


// #include <iostream>
// #include <memory>

// using namespace std;

// class MyClass{
//     public:
//         MyClass(){
//             cout<<"ctor invoked"<<endl;
//         }
//         ~MyClass(){
//             cout<<"dector invoked"<<endl;
//         }
// };

// int main(){
//     // unique_ptr<int>unPtr1 = make_unique<int>(25);
//     // unique_ptr<int>unPtr2 = move(unPtr1);
//     // cout << *unPtr2 <<endl;
//     // cout << *unPtr1 <<endl;
    
//     // unique_ptr<MyClass>unPtr1 = make_unique<MyClass>();

//     // shared_ptr<MyClass>shptr1 = make_shared<MyClass>();

//     // cout<<"shared count: " <<shptr1.use_count()<<endl;
//     // {
//     //     shared_ptr<MyClass>shptr2 = shptr1;

//     //     cout<<"shared count: " <<shptr1.use_count()<<endl;
//     // }

//     // cout<<"shared count: " <<shptr1.use_count()<<endl;

//     weak_ptr<int>weptr1;
//     {
//         shared_ptr<int>shptr1 = make_shared<int>();
//         weptr1 = shptr1;
//     }


//     system("pause>nul");
//     // std::cout << "Drawing Rectangle" << std::endl;

//     // return 0;
// }






// #include<iostream>
// #include<string>

// using namespace std;

// class Printer
// {
//     string _name;
//     int _paper;
// public:
//     Printer(string name, int paper){
//         _name = name;
//         _paper = paper;
//     }

//     void print(string txt){
//         int requiredPaper = txt.length() / 10;

//         if(requiredPaper > _paper){
//             throw "no paper";
//         }

//         cout<<"printing: "<< txt<<endl;
//         _paper -= requiredPaper;    
//     }
// };


// int main(){
//     Printer myprinter("hp", 10);
//     try{
//         myprinter.print("hfjgbfhjlvnlff;nvjlnf;sdsdsddsdsdsdsdddddddddjvbfkdhjbdk");
//         myprinter.print("hfjgbfhjlvnlff;nvjlnf;jvbssssssssssssssssssssfkdhjbdk");
//         myprinter.print("hfjgbfhjlvnlff;nvjlnf;jsssssssssssssssssssssssssssvbfkdhjbdk");
//     }
//     // catch(const char * txtexecption){
//     //     cout<<"execption: "<< txtexecption<<endl;
//     // }
//     catch(int exCode){
//         cout<<"execption: "<< exCode<<endl;
//     }
//     catch(...){
//         cout<<"execption happen!!!!! "<<endl;
//     }

//     return 0;
// }



#include <fstream>
#include <iostream>
#include <fstream>

using namespace std;

int main(){

    // cout << "what did you eat today? ";
    // string food;
    // cin >> food;

    // ofstream file("foods.txt");
    // file<<food;
    // file.close();
    
    ifstream file("foods.txt");
    string food;
    file >> food;
    file.close();

    cout<<food<<endl;

    return 0;
}




















