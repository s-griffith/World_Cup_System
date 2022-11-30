#include "worldcup23a1.h"

world_cup_t::world_cup_t()
{
	// TODO: Your code goes here
}

world_cup_t::~world_cup_t()
{
	// TODO: Your code goes here
}


StatusType world_cup_t::add_team(int teamId, int points)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
	// TODO: Your code goes here
	return StatusType::FAILURE;
}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
                                   int goals, int cards, bool goalKeeper)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
                                        int scoredGoals, int cardsReceived)
{
    if (playerId <= 0 || gamesPlayed < 0 || scoredGoals < 0 || cardsReceived < 0) {
        return StatusType::INVALID_INPUT;
    }
    std::shared_ptr<Player> tmpPlayer;
    try {
        tmpPlayer = m_playersByID.search_and_return_data(playerId);
    }
    catch (const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    (*tmpPlayer).update_gamesPlayed(gamesPlayed);
    (*tmpPlayer).update_cards(cardsReceived);
    (*tmpPlayer).update_goals(scoredGoals);
    update_top_scorers(playerId);//***************************************************************************************
    std::shared_ptr<Team> tmpTeam = (*tmpPlayer).get_team();
    //Update the teams total stats and the top scored player tree + pointer*****************************************************
    tmpTeam->update_team_stats(tmpPlayer, scoredGoals, cardsReceived);
    return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
    if (playerId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    std::shared_ptr<Player> tmpPlayer;
    try {
        tmpPlayer = m_playersByID.search_and_return_data(playerId);
    }
    catch (const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    //Extract the total goals of the player - their personal goals + the total team goals
    std::shared_ptr<Team> tmpTeam = (*tmpPlayer).get_team();
    int totalGames = tmpPlayer->get_gamesPlayed() + tmpTeam->get_goals();
    output_t<int> newOutput(totalGames);
    return newOutput;
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    std::shared_ptr<Team> tmpTeam;
    try {
        tmpTeam = m_teamsByID.search_and_return_data(teamId);
    }
    catch (const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    output_t<int> newOutput(tmpTeam->get_points());
    return newOutput;
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
	// TODO: Your code goes here
	return 2008;
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
    if (teamId == 0) {
        return StatusType::INVALID_INPUT;
    }
    if (teamId > 0) {
        std::shared_ptr<Team> tmpTeam;
        try {
            tmpTeam = m_teamsByID.search_and_return_data(teamId);
        }
        catch (const NodeNotFound& e) {
            return StatusType::FAILURE;
        }
        output_t<int> newOutput(tmpTeam->get_num_players());
        return newOutput;
    }
    output_t<int> newOutput(m_totalNumPlayers);
    return newOutput;
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
	// TODO: Your code goes here
    output[0] = 4001;
    output[1] = 4002;
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
	// TODO: Your code goes here
	return 1006;
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
	// TODO: Your code goes here
	return 2;
}

