#include <iostream>
#include <vector>
#include <string>

using namespace std;

#define MAX_ROW 5

class Book {
public:
    int bookId;
    string bookName;
    string bookAuthor;
    Book *left, *right;

    Book(int id, const string &name = "None", const string &author = "None")
            : bookId(id), bookName(name), bookAuthor(author), left(nullptr), right(nullptr) {}
};

Book *createBook(int id, const string &name = "None", const string &author = "None") {
    return new Book(id, name, author);
}

Book *rightRotate(Book *x) {
    Book *y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

Book *leftRotate(Book *x) {
    Book *y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

Book *splay(Book *root, int bookId) {
    if (root == nullptr || root->bookId == bookId)
        return root;

    if (root->bookId > bookId) {
        if (root->left == nullptr) return root;

        if (root->left->bookId > bookId) {
            root->left->left = splay(root->left->left, bookId);
            root = rightRotate(root);
        } else if (root->left->bookId < bookId) {
            root->left->right = splay(root->left->right, bookId);
            if (root->left->right != nullptr)
                root->left = leftRotate(root->left);
        }
        return (root->left == nullptr) ? root : rightRotate(root);
    } else {
        if (root->right == nullptr) return root;

        if (root->right->bookId > bookId) {
            root->right->left = splay(root->right->left, bookId);
            if (root->right->left != nullptr)
                root->right = rightRotate(root->right);
        } else if (root->right->bookId < bookId) {
            root->right->right = splay(root->right->right, bookId);
            root = leftRotate(root);
        }
        return (root->right == nullptr) ? root : leftRotate(root);
    }
}

Book *insert(Book *root, int bookId, const string &name, const string &author) {
    if (root == nullptr) return createBook(bookId, name, author);

    root = splay(root, bookId);

    if (root->bookId == bookId) return root;

    Book *newBook = createBook(bookId, name, author);
    if (root->bookId > bookId) {
        newBook->right = root;
        newBook->left = root->left;
        root->left = nullptr;
    } else {
        newBook->left = root;
        newBook->right = root->right;
        root->right = nullptr;
    }
    return newBook;
}

Book *search(Book *root, int bookId) {
    return splay(root, bookId);
}

void preOrder(Book *root) {
    if (root != nullptr) {
        cout << root->bookId << " \t" << root->bookName << "\t" << root->bookAuthor << endl;
        preOrder(root->left);
        preOrder(root->right);
    }
}

Book *bst_search(Book *root, int bookId) {
    if (root == nullptr || root->bookId == bookId) {
        return root;
    } else if (root->bookId > bookId) {
        return bst_search(root->left, bookId);
    } else {
        return bst_search(root->right, bookId);
    }
}

struct struct_Graph {
    int bookId;
    string bookName;
    string bookAuthor;
    int flag;
};

class Graph_Book {
public:
    int num_book;
    vector<struct_Graph> Book_data;
    vector<vector<int>> adjList;

    Graph_Book() : num_book(0) {}

    void link() {
        int j;
        cout<<"\n";
        for (int i = 0; i < num_book; i++) {
            cout << i + 1 << ". " << Book_data[i].bookName << endl;
        }
        cout<<"\n";
        for (int i = 0; i < num_book; i++) {
            cout << "Enter the serial number of the book you want to link -1 for Exit: ";
            cin >> j;

            if (j == -1) {
                break;
            }
            if (j < num_book && j > 0){
                adjList[num_book - 1].push_back(j - 1);
                adjList[j-1].push_back(num_book -  1);
            }
            else {
                cout << "Not a valid link id" << endl;
            }
        }
    }

    void addBook(int id, string &name, string &author) {
        struct_Graph book;
        book.bookId = id;
        book.bookName = name;
        book.bookAuthor = author;
        book.flag = 0;

        Book_data.push_back(book);
        adjList.push_back(vector<int>());
        num_book += 1;

        if (num_book > 1) {
            link();
        }
    }
};

struct struct_student {
    int RollNo;
    vector<struct_Graph> list_of_book;
    int top;
    int size;
};

class Student : protected Graph_Book {
public:
    int num_of_student;
    vector<struct_student> student_data;

    Student() : num_of_student(0) {}

    void print_all_Student(){
        if(num_of_student == 0){
            cout << "No Record Available ..." << endl;
            return;
        }

        cout << "The Students Are : " << endl;
        for(int i = 0; i < num_of_student; i++){
            cout << student_data[i].RollNo << "\t\t";
            cout << student_data[i].top << endl;
        }
    }

    void print_all_book(Graph_Book &obj) {
        int n = obj.num_book;
        cout << "The Books are : " << endl << endl;
        for (int i = 0; i < n; i++) {
            cout << obj.Book_data[i].bookId << "\t" << obj.Book_data[i].bookName << "\t" << obj.Book_data[i].bookAuthor;
            if (obj.Book_data[i].flag == 0) {
                cout << "\t" << "Available" << endl;
            } else {
                cout << "\t" << "Issued" << endl;
            }
        }
    }

    void addStudent(int roll) {
        struct_student temp;
        temp.RollNo = roll;
        temp.top = 0;
        temp.size = 2;

        student_data.push_back(temp);
        num_of_student += 1;
    }

    void issue(int roll, Graph_Book &obj) {
        int index = -1;

        for (int i = 0; i < num_of_student; i++) {
            if (student_data[i].RollNo == roll) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            cout << "Not a Valid Register Number ... " << endl;
            return;
        }

        print_all_book(obj);
        cout << endl;

        int issued_book_index;
        cout << "Enter a Book ID: ";
        cin >> issued_book_index;

        int status = -1;
        for (int i = 0; i < obj.num_book; i++) {
            if (obj.Book_data[i].bookId == issued_book_index) {
                status = i;
                break;
            }
        }

        if (status == -1){
            cout << "Not a Valid Book Id" << endl;
            return;
        }else if(obj.Book_data[status].flag == 1){
            cout << "Not Available Book ID ... " << endl;
            return;
        } else {
            if (student_data[index].top == student_data[index].size) {
                cout << "You Already Issued Maximum Number of books " << endl;
                return;
            } else {
                student_data[index].top++;
                student_data[index].list_of_book.push_back(obj.Book_data[status]);
                obj.Book_data[status].flag = 1;
                cout << "Book issued successfully!" << endl;
            }
        }
    }

    void issued_book(int roll) {
        int index = -1;

        for (int i = 0; i < num_of_student; i++) {
            if (student_data[i].RollNo == roll) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            cout << "Not a Valid Register Number ... " << endl;
            return;
        }

        if (student_data[index].list_of_book.empty()) {
            cout << "You Have Not Issued Any Book ... " << endl;
            return;
        } else {
            cout << "Issued Book List : " << endl;
            for (struct_Graph bookIndex : student_data[index].list_of_book) {
                cout << bookIndex.bookId << " \t" << bookIndex.bookName;
                cout << " \t" << bookIndex.bookAuthor << endl;
            }
        }
    }

    void return_book(int roll, Graph_Book obj[], int n) {
    int student_index = -1;

    // Find the student by roll number
    for (int i = 0; i < num_of_student; i++) {
        if (student_data[i].RollNo == roll) {
            student_index = i;
            break;
        }
    }

    if (student_index == -1) {
        cout << "Not a Valid Register Number...\n";
        return;
    }

    if (student_data[student_index].list_of_book.empty()) {
        cout << "You have not issued any book...\n";
        return;
    } else {
        cout << "Issued Book List:\n";
        for (const auto &book : student_data[student_index].list_of_book) {
            cout << book.bookId << "\t" << book.bookName << "\t" << book.bookAuthor << endl;
        }
        int return_book_id;
        cout << "Enter a Book ID to return: ";
        cin >> return_book_id;

        bool found = false;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < obj[i].num_book; j++) {
                if (obj[i].Book_data[j].bookId == return_book_id && obj[i].Book_data[j].flag == 1) {
                    obj[i].Book_data[j].flag = 0;
                    found = true;
                    break;
                }
            }
            if (found) break;
        }

        if (!found) {
            cout << "Invalid Book ID or Book is not issued...\n";
            return;
        }

        // Remove the book from the student's list
        for (auto it = student_data[student_index].list_of_book.begin();
             it != student_data[student_index].list_of_book.end(); ++it) {
            if (it->bookId == return_book_id) {
                student_data[student_index].list_of_book.erase(it);
                student_data[student_index].top--;
                break;
            }
        }

        cout << "Book returned successfully!\n";
    }
    }
};

