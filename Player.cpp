#include "Player.h"

//--------------------------------Player Class Member Functions------------------------------------

//Constructor for Player
Player::Player(const int playerId, const int gamesPlayed, const int goals, const int cards, const bool goalKeeper) :
    m_playerId(playerId),
    m_gamesPlayed(gamesPlayed),
    m_goals(goals),
    m_cards(cards),
    m_goalkeeper(goalKeeper),
    m_team(nullptr)
{
}

//Return the player's id
int Player::get_playerId() const
{
    return m_playerId;
}

//Return the number of games the player played
int Player::get_gamesPlayed() const
{
    return m_gamesPlayed;
}

//Return the number of goals the player scored
int Player::get_goals() const
{
    return m_goals;
}

//Return the number of cards the player received
int Player::get_cards() const
{
    return m_cards;
}

//Return if the player is a goalkeeper
bool Player::get_goalkeeper() const
{
    return m_goalkeeper;
}

//Return the team that the player is part of
std::shared_ptr<Team>& Player::get_team()
{
    return m_team;
}

//Update the number of games the player played
void Player::update_gamesPlayed(const int gamesPlayed)
{
    m_gamesPlayed += gamesPlayed;
}

//Update the number of goals the player scored
void Player::update_goals(const int scoredGoals)
{
    m_goals += scoredGoals;
}

//Update the number of cards the player received
void Player::update_cards(const int cardsReceived)
{
    m_cards += cardsReceived;
}

//Update the team the player plays for
void Player::update_team(std::shared_ptr<Team>& tmpTeam)
{
    m_team = tmpTeam;
}