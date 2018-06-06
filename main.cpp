#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

class Mod
{
private:

        // CLASS VARIABLES
        string modTitle;
        string term;
        string definition;
        string filepath;
        vector<string> terms;
        vector<string> definitions;
        int enumTerms = 0;
        int termsSize;

public:

        // CONSTRUCTORS
        Mod ( string modNum, string modDescription )
        {
            this->modTitle = "Mod " + modNum + " - " + modDescription;
            this->filepath = this->modTitle + ".txt";
        }

        // METHODS
        string GetName( void )
        {
            return( this->modTitle );
        }

        void PrintBanner ( bool currentTerms )
        {
            system("cls");

            cout << "               STUDY JCAC\n";
            cout << "******************************************\n";
            cout << "    " << this->modTitle << endl;
            cout << "******************************************\n";

            if ( currentTerms )
            {
                //int termsSize = this->terms.size();
                cout << "CURRENT TERMS:\n";
                for ( int i = 0; i < this->termsSize; i++ )
                {
                    cout << i + 1 << ") " << this->terms[i] << endl;
                }
            }
        }

        void ReadTerms( void ) // Read terms from a text file and add to vectors
        {
            ifstream infile( this->filepath.c_str());
            string readTerm;
            string readDefinition;
            this->terms.clear();
            this->definitions.clear();

            while( infile.eof() == false )
            {
                getline(infile, readTerm);
                if ( readTerm == "" )
                {
                    continue;
                }
                getline(infile, readDefinition);
                if ( readDefinition == "" )
                {
                    continue;
                }

                this->terms.push_back( readTerm );
                this->definitions.push_back ( readDefinition );
            }
            infile.clear();
            infile.close();
            this->termsSize = this->terms.size();
        }

        void AddTerms( void )
        {
            string writeTerm;
            string writeDefinition;
            string write;
            ofstream outfile( this->filepath.c_str(), ios::app );

            while ( true )
            {
                int termsSize = this->terms.size();

                this->PrintBanner( 1 );

                cout << "\nADD TERMS (Enter 0 to stop)\n";
                cout << "Enter term: ";
                ws(cin);
                getline( cin, writeTerm );
                if ( writeTerm == "0")
                {
                    break;
                }
                this->terms.push_back( writeTerm );
                write = writeTerm + "\n";
                outfile << write;
                cout << "Enter definition: ";
                getline ( cin, writeDefinition );
                this->definitions.push_back(writeDefinition);
                write = writeDefinition + "\n";
                outfile << write;
                outfile.close();
            }
        }

        void Study( void )
        {
            char opt;

            ReadTerms(); // Add all terms from txt file to terms and definitions vectors

            while ( true )
            {
                int termsSize = this->terms.size();

                PrintBanner( 1 );

                cout << "\n(S)tudy terms | (A)dd terms | (R)emove terms | (Q)uit\n"; // HOW TO REMOVE TERMS?
                cout << "> ";
                cin >> opt;
                if ( opt == 's' || opt == 'S' )
                {
                    break;
                }
                else if ( opt == 'a' || opt == 'A' )
                {
                    this->AddTerms();
                }
                else if ( opt == 'q' || opt == 'Q' )
                {
                    break;
                }
            }


            for ( int i = 0; i < termsSize; i++ ) // Uses VectorSize variable to save CPU from having to call size() method each loop
            {
                this->PrintBanner( 0 );

                cout << "TERM " << i + 1 << " OF " << termsSize << "\n\n";
                cout << this->terms[i] + ":\n" + this->definitions[i]; // Outputs formatted term and definition
                cout << "\n\n(P)revious | (N)ext\n";
                cout << "> ";
                cin >> opt;
            }
        }

