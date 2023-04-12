#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip> // include this header for setw()

using namespace std;

const int HUNDRED_ANTS = 100;
const int FIVE_DOODLEBUGS = 5;
const int row_length = 20 , col_length = 20;
class Organism
{
private:
  // child class can access this property through getters only
  int moves_taken;

public:
  virtual ~Organism() = default;
  virtual string getIdentity() const = 0;
  // take_move will call incrementMove,
  // we will handle the moving logic on the board instead.
  virtual bool breedable() const = 0; // for ants , check moves taken moves_taken % 3,
  virtual int getDaysStarved() = 0;
  virtual void take_move() = 0;
  void incrementMovesTaken();
  virtual bool starved_to_death() = 0;
  virtual void setDayStarved() = 0;
  int getMovesTaken() const { return this->moves_taken; }
  Organism() : moves_taken(0) {}
};
void Organism::incrementMovesTaken() {
  this->moves_taken = moves_taken+=1;
}
class Ant : public Organism
{
public:
  Ant() : Organism(){};
  string getIdentity() const override
  {
    return "Ants";
  }
  virtual ~Ant() = default;
  void take_move() override;
  bool breedable() const override;
  int getDaysStarved() override;
  void setDayStarved() override;
  bool starved_to_death() override;
};
// randomly move up down left right
void Ant::take_move()
{
  // access member function use ->, access member variable use .
  this->incrementMovesTaken();
}

bool Ant::breedable() const
{
  return (this->getMovesTaken() % 3) == 0;
}
void Ant::setDayStarved() {
  cout << "Ants won't be starved";
}
int Ant::getDaysStarved()
{
  return 0;
}

bool Ant::starved_to_death() {
  return false;
}

class Doodlebug : public Organism
{
private:
  int days_starved;

public:
  Doodlebug() : Organism(), days_starved(0) {}
  string getIdentity() const override
  {
    return "Doodlebug";
  }
  virtual ~Doodlebug() = default;
  void take_move() override;
  bool breedable() const override;
  int getDaysStarved() override;
  void setDayStarved() override;
  void incrementDayStarved();
  bool starved_to_death() override;
};
// only when it eats, we reset the doodlebug's day_starved  = 0;
void Doodlebug::setDayStarved() {
  this->days_starved = 0;
}
void Doodlebug::incrementDayStarved() {
  this->days_starved = this->days_starved + 1;
}
void Doodlebug::take_move() {
  this->incrementMovesTaken();
  this->incrementDayStarved();
}
// survive 8 days
bool Doodlebug::breedable() const
{
  return (this->getMovesTaken() % 8) == 0;
}

int Doodlebug::getDaysStarved()
{
  return this->days_starved;
}
bool Doodlebug::starved_to_death()
{
  return (this->getDaysStarved() % 8) == 0;
}

void fill_organism(vector<vector<Organism *>> &organism, int rowLength, int colLength)
{
  // create 100 ants
  for (int ants_num = 0; ants_num < HUNDRED_ANTS; ants_num++)
  {
    // pointers assigned to new ant object
    Ant *ant = new Ant();
    // generate a random row, col
    int row = rand() % rowLength;
    int col = rand() % colLength;
    // while the space is occupied
    while (organism[row][col])
    {
       row = rand() % rowLength;
       col = rand() % colLength;
    }
    organism[row][col] = ant;
  }

  for (int doodle_bugs_num = 0; doodle_bugs_num < FIVE_DOODLEBUGS; doodle_bugs_num++) {
    Doodlebug *doodlebug = new Doodlebug();

    int row = rand() % rowLength;
    int col = rand() % colLength;
    // while the space is occupied
    while (organism[row][col])
    {
       row = rand() % rowLength;
       col = rand() % colLength;
    }
    organism[row][col] = doodlebug;
  }
}

void visualizeOrganism(vector<vector<Organism *>> &organism, int rowLength, int colLength)
{
  cout << "+";
  for (int j = 0; j < colLength; j++)
  {
    cout << "---+";
  }
  cout << endl;
  for (int i = 0; i < rowLength; i++)
  {
    cout << "|";
    for (int j = 0; j < colLength; j++)
    {
       if (organism[i][j] == nullptr)
       {
         cout << "   |";
       }
       else
       {
         string s = organism[i][j]->getIdentity();
         if (s == "Ants")
           cout << " A |";
         else if (s == "Doodlebug")
           cout << " D |";
       }
    }
    cout << endl
         << "+";
    for (int j = 0; j < colLength; j++)
    {
       cout << "---+";
    }
    cout << endl;
  }
}

vector<pair<int, int>> getAdjacentAntsPosition(vector<vector<Organism *>> &organism, int currRow, int currCol, int rowLength, int colLength)
{
  vector<pair<int, int>> antsPosition;
  // going UP
  if (currRow > 0 && organism[currRow - 1][currCol]) {
    if (organism[currRow - 1][currCol]->getIdentity() == "Ants") {
      antsPosition.push_back({currRow - 1, currCol});
    }
  }
  // going DOWN
  if (currRow < rowLength - 1 && organism[currRow + 1][currCol]) {
    if (organism[currRow + 1][currCol]->getIdentity() == "Ants")
    {
      antsPosition.push_back({currRow + 1, currCol});
    }
  }
  // going LEfT
  if (currCol > 0 && organism[currRow][currCol - 1]) {
    if (organism[currRow][currCol - 1]->getIdentity() == "Ants")
    {
      antsPosition.push_back({currRow, currCol - 1});
    }
  }
  // going RIGHT
  if (currCol < colLength - 1 && organism[currRow][currCol + 1]) {
    if (organism[currRow][currCol + 1]->getIdentity() == "Ants")
    {
      antsPosition.push_back({currRow, currCol + 1});
    }
  }
  cout << "the ant position size is : " <<antsPosition.size() << endl;
  return antsPosition;
}

