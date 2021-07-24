//
//  main.cpp
//  AlphaBeta
//
//  Created by Erik M. Buck on 7/21/21.
//

#include "CTAlphaBeta.hpp"
#include "play.hpp"
#include "CTTicTacToeBoard.hpp"
#include "CTConnect4Board.hpp"
#include <iostream>
#include <string>
#include <cstring>               // strncmp()
#include <cctype>                // toupper()


int main(int argc, const char * argv[]) {
   if(1 < argc)
   {
      if(0 == strncmp(argv[1], "t", 1))
      {
         CTTicTacToeBoard::Properties props;
         playAlphaBeta<CTTicTacToeBoard>(2 < argc, 9, props);
      }
      else if(0 == strncmp(argv[1], "c", 1))
      {
         CTConnect4Board::Properties props;
         playAlphaBeta<CTConnect4Board>(2 < argc, 10, props);
      }
      else
      {
         CTConnect4Board::Properties props;
         playMiniMax<CTConnect4Board>(2 < argc, 10, props);
      }
   }
   return 0;
}
