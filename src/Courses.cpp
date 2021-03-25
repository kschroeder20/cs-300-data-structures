//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Kevin Schroeder
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <time.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <locale>
#include <functional>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// define a structure to hold course information
struct Course {
    string courseNumber; // unique identifier
    string courseTitle;
    vector<string> prerequisites;
    Course() { // default constructor
      courseNumber = "none";
      courseTitle = "none";
    }
};

// Internal structure for tree node
struct Node {
    Course course;
    Node *left;
    Node *right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with course
    Node(Course aCourse) {
        left = nullptr;
        right = nullptr;
        this->course = aCourse;
    }

};

//============================================================================
// Binary Search Tree class definition
//============================================================================
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);

  public:
    BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    Course Search(string courseNumber);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    //root is equal to nullptr
    root = nullptr;
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // call inOrder fuction and pass root
    this->inOrder(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    // if root equal to null ptr
    if (root == nullptr)
    {
        // root is equal to new node course
        root = new Node(course);
    }
    else {
        // add Node root and course
        this->addNode(root, course);
    }
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseNumber) {
    // set current node equal to root
    Node *current = root;

    // keep looping downwards until bottom reached or matching courseNumber found
    while (current != nullptr)
    {
        // make both strings lowercased for comparison
        string lowerUserCourseNum = courseNumber;
        string lowerCurrentCourseNum = current->course.courseNumber;        
        transform(lowerUserCourseNum.begin(), lowerUserCourseNum.end(), lowerUserCourseNum.begin(), ::tolower);
        transform(lowerCurrentCourseNum.begin(), lowerCurrentCourseNum.end(), lowerCurrentCourseNum.begin(), ::tolower);
        
        // return course if user-entered courseNumber is contained in current
        // course number
        if (lowerCurrentCourseNum.find(lowerUserCourseNum) != std::string::npos)
        {
            return current->course;
        }
        // if course is smaller than current node then traverse left
        if (lowerUserCourseNum.compare(lowerCurrentCourseNum) < 0)
        {
            current = current->left;
        }
        else {
            // else larger so traverse right
            current = current->right;
        }
    }
    // return empty course if not found
    Course course;
    return course;
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    // if node is larger then add to left
    if (node->course.courseNumber.compare(course.courseNumber) > 0) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = new Node(course);
        }
        else {
            // else recurse down the left node
            this->addNode(node->left, course);
        }
    }
    else {
        // if no right node
        if (node->right == nullptr)
        {
            // this node becomes right
            node->right = new Node(course);
        }
        else
        {
            // else recurse down the right node
            this->addNode(node->right, course);
        }
    }
}

/**
 * Orders course list recursively
 * @param node Current node in tree
 */
void BinarySearchTree::inOrder(Node* node) {
    //if node is not equal to null ptr
    if (node != nullptr)
    {
        //InOrder not left
        inOrder(node->left);
        //output courseNumber, title
        cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
        // if the course contains prerequisites
        if (node->course.prerequisites.size() > 0)
        {
            cout << " Prerequisites: ";
            // loop and print prerequisites
            for (vector<string>::iterator it = node->course.prerequisites.begin(); it != node->course.prerequisites.end(); ++it)
            {
                cout << *it << " ";
            }
        }
        cout << endl << endl;
        //InOder right
        inOrder(node->right);
      }
}

/**
 * Splits a string into a vector
 * @param s string
 * @param delimiter character to split string by
 */
vector<string> split(const string &s, char delimiter)
{
  vector<string> tokens;
  string token;
  istringstream tokenStream(s);
  while (getline(tokenStream, token, delimiter))
  {
    tokens.push_back(token);
  }
  return tokens;
}

/**
 * Load a TXT file containing courses into a container
 *
 * @return a container holding all the courses read
 */
