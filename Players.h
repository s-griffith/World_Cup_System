#ifndef PLAYERS.h
#define PLAYERS.h

#include "Teams.h"


class Player {
public:

private:
    int m_id;
    Team* m_currentTeam;
    int m_gamesPlayed;
    int m_goals;
    int m_cards;
    bool m_goalkeeper;
};

#endif //PLAYERS.h