        void Quiz() // random number gen, picks random definition, must type matched term, does not repeat questions, tracks number wrong, gives answers to incorrect questions
        {
            srand( time(NULL) );

            char opt;
            string answer;
            string wrong;

            while ( true )
            {
                vector<string> wrongAnswers;
                vector<string> wrongTerms;

                ReadTerms(); // Add all terms from txt file to terms and definitions vectors
                int termsSize = this->terms.size();

                this->PrintBanner( 1 );

                cout << "\n(R)eady to quiz? | (Q)uit\n";
                cout << "> ";
                cin >> opt;
                if ( opt == 'r' || opt == 'R' )
                {
                    for ( int x = 0; x < termsSize; x++ )
                    {
                        int definitionsSize = this->definitions.size();

                        unsigned int randomIndex = rand() % definitionsSize;

                        this->PrintBanner( 0 );

                        cout << "QUESTION " << x + 1 << " OF " << termsSize << "\n\n";
                        cout << this->definitions[randomIndex]; // Outputs definition without term

                        while ( true )
                        {
                            cout << "\n\nEnter the term: ";
                            ws( cin );
                            getline ( cin, answer );
                            cout << answer << endl;
                            cout << terms[randomIndex] << endl;
                            if ( answer == this->terms[randomIndex] )
                            {
                                cout << "Correct! (C)ontinue? > ";
                                cin >> opt;
                                if ( opt == 'c' || opt == 'C' )
                                {
                                    break;
                                }
                            }
                            else
                            {
                                wrong = this->terms[randomIndex] + ":\n" + this->definitions[randomIndex];
                                wrongTerms.push_back( wrong );
                                wrongAnswers.push_back( answer );
                                cout << "Incorrect! (C)ontinue? > ";
                                cin >> opt;
                                if ( opt == 'c' || opt == 'C' )
                                {
                                    break;
                                }
                            }

                        }
                        definitions.erase(definitions.begin() + randomIndex);
                        terms.erase(terms.begin() + randomIndex);
                    }

                    int numWrong = wrongAnswers.size();
                    for ( int w = 0; w < numWrong; w++ )
                    {
                        PrintBanner( 0 );
                        cout << "INCORRECT TERM " << w + 1 << " OF " << numWrong << "\n\n";
                        cout << wrongTerms[w] << endl; // Outputs definition and term for review
                        cout << "\nYou answered: " << wrongAnswers[w] << endl;
                        cout << "\n(C)ontinue? > ";
                        cin >> opt;
                    }
                }
            }
        }
};

// FUNCTION PROTOTYPES
void PopulateMods( vector<Mod>& mods ); // Calls GetName() method of all mods in vector to create a menu

int main( void )
{
    int selection; //
    vector<Mod> mods; // Declares a vector to hold all Mod objects

    Mod mod3 ("3", "Programming Fundamentals"); // Creates Mod object and assigns mod number and description
    mods.push_back( mod3 ); // Adds mod3 object to mods vector

    // MAIN MENU
    while ( true )
    {
        system("cls"); // Change to <system("clear")> for linux systems

        cout << "               STUDY JCAC\n";
        cout << "******************************************\n";
        cout << "              Current Mods\n";
        cout << "******************************************\n";
        PopulateMods( mods );
        cout << "0) Quit\n\n";
        cout << "Select a mod: ";
        cin >> selection;
        if ( selection == 0 )
        {
            return(0);
        }
        else if ( selection > 0 )
        {
            selection -= 1; // Decrements to match zero based index of mods vector
        }
        else
        {
            continue; // Restarts the menu loop if a selection is invalid
        }

        // SUB MENU FOR INDIVIDUAL MOD
        while ( true )
        {
            char option;

            mods[selection].PrintBanner( 0 );
            cout << "1) Study\n";
            cout << "2) Quiz\n";
            cout << "0) Exit\n\n";
            cout << "> ";
            cin >> option;

            if ( option == '1' )
            {
                mods[selection].Study();
            }
            else if ( option == '2' )
            {
                mods[selection].Quiz();
            }
            else if ( option == '0' )
            {
                break; // Return to main menu
            }
            else
            {
                continue;
            }
        }
    }
}

void PopulateMods( vector<Mod>& mods ) // '&' character is used to reference mods vector created in main function
{
    int vectorSize = mods.size();

    for ( int i = 0; i < vectorSize; i++ ) // Uses VectorSize variable to save CPU from having to call size() method each loop
    {
        cout << i + 1 << ") " << mods[i].GetName() << endl;
    }
}
