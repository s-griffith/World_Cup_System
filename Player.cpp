#include "Player.h"

//--------------------------------Player Class Member Functions------------------------------------

//Constructor for Player
Player::Player(const int playerId, const int gamesPlayed, const int goals, const int cards, const bool goalKeeper,
               Team* tmpTeam) :
    m_playerId(playerId),
    m_gamesPlayed(gamesPlayed),
    m_goals(goals),
    m_cards(cards),
    m_goalkeeper(goalKeeper),
    m_team(tmpTeam),
    m_closestLeft(nullptr),
    m_closestRight(nullptr)
{}

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
Team* Player::get_team()
{
    return m_team;
}

Player* Player::get_closest_right()
{
    return m_closestRight;
}

Player* Player::get_closest_left()
{
    return m_closestLeft;
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
void Player::update_team(Team* tmpTeam)
{
    m_team = tmpTeam;
}

void Player::update_closest_left(Player* closestLeft)
{
    m_closestLeft = closestLeft;
}

void Player::update_closest_right(Player* closestRight)
{
    m_closestRight = closestRight;
}

//Get the ID of the closest player to the current player
int Player::get_closest(Player* player1, Player* player2)
{
    if (player1 != nullptr && player2 == nullptr) {
        return player1->m_playerId;
    }
    else if (player1 == nullptr && player2 != nullptr) {
        return player2->m_playerId;
    }
    else if (player1 != nullptr && player2 != nullptr) {
        //Return the player with the closest goal count
        int diff = check_diff(m_goals, player1->m_goals, player2->m_goals);
        if (diff == 1) {
            return player1->m_playerId;
        }
        else if (diff == 2) {
            return player2->m_playerId;
        }
        //Return the player with the closest card count
        diff = check_diff(m_cards, player1->m_cards, player2->m_cards);
        if (diff == 1) {
            return player1->m_playerId;
        }
        else if (diff == 2) {
            return player2->m_playerId;
        }
        //Return the player with the closest player id
        diff = check_diff(m_playerId, player1->m_playerId, player2->m_playerId);
        if (diff == 1) {
            return player1->m_playerId;
        }
        else if (diff == 2) {
            return player2->m_playerId;
        }
        //Return the player with the bigger player id
        if (player1->m_playerId > m_playerId) {
            return player1->m_playerId;
        }
        return player2->m_playerId;
    }
    return -1;
}


//Check the difference between the current value and the other two values
//Return 1 if p1 is closer to current, 2 if p2 is closer to current, 0 if they are of the same distance
int Player::check_diff(const int current, const int p1, const int p2)
{
    int diff1 = 0, diff2 = 0;
    if (current >= p1) {
        diff1 = current - p1;
    }
    else {
        diff1 = p1 - current;
    }
    if (current >= p2) {
        diff2 = current - p2;
    }
    else {
        diff2 = p2 - current;
    }
    if (diff1 < diff2) {
        return 1;
    }
    if (diff2 < diff1) {
        return 2;
    }
    return 0;
}
