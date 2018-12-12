#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>
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
  bool gotValue = false;
  bool badValue = false;
  while (*sentence >> words)
  {
    temp = pass->getHashNode(words); // will still work even with storing weirdness
    if (temp != NULL)
    {
      gotValue = true;
      badValue = false;
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
    else
    {
      if (gotValue == false)
      {
        badValue = true;
      }
    }
    if (badValue)
    {
      cout << "No recognized word." << endl;
      cout << endl;
      return;
    }
    else if (words[0] == ' ')
    {
      cout << words.substr(1) << " ";
    }
    else
    {
      cout << words << " "; // adds new part of sentence every loop
    }
  }
  cout << endl;
  cout << endl;
  return;
}

void indireThesaur(stringstream* sentence, hashLib* pass, heapNode top3[3], heapNode* tip3[3])
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
  bool gotValue = false;
  bool badValue = false;
  while (*sentence >> words)
  {
    temp = pass->getHashNode(words);
    if (temp != NULL)
    {
      badValue = false; // reset this in case it got flipped
      gotValue = true; // set this so that we know it won't break
      for (int i = 0; i < 3; i++)
      {
        top3[i] = temp->synonyms[i + 1];
        tip3[i] = &(temp->synonyms[i + 1]);
      }
      if (top3[2].closeness < temp->synonyms[4].closeness || top3[2].closeness < temp->synonyms[5].closeness)
      {
        if (temp->synonyms[4].closeness >= temp->synonyms[5].closeness)
        {
          top3[2] = temp->synonyms[4];
          tip3[2] = &(temp->synonyms[4]);
        }
        else
        {
          top3[2] = temp->synonyms[5];
          tip3[2] = &(temp->synonyms[5]);
        }
      }
      if (top3[1].closeness < temp->synonyms[6].closeness || top3[1].closeness < temp->synonyms[7].closeness) // left and right (check child of both sides compared to other child)
      {
        if (temp->synonyms[6].closeness >= temp->synonyms[7].closeness)
        {
          top3[1] = temp->synonyms[6];
          tip3[1] = &(temp->synonyms[6]);
        }
        else
        {
          top3[1] = temp->synonyms[7];
          tip3[1] = &(temp->synonyms[7]);
        }
      }
      cout << temp->word << ":" << endl;
      cout << top3[0].word << "   " << top3[1].word << "   " << top3[2].word << endl;

      // Ok, this is all so that I can increment based on user input
      if (head->word == "")
      {
        head->word = temp->word; // set start of LL as element in hashtable
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
    else
    {
      if (gotValue == false)
      {
        badValue = true;
      }
    }
  }
  if (badValue == true) // that is, there has been no output and the program will break
  {
    cout << "No recognized word" << endl;
    cout << endl;
    return;
  }
  string user;
  cout << endl;
  cout << "Would you like to provide feedback to improve advise?" << endl;
  while(true)
  {
    cin >> user;
    for (int i = 0; i < user.length(); i++) // essentially, to lower without including cctype
    {
      user[i] = tolower(user[i]);
    }
    cout << endl;
    if (user == "y" || user == "yes" || user == "sure") // ASCII a = 97, z = 122. A = 65, Z = 90. Difference of 32
    {
      LLstorage* check = head;
      while (check != NULL)
      {
        string user2;
        hashNode* tempp = pass->getHashNode(check->word);
        // cout << tempp << endl;
        cout << "Which synonym (1, 2, 3) was best for:" << endl;
        cout << check->word << endl;
        cout << "(1)" << check->top3[0].word << "     (2)" << check->top3[1].word << "      (3)" << check->top3[2].word << endl;
        cin >> user2;
        if (user2 == "1")
        {
          for (int i = 1; i < tempp->synonyms.size(); i++)
          {
            // cout << temp->synonyms[i].word << endl;
            if (tempp->synonyms[i].word == check->top3[0].word)
            {
              tempp->synonyms[i].closeness++;
              // tip3[0]->closeness++;
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
              // tip3[1]->closeness++;
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
              // tip3[2]->closeness++;
              // cout << tempp->synonyms[i].closeness << endl;
              tempp->repairUpward(i);
              break;
            }
          }
        }
        cout << "Thank you." << endl;
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

  ifstream in;
  in.open(argv[2]);
  if (in.fail())
  {
    cout << "Error reading file." << endl;
    in.close();
    return 0;
  }
  string worSyn; // word and synonyms
  while (getline(in, worSyn, '\n'))
  {
    if (worSyn.empty() || worSyn == "https://justenglish.me/2014/04/18/synonyms-for-the-96-most-commonly-used-words-in-english/")
    {
      break;
    }
    //cout << worSyn << endl;
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
    string spacesyn;
    string onesyn;
    vector<string> sys;
    hashNode* hold = test.getHashNode(wor);
    while (getline(syns, onesyn, ','))
    {
      if (onesyn[0] == ' ')
      {
        if (onesyn.empty() == true)
        {
          break;
        }
        spacesyn = onesyn.substr(1);
      }
      else
      {
        spacesyn = onesyn;
      }
      // cout << spacesyn << endl;
      hashNode* synHasLoc = test.getHashNode(spacesyn);
      if (synHasLoc == NULL) // only want to add it once
      {
        test.addWord(spacesyn); // want to add these to Hash table as well to increase scope
      }
      hold->addSyn(spacesyn, 1, 0); // start with all closeness values at 1, natural is TODO
      sys.push_back(spacesyn);
    } // pass in synonyms to word, then pass synonyms to the synonyms' hashes
    for (int g = 0; g < wor.size(); g++)
    {
      wor[g] = tolower(wor[g]);
    }
    for (int i = 0; i < sys.size(); i++)
    {
      for (int j = 0; j < sys.size(); j++)
      {
        if (sys[i] == sys[j])
        {
          test.getHashNode(sys[i])->addSyn(wor, 1, 0); // pass wor to it instead instead of itself as a synonym of itself
        }
        else
        {
          test.getHashNode(sys[i])->addSyn(sys[j], 1, 0);
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
    cout << "Would you like to (I)mprove a sentence, (G)et advise on one, access the (L)ibrary, or do you want to (Q)uit?" << endl;
    cin >> ia;
    cin.ignore();
    cout << endl;
    string temp;
    stringstream sentence;
    if (ia == 'I' || ia == 'i')
    {
      cout << "Please enter a sentence to be improved:" << endl;
      getline(cin, temp);
      sentence << temp;
      cout << endl;
      directThesaur(&sentence, &test);
      // cout << endl;
      //????
    }
    else if (ia == 'G' || ia == 'g')
    {
      cout << "Please enter a sentence to get advise on:" << endl;
      getline(cin, temp);
      sentence << temp;
      cout << endl;
      heapNode top3[3];
      heapNode* tip3[3]; // backup, not really needed
      indireThesaur(&sentence, &test, top3, tip3);
      // cout << endl;
      //????
    }
    else if (ia == 'L' || ia == 'l')
    {
      cout << "Would you like to see (R)ecognized words, or (S)ynonyms for a specific word?" << endl;
      string ipt;
      while(true)
      {
        cin >> ipt;
        cout << endl;
        if (ipt == "R" || ipt == "r")
        {
          test.printHashTable();
          cout << endl;
          cout << "NOTE: EVERY WORD SHOWN HERE IS RECOGNIZED, THE ARROWS AND NUMBERS DON'T MEAN MUCH, THIS IS SIMPLY A VISUALIZATION OF THE DATA." << endl;
          cout << endl;
          break;
        }
        else if (ipt == "S" || ipt == "s")
        {
          cout << "What word would you like to see synonyms for?" << endl;
          string ip;
          cin.ignore();
          while (true)
          {
            getline(cin, ip);
            if (ip != "")
            {
              break;
            }
            else
            {
              ip = "";
            }
          }
          if (ip[ip.length() - 1] == ' ')
          {
            ipt = ip.substr(0, ip.length() - 1);
          }
          else
          {
            ipt = ip;
          }
          cout << endl;
          for (int i = 0; i < ipt.length(); i++)
          {
            ipt[i] = tolower(ipt[i]); // to lower
          }
          hashNode* pmet = test.getHashNode(ipt);
          if (pmet == NULL)
          {
            cout << "Could not find word." << endl;
            break;
          }
          cout << ipt << ":" << endl;
          for (int j = 1; j < pmet->synonyms.size() - 1; j++)
          {
            cout << pmet->synonyms[j].word << ", " << flush;
          }
          cout << pmet->synonyms[pmet->synonyms.size() - 1].word << flush;
          cout << endl;
          cout << endl;
          cout << "The earlier a synonym appears, the better it is." << endl;
          cout << endl;
          cout << "Would you like to give feedback on a synonym?" << endl;
          while (true)
          {
            cin >> ipt;
            cout << endl;
            if (ipt == "Y" || ipt == "y" || ipt == "yes" || ipt == "YES" || ipt == "Yes" || ipt == "yEs" || ipt == "yeS" || ipt == "YeS" || ipt == "yES" || ipt == "YEs" || ipt == "ye" || ipt == "Ye" || ipt == "yE")
            {
              cout << "What synonym would you like to give feedback to?" << endl;
              cin >> ipt;
              cout << endl;
              for (int u = 0; u < ipt.length(); u++)
              {
                ipt[u] = tolower(ipt[u]);
              }
              bool a = false;
              for (int k = 0; k < pmet->synonyms.size(); k++)
              {
                if (pmet->synonyms[k].word == ipt)
                {
                  a = true;
                  cout << "Is this a good or bad synonym?" << endl;
                  while (true)
                  {
                    cin >> ipt;
                    for (int h = 0; h < ipt.length(); h++)
                    {
                      ipt[h] = tolower(ipt[h]); // to lower
                    }
                    if (ipt == "good" || ipt == "g" || ipt == "goo" || ipt == "go")
                    {
                      pmet->synonyms[k].closeness++;
                      pmet->repairUpward(k);
                      break;
                    }
                    else if (ipt == "b" || ipt == "ba" || ipt == "bad")
                    {
                      if (pmet->synonyms[k].closeness == 0) // if implement file storage, make this 1
                      {
                        break;
                      }
                      pmet->synonyms[k].closeness--;
                      pmet->repairDownward(k);
                      break;
                    }
                  }
                }
              }
              if (a == false)
              {
                cout << "Could not find synonym" << endl;
              }
              cout << endl;
              cout << "Would you like to give feedback on another synonym?" << endl;
            }
            else if (ipt == "N" || ipt == "n" || ipt == "no" || ipt == "NO" || ipt == "No" || ipt == "nO")
            {
              break;
            }
          }
          break;
        }
      }
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
