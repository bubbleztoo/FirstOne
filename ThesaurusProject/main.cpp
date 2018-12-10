#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include "library.hpp"

using namespace std;

struct LLstorage
{
  string word;
  heapNode top3[3];
  LLstorage* next;
};

void directThesaur(stringstream* sentence, hashLib* pass)
{
  if (sentence->str() == "")
  {
    cout << "Error, no sentence." << endl;
    return;
  }
  string words;
  hashNode* temp = NULL;
  while (*sentence >> words)
  {
    temp = pass->getHashNode(words); // will still work even with storing weirdness
    if (temp != NULL)
    {
      if (words[0] == ' ' || words[0] == '.' || words[0] == ',')
      {
        string temp1;
        temp1[0] = words[0];
        words = temp->synonyms[1].word; // get max heap
        temp1 += words;
        words = temp1;
      }
      else if (words[words.length()] == ' ')
      {
        words = temp->synonyms[1].word;
        words += " ";
      }
      else if (words[words.length()] == '.')
      {
        words = temp->synonyms[1].word;
        words += ".";
      }
      else if (words[words.length()] == ',')
      {
        words = temp->synonyms[1].word;
        words += ",";
      }
      else
      {
        words = temp->synonyms[1].word;
      }
    }
    if (words[0] == ' ')
    {
      cout << words.substr(1) << " ";
    }
    cout << words << " "; // adds new part of sentence every loop
  }
  return;
}

void indireThesaur(stringstream* sentence, hashLib* pass, heapNode top3[3])
{
  if (sentence->str() == "")
  {
    cout << "Error, no sentence." << endl;
    return;
  }
  string words;
  hashNode* temp = NULL;
  LLstorage* head = new LLstorage;
  LLstorage* temp1 = new LLstorage;
  while (*sentence >> words)
  {
    temp = pass->getHashNode(words);
    if (temp != NULL)
    {
      for (int i = 0; i < 3; i++)
      {
        top3[i] = temp->synonyms[i + 1];
      }
      if (top3[2].closeness < temp->synonyms[4].closeness || top3[2].closeness < temp->synonyms[5].closeness)
      {
        if (temp->synonyms[4].closeness >= temp->synonyms[5].closeness)
        {
          top3[2] = temp->synonyms[4];
        }
        else
        {
          top3[2] = temp->synonyms[5];
        }
      }
      if (top3[1].closeness < temp->synonyms[6].closeness || top3[1].closeness < temp->synonyms[7].closeness) // left and right (check child of both sides compared to other child)
      {
        if (temp->synonyms[6].closeness >= temp->synonyms[7].closeness)
        {
          top3[1] = temp->synonyms[6];
        }
        else
        {
          top3[1] = temp->synonyms[7];
        }
      }
      cout << temp->word << ":" << endl;
      cout << top3[0].word << " " << top3[1].word << " " << top3[2].word << endl;
      if (head->word == "")
      {
        head->word = temp->word;
        for (int b = 0; b < 3; b++)
        {
          head->top3[b] = top3[b];
        }
        head->next = NULL;
      }
      else if (head->next == NULL) // don't want to increment head
      {
        temp1->word = temp->word;
        for (int b = 0; b < 3; b++)
        {
          temp1->top3[b] = top3[b];
        }
        temp1->next = NULL;
        head->next = temp1;
      }
      else
      {
        LLstorage* temp2 = new LLstorage;
        temp2->word = temp->word;
        for(int q = 0; q < 3; q++)
        {
          temp2->top3[q] = top3[q];
        }
        temp2->next = NULL;
        temp1->next = temp2;
        temp1 = temp1->next;
      }
    }
  }
  string user;
  cout << endl;
  cout << "Would you like to provide feedback to improve advise?" << endl;
  while(true)
  {
    cin >> user;
    for (int i = 0; i < user.length(); i++) // essentially, to lower without including cctype
    {
      if (user[i] <= 90 || user[i] >= 65)
      {
        user[i] + 32;
      }
    }
    cout << endl;
    if (user == "y" || user == "yes" || user == "sure") // ASCII a = 97, z = 122. A = 65, Z = 90. Difference of 32
    {
      LLstorage* check = head;
      while (check != NULL)
      {
        string user2;
        hashNode* tempp = pass->getHashNode(check->word);
        cout << "Which synonym (1, 2, 3) was best for:" << endl;
        cout << check->word << endl;
        cout << "(1)" << check->top3[0].word << "     (2)" << check->top3[1].word << "      (3)" << check->top3[2].word << endl;
        cin >> user2;
        if (user2 == "1")
        {
          for (int i = 1; i < tempp->synonyms.size(); i++)
          {
            cout << temp->synonyms[i].word << endl;
            if (tempp->synonyms[i].word == check->top3[0].word)
            {
              tempp->synonyms[i].closeness++;
              tempp->repairUpward(i);
              break;
            }
          }
        }
        else if (user2 == "2")
        {
          for (int i = 1; i < tempp->synonyms.size(); i++)
          {
            if (tempp->synonyms[i].word == check->top3[1].word)
            {
              tempp->synonyms[i].closeness++;
              tempp->repairUpward(i);
              break;
            }
          }
        }
        else if (user2 == "3")
        {
          for (int i = 1; i < tempp->synonyms.size(); i++)
          {
            if (tempp->synonyms[i].word == check->top3[2].word)
            {
              tempp->synonyms[i].closeness++;
              tempp->repairUpward(i);
              break;
            }
          }
        }
        cout << endl;
        // else skip
        check = check->next;
      }
      break;
    }
    else if (user == "n" || user == "no" || user == "nope")
    {
      break;
    }
    // else don't do anything
  }
}

