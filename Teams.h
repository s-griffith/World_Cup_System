#ifndef TEAMS_H
#define TEAMS_H

class Player;

#include "Player.h"
#include "AVLTree.h"
#include <memory>
#include "wet1util.h"
#include "AVLMultiVariable.h"
#include "AVLTreeExtraPointer.h"

class Team {
public:
    Team() = default; //maybe can get rid of this at the end
    //Creates an empty team with no players
    Team(const int teamID, const int points);

    Team(const Team& other) = delete; //bc two teams can't have the same id
    Team& operator=(const Team& other);

    ~Team() = default; //check this!


    void update_team_id(const std::shared_ptr<Team>& team);
    void update_team_stats(const int goals, const int cards);


    //Getters for the team's stats
    int get_points() const;
    int get_cards() const;
    int get_goals() const;
    int get_num_players() const;
    int get_games() const;
    const std::shared_ptr<Player>& get_top_scorer() const;
    int get_teamID() const;
    void print_team();
    //Helper functions for World Cup
    void unite_teams(std::shared_ptr<Team> team1, std::shared_ptr<Team> team2);
    int get_closest_team_player(const int playerId);
    void get_all_team_players(int* const output);
    StatusType add_player(const std::shared_ptr<Player>& player, const int id, const int goals, const int cards, const bool goalkeeper, ComplexNode<std::shared_ptr<Player>>* otherTree);
    void remove_player(const int goals, const int cards, const int playerID);
    bool is_valid() const;
   // Team knockout_copy();
    void knockout_setID();
    void knockout_unite(Team& winner, Team& loser);
    void remove_player_by_score(const int goals, const int cards, const int id);
    void insert_player_by_score(const std::shared_ptr<Player>& player, const int id, const int goals, const int cards);
    //Helper functions to update teams stats
    void add_game();
    void update_points(const int k);
    void update_num_goals(const int goals);
    void update_num_cards(const int cards);
    void update_top_player();

private:
    int m_id;
    int m_points;
    int m_numPlayers;
    int m_numGoalkeepers;
    int m_numGoals;
    int m_numCards;
    int m_numGames;
    //AVL Tree organized by player ID
    TreeExtraPointer<std::shared_ptr<Player>> m_playersByID;
    //AVL Tree organized by best-scoring players
    MultiTree<std::shared_ptr<Player>> m_playersByScore;
    std::shared_ptr<Player> m_topScorer;
};

#endif //TEAMS_H