#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip> // include this header for setw()

using namespace std;

const int HUNDRED_ANTS = 100;
const int FIVE_DOODLEBUGS = 5;
const int row_length = 20, col_length = 20;
// could make these counts into member variable and have getter/setter but i'm lazy
int ant_count = 5, doodlebug_count = 105;
int stimulationRanCount = 0;
void freeValidPositionVector(vector<pair<int, int>> &validPosition);
void freeVectors(vector<pair<int, int>> &antPositions, vector<pair<int, int>> &validPosition);
class Organism
{
private:
  // child class can access this property through getters only
  int moves_taken;
  bool hasMoved;
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
  void setHasMoved() {this->hasMoved = !hasMoved;}
  bool getHasMoved() {return this->hasMoved;}
  Organism() : moves_taken(0), hasMoved(false) {}
};
void Organism::incrementMovesTaken()
{
  this->moves_taken = moves_taken += 1;
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
  this->setHasMoved();
  this->incrementMovesTaken();
}

bool Ant::breedable() const
{
  return (this->getMovesTaken() % 3) == 0;
}
void Ant::setDayStarved()
{
  cout << "Ants won't be starved";
}
int Ant::getDaysStarved()
{
  return 0;
}

bool Ant::starved_to_death()
{
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
void Doodlebug::setDayStarved()
{
  this->days_starved = 0;
}
void Doodlebug::incrementDayStarved()
{
  this->days_starved = this->days_starved + 1;
}
void Doodlebug::take_move()
{
  this->setHasMoved();
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

bool gameEndCondition() {
  int totalOrganism = ant_count + doodlebug_count;
  bool overCapacity = totalOrganism >= 400;
  bool organismExtinction = (ant_count == 0 && doodlebug_count == 0);

  return overCapacity && organismExtinction;
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

  for (int doodle_bugs_num = 0; doodle_bugs_num < FIVE_DOODLEBUGS; doodle_bugs_num++)
  {
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
          cout << "\033[1;32m A \033[0m|"; // set color to green for ants
        else if (s == "Doodlebug")
          cout << "\033[1;31m D \033[0m|"; // set color to red for doodlebugs
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

  // Check adjacent cells for ants
  // check above
  if (currRow > 0 && organism[currRow - 1][currCol] && organism[currRow - 1][currCol]->getIdentity() == "Ants" && organism[currRow][currCol]->getIdentity() != organism[currRow - 1][currCol]->getIdentity())
    antsPosition.push_back({currRow - 1, currCol});
  // check below
  if (currRow < rowLength - 1 && organism[currRow + 1][currCol] && organism[currRow + 1][currCol]->getIdentity() == "Ants" && organism[currRow][currCol]->getIdentity() != organism[currRow + 1][currCol]->getIdentity())
    antsPosition.push_back({currRow + 1, currCol});
  // check
  if (currCol > 0 && organism[currRow][currCol - 1] && organism[currRow][currCol - 1]->getIdentity() == "Ants" && organism[currRow][currCol]->getIdentity() != organism[currRow][currCol - 1]->getIdentity())
    antsPosition.push_back({currRow, currCol - 1});

  if (currCol < colLength - 1 && organism[currRow][currCol + 1] && organism[currRow][currCol + 1]->getIdentity() == "Ants" && organism[currRow][currCol]->getIdentity() != organism[currRow][currCol + 1]->getIdentity())
    antsPosition.push_back({currRow, currCol + 1});

  return antsPosition;
}

vector<pair<int, int>> getAdjacentValidPosition(vector<vector<Organism *>> &organism, int currRow, int currCol, int rowLength, int colLength)
{
  vector<pair<int, int>> validPosition;
  // up
  if (currRow > 0 && organism[currRow - 1][currCol] == nullptr)
    validPosition.push_back({currRow - 1, currCol});
  // D
  if (currRow < rowLength - 1 && organism[currRow + 1][currCol] == nullptr)
    validPosition.push_back({currRow + 1, currCol});
  // L
  if (currCol > 0 && organism[currRow][currCol - 1] == nullptr)
    validPosition.push_back({currRow, currCol - 1});
  if (currCol < colLength - 1 && organism[currRow][currCol + 1] == nullptr)
    validPosition.push_back({currRow, currCol + 1});
  // cout << "valid position size for Doodlebug currently is :" << validPosition.size() << endl;
  return validPosition;
}
// void swapPosition(vector<vector<Organism *>> &organism, int row, int col, int new_row, int new_col)
// {
//   Organism *temp = organism[row][col];
//   organism[row][col] = organism[new_row][new_col];
//   organism[new_row][new_col] = temp;
//   // Set the original position of the doodlebug to nullptr
//   organism[row][col] = nullptr;
// }

// Doodlebugs and Ants are not swapped correctly.
// Info: Doodlebugs stayed in place and ants is set to nullPtr.
// void swapPosition(vector<vector<Organism *>> &organism, int row, int col, int new_row, int new_col)
// {
//   // if currentOrganism is an ant, we only want it to move to a nullptr position

//   // if currentOrganism is an doodlebug, we can move it to the ant position
//   // or if the organism[new_row][new_col] is not an ant we want to move it to the nullptr position
//   Organism *temp = organism[row][col];
//   if (temp == nullptr) return;

//   if (organism[row][col]->getIdentity() == "Ants" && organism[new_row][new_col] == nullptr)
//   {
//     // assign current position to nullptr
//     organism[row][col] = organism[new_row][new_col];
//     // new position to our current position.
//     organism[new_row][new_col] = temp;
//   }

//   if (organism[row][col]->getIdentity() == "Doodlebug" && organism[new_row][new_col] == nullptr)
//   {
//     organism[row][col] = organism[new_row][new_col];
//     organism[new_row][new_col] = temp;
//   }
//   if (organism[row][col]->getIdentity() == "Doodlebug" && organism[new_row][new_col] != nullptr && organism[new_row][new_col]->getIdentity() == "Ants")
//   {
//     // might actually need to delete the new position's ant first actually.
//     organism[new_row][new_col] = nullptr;
//     organism[new_row][new_col] = organism[row][col];
//     organism[row][col] = nullptr;
//   }
// }

void swapPosition(vector<vector<Organism *>> &organism, int row, int col, int new_row, int new_col)
{
  Organism *temp = organism[row][col];
  if (temp == nullptr)
    return; // Return if the current organism is nullptr.

  if (temp->getIdentity() == "Ants" && organism[new_row][new_col] == nullptr)
  {
    // Move ant to the new position.
    organism[new_row][new_col] = temp;
    organism[row][col] = nullptr;
  }
  else if (temp->getIdentity() == "Doodlebug")
  {
    if (organism[new_row][new_col] == nullptr)
    {
      // Move doodlebug to the new position.
      organism[new_row][new_col] = temp;
      organism[row][col] = nullptr;
    }
    else if (organism[new_row][new_col]->getIdentity() == "Ants")
    {
      // Eat the ant at the new position and move doodlebug to that position.
      //  delete organism[new_row][new_col];
      organism[new_row][new_col] = temp;
      organism[row][col] = nullptr;
    }
  }
}

void nextStep(vector<vector<Organism *>> &organism, int rowLength, int colLength)
{
  int totalOrganism = ant_count + doodlebug_count;
  if (totalOrganism >= 400) {
    cout << "Total organism has exceed capacity" << endl;
  }
  if (ant_count == 0 && doodlebug_count == 0) {
    cout << "All organism has gone extinct" << endl;
  }

  // move the doodlebugs first, afterwards move the ants
  for (int row = 0; row < rowLength; row++)
  {
    for (int col = 0; col < colLength; col++)
    {
      Organism *currOrganism = organism[row][col];
      vector<pair<int, int>> antPositions;
      vector<pair<int, int>> validPosition;
      int new_row = row, new_col = col;
        if (currOrganism != nullptr && currOrganism->getHasMoved()) {
          currOrganism->setHasMoved();
          continue;
        }
        // if it's a doodlebugs
        if (currOrganism && currOrganism->getIdentity() == "Doodlebug")
        { // moves first, this increment the moves taken
          currOrganism->take_move();
          // write logic to actually move the currOrganism on the board to adjacent position
          antPositions = getAdjacentAntsPosition(organism, row, col, rowLength, colLength);
          // for (auto pos : antPositions)
          // {
          //   std::cout << "(" << pos.first << ", " << pos.second << ")" << std::endl;
          // }

          // if there's an adjacent ant, eat it -> setDayStarve(0)
          // move the doodlebug to that position by: deleting the ant, and move doodlebug to that position
          if (antPositions.size() > 0)
          { // if there's an ant nearby
            // cout << "how many ants are nearby this doodlebug " << antPositions.size() << endl;
            currOrganism->setDayStarved();
            // auto [x, y] is an c++ 17 extension
            //  auto [ant_row, ant_col] = antPositions[rand() % antPositions.size()];
            auto ant_position = antPositions[rand() % antPositions.size()];
            int new_row = ant_position.first;
            int new_col = ant_position.second;
            Organism *adjacentAnt = organism[new_row][new_col];
            //  delete adjacentAnt;
            organism[new_row][new_col] = nullptr;
            swapPosition(organism, row, col, new_row, new_col);
            ant_count--;
            cout << "a doodlebug has eaten an ant" << endl;
            //  cout << "visualize the board after the swap position" << endl;
            //  visualizeOrganism(organism, rowLength, colLength);
          }
          else
          { // else there's no adjacent ant, move it to a random valid position
            validPosition = getAdjacentValidPosition(organism, row, col, rowLength, colLength);
            // edge case: to handle, if adjacentPosition is empty
            if (validPosition.size() > 0)
            {
              auto new_position = validPosition[rand() % validPosition.size()];
              new_row = new_position.first;
              new_col = new_position.second;
              swapPosition(organism, row, col, new_row, new_col);
            }
          }
          // visualize After moving either to consume ants, or to empty space
          //  cout << "visualize After moving either to consume ants, or to empty space" << endl;
          //  visualizeOrganism(organism, rowLength, colLength);
          if (currOrganism->starved_to_death())
          {
            // delete here causes error.
            // delete currOrganism;
            cout << "a doodlebug has died due to starvation" << endl;
            doodlebug_count--;
            organism[new_row][new_col] = nullptr;
          }
          // if it's breedable, creates a new ant in an adjacent cell that is empty
          // could be possible that it's nullptr
          if (currOrganism != nullptr && currOrganism->getIdentity() == "Doodlebug" && currOrganism->breedable())
          {
            // using the new_row, new_col check validAdjacentPosition
            validPosition = getAdjacentValidPosition(organism, new_row, new_col, row_length, col_length);
            // if there is a valid position
            if (validPosition.size())
            {
              Doodlebug *db = new Doodlebug;
              auto spawn_position = validPosition[rand() % validPosition.size()];
              int spawn_row = spawn_position.first;
              int spawn_col = spawn_position.second;
              organism[spawn_row][spawn_col] = db;
              cout << "a new doodlebug has spawned" << endl;
              doodlebug_count++;
            }
          }
      }
      freeVectors(antPositions, validPosition);
    }
  }

  // moves ants
  for (int row = 0; row < rowLength; row++)
  {
    for (int col = 0; col < colLength; col++)
    {
      Organism *currOrganism = organism[row][col];
      vector<pair<int, int>> validPosition;
      int new_row = row, new_col = col;

      if (currOrganism != nullptr && currOrganism->getHasMoved())
      {
          currOrganism->setHasMoved();
          continue;
      }
      if (currOrganism && currOrganism->getIdentity() == "Ants")
      {
          currOrganism->take_move();
          validPosition = getAdjacentValidPosition(organism, row, col, rowLength, colLength);
          if (validPosition.size())
          {
            auto new_position = validPosition[rand() % validPosition.size()];
            new_row = new_position.first;
            new_col = new_position.second;
            swapPosition(organism, row, col, new_row, new_col);
          }

          if (currOrganism != nullptr && currOrganism->getIdentity() == "Ants" && currOrganism->breedable())
          {
            validPosition = getAdjacentValidPosition(organism, new_row, new_col, row_length, col_length);
            if (validPosition.size())
            {
              Ant *ant = new Ant;
              auto spawn_position = validPosition[rand() % validPosition.size()];
              int spawn_row = spawn_position.first;
              int spawn_col = spawn_position.second;
              organism[spawn_row][spawn_col] = ant;
              cout << "a new ant has been spawned" << endl;
              ant_count++;
            }
          }
      }
      freeValidPositionVector(validPosition);
    }
  }
}

void freeValidPositionVector(vector<pair<int, int>> &validPosition) {
  validPosition.clear();
  vector<pair<int, int>>().swap(validPosition);
}

void freeVectors(vector<pair<int, int>> &antPositions, vector<pair<int, int>> &validPosition)
{
  antPositions.clear();
  vector<pair<int, int>>().swap(antPositions);
  validPosition.clear();
  vector<pair<int, int>>().swap(validPosition);
}

void freeOrganismVector(vector<vector<Organism *>> &organism)
{
  // iterate over each element in the 2D vector
  for (int i = 0; i < row_length; i++)
  {
    for (int j = 0; j < col_length; j++)
    {
      // if the element is not null, delete the Organism object it points to
      if (organism[i][j] != nullptr)
      {
          delete organism[i][j];
      }
    }
    // clear the inner vector to free up its memory
    organism[i].clear();
  }
  // clear the outer vector to free up its memory
  organism.clear();
}

// it's allowing ANT eat ANT, 7:04 PM. mgiht have fixed
/*
Once I moved the doodlebug, it goes to let's say the next row it'll move again. I NEED TO MAKE SURE THAT it doesn't move again..
This is probabily why i'm getting more than 5 doodlebugs moving sometimes.

add a hasMoved property, and setMoved property, if the property is true
then we want to setMove back to false and continue on the next item in the list.
*/

void runStimulation(vector<vector<Organism *>> organism) {
  nextStep(organism, row_length, col_length);
  visualizeOrganism(organism, row_length, col_length);
}

int main()
{
  // random seed
  srand(time(0));
  vector<vector<Organism *>> organism(row_length, vector<Organism *>(col_length, nullptr));
  fill_organism(organism, row_length, col_length);
  cout << "visualize initial stage" << endl;
  visualizeOrganism(organism, row_length, col_length);
  while (!gameEndCondition()) {
    runStimulation(organism);
    stimulationRanCount++;
    cout << "current amount of doodlebugs left: " << doodlebug_count << endl;
    cout << "current amount of ants left: " << ant_count << endl;
  }
  cout << "the end ended when the stimulation has ran for" << stimulationRanCount << endl;
}