vector<pair<int, int>>getAdjacentValidPosition(vector<vector<Organism *>> &organism, int currRow, int currCol, int rowLength, int colLength) {
  vector<pair<int, int>> validPosition;
  // up
  if (currRow > 0 && organism[currRow - 1][currCol] == nullptr) validPosition.push_back({currRow - 1, currCol});
  // D
  if (currRow < rowLength - 1 && organism[currRow + 1][currCol] == nullptr) validPosition.push_back({currRow + 1, currCol});
  // L
  if (currCol > 0 && organism[currRow][currCol - 1] == nullptr) validPosition.push_back({currRow, currCol - 1});
  if (currCol < colLength - 1 && organism[currRow][currCol + 1] == nullptr) validPosition.push_back({currRow, currCol + 1});
  cout << "valid position size for Doodlebug currently is :" << validPosition.size() << endl;
  return validPosition;
}
void swapPosition(vector<vector<Organism *>> &organism, int row, int col, int ant_row, int ant_col)
{
  Organism *temp = organism[row][col];
  organism[row][col] = nullptr;
  organism[ant_row][ant_col] = temp;
}

void nextStep(vector<vector<Organism *>> &organism, int rowLength, int colLength)
{
  // move the doodlebugs first, afterwards move the ants

  for (int row = 0; row < rowLength; row++) {
    for (int col = 0; col < colLength; col++) {
      Organism* currOrganism = organism[row][col];
      vector<pair<int, int>> antPositions;
      vector<pair<int, int>> validPosition;
      int new_row = row, new_col = col;
      // if it's a doodlebugs
      if (currOrganism && currOrganism->getIdentity() == "Doodlebug") {
        // moves first, this increment the moves taken
         currOrganism->take_move();
         // write logic to actually move the currOrganism on the board to adjacent position
         antPositions = getAdjacentAntsPosition(organism, row, col, rowLength, colLength);
         // if there's an adjacent ant, eat it -> setDayStarve(0)
         // move the doodlebug to that position by: deleting the ant, and move doodlebug to that position
         if (antPositions.size() > 0) { // if there's an ant nearby
           currOrganism->setDayStarved();
           // auto [x, y] is an c++ 17 extension
          //  auto [ant_row, ant_col] = antPositions[rand() % antPositions.size()];
           auto ant_position = antPositions[rand() % antPositions.size()];
           int new_row = ant_position.first;
           int new_col = ant_position.second;
           Organism* adjacentAnt = organism[new_row][new_col];
           delete adjacentAnt;
           organism[new_row][new_col] = nullptr;
           swapPosition(organism, row, col, new_row, new_col);
           cout << "visualize the board after the swap position" << endl;
           visualizeOrganism(organism, row_length, col_length);
         } else { // else there's no adjacent ant, move it to a random valid position
          validPosition = getAdjacentValidPosition(organism, row, col, rowLength, colLength);
          // this is c++ 17 extension
          // auto [new_row, new_col] = validPosition[rand() % validPosition.size()];
          // edge case: to handle, if adjacentPosition is empty
          if (validPosition.size() > 0) {
            auto new_position = validPosition[rand() % validPosition.size()];
            new_row = new_position.first;
            new_col = new_position.second;
            swapPosition(organism, row, col, new_row, new_col);
          }
         }
         // handle edge case first: starve: after we already moved, check starve -> dies
         if (currOrganism->starved_to_death()) {
          // has starved to death, remove the content and delete pointers
            delete currOrganism;
            organism[row][col] = nullptr;
         }
         // if it's breedable, creates a new ant in an adjacent cell that is empty
         if (currOrganism->breedable()) {
            // using the new_row, new_col check validAdjacentPosition
            validPosition = getAdjacentValidPosition(organism, new_row, new_col, row_length, col_length);
            // if there is a valid position
            if (validPosition.size()) {
              Doodlebug* db = new Doodlebug;
              auto spawn_position = validPosition[rand() % validPosition.size()];
              int spawn_row = spawn_position.first;
              int spawn_col = spawn_position.second;
              organism[spawn_row][spawn_col] = db;
            }
         }
      }


    }
  }

// moves ants
  for (int row = 0; row < rowLength; row++) {
    for (int col = 0; col < colLength; col++) {
      Organism* currOrganism = organism[row][col];
      if (currOrganism && currOrganism->getIdentity() == "Ants") {
        currOrganism->take_move();

      }
    }
  }
}

// BUG CHECK: correctly outputs two doodlebugs adjacent ant size and validposition size,
// then it error: segmentation fault:11


    int main()
{
  // random seed
  srand(time(0));
  vector<vector<Organism *>> organism(row_length, vector<Organism *>(col_length, nullptr));
  fill_organism(organism, row_length, col_length);
  cout << "visualize initial board" << endl;
  visualizeOrganism(organism, row_length, col_length);
  nextStep(organism, row_length, col_length);
  cout << "visualize organism after doodlebug takes another step " << endl;
  // visualizeOrganism(organism, row_length, col_length);


}