int main(int argc, char *argv[]) // hashSize, Thesaurus Lib file
{
  hashLib test(stoi(argv[1]));
  // test.addWord("Testing");
  // test.addWord("Testing");
  // test.addWord("Failing");
  // test.addWord("Running");
  // test.addWord("Dancing");
  // test.addWord("Reading");
  // test.addWord("Meading");
  // test.addWord("Deading");
  //test.printHashTable();

  ifstream in;
  in.open(argv[2]);
  if (in.fail())
  {
    cout << "Error reading file." << endl;
    in.close();
    return 0;
  }
  string worSyn; // word and synonyms
  while (getline(in, worSyn))
  {
    if (worSyn.empty() || worSyn == "https://justenglish.me/2014/04/18/synonyms-for-the-96-most-commonly-used-words-in-english/")
    {
      break;
    }
    // split word and synonyms
    string wor;
    stringstream syns;
    for (int i = 0; i < worSyn.length(); i++)
    {
      if (worSyn[i] == '-') // maybe "-"
      {
        wor = worSyn.substr(0, i - 1); // b/c space before i
        syns << worSyn.substr(i + 2); // want it to go to end, this is right?
        break;
      }
    }
    test.addWord(wor); // add the wor to Hash
    string onesyn;
    while (getline(syns, onesyn, ','))
    {
      // string::iterator a = remove(onesyn.begin(), onesyn.end(), ' ');
      // onesyn.erase(a, onesyn.end());
      cout << onesyn << endl;
      test.addWord(onesyn); // want to add these to Hash table as well to increase scope
      test.getHashNode(wor)->addSyn(onesyn, 1, 0); // start with all closeness values at 1, natural is TODO
    } // pass in synonyms to word, then pass synonyms to the synonyms' hashes
    for (int i = 1; i < test.getHashNode(wor)->synonyms.size(); i++) // adding all synonyms for every value in this vector
    {
      string syncopy = test.getHashNode(wor)->synonyms[i].word;
      for(int j = 1; j < test.getHashNode(wor)->synonyms.size(); j++) // go through again, skipping i
      {
        if (j == i)
        {
          test.getHashNode(syncopy)->addSyn(wor, 1, 0); // pass in wor at this location as a synonym instead of itself as one
        }
        else
        {
          test.getHashNode(syncopy)->addSyn(test.getHashNode(wor)->synonyms[j].word, 1, 0); // pass other values to it
        }
      }
    }
  }
  in.close();
  // test.printHashTable();
  // for (int i = 0; i < test.getHashNode("Amazing")->synonyms.size(); i++)
  // {
  //   cout << test.getHashNode("Amazing")->synonyms[i].word << endl;
  // }
  cout << "Success! Data Loaded!" << endl;

  // Take sentence, parse it if the word is in the hashtable return top 3 words from heap.
  // Return sentence, but with the words replaced by the max heap word
  while(true)
  {
    char ia;
    cout << "Would you like to (I)mprove a sentence, (G)et advise on one, or do you want to (Q)uit?" << endl;
    cin >> ia;
    cin.ignore();
    string temp;
    stringstream sentence;
    if (ia == 'I' || ia == 'i')
    {
      cout << "Please enter a sentence to be improved:" << endl;
      getline(cin, temp);
      sentence << temp;
      directThesaur(&sentence, &test);
      // cout << endl;
      //????
    }
    else if (ia == 'G' || ia == 'g')
    {
      cout << "Please enter a sentence to get advise on:" << endl;
      getline(cin, temp);
      sentence << temp;
      heapNode top3[3];
      indireThesaur(&sentence, &test, top3);
      // cout << endl;
      //????
    }
    else if (ia == 'Q' || ia == 'q')
    {
      cout << "Goodbye" << endl;
      break;
    }
    else
    {
      cout << "Error, invalid input." << endl;
    }
  }
}