void print_all_book(Graph_Book &obj) {
    int n = obj.num_book;
    cout << "The Books are : " << endl << endl;
    for (int i = 0; i < n; i++) {
        cout << obj.Book_data[i].bookId << "\t" << obj.Book_data[i].bookName << "\t" << obj.Book_data[i].bookAuthor;
        if (obj.Book_data[i].flag == 0) {
            cout << "\t" << "Available" << endl;
        } else {
            cout << "\t" << "Issued" << endl;
        }
    }
}

void Related_Book(int bookId, Graph_Book &obj) {
    int index = -1;
    for (int i = 0; i < obj.num_book; i++) {
        if (obj.Book_data[i].bookId == bookId) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        if (!obj.adjList[index].empty()) {
            cout << "Related books are : " << endl;
            for (int temp : obj.adjList[index]) {
                cout << obj.Book_data[temp].bookId << "\t" << obj.Book_data[temp].bookName << endl;
            }
        }
    }
}

void find_related_book(int roll, Graph_Book obj){
    int index;
    print_all_book(obj);

    cout << endl << endl;

    cout << "Enter book id -> to fin related book : ";
    cin >> index;

    Related_Book(index, obj);
};

int main() {
    int top = 0;
    int choice;
    int person;

    vector<string> nickName;

    Book *root_record[MAX_ROW] = {nullptr};
    Graph_Book obj[MAX_ROW];

    Student students;
    cout<<"\n\t\t\t\t\tLIBRARY MANAGEMENT SYSTEM";
    do {
        cout << "\n\n1. Admin\n2. Student Issue\n3. Student Return\n4. Exit\n\nEnter your choice : ";
        cin >> person;

        if (person == 1) {
            do {
                cout << "\n1. Create New Rack\n2. Insert\n3. Search\n4. Recently Accessed Books\n5. Book Details\n6. Add Student\n7. Print All Students\n8. Exit\n\nEnter your choice: ";
                cin >> choice;

                if (choice == 1) {
                    if (top >= MAX_ROW) {
                        cout << "Maximum number of trees reached.\n";
                        continue;
                    }

                    int bookId;
                    string bookName, bookAuthor, nick;
                    cout << "Enter Book ID: ";
                    cin >> bookId;
                    cout << "Enter Book Name: ";
                    cin.ignore();
                    getline(cin, bookName);
                    cout << "Enter Book Author: ";
                    getline(cin, bookAuthor);
                    cout << "Enter Nickname: ";
                    getline(cin, nick);
                    nickName.push_back(nick);

                    obj[top].addBook(bookId, bookName, bookAuthor);

                    root_record[top++] = createBook(bookId, bookName, bookAuthor);

                } 
                else if (choice == 6) {
                    int id;
                    cout << "Enter Student ID: ";
                    cin >> id;

                    students.addStudent(id);

                } 
                else if(choice == 7){
                    students.print_all_Student();
                    cout << endl;
                }
                else if(choice == 8){
                    break;
                } 
                else {
                    if (top == 0) {
                        cout << "\tCreate a Tree First\n";
                        continue;
                    }
                    int choice2;
                    cout<<"\n";
                    for (int i = 0; i < top; i++) {
                        cout << i + 1 << ". " << nickName[i] << endl;
                    }
                    cout<<"\n";
                    cout << "Choose One Tree From that: ";
                    cin >> choice2;
                    choice2 -= 1;

                    if (choice2 < 0 || choice2 >= top) {
                        cout << "Invalid Tree Index\n";
                        continue;
                    }

                    if (choice == 2) {
                        int bookId;
                        string bookName, bookAuthor;
                        cout << "Enter Book ID: ";
                        cin >> bookId;
                        cout << "Enter Book Name: ";
                        cin.ignore();
                        getline(cin, bookName);
                        cout << "Enter Book Author: ";
                        getline(cin, bookAuthor);
                        obj[choice2].addBook(bookId, bookName, bookAuthor);
                        root_record[choice2] = insert(root_record[choice2], bookId, bookName, bookAuthor);

                    } else if (choice == 3) {
                        int bookId;
                        cout << "Enter Book ID: ";
                        cin >> bookId;
                        root_record[choice2] = search(root_record[choice2], bookId);
                        Book *temp_search = bst_search(root_record[choice2], bookId);

                        if (temp_search == nullptr) {
                            cout << "Book Not present here ..." << endl;
                        } else {
                            cout<<"\n";
                            cout << "Book ID: " << temp_search->bookId << endl;
                            cout << "Book Name: " << temp_search->bookName << endl;
                            cout << "Book Author: " << temp_search->bookAuthor << endl;
                            cout << endl;
                            Related_Book(temp_search->bookId, obj[choice2]);
                        }
                    } else if (choice == 4) {
                        preOrder(root_record[choice2]);
                        cout << endl;
                    } else if (choice == 5) {
                        print_all_book(obj[choice2]);
                    }
                }
            } while (choice < 8);
        } else if (person == 2) {
            do{
                cout << "\n1. Issue Book\n2. Issued Book\n3. Find Related Book\n4.Exit\nEnter your choice: ";
                cin >> choice;

                if (choice == 1) {
                    if (top == 0) {
                        cout << "No Book Available\n";
                        continue;
                    }
                    int choice2;
                    for (int i = 0; i < top; i++) {
                        cout << i + 1 << ". " << nickName[i] << endl;
                    }
                    cout << "Choose a subject from the list: ";
                    cin >> choice2;
                    choice2--;

                    if (choice2 < 0 || choice2 >= top) {
                        cout << "Invalid Subject Index\n";
                        continue;
                    }

                    int roll;
                    cout << "Enter Student Roll Number: ";
                    cin >> roll;
                    students.issue(roll, obj[choice2]);

                }else if (choice == 2) {
                    int roll;
                    cout << "Enter Student Roll Number: ";
                    cin >> roll;
                    students.issued_book(roll);
                }
                else if(choice == 3){
                    int roll;
                    cout << "Enter Student Roll Number : ";
                    cin >> roll;

                    if (top == 0) {
                        cout << "Create a Tree First\n";
                        continue;
                    }
                    int choice2;

                    for (int i = 0; i < top; i++) {
                        cout << i + 1 << ". " << nickName[i] << endl;
                    }
                    cout << "Choose One Tree From that: ";
                    cin >> choice2;
                    choice2 -= 1;

                    if (choice2 < 0 || choice2 >= top) {
                        cout << "Invalid Tree Index\n";
                        continue;
                    }

                    find_related_book(roll, obj[choice2]);

                }else{
                    break;
                }
            }while(choice < 4);
        }else if(person == 3){
            if (top == 0) {
                cout << "No Book Available\n";
                continue;
            }

            int roll;
            cout << "Enter Student Roll Number: ";
            cin >> roll;
            students.return_book(roll, obj, top);
        }else{
            break;
        }
    } while (person < 4);
    return 0;
}
