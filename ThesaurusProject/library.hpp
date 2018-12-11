#ifndef HASH_LIBRARY_HPP
#define HASH_LIBRARY_HPP

#include <vector>
#include <string>
#include <cctype>

using namespace std;

// store all words of data into a weighted graph, so that edges can be added if synonym and weight can be changed based on user response
// might end up with seperate graphs, in this case can use another datastructure to point to the correct graph
//
// class graph
// {
// public:
//   void addNode(string word); // dynamically allocate new memeory for string, all have edge to root
//   int findNode(string word); // returns location of word from access array
//   void findSyn(int wordloc); // takes in array of 3 nodes, clears it and returns top 3 synonyms for this node
//   void addEdge_changeWeight(int weight, string a, string b); // should add connection between nodes a and b
// private:
//   string top3[3];
//   // nodes will be dynamically allocated connected to a nullptr at head
//   vector<vector<int>> edges;// adjacency int list, stores weights. if weight == 0 then no connection
//   vector<*string> acc; // TODO make into a hash table, stores pointers to dynamically allocated nodes from root
//   graph();
//   ~graph();
// };
// graph::graph()
// {
//   for (int i = 0; i < 3; i++)
//   {
//     top3[i] = "";
//   }
// }
// graph::~graph()
// {
//   for (int i = 0; i < 3; i++)
//   {
//     top3[i] = NULL;
//   }
//   for (int i = 0; i < edges.size(); i++)
//   {
//     edges[i].clear();
//   }
//   edges.clear();
//   for (int i = 0; i < acc.size(); i++)
//   {
//     delete acc[i];
//   }
//   acc.clear();
// }
// void graph::addNode(string word)
// {
//   string *temp = new string;
//   *temp = word; // dereference temp to assign data
//   edges.push_back(0); // add edge between root and this
//   int size = edges.size();
//   for(int i = 0; i < size - 1; i++) // adds a new row to all but last element
//   {
//     edges[i].push_back(0);
//   }
//   for(int i = 0; i < size; i++)
//   {
//     edges[size].push_back(0); // this should make the array n by n size
//   }
//   acc.push_back(temp); // this should make this a vector of addresses to new strings, add new node to access vector
//   return;
// }
// void graph::addEdge_changeWeight(int weight, string a, string b)
// {
//   int aloc = findNode(a);
//   int bloc = findNode(b);
//   if (aloc == -1 || bloc == -1)
//   {
//     break; // maybe add an error statement
//   }
//   edges[a][b] = weight;
//   edges[b][a] = weight;
//   // therefore there is a link between these two nodes, undirected or the link has been updated
//   return;
// }
// int graph::findNode(string word)
// {
//   for (int i = 0; i < acc.size(); i++) // go through ptr array to find node
//   {
//     if (*acc[i] == word)
//     {
//       return i; // return location
//     }
//   }
//   return -1; // node not found
// }
// void graph::findSyn(int wordloc)
// {
//   for (int i = 0; i < 3; i++)
//   {
//     top3[i] = NULL; // or empty string
//   }
//   // TODO pass HEIGHEST weights to top3
// }

struct heapNode // heap for data storage of "best" synonyms, MAXHEAP
{
  string word;
  int closeness; // how "good" of a synonym it is
  int natural; // how natural the word is in this sentence case, called in returning top words
};

