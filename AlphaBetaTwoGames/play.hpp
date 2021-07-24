//
//  play.hpp
//  AlphaBeta
//
//  Created by Erik M. Buck on 7/23/21.
//

#ifndef play_hpp
#define play_hpp

#include "CTAlphaBeta.hpp"
#include <iostream>
#include <string>
#include <cstring>               // strncmp()
#include <cctype>                // toupper()

struct GameProperties
{
   virtual ~GameProperties() {}
   virtual int inputConverter(std::string s) {return std::stoi(s);}
   virtual void ouputOptionalTaunt(int score) {}
   virtual void ouputWinner(int score) {}
};

template<typename board>
void playAlphaBeta(bool hasHumanPlayer, int depth, GameProperties& properties)
{
   int value {0};
   
   board next{};
   next.draw();
   
   bool isMaximizer {true};
   do
   {
      if(hasHumanPlayer && isMaximizer)
      {
         std::string move{};
         do
         {
            std::cout << "Move: ";
            std::cin >> move;
         }
         while(!next.makeHumanMove(properties.inputConverter(move), next));
         value = next.score();
         next.draw();
         isMaximizer = false; // Alternate turns [human just finished a turn]
      }
      
      auto scoreAndBoard = CTAlphaBeta<board>(next, depth, -1000, 1000, isMaximizer, 1);
      value = scoreAndBoard.first;
      next = scoreAndBoard.second;
      
      if(hasHumanPlayer)
      {
         next.draw();
         if(!next.isGameOver())
         {
            properties.ouputOptionalTaunt(value);
         }
     }
      
      isMaximizer = !isMaximizer; // Alternate turns
   }
   while(!next.isGameOver());
   
   next.draw();
   properties.ouputWinner(value);
}

template<typename board>
void playMiniMax(bool hasHumanPlayer, int depth, GameProperties& properties)
{
   int value {0};
   
   board next{};
   next.draw();
   
   bool isMaximizer {true};
   do
   {
      if(hasHumanPlayer && isMaximizer)
      {
         std::string move{};
         do
         {
            std::cout << "Move: ";
            std::cin >> move;
         }
         while(!next.makeHumanMove(properties.inputConverter(move), next));
         value = next.score();
         next.draw();
         isMaximizer = false; // Alternate turns [human just finished a turn]
      }
      
      auto scoreAndBoard = CTMiniMax<board>(next, depth, isMaximizer, 1);
      value = scoreAndBoard.first;
      next = scoreAndBoard.second;
      
      if(hasHumanPlayer)
      {
         next.draw();
         if(!next.isGameOver())
         {
            properties.ouputOptionalTaunt(value);
         }
     }
      
      isMaximizer = !isMaximizer; // Alternate turns
   }
   while(!next.isGameOver());
   
   next.draw();
   properties.ouputWinner(value);
}

#endif /* play_hpp */
