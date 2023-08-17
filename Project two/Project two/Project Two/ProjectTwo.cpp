#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Course {
public:
    string number;
    string title;
    vector<string> prereqs;

    Course(string num, string t, vector<string> p) : number(num), title(t), prereqs(p) {}
};

class TreeNode {
public:
    Course course;
    TreeNode* leftChild;
    TreeNode* rightChild;

    TreeNode(Course c) : course(c), leftChild(nullptr), rightChild(nullptr) {}
};

class CourseTree {
public:
    TreeNode* root;

    CourseTree() : root(nullptr) {}

    TreeNode* insertCourse(TreeNode* node, Course course) {
        if (node == nullptr) {
            node = new TreeNode(course);
        }
        else if (course.number < node->course.number) {
            node->leftChild = insertCourse(node->leftChild, course);
        }
        else {
            node->rightChild = insertCourse(node->rightChild, course);
        }
        return node;
    }

    void printCourseList(TreeNode* node) {
        if (node != nullptr) {
            printCourseList(node->leftChild);
            cout << node->course.number << ", " << node->course.title << endl;
            printCourseList(node->rightChild);
        }
    }

    TreeNode* findNode(TreeNode* node, string courseNum) {
        if (node == nullptr || node->course.number == courseNum) {
            return node;
        }
        else if (courseNum < node->course.number) {
            return findNode(node->leftChild, courseNum);
        }
        else {
            return findNode(node->rightChild, courseNum);
        }
    }
};

void readFile(const string& fileName, CourseTree& courseTree) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << fileName << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        vector<string> tokens;
        size_t start = 0, end;
        while ((end = line.find(',', start)) != string::npos) {
            tokens.push_back(line.substr(start, end - start));
            start = end + 1;
        }
        tokens.push_back(line.substr(start));

        if (tokens.size() < 2) {
            cout << "Error: Invalid data format in line: " << line << endl;
            continue;
        }

        string courseNum = tokens[0];
        string courseTitle = tokens[1];
        vector<string> prereqs(tokens.begin() + 2, tokens.end());

        Course course(courseNum, courseTitle, prereqs);
        courseTree.root = courseTree.insertCourse(courseTree.root, course);
    }

    file.close();
}

int main() {
    CourseTree courseTree;
    string fileName;
    bool dataLoaded = false;

    cout << "Enter the name of the data file: ";
    cin >> fileName;

    readFile(fileName, courseTree);
    dataLoaded = true;

    int choice;
    while (true) {
        cout << "1. Load Data Structure.\n"
            << "2. Print Course List.\n"
            << "3. Print Course.\n"
            << "4. Exit\n"
            << "What would you like to do? ";
        cin >> choice;

        if (choice == 1) {
            if (dataLoaded) {
                cout << "Data structure loaded successfully" << endl;
            }
        }
        else if (choice == 2) {
            cout << "Course List:" << endl;
            courseTree.printCourseList(courseTree.root);
        }
        else if (choice == 3) {
            string courseNum;
            cout << "What course do you want to know about? ";
            cin >> courseNum;
            TreeNode* node = courseTree.findNode(courseTree.root, courseNum);
            if (node != nullptr) {
                cout << node->course.number << ", " << node->course.title << endl;
                cout << "Prerequisites: ";
                for (const string& prereq : node->course.prereqs) {
                    cout << prereq << " ";
                }
                cout << endl;
            }
            else {
                cout << "Course not found." << endl;
            }
        }
        else if (choice == 4) {
            cout << "Thank you for using the course planner!" << endl;
            break;
        }
        else {
            cout << "Invalid option. Please choose a valid option." << endl;
        }
    }

    return 0;
}
