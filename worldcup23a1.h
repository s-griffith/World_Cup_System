// 
// 234218 Data Structures 1.
// Semester: 2023A (winter).
// Wet Exercise #1.
// 
// Recommended TAB size to view this file: 8.
// 
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT erase or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
// 

#ifndef WORLDCUP23A1_H_
#define WORLDCUP23A1_H_

#include "wet1util.h"
#include "Teams.h"
#include "Player.h"
#include "AVLTree.h"

class world_cup_t {
private:
    //Total number of players throughout the entire game
    int m_totalNumPlayers;
    //Shared pointer to the player with the top stats (goals + cards + ID)
    std::shared_ptr<Player> m_overallTopScorer;
    //Tree of shared pointers of the type team, with all the teams in the game sorted by their ID
    Tree<std::shared_ptr<Team>> m_teamsByID;
    //Tree of shared pointers of the type team, with all the teams in the game with more than 11 players, and at least
    //      one goalkeeper, sorted by the team ID
    Tree<std::shared_ptr<Team>> m_qualifiedTeams;
    //Tree of shared pointers of the type player, with all the players in the game sorted by their ID
    Tree<std::shared_ptr<Player>> m_playersByID;
    //Tree of shared pointers of the type player, with all the players in the game sorted by their goals, cards and ID
    Tree<std::shared_ptr<Player>> m_playersByScore;

	
public:
	// <DO-NOT-MODIFY> {
	
	world_cup_t();
	virtual ~world_cup_t();
	
	StatusType add_team(int teamId, int points);
	
	StatusType remove_team(int teamId);
	
	StatusType add_player(int playerId, int teamId, int gamesPlayed,
	                      int goals, int cards, bool goalKeeper);
	
	StatusType remove_player(int playerId);
	
	StatusType update_player_stats(int playerId, int gamesPlayed,
	                                int scoredGoals, int cardsReceived);
	
	StatusType play_match(int teamId1, int teamId2);
	
	output_t<int> get_num_played_games(int playerId);
	
	output_t<int> get_team_points(int teamId);
	
	StatusType unite_teams(int teamId1, int teamId2, int newTeamId);
	
	output_t<int> get_top_scorer(int teamId);
	
	output_t<int> get_all_players_count(int teamId);
	
	StatusType get_all_players(int teamId, int *const output);
	
	output_t<int> get_closest_player(int playerId, int teamId);
	
	output_t<int> knockout_winner(int minTeamId, int maxTeamId);
	
	// } </DO-NOT-MODIFY>
};

#endif // WORLDCUP23A1_H_
