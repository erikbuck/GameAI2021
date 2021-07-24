//
//  CTConnect4Board.hpp
//  AlphaBeta
//
//  Created by Erik M. Buck on 7/23/21.
//

#ifndef CTConnect4Board_hpp
#define CTConnect4Board_hpp

#include "CTAlphaBeta.hpp"
#include "play.hpp"
#include <vector>


class CTConnect4Board
{
   int m_boardPositions[7][6];
   int m_score {0};
   bool m_hasBeenScored {false};
   
   int getAvailableRow(int col) const;
   bool drop(int col, bool isMaximizer);
   
   //static const std::vector<winPattern_t> &getAllWinningPatterns();
   int patternScore() const;
   
public:
   CTConnect4Board();
   CTConnect4Board(const CTConnect4Board& orig, int move, bool isMaximizer);
   
   int score();
   void draw() const;
   bool isGameOver();
   bool makeHumanMove(int position, CTConnect4Board& nextBoard);
   
   static std::vector<CTConnect4Board> getSuccessors(CTConnect4Board board, bool isMaximizer);

   struct Properties : public GameProperties
   {
      virtual int inputConverter(std::string s);
      virtual void ouputOptionalTaunt(int score);
      virtual void ouputWinner(int score);
   };
};

#endif /* CTConnect4Board_hpp */
