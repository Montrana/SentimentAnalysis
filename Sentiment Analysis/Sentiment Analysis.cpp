// Sentiment Analysis.cpp : This program takes a review from a file as input, 
// changes it to be more positive or negative, then outputs the new review into a new file.
// Name: Montana Nicholson
// Date: 3/5/2023

#include "Sentiment.h"

/// <summary>
/// main has most of the function calls and macro program logic
/// </summary>
/// <returns>an int that isn't used</returns>
int main()
{
    // https://cplusplus.com/reference/map/map/
    map<string, double> negWords;
    map<string, double> allWords;
    map<string, double> posWords;

    ifstream inFileDic; //dictionary input file
    ifstream inFileRev; //review input file
    string inFileNameDic, inFileNameRev, outFileNameRev; // the string forms of the file names that we use
    double sentimentValTot;

    string continueInput; // holds user choice for if they want to run another review or not

    // inputing a list of words from a file into a map
    inFileNameDic = getFile("dictionary");
    readDictionary(inFileNameDic, allWords, posWords, negWords);

    do // larger loop of the majority of the program.
    { // If user wants to read another file, the loop will continue.
        // https://cplusplus.com/reference/queue/queue/
        queue<wordData> neutralReviewWords;
        queue<wordData> posReviewWords;
        queue<wordData> negReviewWords;

        inFileNameRev = getFile("review");

        sentimentValTot = readReview(inFileNameRev, neutralReviewWords, posReviewWords, negReviewWords, allWords);
        cout << "The original text is:\n";
        printReview(writeNewReview(posReviewWords, negReviewWords, neutralReviewWords));
        cout << "The original sentiment of this file is: " << sentimentValTot << endl << endl;

        string posOrNegInput;
        do // Loops until user gives a valid choice
        {
            cout << "What would you like to do?\n\n";
            cout << "P - Change from Negative to Positive\n";
            cout << "N - Change from Positive to Negative\n";
            cout << "Q - to Quit\n";
            cout << "Choice: ";
            cin >> posOrNegInput;
            posOrNegInput[0] = toupper(posOrNegInput[0]);
            
            if (posOrNegInput[0] == 'P')
            {
                cout << "Okay! I will edit file: " << inFileNameRev << " to be more positive!\n";
                cout << endl;
                sentimentValTot += changeQueue(negReviewWords, posWords);
            }
            else if (posOrNegInput[0] == 'N')
            {
                cout << "Okay! I will edit file: " << inFileNameRev << " to be more negative!\n";
                cout << endl;
                sentimentValTot += changeQueue(posReviewWords, negWords);
            }
            else if (posOrNegInput[0] == 'Q')
            {
                return 0;
            }
            else {
                cout << "Please enter a valid input.\n";
            }
        } while (posOrNegInput[0] != 'P' && posOrNegInput[0] != 'N');

        cout << endl;
        outFileNameRev = findOutputFile(inFileNameRev);
        printReview(outFileNameRev,
            writeNewReview(posReviewWords, negReviewWords, neutralReviewWords),
            sentimentValTot);
        cout << endl;
        cout << "The new output file will be stored in: " << outFileNameRev << endl;

        cout << endl;

        cout << "Would you like to go again? (Y or N): ";
        cin >> continueInput;
        continueInput[0] = toupper(continueInput[0]);

    } while (continueInput[0] == 'Y');
}