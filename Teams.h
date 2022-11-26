#ifndef TEAMS_H
#define TEAMS_H

#include "AVLTree.h"
#include "Player.h"
#include <memory>
#include "wet1util.h"

class Team {
public:
    //Creates an empty team with no players
    Team(const int teamID, const int points);

    Team(const Team& other) = delete; //bc two teams can't have the same id

    ~Team() = default; //check this!

    StatusType add_player(const std::shared_ptr<Player>& player, const int id, const int goals, const int cards, const bool goalkeeper);
    void remove_player(int playerID);

    bool is_valid() const;

    void update_team_stats(const std::shared_ptr<Player> player, const int goals, const int cards);
    void add_game();
    void update_points(const int k);
    void update_num_goals(const int goals);
    void update_num_cards(const int cards);

    int get_points() const;
    int get_cards() const;
    int get_goals() const;
    int get_num_players() const;
    const std::shared_ptr<Player> get_top_scorer() const;

private:
    int m_id;
    int m_points;
    int m_numPlayers;
    int m_numGoalkeepers;
    int m_numGoals;
    int m_numCards;
    int m_numGames;
    std::shared_ptr<Player> m_topScorer;
    //AVL Tree organized by player ID
    Tree<std::shared_ptr<Player>> m_playersByID;  //Needs to be of the type that Lior is creating with extra field
    //AVL Tree organized by best-scoring players
    Tree<std::shared_ptr<Player>> m_playersByScore; //Needs to be of the type that we create that organizes the tree by 3 variables

    void update_top_player(const std::shared_ptr<Player> player);
};

#endif //TEAMS_H