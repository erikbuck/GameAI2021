//
//  CTConnect4Board.cpp
//  AlphaBeta
//
//  Created by Erik M. Buck on 7/23/21.
//

#include "CTConnect4Board.hpp"
#include <iostream>
#include <array>


typedef std::pair<int, int> coord_t;
typedef struct {coord_t a; coord_t b; coord_t c; coord_t d;} winPattern_t;
typedef struct {int score; winPattern_t* p;} scoreAndPattern_t;

static const int s_initialBoard[7][6] = {
   {0,0,0,0,0,0},
   {0,0,0,0,0,0},
   {0,0,0,0,0,0},
   {0,0,0,0,0,0},
   {0,0,0,0,0,0},
   {0,0,0,0,0,0},
   {0,0,0,0,0,0}};


static const std::vector<winPattern_t> &s_getAllWinningPatterns()
{
   static std::vector<winPattern_t> allWinningPattens;
   
   if(0 == allWinningPattens.size()) {
      // Since board fills from bottom up, look in bottom rows first
      for(int y = 0; y < 6; ++y)
         for(int x = 0; x < (7-3); ++x)
            allWinningPattens.push_back({{x,y},{x+1, y},{x+2,y},{x+3,y}});
      for(int y = 0; y < (6 - 3); ++y)
         for(int x = 0; x < 7; ++x)
            allWinningPattens.push_back({{x,y},{x, y+1},{x,y+2},{x,y+3}});
      for(int y = 3; y < 6; ++y)
         for(int x = 0; x < (7 - 3); ++x)
            allWinningPattens.push_back({{x,y},{x+1, y-1},{x+2,y-2},{x+3,y-3}});
      for(int y = 0; y < (6 - 3); ++y)
         for(int x = 0; x < (7 - 3); ++x)
            allWinningPattens.push_back({{x,y},{x+1, y+1},{x+2,y+2},{x+3,y+3}});
   }
   
   return allWinningPattens;
}

int CTConnect4Board::getAvailableRow(int col) const
{
   for(int r = 0; r < 6; ++r) {
      if(0 == m_boardPositions[col][r]) return r;
   }
   
   return -1;
}

bool CTConnect4Board::drop(int col, bool isMaximizer)
{
   if(0 <= col && col < 7) {
      int row = getAvailableRow(col);
      if(0 <= row) {
         m_boardPositions[col][row] = isMaximizer ? 1 : -1;
         return true;
      }
   }
   
   return false;
}

int CTConnect4Board::patternScore() const
{
   int score = 0;
   for(winPattern_t p : s_getAllWinningPatterns())
   {
      int sum = m_boardPositions[p.a.first][p.a.second];
      sum += m_boardPositions[p.b.first][p.b.second];
      sum += m_boardPositions[p.c.first][p.c.second];
      sum += m_boardPositions[p.d.first][p.d.second];
      if(4 <= sum || -4 >= sum) {
         score = sum;
         break;
      }
   }
   
   return score;
}

CTConnect4Board::CTConnect4Board()
{
   std::memcpy((int *)m_boardPositions, (const int *)s_initialBoard, sizeof(s_initialBoard));
}

CTConnect4Board::CTConnect4Board(const CTConnect4Board& orig, int move, bool isMaximizer)
{
   std::memcpy((int *)m_boardPositions, (const int *)orig.m_boardPositions, sizeof(m_boardPositions));
   drop(move, isMaximizer);
}

int CTConnect4Board::score()
{
   if(!m_hasBeenScored)
   {
      m_score = patternScore();
      m_hasBeenScored = true;
   }
   
   return m_score;
}

void CTConnect4Board::draw() const
{
   std::cout << "\n" << " A  B  C  D  E  F  G \n";
   for(int r = 5; r >= 0; --r) {
      for(int c = 0; c < 7; ++c) {
         if(m_boardPositions[c][r] == 0) std::cout << "   ";
         else if(m_boardPositions[c][r] == -1) std::cout << " O ";
         else std::cout << " X ";
      }
      std::cout << "\n";
   }
   std::cout << std::flush;
}

bool CTConnect4Board::isGameOver()
{
   bool result { 4 <= score() || -4 >= score() };
   if(!result)
   {
      result = true; // Assume game over
      for(int c = 0; result && c < 7; ++c) {
         if(0 == m_boardPositions[c][5]) result = false; // found open column
      }
   }
   return result;
}

bool CTConnect4Board::makeHumanMove(int position, CTConnect4Board& nextBoard)
{
   if(0 > position || 7 < position || 0 != m_boardPositions[position][5])
   {  // Invalid move
      return false;
   }
   nextBoard.drop(position, true);
   return true;
}

std::vector<CTConnect4Board> CTConnect4Board::getSuccessors(CTConnect4Board board, bool isMaximizer)
{
   static std::array<int, 7> columnsInBestOrder{3, 2, 4, 1, 5, 0, 6};
   std::vector<CTConnect4Board> result{};
   for(auto c : columnsInBestOrder)
   {
      if(0 == board.m_boardPositions[c][5])
      {
         CTConnect4Board nextBoard{board, c, isMaximizer};
         result.push_back(nextBoard);
      }
   }
   return result;
}

int CTConnect4Board::Properties::inputConverter(std::string s)
{
   int result = -1;
   if(0 < s.size()) { result = std::toupper(s[0]) - ((int)'A'); }
   
   return result;
}

void CTConnect4Board::Properties::ouputOptionalTaunt(int score)
{
   if(-4 >= score) { std::cout << "O is going to win!\n"; }
   else if(4 <= score) { std::cout << "X is going to win!\n"; }
}

void CTConnect4Board::Properties::ouputWinner(int score)
{
   if(-4 >= score) { std::cout << "O wins!\n"; }
   else if(4 <= score) { std::cout << "X wins!\n"; }
   else { std::cout << "It's a tie.\n"; }
}

