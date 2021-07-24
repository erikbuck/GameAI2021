//
//  CTTicTacToe.cpp
//  
//
//  Created by Erik Buck on 4/12/17.
//
//

#include "CTTicTacToeBoard.hpp"
#include <iostream>
#include <cstring>


struct WinPattern {int a; int b; int c;};

static const std::vector<WinPattern> s_winningPatterns
{
   {0,1,2},
   {3,4,5},
   {6,7,8},
   {0,3,6},
   {1,4,7},
   {2,5,8},
   {0,4,8},
   {6,4,2},
};

CTTicTacToeBoard::CTTicTacToeBoard()
{
   static const std::array<int, 9> s_initialBoard
   {
      0, 0, 0,
      0, 0, 0,
      0, 0, 0,
   };
   
   m_boardPositions = s_initialBoard;
}

CTTicTacToeBoard::CTTicTacToeBoard(const CTTicTacToeBoard& orig, int move, bool isMaximizer)
{
   m_boardPositions = orig.m_boardPositions;
   m_boardPositions[move] = isMaximizer ? 1 : -1;
}

int CTTicTacToeBoard::score() const
{
   int result = 0;
   if(m_hasBeenScored)
   {
      result = m_score;
   }
   else
   {
      for(WinPattern p : s_winningPatterns)
      {
         int score = m_boardPositions[p.a];
         score += m_boardPositions[p.b];
         score += m_boardPositions[p.c];
         
         if(3 == score || -3 == score) {
            result = score;
            break;
         }
      }
   }
   return result;
}

int CTTicTacToeBoard::score()
{
   if(!m_hasBeenScored)
   {
      m_score = const_cast<const CTTicTacToeBoard*>(this)->score();
      m_hasBeenScored = true;
   }
   return m_score;
}

void CTTicTacToeBoard::draw() const
{
   std::cout << "\n";
   for(int j = 0; j < 3; ++j)
   {
      for(int i = 0; i < 3; ++i)
      {
         if(m_boardPositions[i + j*3] == 0)
         {
            std::cout << "   ";
         }
         else if(m_boardPositions[i + j*3] == -1)
         {
            std::cout << " O ";
         }
         else
         {
            std::cout << " X ";
         }
         if(i < 2)
         {
            std::cout << "|";
         }
      }
      std::cout << "\n-----------\n";
   }
}

std::vector<CTTicTacToeBoard>
CTTicTacToeBoard::getSuccessors(CTTicTacToeBoard board, bool isMaximizer)
{
   std::vector<CTTicTacToeBoard> result;
   
   if(3 != board.score() && -3 != board.score())
   {
      for(int i = 0; i < board.m_boardPositions.size(); ++i)
      {
         if(0 == board.m_boardPositions[i])
         {
            result.push_back(CTTicTacToeBoard{board, i, isMaximizer});
         }
      }
   }
   return result;
}

bool CTTicTacToeBoard::isGameOver() const
{
   bool result { 3 <= score() || -3 >= score() };
   if(!result)
   {
      int openPositionsNumber = 0;
      for(int i : m_boardPositions)
      {
         openPositionsNumber += (0 == i) ? 1 : 0;
      }
      result = (0 == openPositionsNumber);
   }
   return result;
}

bool CTTicTacToeBoard::makeHumanMove(int position, CTTicTacToeBoard& nextBoard)
{
   if(0 > position || 8 < position || 0 != m_boardPositions[position])
   {
      return false;
   }
   nextBoard.m_boardPositions[position] = 1;
   return true;
}

void CTTicTacToeBoard::Properties::ouputOptionalTaunt(int score)
{
   if(-3 >= score) { std::cout << "O is going to win!\n"; }
   else if(3 <= score) { std::cout << "X is going to win!\n"; }
}

void CTTicTacToeBoard::Properties::ouputWinner(int score)
{
   if(-3 >= score) std::cout << "O wins!\n";
   else if(3 <= score) std::cout << "X wins!\n";
   else std::cout << "It's a tie.\n";
}
