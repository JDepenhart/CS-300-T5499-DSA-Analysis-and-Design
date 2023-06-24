/*
Name: Jeremy Depenhart
Assignment: Project 2
Description: Final Project for DSA: Analysis and Design 
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Defines a vector to store file information
struct Course {
	string courseNumber;
	string courseTitle;
	string prerequisites; 
};

// Used to display course information
void displayCource(Course course) {
	cout << course.courseNumber << " " << course.courseTitle << " " << course.prerequisites << endl;
	return;
}

// Reads file and loads information into Vector
vector<Course> loadCourses(string fileName) {
	ifstream fileStream(fileName); // opens file
	vector<Course> courses; // creates vector for courses
	string line;

	// while there are lines to read in file
	while (getline(fileStream, line)) {
		stringstream ss(line);

		Course course; // course item 

		getline(ss, course.courseNumber, ','); // Reads to 1st ',' sets courseNumber 
		getline(ss, course.courseTitle, ','); // Reads to 2nd ',' sets courseTitle
		getline(ss, course.prerequisites, ' '); // reads the rest of line and sets prerequisites

		// removes commas from prerequisites
		replace(course.prerequisites.begin(), course.prerequisites.end(), ',', ' ');

		// adds course object to vector
		courses.push_back(course);
	}

	fileStream.close(); // closes file
	return courses;
}

// Splits vector based on pivor, used for quick sort
int partition(vector<Course>& cources, int begin, int end) {

	//defines low and high int and sets them to begin and end
	int low = begin;
	int high = end;

	int midpoint = low + (high - low) / 2; // defines a midpoint integer
	Course pivot = cources[midpoint]; // sets Bid pivot as the midpoint index of bids

	bool done = false; // defines done bool as false 

	// While not done
	while (!done) {

		// increments low index while bids[low] < bids[pivot]
		while (cources[low].courseNumber < pivot.courseNumber) {
			low += 1;
		}

		// decrements high index while bids[pivot] < bids[high]
		while (pivot.courseNumber < cources[high].courseNumber) {
			high -= 1;
		}

		// If there are zero or one elements remaining
		if (low >= high) {
			done = true; // sets done to true and exits loop 
		}
		else {
			swap(cources[low], cources[high]); // swaps the low and high bids

			// increments low and deincrements high by 1
			low += 1;
			high -= 1;
		}
	}

	return high; // returns high index
}

// Sorts vector alphanumerically
void quickSort(vector<Course>& cources, int begin, int end) {
	int mid = 0; // defines mid integer as 0

	//Base case: If there are 1 or zero bids to sort
	if (begin >= end) {
		return; // returns, ends algorithm
	}

	// Sets mid to return of partition()
	mid = partition(cources, begin, end);

	// recursively sort low partition (begin to mid)
	quickSort(cources, begin, mid);

	// recursively sort high partition (mid+1 to end)
	quickSort(cources, mid + 1, end);
}

// Searches vector for a matching course number
void searchCources(vector<Course>& cources, string number) {

	Course currentCourse;

	// loops through course vector
	for (int i = 0; i < cources.size(); ++i) {
		currentCourse = cources[i];

		// if currentCourse number is the same as number
		if (currentCourse.courseNumber.compare(number) == 0) {

			// adds , back to Prerequisites
			replace(currentCourse.prerequisites.begin(), currentCourse.prerequisites.end(), ' ', ',');

			// Prints course info
			cout << currentCourse.courseNumber << ", " << currentCourse.courseTitle << endl;
			cout << "Prerequisites: " << currentCourse.prerequisites << endl;
		}
	}
}

// File Checking functions
#pragma region File Checking

// Used to check Prerequisites
int checkPrerequisite(string prerequisite, string fileName) {
	ifstream checkStream(fileName); // opens file

	stringstream ss(prerequisite); // reads prerequisite

	// Variables for line info
	string checkNumber, checkLine;
	string currentPrereq;
	string additionalPrereqs;

	getline(ss, currentPrereq, ','); // Reads prerequisit to ',' and sets currentPrereq
	getline(ss, additionalPrereqs, ' '); // Reads rest of prerequisit and sets additionalPrereqs

	bool prereqExists = false; // bool for while loop

	// While there are lines to read in file
	while (getline(checkStream, checkLine)) {
		stringstream newSS(checkLine);

		getline(newSS, checkNumber, ','); // Reads to 1st ',' sets number 

		// if number is the same as currentPrereq
		if (currentPrereq.compare(checkNumber) == 0) {
			prereqExists = true; // sets prereqExists to true
		}
	}

	// if prereqExists is never set to true
	if (!prereqExists) {
		// Gives error and name of prerequisite
		cout << "ERROR: " << currentPrereq << " Does not exist" << endl;
		cout << "Please Check That all prerequisites match a Course Number." << endl;
		cout << endl;
	}

	// if additional prerequisites exist
	if (!additionalPrereqs.empty()) {
		checkNumber.erase(); // earases checkNumber

		// recurses through this function again but with the additionalPrereqs variable
		checkPrerequisite(additionalPrereqs, fileName); 
	}

	prereqExists = false; 

	checkNumber.erase(); // earases checkNumber

	checkStream.close(); // closes file

	return 0;
}

// Used to check file for errors
int checkFile(string fileName) {
	ifstream fileStream(fileName); // opens file

	// If file fails to open
	if (!fileStream.is_open()) {
		cout << "File failed to open. Please double check file name" << endl;
		cout << fileName << " is not in project files, make sure you typed the file extension" << endl;
		cout << endl;
		return 0;
	}

	// Variables for line information
	string number, title, tempPrereq;
	string line;

	// While there are lines to read in file
	while (getline(fileStream, line)) {
		stringstream ss(line);

		getline(ss, number, ','); // Reads to 1st ',' sets number 
		getline(ss, title, ','); // Reads to 2nd ',' sets title 
		getline(ss, tempPrereq, ' '); // Reads the rest of line and sets tempPrereq

		// if number is empty
		if (number.empty()) {
			cout << "Error, an Item has no class number!" << endl; // print error
			cout << "Please fix file formatting." << endl;
			cout << endl;
		}

		// if title is empty 
		if (title.empty()) {
			cout << "Error, an Item has no class title!" << endl; // print error
			cout << "Please fix file formatting." << endl;
			cout << endl;
		}

		// if tempPrereq is NOT empty
		if (!tempPrereq.empty()) {
			// Call checkPrerequisite with tempPrereq and file name
			checkPrerequisite(tempPrereq, fileName); 
		}

		// erases line information for next loop
		number.erase(); 
		title.erase();
		tempPrereq.erase();
	}

	fileStream.close(); // closes file

	return 0;
}
#pragma endregion

int main() {
	vector<Course> courses;
	string courseNumber;
	string fileName;
	int choice = 0;

	// loops until 9 is entered
	while (choice != 9) {
		// Menu
		cout << "Welcome to the course planner." << endl;
		cout << endl;
		cout << "   1. Load Data Structure." << endl;
		cout << "   2. Print Course List." << endl;
		cout << "   3. Print Course." << endl;
		cout << "   9. Exit." << endl;
		cout << endl;
		cout << "What would you like to do? ";
		cin >> choice; // takes in choice
		cout << endl;

		// checks that input is a number
		if (cin.fail()) {
			cin.clear();
			cin.ignore(123, '\n');
		}
		else {
			// Switch case for choice
			switch (choice) {

			default: // if any number other than 1, 2, 3, 9 are entered 
				// prints error
				cout << "ERROR: Input invalid, please enter 1, 2, 3 ,9" << endl;
				break;

			case 1: // case 1 load data
				cout << endl << "Please enter File Name "; // prompts for fileName
				cin >> fileName; // gets fileName
				cout << endl;

				checkFile(fileName); // calls check file with fileName
				courses = loadCourses(fileName); // loads courses into vector after checking file
				break;

			case 2: // case 2 print schedual
				quickSort(courses, 0, courses.size() - 1); // calls quickSort function

				// loops through sorted vector
				for (int i = 0; i < courses.size(); ++i) {
					displayCource(courses[i]); // calls displayCource function
				}
				cout << endl;

				break;

			case 3: // case 3 Print Course
				cout << "What course do you want to know about? "; // prompts for courseNumber
				cin >> courseNumber; // gets courseNumber
				cout << endl;

				searchCources(courses, courseNumber); // calls searchCources function with courseNumber
				cout << endl;

				break;

			case 9: // exits loop
				break;
			}
		}
	}

	cout << "Thank you for using the course planner!" << endl;

	return 0;
}