void loadCourses(BinarySearchTree* bst) {
    string txtFileLineForCourseNums; // used to grab all course numbers
    string txtFileLine; // used to save courses
    int lineNum = 1; // used for error handling
    vector<string> allCourseNumbers; // used to save all course numbers

    // open a text file
    ifstream txtFileForCourseNums("project_one_sorting_pseudocode.txt");
    //read the file line by line
    while (getline(txtFileForCourseNums, txtFileLineForCourseNums))
    {
        // save the courseNumber to the allCourseNumbers
        vector<string> lineVector = split(txtFileLineForCourseNums, ',');
        allCourseNumbers.push_back(lineVector[0]);
    }
    // reopen text file
    ifstream txtFile("project_one_sorting_pseudocode.txt");
    //read the file line by line
    while (getline(txtFile, txtFileLine))
    {
        string errorMessage = "none"; // used for error handling
        // save each line to a vector
        vector<string> lineVector = split(txtFileLine, ',');
        // log an error if courseNumber or courseTitle is empty
        if (lineVector.size() < 2) {
            errorMessage = "Error on line: " + to_string(lineNum);
        }
        else {
            // save line to course
            Course course;
            course.courseNumber = lineVector[0];
            course.courseTitle = lineVector[1];
            // if the course has prerequisites
            if (lineVector.size() > 2)
            {
                // loop through prerequisites and save to prerequisites vector
                for (vector<string>::iterator it = lineVector.begin() + 2; it != lineVector.end(); ++it) {
                    if (find(allCourseNumbers.begin(), allCourseNumbers.end(), *it) != allCourseNumbers.end() + 1)
                    {
                        course.prerequisites.push_back(*it);
                    }
                }
            }
            // insert course into binary search tree
            // this will order the tree with the new addition
            bst->Insert(course);
        }
        // print out error if applicable
        if (errorMessage != "none") {
            cout << errorMessage << endl;
        }
        // used for error handling
        ++lineNum;        
    }
    cout << endl;
    cout << "Text file loaded" << endl;
    cout << endl;
}

/**
 * Display the course information to the console (std::out)
 *
 * @param course struct containing the course info
 */
void displayCourse(Course course)
{
    // display coursNumber and courseTitle
    cout << course.courseNumber << ", " << course.courseTitle << endl;
    // if the course has prerequisites
    if (course.prerequisites.size() > 0) {
        cout << " Prerequisites: ";
        // loop and print prerequisites vector
        for (vector<string>::iterator it = course.prerequisites.begin(); it != course.prerequisites.end(); ++it)
        {
            cout << *it << " ";
        }
    }
    cout << endl << endl;
    return;
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {
    // define a binary search tree to hold all bids
    BinarySearchTree* bst;
    // instantiate a Course
    Course course;
    // initialize boolean to check if bids have been loaded
    bool coursesLoaded = false;

    // display menu
    int choice = 0;
    while (choice != 4) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  4. Exit" << endl;
        // handle invalid input
        while (cout << "Enter choice: " && !(cin >> choice))
        {
            cin.clear(); //clear bad input flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); //discard input
            cout << endl;
            cout << "Invalid input; please re-enter.\n";
            cout << endl;
        }

        switch (choice) {

        case 1:
            // initialize binary search tree
            bst = new BinarySearchTree();

            // complete the method call to load the bids
            loadCourses(bst);

            // used for other choices
            coursesLoaded = true;

            break;

        case 2:
            // if courses are loaded
            if (coursesLoaded == true) {
                // display courses in order
                bst->InOrder();
            }
            // else
            else {
                // tell user to load courses
                cout << endl;
                cout << "Please load courses before displaying" << endl;
                cout << endl;
            }
            break;

        case 3:
            // if courses are loaded
            if (coursesLoaded == true)
            {
                string userCourseNumber;
                // gather user input on which course to search for
                cout << endl;
                cout << "What course do you want to know about?" << endl;
                cin >> userCourseNumber;
                
                // search for course
                course = bst->Search(userCourseNumber);

                // if a course is found
                if (course.courseNumber != "none")
                {
                    cout << endl;
                    // display course information
                    displayCourse(course);
                }
                // if a course is not found
                else
                {
                    // tell user that the course doesn't exist
                    cout << endl;
                    cout << "Course number " << userCourseNumber << " not found." << endl << endl;
                }
            }
            // else
            else
            {
                // tell user to load courses
                cout << endl;
                cout << "Please load courses before searching" << endl
                     << endl;
            }

            break;
        }
    }
    // goodbye message
    cout << endl;
    cout << "Thank you for using the course planner!" << endl;
    cout << endl;

    return 0;
}
