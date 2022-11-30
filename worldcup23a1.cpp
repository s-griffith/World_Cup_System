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
    if (playerId <= 0 || teamId <= 0 || gamesPlayed < 0 || goals < 0 || cards < 0) {
        return StatusType::INVALID_INPUT;
    }
    if ((gamesPlayed == 0) && (goals > 0 || cards > 0)) {
        return StatusType::INVALID_INPUT;
    }
    //Check if there already is a player with this ID.
    //If not then continue in "catch", otherwise return failure.
    try {
        m_playersByID.search_and_return_data(playerId);
    }
    catch (const NodeNotFound& e) {
        std::shared_ptr<Team> tmpTeam;
        //Check if there already is a team with this ID.
        //If there is then continue after "catch", otherwise return failure.
        try {
            tmpTeam = m_teamsByID.search_and_return_data(teamId);
        }
        catch (const NodeNotFound& e) {
            return StatusType::FAILURE;
        }
        //Update the number of games the player played - add get_games() to teams***********************************************************************
        int playerGames = gamesPlayed - tmpTeam->get_games();
        //The inputs are okay - continue adding player:
        std::shared_ptr<Player> tmpPlayer(new Player(playerId, playerGames, goals, cards, goalKeeper, tmpTeam));
        m_playersByID.insert(tmpPlayer, playerId);
        //Add player to team trees + update team stats*******************************************************************************
        tmpTeam->add_player(tmpPlayer, playerId, goals, cards, goalKeeper);
        //Add team to qualified team tree if the team is valid (more than 11 players + at least one goalkeeper)
        if (tmpTeam->is_valid()) {
            try {
                m_qualifiedTeams.search_and_return_data(teamId);
            }
            catch (const NodeNotFound& e) {
                m_qualifiedTeams.insert(tmpTeam, teamId);
            }
        }
        //Update top scorers
        m_playersByScore.update_tree_by_id(playerId);//***************************************************************************************
        //Add one to total num players (in entire game)
        m_totalNumPlayers++;
        return StatusType::SUCCESS;
    }
	return StatusType::FAILURE;
}

StatusType world_cup_t::remove_player(int playerId)
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
    //Remove player from team trees + update team stats**************************************************************************
    std::shared_ptr<Team> tmpTeam = tmpPlayer->get_team();
    tmpTeam->remove_player(playerId);
    if (!(tmpTeam->is_valid())) {
        try {
            //Remove team from tree of qualified teams***************************************************************************
            m_qualifiedTeams.remove(tmpTeam.get_teamID());
        }
        catch (const NodeNotFound& e) {}
    }
    //Remove player from tree of all scorers*************************************************************************************
    m_playersByScore.remove(tmpPlayer->get_goals(), tmpPlayer->get_cards(), playerId);
    //Change top scorer if the requested player is the top scorer****************************************************************
    if (m_overallTopScorer->get_playerId() == tmpPlayer->get_playerId()) {
        m_overallTopScorer = m_playersByScore.search_and_return_max();
    }
    //Remove player from tree of all players
    m_playersByID.remove(playerId);
    //Remove one from the counter of all players in game
    m_totalNumPlayers--;
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
    tmpPlayer->update_gamesPlayed(gamesPlayed);
    tmpPlayer->update_cards(cardsReceived);
    tmpPlayer->update_goals(scoredGoals);
    m_playersByScore.update_tree_by_id(playerId);//*********************************************************************************
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
        if (tmpTeam->get_num_players() == 0) {
            return StatusType::FAILURE;
        }
        output_t<int> newOutput(tmpTeam->get_top_scorer()->get_playerId());
        return newOutput;
    }
    if (m_totalNumPlayers == 0) {
        return StatusType::FAILURE;
    }
    output_t<int> newOutput(m_overallTopScorer->get_playerId());
    return newOutput;
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
    if (playerId <= 0 || teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    //If there are less than two players in the entire game, return failure
    if (m_totalNumPlayers <= 1) {
        return StatusType::FAILURE;
    }
    std::shared_ptr<Team> tmpTeam;
    try {
        tmpTeam = m_teamsByID.search_and_return_data(teamId);
    }
    catch (const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    int closestPlayerId = 0;
    try {
        closestPlayerId = tmpTeam.get_closest_team_player(playerId);
    }
    catch (const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    output_t<int> newOutput(closestPlayerId);
    return newOutput;
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
	// TODO: Your code goes here
	return 2;
}


//-------------------------------------------Helper Functions----------------------------------------------