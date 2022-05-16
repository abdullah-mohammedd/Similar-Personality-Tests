// File: driver.h
// Author: Mohammed Abdullah
// Course: CS 251 Data Structures
// This file has the helper functions for the personality test application
// which asks the user a set of questions and it displays the closest similar person

#include <math.h>
#include <limits>
#include <string>
#include <map>
#include <set>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <typeinfo>
#include "myrandom.h"

using namespace std;

constexpr double lowest_double = std::numeric_limits<double>::lowest();

/* Type: Question
 *
 * Type representing a personality quiz question.
 */
struct Question {
    string questionText;  // Text of the question
    map<char, int> factors;   // Map from factors to +1 or -1
    friend bool operator< (const Question& lhs, const Question& rhs) {
        return lhs.questionText < rhs.questionText;
    }
    friend bool operator== (const Question& lhs, const Question& rhs) {
        return lhs.questionText == rhs.questionText;
    }
    friend bool operator!= (const Question& lhs, const Question& rhs) {
        return lhs.questionText != rhs.questionText;
    }
};

/* Type: Person
 *
 * Type representing a person, used to represent people when determining
 * who's the closest match to the user.
 */
struct Person {
    string name;      // Name of the person
    map<char, int> scores;  // Map from factors to +1 or -1
    friend bool operator< (const Person& lhs,   const Person& rhs) {
        return lhs.name < rhs.name;
    }
    friend bool operator== (const Person& lhs, const Person& rhs) {
        return lhs.name == rhs.name;
    }
    friend bool operator!= (const Person& lhs, const Person& rhs) {
        return lhs.name != rhs.name;
    }
};

/* randomElement
 *
 * This function selects, at random, a Question from the inputted questions set
 * and returns the question.  Note, this function does not remove the randomly
 * selected question from the set.
*/
Question randomElement(set<Question>& questions) {
    int ind = randomInteger(0, (int)questions.size()-1);
    int i = 0;
    for (auto e : questions) {
        if (i == ind) {
            return e;
        }
        i++;
    }
    return {};
}

/* randomQuestionFrom
 *
 * This function checks if the set is empty or otherwise call randomElement
 * to select arandom question and delete that question from the set
 * parameter : set of questions
 * return : a random question
*/
Question randomQuestionFrom(set<Question>& questions) {
    if (questions.empty() == true) {
        throw invalid_argument("set: empty");
    }  else {
        Question random = randomElement(questions);
        questions.erase(random);
        return random;
    }
}

/* scoresFrom
 *
 * This function calculates the scores of O C E A N characters and stores it in a map
 * parameter : map of question and their answers
 * return : map of ocean scores
*/
map<char, int> scoresFrom(map<Question, int>& answers) {
    map <char, int> scores;
    for (auto e : answers) {
        for (auto i : e.first.factors) {
            if (i.second != 0) {
                int score = scores[i.first];
                scores[i.first] = (((e.second - 3) * i.second) + score);
            }
        }
    }
    return scores;
}

/* normalize
 *
 * This function normalizes the score using algebric algorithm
 * parameter : map of unnormalized scores
 * return : map of normalized scores
*/
map<char, double> normalize(map<char, int>& scores) {
    map <char, double> normalizedScores;
    double ComputeVal = 0;
    int val;
    for (auto e : scores) {
        val = e.second * e.second;
        ComputeVal += val;
    }
    ComputeVal = sqrt(ComputeVal);
    if (ComputeVal <0.1) {
        throw invalid_argument("Invalid Calculation");
    } else {
        for (auto e : scores) {
            normalizedScores[e.first] = ((e.second * 1.0) / ComputeVal);
        }
    }
    return normalizedScores;
}

/* consineSimilarityOf
 *
 * This function takes 2 maps and calculates the similarity using cosine formula
 * parameter : 2 maps of normalized scores
 * return : a similarity constant betweenthem
*/
double cosineSimilarityOf(map<char, double>& lhs,
                          map<char, double>& rhs) {
    double similarity;
    for (auto e : lhs) {
        if (rhs.count(e.first) > 0) {
            similarity += (e.second * (rhs[e.first]);
        }
    }
    return similarity;
}

/* mostSimilarTo
 *
 * This function goes through the set of people to find the closest match to the user
 * parameter : maps of user scores and the set of people to compare
 * return : a similar person
*/
Person mostSimilarTo(map<char, int>& scores, set<Person>& people) {
    map<char, double> Playerscores, PeopleScore;
    Person similarCeleb;
    double highestSimilarity = -1, similarity;
    if (people.empty() == true) {
        throw invalid_argument("set: empty");
    } else {
        Playerscores = normalize(scores);
        for (auto e : people) {
            PeopleScore = normalize(e.scores);
            similarity = cosineSimilarityOf(PeopleScore, Playerscores);
            if (highestSimilarity < similarity) {
                highestSimilarity = similarity;
                similarCeleb = e;
            }
        }
    }
    return similarCeleb;
}