struct hashNode
{
  hashNode()
  {
    this->LLnext = NULL;
    this->word = "";
  }
  hashNode* LLnext; // next in chaining hash linked list
  string word;
  vector<heapNode> synonyms; // heap of synonyms in vector (undetermined size) for this word
  // synonyms is declared public to allow for get top3 answers easier
  void addSyn(string wor, int closeness, int natural); // add to heap
  // to adjust heap for this node in hash
  void repairUpward(int nodeIndex);
  void increClose(int nodeIndex);
private:
  // void repairDownward(int nodeIndex); // I actually don't think we need this. Not deleting any nodes
  void swap(heapNode* swapper, heapNode* swappee); // NOTE: swap takes pointers, but synonyms is not a vector of pointers
};
void hashNode::increClose(int nodeIndex)
{
  if (nodeIndex < 1)
  {
    cout << "Error, invalid increment. User inpt. not stored." << endl;
    return;
  }
  else
  {
    synonyms[nodeIndex].closeness++;
    return;
  }
}
void hashNode::addSyn(string wor, int clo, int nat)
{
  if (synonyms.size() == 0)
  {
    heapNode none;
    none.word = "Placehodler";
    none.closeness = 0;
    none.natural = 0;
    synonyms.push_back(none);
  }
  for (int l = 0; l < wor.length(); l++)
  {
    wor[l] = tolower(wor[l]);
  }
  heapNode* temp = new heapNode;
  temp->word = wor;
  temp->closeness = clo;
  temp->natural = nat;
  synonyms.push_back(*temp); // dereference temp, because it is not a vector of pointers
  // while (index != 1 && heapArr[parent(index)] > heapArr[index])
  // {
  //    swap(&heapArr[index], &heapArr[parent(index)]);
  //
  //    index = parent(index);
  // }
  repairUpward(synonyms.size() - 1); // fix the heap
  return;
}
void hashNode::swap(heapNode* swapper, heapNode* swappee)
{
  heapNode* temp = new heapNode;
  temp->word = swappee->word;
  temp->closeness = swappee->closeness;
  temp->natural = swappee->natural;
  swappee->word = swapper->word;
  swappee->closeness = swapper->closeness;
  swappee->natural = swapper->natural;
  swapper->word = temp->word;
  swapper->closeness = temp->closeness;
  swapper->natural = temp->natural;
  delete temp;
  return;
}
void hashNode::repairUpward(int nodeIndex)
{
  if (nodeIndex < 1) // just to make sure I didn't screw anything up
  {
    cout << "How did this happen?" << endl;
    return;
  }

  if (nodeIndex == 1) // i.e. at start of heap
  {
    // cout << "At start of heap." << endl;
    // if (synonyms.size() < 5)
    // {
    //   return;
    // }
    // for (int i = 1; i <= 3; i++)
    // {
    //   cout << synonyms[i].word << endl;
    //   cout << synonyms[i].closeness << endl;
    // }
    return;
  }
  /**
  Alternatively:
  if (nodeIndex % 2 == 0) // left child
  {
    if (synonyms[nodeIndex].closeness > synonyms[nodeIndex / 2].closeness)
    {
      heapNode* temp1 = synonyms[nodeIndex];
      heapNode* temp2 = synonyms[nodeIndex / 2];
      swap(temp1, temp2);
    }
  }
  else if (nodeIndex % 2 == 1) // right child (can just be else)
  {
    if (synonyms[nodeIndex].closeness > synonyms[(nodeIndex - 1) / 2].closeness) // should swap
    {
      heapNode* temp1 = synonyms[nodeIndex];
      heapNode* temp2 = synonyms[(nodeIndex - 1) / 2];
      swap (temp1, temp2);
    }
  }
  **/
  if (nodeIndex > synonyms.size())
  {
    cout << "Error, no value to swap." << endl;
    return;
  }
  heapNode* temp1 = &synonyms[nodeIndex]; // child
  if (nodeIndex % 2 != 0)
  {
    nodeIndex = (nodeIndex - 1) / 2; // set nodeIndex to be parent nodeIndex
  }
  else
  {
    nodeIndex = nodeIndex / 2;
  }
  heapNode* temp2 = &synonyms[nodeIndex]; // parent
  // cout << temp2->word << temp1->word << endl;
  if (temp1->closeness > temp2->closeness) // max heap
  {
    swap(temp1, temp2);
    // if (synonyms.size() >= 4)
    // {
    //   cout << synonyms[1].word;
    //   cout << synonyms[2].word;
    //   cout << synonyms[3].word << endl;
    //   cout << synonyms[1].closeness << endl;
    //   cout << synonyms[2].closeness << endl;
    //   cout << synonyms[3].closeness << endl;
    // }
    repairUpward(nodeIndex); // since nodeIndex is now set as the node we (possibly) swapped to, should run again at correct location
  }
  // if (synonyms.size() >= 4)
  // {
  //   cout << synonyms[1].word;
  //   cout << synonyms[2].word;
  //   cout << synonyms[3].word << endl;
  //   cout << synonyms[1].closeness << endl;
  //   cout << synonyms[2].closeness << endl;
  //   cout << synonyms[3].closeness << endl;
  // }
  return;
}

// void hashNode::repairUpward(int nodeIndex)
// {
//   int l = leftChild(i);
//   int r = rightChild(i);
//
//   // Finds the smallest of node, left Child, and right child
//  // The parent is swapped with the smallest of all 3
//   int smallest = i;
//   if (l <= currentSize && heapArr[l] < heapArr[i])
//       smallest = l;
//   if (r <= currentSize && heapArr[r] < heapArr[smallest])
//       smallest = r;
//
//   // Similarly do this till the leaf
//   if (smallest != i)
//   {
//       swap(&heapArr[i], &heapArr[smallest]);
//       repairUpward(smallest);
//   }
// }

