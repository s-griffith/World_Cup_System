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

    ~Team() = default; //check this!

    StatusType add_player(const std::shared_ptr<Player>& player, const int id, const int goals, const int cards, const bool goalkeeper);
    void remove_player(const int goals, const int cards, const int playerID);

    bool is_valid() const;

    void update_team_stats(const std::shared_ptr<Player> player, const int goals, const int cards);
    void remove_player_by_score(const int goals, const int cards, const int id);
    void insert_player_by_score(const std::shared_ptr<Player>& player, const int goals, const int cards, const int id);
    void remove_player_by_score(const std::shared_ptr<Player>& player, const int id, const int goals, const int cards, const bool goalkeeper);
    void add_game();
    void update_points(const int k);
    void update_num_goals(const int goals);
    void update_num_cards(const int cards);

    int get_points() const;
    int get_cards() const;
    int get_goals() const;
    int get_num_players() const;
    int get_games() const;
    const std::shared_ptr<Player> get_top_scorer() const;
    int get_teamID() const;

    void unite_teams(std::shared_ptr<Team> team1, std::shared_ptr<Team> team2);
    int get_closest_team_player(const int playerId);
    void get_all_team_players(int* const output);

    std::shared_ptr<Team> knockout_copy(std::shared_ptr<Team> teamCopy);
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
    TreeExtraPointer<std::shared_ptr<Player>> m_playersByID;
    //AVL Tree organized by best-scoring players
    MultiTree<std::shared_ptr<Player>> m_playersByScore;

    void update_top_player(const std::shared_ptr<Player> player);
};

#endif //TEAMS_H