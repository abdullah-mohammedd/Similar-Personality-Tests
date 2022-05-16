// File: main.cpp
// Program - Personality Tests
// Author - Mohammed Abdullah

#include <set>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "driver.h"  //functions that help in the algorithm

using namespace std;

/* splitData
 * Function to split the factors/scores from the OCEAN characters
 * parameters : It takes in map vectors as reference and the string that holds the data
 * return : void
 */
void splitData(map<char, int> &factors, string attributes) {
    unsigned long int find, difference, i;
    int score;
    char ocean;
    // loop to go through line and split the factors and chars
    for (i = 0 ; i < attributes.length() ; i++) {
        find = attributes.find(" ", i);
        difference = find - i;
        string eachFactor = attributes.substr(i, difference);
        i = find;
        find = eachFactor.find(":");
        ocean = eachFactor.substr(0,find)[0];
        score = stoi(eachFactor.substr(find+1, eachFactor.length()));
        factors[ocean] = score;
        if (i == -1) break;
    }
}

/* readQuestions
 * Function to read questions from questions.txt file.txt file to a set
 * parameters : N/A
 * return : set of questions of type Question
 */
set <Question> readQuestions() {
    set <Question> questions;
    ifstream Data;
    Data.open("questions.txt");  // Declare an input stream for reading
    string details, questionText, attributes;
    unsigned long int period;
    map<char, int> factors;
    // loop to go through entire file
    while (Data.good()) {
        factors.clear();
        getline(Data, details, '\n');
        if (details != "") {
            period = details.find(".");
            questionText = details.substr(0, period+1);
            attributes = details.substr(period+2, details.length());
            splitData(factors, attributes);
        }
            questions.insert({questionText, factors});
    }
    return questions;
}

/* readPeople
 * Function to read people datafile from .people to a set
 * parameters : file name
 * return : set of people of tu[ype People
 */
// NOTE: the readQuestion and readFunction look same but return a different data type
set <Person> readPeople(string filename) {
    set <Person> people;
    ifstream Data;
    Data.open(filename);  // Declare an input stream for reading
    string details, questionText, attributes;
    unsigned long int period;
    map<char, int> scores;
    // loop to go through entire file
    while (Data.good()) {
        scores.clear();
        getline(Data, details, '\n');
        if (details != "") {
            period = details.find(".");
            questionText = details.substr(0, period);
            attributes = details.substr(period+2, details.length());
            splitData(scores, attributes);
        }
            people.insert({questionText, scores});
    }
    return people;
}

/* QuestionAnswer
 * Function to ask user some questions and return a OCEAN scores in a map
 * parameters : set of questions
 * return : returns OCEAN scores of the user
 */
map<char, int> QuestionAnswer(set <Question> questions) {
    int questionInput, counter = 0, answer;
    map<Question, int> answers;
    map<char, int> scores;
    Question randomQuestion;
    cout << "Welcome to the Personality Quiz!" << endl << endl
        << "Choose number of questions: ";
    cin >> questionInput;
    cout << endl;
    //loop to ask the user the number of questions they entered
    while (counter < questionInput) {
        randomQuestion = randomQuestionFrom(questions);
        cout << "How much do you agree with this statement?" << endl
            << '"' << randomQuestion.questionText << '"' << endl << endl;

        cout << "1. Strongly disagree" << endl
            << "2. Disagree" << endl
            << "3. Neutral" << endl
            << "4. Agree" << endl
            << "5. Strongly agree" << endl << endl
            << "Enter your answer here (1-5): ";
        cin >> answer;
        answers[randomQuestion] = answer;
        cout << endl;
        counter++;
    }
    scores = scoresFrom(answers);
    return scores;
}

/* whichPersonalityCompare
 * Function to ask user which file to compare and display
 * the personality of the person
 * parameters : set of People and users normalized scores
 * return : void
 */
void whichPersonalityCompare(set <Person> &people, map<char, int> scores) {
    int choice;
    // loop to compare the score of user with different .people file until they enter 0
    do {
        Person similarCeleb;
        string filename;
        cout << "1. BabyAnimals" << endl
            << "2. Brooklyn99" << endl
            << "3. Disney" << endl
            << "4. Hogwarts" << endl
            << "5. MyersBriggs" << endl
            << "6. SesameStreet" << endl
            << "7. StarWars" << endl
            << "8. Vegetables" << endl
            << "9. mine" << endl
            << "0. To end program." << endl << endl
            << "Choose test number (1-9, or 0 to end): ";
        cin >> choice;
        switch (choice) {
            case 0:
                cout << "Goodbye!" << endl;
                exit(0);
            case 1:
                filename = "BabyAnimals.people";
                break;
            case 2:
                filename = "Brooklyn99.people";
                break;
            case 3:
                filename = "Disney.people";
                break;
            case 4:
                filename = "Hogwarts.people";
                break;
            case 5:
                filename = "MyersBriggs.people";
                break;
            case 6:
                filename = "SesameStreet.people";
                break;
            case 7:
                filename = "StarWars.people";
                break;
            case 8:
                filename = "Vegetables.people";
                break;
            case 9:
                filename = "mine.people";
                break;
        }
        people = readPeople(filename);
        similarCeleb = mostSimilarTo(scores, people);
        cout << "You got " << similarCeleb.name << "!" << endl << endl;
    } while (choice != 0);
}


// PROGRAM EXECUTION START
int main() {
    set <Question> questions;
    set <Person> people;
    map<char, int> scores;
    questions = readQuestions();
    scores = QuestionAnswer(questions);
    whichPersonalityCompare(people, scores);
    return 0;
}
