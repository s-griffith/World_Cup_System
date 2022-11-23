#ifndef PLAYER_h
#define PLAYER_h

#include "Teams.h"
#include <iostream>
#include <string>

class Player {
public:
    /*
     * Constructor of Player class
     *
     * @param name - The player's ID, the number of games, goals and cards of the player,
     *      and also if the player is a goalkeeper
     * @return - A new instance of Player.
    */
    Player(const int playerId, const int gamesPlayed, const int goals, const int cards, const bool goalKeeper);

    /*
    * Return the player's ID
    * @return - integer of the player's ID
    */
    int get_playerId() const;

    /*
     * Return the number of games the player played
     * @return - integer of the number of games the player played
     */
    int get_gamesPlayed() const;

    /*
    * Return the number of goals the player scored
    * @return - integer of the number of goals the player scored
    */
    int get_goals() const;

    /*
    * Return the number of cards the player received
    * @return - integer of the number of cards the player received
    */
    int get_cards() const;

    /*
    * Return if the player is a goalkeeper
    * @return - bool, with true if the player is a goalkeeper, otherwise false
    */
    bool get_goalkeeper() const;

    /*
    * Update the number of games the player played
    * @param - the number of new games the player played
    * @return - none
    */
    void set_gamesPlayed(const int gamesPlayed);

    /*
    * Update the number of goals the player scored
    * @param - the number of new goals the player scored
    * @return - none
    */
    void set_goals(const int scoredGoals);

    /*
    * Update the number of cards the player received
    * @param - the number of new cards the player received
    * @return - none
    */
    void set_cards(const int cardsReceived);

    /*
     * Use the default methods for the copy constructor, destructor and the assignment operator
    */
    Player(const Player&) = default;
    ~Player() = default;
    Player& operator=(const Player& other) = default;

private:
    int m_playerId;
    int m_gamesPlayed;
    int m_goals;
    int m_cards;
    bool m_goalkeeper;
};

#endif //PLAYER_h