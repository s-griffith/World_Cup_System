#ifndef PLAYER_H
#define PLAYER_H

class Team;

#include "Teams.h"
#include <memory>

class Player {
public:

    /*
    * Constructor of Player class
    * @param name - The player's ID, the number of games, goals and cards of the player,
    *      and whether the player is a goalkeeper
    * @return - A new instance of Player.
    */
    Player(const int playerId, const int gamesPlayed, const int goals, const int cards, const bool goalKeeper, 
                Team* tmpTeam);

    /*
    * Copy Constructor and Assignment Operator of Player class
    * world_cup does not allow two of the same player or team (repeating ID's).
    * Therefore the system does not allow a copy constructor or assignment operator.
    * Additionally, we ensure that internal fields are not initialized as garbage values by deleting the default
    *   constructor.
    */
    Player() = delete;
    Player(const Player& other) = delete;
    Player& operator=(const Player& other) = delete;

    /*
    * Destructor of Player class
    * @param - none
    * @return - void
    */
    ~Player();

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
    * Return the team ID of the team that the player belongs to
    * @return - int, the team ID of the player
    */
    Team* get_team();

    /*
    * Return the closest right player of the given player
    * @return - Player*, the closest right player
    */
    Player* get_closest_right();

    /*
    * Return the closest left player of the given player
    * @return - Player*, the closest left player
    */
    Player* get_closest_left();

    /*
    * Update the number of games the player played
    * @param - the number of new games the player played
    * @return - none
    */
    void update_gamesPlayed(const int gamesPlayed);

    /*
    * Update the number of goals the player scored
    * @param - the number of new goals the player scored
    * @return - none
    */
    void update_goals(const int scoredGoals);

    /*
    * Update the number of cards the player received
    * @param - the number of new cards the player received
    * @return - none
    */
    void update_cards(const int cardsReceived);

    /*
    * Update the team that the player plays for
    * @param - shared pointer to the team that the player plays for
    * @return - none
    */
    void update_team(Team* tmpTeam);

    /*
    * Update the pointer to the closest right to the given pointer
    * @param - pointer to the new closest right player
    * @return - none
    */
    void update_closest_right(Player* closestRight);

    /*
    * Update the pointer to the closest left to the given pointer
    * @param - pointer to the new closest left player
    * @return - none
    */
    void update_closest_left(Player* closestLeft);

    /*
    * Helper function for get_closest_player in world_cup:
    * Check which player from input is "closer" to the current player according to number of goals, cards and ID
    * @param - two shared pointers to players to compare
    * @return - the ID of the closest player
    */
    int get_closest(Player* player1, Player* player2);

private:

    /*
    * Helper function for get_closest_player in world_cup:
    * Check which player from input is "closer" to the current player according to number of goals, cards and ID
    * @param - two shared pointers to players to compare
    * @return - the ID of the closest player
    */
    int check_diff(const int current, const int p1, const int p2);

    /*
     * The internal fields of Player:
     *   The player's ID
     *   The number of games the player has played
     *   The number of goals the player scored
     *   The number of cards the player has
     *   Whether or not the player is a goalkeeper
     *   A pointer to the player's team
     *   A pointer to the closest left and closest right players, according to goals, cards, and ID
     */    
    int m_playerId;
    int m_gamesPlayed;
    int m_goals;
    int m_cards;
    bool m_goalkeeper;
    Team* m_team;
    Player* m_closestLeft;
    Player* m_closestRight;
};


#endif //PLAYER_H