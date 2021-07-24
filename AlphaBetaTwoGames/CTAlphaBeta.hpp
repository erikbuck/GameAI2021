//
//  CTAlphaBeta.hpp
//  ConnectFourAlphaBetaCpp
//
//  Created by Erik Buck on 7/7/16.
//  Copyright © 2016 CT. All rights reserved.
//

#ifndef CTAlphaBeta_hpp
#define CTAlphaBeta_hpp

#include <vector>
#include <algorithm>
#include <thread>
#include <future>

template<typename board_t>
std::pair<int, board_t> CTMiniMax(board_t aBoard,
                                  int searchDepth,
                                  bool isMaximizer,
                                  int threadingDepth = 1)
{
   int result {aBoard.score()};
   board_t resultBoard {aBoard};
   
   if(searchDepth > 0 && !aBoard.isGameOver())
   {
      std::vector<board_t> availableBoards = board_t::getSuccessors(aBoard, isMaximizer);
      if(isMaximizer)
      {
         int value = -1000;
         if(0 >= threadingDepth)
         {  // Do serial serach
            int value = -1000;
            for(auto b : availableBoards)
            {
               auto nextScoreAndBoard = CTMiniMax(b, searchDepth - 1, false,
                                                  threadingDepth - 1);
               if(nextScoreAndBoard.first > value)
               {
                  value = nextScoreAndBoard.first;
                  resultBoard = b;
                  result = value;
               }
            }
         }
         else
         {  // Do parallel serach
            std::vector<std::future<std::pair<int, board_t> > > futures{};
            for(int i = 0; i < availableBoards.size(); ++i)
            {
               auto lambda{[=](){
                  return CTMiniMax(availableBoards[i], searchDepth - 1, false,
                                   threadingDepth - 1);
               }};
               futures.push_back(std::async(std::launch::async, lambda));
            }
            for(int i = 0; i < futures.size(); ++i)
            {
               auto nextScoreAndBoard { futures[i].get() };
               if(nextScoreAndBoard.first > value)
               {
                  value = nextScoreAndBoard.first;
                  resultBoard = availableBoards[i];
                  result = value;
               }
            }
         }
      }
      else
      {  // Minimizer
         int value = 1000;
         if(0 >= threadingDepth)
         {  // Do serial serach
            for(auto b : availableBoards)
            {
               auto nextScoreAndBoard = CTMiniMax(b, searchDepth - 1, true,
                                                  threadingDepth - 1);
               if(nextScoreAndBoard.first < value)
               {
                  value = nextScoreAndBoard.first;
                  resultBoard = b;
                  result = value;
               }
            }
         }
         else
         {  // Do parallel serach
            std::vector<std::future<std::pair<int, board_t> > > futures{};
            for(int i = 0; i < availableBoards.size(); ++i)
            {
               auto lambda{[=](){
                  return CTMiniMax(availableBoards[i], searchDepth - 1, true,
                                   threadingDepth - 1);
               }};
               futures.push_back(std::async(std::launch::async, lambda));
            }
            for(int i = 0; i < futures.size(); ++i)
            {
               auto nextScoreAndBoard { futures[i].get() };
               if(nextScoreAndBoard.first < value)
               {
                  value = nextScoreAndBoard.first;
                  resultBoard = availableBoards[i];
                  result = value;
               }
            }
         }
      }
   }
   
   return {result, resultBoard};
}

template<typename board_t>
std::pair<int, board_t> CTAlphaBeta(board_t aBoard,
                                    int searchDepth,
                                    int alpha,
                                    int beta,
                                    bool isMaximizer,
                                    int threadingDepth = 1)
{
   int result {aBoard.score()};
   board_t resultBoard {aBoard};
   
   if(searchDepth > 0 && !aBoard.isGameOver())
   {
      std::vector<board_t> availableBoards = board_t::getSuccessors(aBoard, isMaximizer);
      if(isMaximizer)
      {
         int value = -1000;
         if(0 >= threadingDepth)
         {  // Do serial serach
            int value = -1000;
            for(auto b : availableBoards)
            {
               auto nextScoreAndBoard = CTAlphaBeta(b, searchDepth - 1,
                                                    alpha, beta, false,
                                                  threadingDepth - 1);
               if(nextScoreAndBoard.first >= beta)
               {  // Opponent already has a better move so stop looking
                  value = nextScoreAndBoard.first;
                  resultBoard = b;
                  result = value;
                  break;
               }
               if(nextScoreAndBoard.first > value)
               {
                  value = nextScoreAndBoard.first;
                  resultBoard = b;
                  result = value;
                  alpha = std::max(alpha, value);
               }
            }
         }
         else
         {  // Do parallel serach
            std::vector<std::future<std::pair<int, board_t> > > futures{};
            for(int i = 0; i < availableBoards.size(); ++i)
            {
               auto lambda{[=](){
                  return CTAlphaBeta(availableBoards[i], searchDepth - 1,
                                     alpha, beta, false,
                                   threadingDepth - 1);
               }};
               futures.push_back(std::async(std::launch::async, lambda));
            }
            for(int i = 0; i < futures.size(); ++i)
            {
               auto nextScoreAndBoard { futures[i].get() };
               if(nextScoreAndBoard.first > value)
               {
                  value = nextScoreAndBoard.first;
                  resultBoard = availableBoards[i];
                  result = value;
               }
            }
         }
      }
      else
      {  // Minimizer
         int value = 1000;
         if(0 >= threadingDepth)
         {  // Do serial serach
            for(auto b : availableBoards)
            {
               auto nextScoreAndBoard = CTAlphaBeta(b, searchDepth - 1,
                                                    alpha, beta, true,
                                                  threadingDepth - 1);
               if(nextScoreAndBoard.first <= alpha)
               {  // Opponent already has a better move so stop looking
                  value = nextScoreAndBoard.first;
                  resultBoard = b;
                  result = value;
                  break;
               }
               if(nextScoreAndBoard.first < value)
               {
                  value = nextScoreAndBoard.first;
                  resultBoard = b;
                  result = value;
                  beta = std::min(beta, value);
               }
            }
         }
         else
         {  // Do parallel serach
            std::vector<std::future<std::pair<int, board_t> > > futures{};
            for(int i = 0; i < availableBoards.size(); ++i)
            {
               auto lambda{[=](){
                  return CTAlphaBeta(availableBoards[i], searchDepth - 1,
                                     alpha, beta, true,
                                   threadingDepth - 1);
               }};
               futures.push_back(std::async(std::launch::async, lambda));
            }
            for(int i = 0; i < futures.size(); ++i)
            {
               auto nextScoreAndBoard { futures[i].get() };
               if(nextScoreAndBoard.first < value)
               {
                  value = nextScoreAndBoard.first;
                  resultBoard = availableBoards[i];
                  result = value;
               }
            }
         }
      }
   }
   
   return {result, resultBoard};
}


#endif /* CTAlphaBeta_hpp */
