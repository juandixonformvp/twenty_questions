#include "chang_proj.h"
//#include "chang_proj_class.h"

double BinToDec(string pattern) {

	int arraySize = size(pattern);
	int* patternPtr = NULL;
	patternPtr = new int[arraySize];

	char str[30];
	strcpy_s(str, pattern.c_str());

	int i = 0;
	for (int *p = patternPtr; p < patternPtr + arraySize; p++) {
		*p = str[i] - '0';
		i++;
	}

	double intOut = 0;
	i = 0;
	for (int *p = patternPtr; p < patternPtr + arraySize; p++) {
		intOut += *p * pow(2.0, arraySize - 1 - i);
		i++;
	}
	return intOut;
}

class Element {
public:
	string all, animal, pattern;
	Element(string _all = " ", string _animal = " ", string _pattern = "111");       // constructor prototype
	double myId;
};

Element::Element(string _all, string _animal, string _pattern) {
	all = _all;
	animal = _animal;
	pattern = _pattern;
}

class Question {
public:
	string theQuestion;
	Question(string _theQuestion = " ");       // constructor prototype
};

Question::Question(string _theQuestion) {
	theQuestion = _theQuestion;
}

void comparePattern(Element *arr, Question *qArr, int userAnimal, string finalPattern, int numQuestions) {
	arr = arr + (userAnimal - 1);
	string animalPatt = '1' + arr->pattern;
	string newfinalPattern = finalPattern.substr(0, finalPattern.size() - (finalPattern.size() - animalPatt.size()));
	//cout << animalPatt << endl << newfinalPattern << endl;
	//for (int i = 0; i < newfinalPattern.size(); i++) {
	int i = 1;
	for (Question *q = qArr; q < qArr + numQuestions; q++) {
		if (i < newfinalPattern.size()) {
			if (newfinalPattern[i] != animalPatt[i]) {
				cout << endl << "Check this question again, are you sure about your response? " << endl << q->theQuestion << endl;
			}
		}
		i++;
	}
	cout << endl;
}

void display(const Element & e, int numAnimal)
{
	cout << numAnimal << ") " << e.animal << endl;
}

int displayPtr(Element *arr, int numItems)
{
	int numAnimal = 0;
	for (Element *p = arr; p < arr + numItems; p++) {
		numAnimal++;
		display(*p, numAnimal);
	}
	int userInput = 0;
	cin >> userInput;
	return userInput;
}

bool displayQuestion(const Question & q)
{
	cout << endl;
	cout << q.theQuestion << endl;
	char userInput;
	bool isTrue;

	while (true) {
		cout << endl << "Make a selection (y/n): " << endl;
		cin >> userInput;
		if ((userInput == 'y') || (userInput == 'n')) {
			break;
		}
	}

	(userInput == 'y') ? isTrue = true: isTrue = false;
	return isTrue;
}

string displayQuestionPtr(Question *arr, Element *animalArray, int numQuestions, int numItems)
{
	string user_pattern = "1";
	char userInput = 'n';
	//int endValue = 0;
	for (Question *p = arr; p < arr + numQuestions; p++) {
		
		int isTrue = displayQuestion(*p);
		user_pattern = user_pattern + to_string(isTrue);
		double idSoFar = BinToDec(user_pattern);
		//cout << user_pattern << endl << idSoFar;

		for (Element *aa = animalArray; aa < animalArray + numItems; aa++) {
			if (idSoFar == aa->myId) {
				while (true) {
					cout << endl << "Is it a " << aa->animal << "? (y/n)" << endl;
					cin >> userInput;
					if ((userInput == 'y') || (userInput == 'n')) {
						break;
					}
				}
			}
			
		}
		if (userInput == 'y') {
			user_pattern = "999";
			break;
		}
	}	
	return user_pattern;
}

void parseAnimal(Element *arr, int numItems) {
	for (Element *p = arr; p < arr + numItems; p++) {
		string str = p->all;
		stringstream ss(str);
		getline(ss, p->animal, '\n');
		getline(ss, p->pattern, '\n');
		//the unique ID for each animal is the decimal representation of the binary pattern
		//however because of leading zeros, I need to add a leading 1 to the binary pattern to get a truly unique decimal ID
		p->myId = BinToDec(('1'+p->pattern));
	}
}

int main() {
	//Reads in Questions file
	ifstream inFile;
	inFile.open("questions.txt");
	if (inFile.fail())
		cout << "Unable to open files";

	int numQuestions = 0; 
	while (!inFile.eof()) //add items to the vector until you reach eof
	{
		string line;
		getline(inFile, line);
		numQuestions++; //counts the number of items in the file
	}
	inFile.close();

	Question* questionPtr = NULL;   // Pointer to element object, initialized to NULL
	questionPtr = new Question[numQuestions];	// Dynamically sets the array size to number of animals in the file

	inFile.open("questions.txt");

	while (!inFile.eof()) //add items to the vector until you reach eof
	{
		string line;
		getline(inFile, line);
		Question myQuestion(line);
		*questionPtr = myQuestion; //uses pointers to add items to the array
		questionPtr++;
	}
	inFile.close();
	questionPtr = questionPtr - numQuestions; //resets the pointer back to the beginning of the memory block

	//Reads in Answers file
	ifstream inFile2;
	inFile2.open("animals.txt");
	if (inFile2.fail())
		cout << "Unable to open files";

	//I delimit my separate animals with a '*', beginning with the 2nd animal.
	int numItems = 1; //set to 1 to make sure you get the first item which doesn't have a '*'
	while (!inFile2.eof()) //add items to the vector until you reach eof
	{
		string line;
		getline(inFile2, line);
		if (line[0] == '*') {
			numItems++; //counts the number of items in the file
		}
	}
	inFile2.close();

	Element* arrayPtr = NULL;   // Pointer to element object, initialized to NULL
	arrayPtr = new Element[numItems];	// Dynamically sets the array size to number of animals in the file

	inFile2.open("animals.txt");

	while (!inFile2.eof()) //add items to the vector until you reach eof
	{
		string line;
		getline(inFile2, line, '*');
		Element myElement(line);
		*arrayPtr = myElement; //uses pointers to add items to the array
		arrayPtr++;
	}
	inFile2.close();
	arrayPtr = arrayPtr - numItems; //resets the pointer back to the beginning of the memory block
	parseAnimal(arrayPtr, numItems);
	string finalPattern = displayQuestionPtr(questionPtr, arrayPtr, numQuestions, numItems);
	int endValue = 0;
	if (finalPattern == "999") {
		endValue = 1;
	}
	int userAnimal = 0;
	switch (endValue) {
	case 0:
		cout << endl << "Hmmm.. we couldn't guess your animal. Please pick your animal from this list.\n" << endl;
		userAnimal = displayPtr(arrayPtr, numItems);
		comparePattern(arrayPtr, questionPtr, userAnimal, finalPattern, numQuestions);
		break;
	case 1:
		cout << endl << "We guessed it! Thanks for playing!\n" << endl;
		break;
	default:
		cout << "Invalid" << endl;
	}

	return 0;
}