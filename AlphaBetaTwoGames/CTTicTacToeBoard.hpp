//
//  CTTicTacToe.hpp
//  
//
//  Created by Erik Buck on 4/12/17.
//
//

#ifndef CTTicTacToe_hpp
#define CTTicTacToe_hpp

#include "CTAlphaBeta.hpp"
#include "play.hpp"
#include <array>
#include <vector>
#include <iostream>


class CTTicTacToeBoard
{
   std::array<int, 9> m_boardPositions;
   int m_score {0};
   bool m_hasBeenScored {false};
   
public:
   CTTicTacToeBoard();
   CTTicTacToeBoard(const CTTicTacToeBoard& orig, int move, bool isMaximizer);
   
   int score() const;
   int score();
   void draw() const;
   bool isGameOver() const;
   bool makeHumanMove(int position, CTTicTacToeBoard& nextBoard);
   
   static std::vector<CTTicTacToeBoard> getSuccessors(CTTicTacToeBoard board, bool isMaximizer);

   struct Properties : public GameProperties
   {
      virtual void ouputOptionalTaunt(int score);      
      virtual void ouputWinner(int score);
   };
};

#endif /* CTTicTacToe_hpp */