class hashLib
{
public:
  hashLib(int size);
  ~hashLib();
  void addWord(string wor); // do some file i/o using this to create a hashtable from text file
  // remove node functionality?
  hashNode* getHashNode(string wor); // return pointer to location of word/hashNode in the hash table
  // possibly a file i/o statement to write to file?
  void printHashTable()
  {
    for (int i = 0; i < hashSize; i++)
    {
      cout << i << ":\t" << flush;
      if (list[i].word != "")
      {
        cout << list[i].word << flush;
        hashNode* temp = list[i].LLnext;
        while(temp != NULL)
        {
          cout << " -> " << temp->word << flush;
          temp = temp->LLnext;
        }
      }
      cout << endl;
    }
  }
private:
  int hash(string wor);
  hashNode* list; // pointer to first element in hash table list
  int hashSize;
};
hashLib::hashLib(int size)
{
  this->list = new hashNode[size];
  for (int i = 0; i < size; i++)
  {
    this->list[i].LLnext = NULL;
    this->list[i].word = "";
  }
  this->hashSize = size;
}
hashLib::~hashLib()
{
  delete[] this->list;
  this->hashSize = 0;
}
void hashLib::addWord(string wor)
{
  if (getHashNode(wor) != NULL)
  {
    // cout << "Word already in library" << endl;
    return;
  }
  for (int l = 0; l < wor.length(); l++)
  {
    wor[l] = tolower(wor[l]);
  }
  int loc = hash(wor);
  if (list[loc].word != "") // if node exists at this location in hashtable
  {
    hashNode* temp = list[loc].LLnext;
    hashNode* prev = &list[loc];
    while(temp != NULL) // go until end of chain
    {
      prev = temp;
      temp = temp->LLnext;
    }
    temp = new hashNode;
    temp->word = wor;
    temp->LLnext = NULL;
    prev->LLnext = temp;
    return;
  }
  else
  {
    list[loc].word = wor;
    // list[loc].LLnext = NULL; Don't need b/c constructor already sets LLnext = NULL
    return;
  }
}
hashNode* hashLib::getHashNode(string wor)
{
  string wo;
  string wah;
  if (wor[wor.length() - 1] == '.' || wor[wor.length() - 1] == ',' || wor[wor.length() - 1] == '?' || wor[wor.length() - 1] == '!')
  {
    wah = wor.substr(0, wor.length() - 1);
    // cout << wo << endl;
  }
  else
  {
    wah = wor;
  }
  if (wah[0] == ' ')
  {
    wo = wah.substr(1, wah.length());
  }
  else
  {
    wo = wah;
  }
  for (int l = 0; l < wo.length(); l++)
  {
    wo[l] = tolower(wo[l]);
  }
  // cout << wo << endl;
  // here too
  int loc = hash(wo);
  hashNode* temp = NULL;
  if (list[loc].word == wo)
  {
    temp = &list[loc];
    return temp;
  }
  else
  {
    temp = list[loc].LLnext;
    while (temp != NULL && temp->word != wo)
    {
      temp = temp->LLnext;
    }
    if (temp == NULL)
    {
      // cout << "Word not in library." << endl;
      return NULL;
    }
    return temp;
  }
}
int hashLib::hash(string wor)
{
  string wo;
  if (wor[0] == ' ')
  {
    wo = wor.substr(1, wor.length());
  }
  else
  {
    wo = wor;
  }
  for (int l = 0; l < wo.length(); l++)
  {
    wo[l] = tolower(wo[l]);
  }
  // Ok, it breaks here...
  // cout << wo << endl;
  int ret = 3784;
  for (int i = 0; i < wo.length(); i++)
  {
    ret = ret + wo[i];
  }
  // if (ret < 0)
  // {
  //   ret = 0 - ret; // right? this should return same if it overflows
  // }
  ret = ret % this->hashSize; // I don't need to use this-> do I?
  return ret;
}

// When you add word/add syn you add it to the other place too
// The advantage of using a graph is I wouldn't need to add it for every heap, the connections would already be made
// I mean have a synonym of a synonym already be connected by higher weight, so practiacally, it dosen't matter

// ignore top 100 most common words, this will happen to be a, an, i, and so on

// MAKE A LEARNING FUNCTION WHERE YOU RETURN THE BEST ONE back to it AND IT'LL do ++ to that closeness var
// THEN MAKE A THESAURIZE FULLY FUNCTION WHERERIN IT JUST REPLACES IT WITH TOP FROM HEAP

#endif
