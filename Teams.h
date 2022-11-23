#ifndef TEAMS.h
#define TEAMS.h

#include "AVLTree.h"
#include "Player.h"
#include <memory>

class Team {
public:

private:
    int m_id;
    int m_points;
    Tree<std::shared_ptr<Player>> m_rootPlayer;
    std::shared_ptr<Player> m_topScorer;
    int m_numPlayers;
    int m_numGoalkeepers;
    int m_numGoals;
    int m_numCards;
    int m_numGames;
};

#endif //TEAMS.h