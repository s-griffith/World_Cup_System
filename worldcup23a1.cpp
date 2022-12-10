#include "worldcup23a1.h"

world_cup_t::world_cup_t() :
    m_totalNumPlayers(0),
    m_overallTopScorer(nullptr),
    m_teamsByID(),
    m_qualifiedTeams(),
    m_playersByID(),
    m_playersByScore()
{}

world_cup_t::~world_cup_t() //I feel like this should be default
{
	// TODO: Your code goes here
}


StatusType world_cup_t::add_team(int teamId, int points)
{
    if (teamId <= 0 || points < 0) {
        return StatusType::INVALID_INPUT;
    }
    try  {
        std::shared_ptr<Team> t = std::make_shared<Team>(teamId, points);
	    m_teamsByID.insert(t, teamId);
    }
    catch (std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
    catch(InvalidID& e) {
        return StatusType::FAILURE;
    }
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    try {
        std::shared_ptr<Team> team = m_teamsByID.search_and_return_data(teamId);
        if (team->get_num_players()) {
            return StatusType::FAILURE;
        }
        m_teamsByID.remove(teamId);
    }
    catch(NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
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
        std::shared_ptr<Player> tmpPlayer = std::make_shared<Player>(playerId, playerGames, goals, cards, goalKeeper, tmpTeam);
        try {
            m_playersByID.insert(tmpPlayer, playerId);
            //Update top scorers************************************************************************************************************************
            m_playersByScore.insert(tmpPlayer, playerId, goals, cards);
            m_overallTopScorer = m_playersByScore.search_and_return_max();
            //Add player to team trees + update team stats + update top team scorer*********************************************************************
            tmpTeam->add_player(tmpPlayer, playerId, goals, cards, goalKeeper, &(m_playersByScore.search_specific_id(playerId, goals, cards)));
        }
        catch (const std::bad_alloc& e) {
            return StatusType::ALLOCATION_ERROR;
        }
        catch (const InvalidID& e) {
            return StatusType::FAILURE;
        }
        //Add team to qualified team tree if the team is valid (more than 11 players + at least one goalkeeper)
        if (tmpTeam->is_valid()) {
            try {
                m_qualifiedTeams.search_and_return_data(teamId);
            }
            catch (const NodeNotFound& e) {
                try {
                    m_qualifiedTeams.insert(tmpTeam, teamId);
                }
                catch (const std::bad_alloc& e) {
                    return StatusType::ALLOCATION_ERROR;
                }
            }
        }
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
    //Remove player from team players by score tree
    std::shared_ptr<Team> tmpTeam = tmpPlayer->get_team();
    tmpTeam->remove_player(tmpPlayer->get_goals(), tmpPlayer->get_cards(), playerId);
    //Remove team from tree of qualified teams
    if (!(tmpTeam->is_valid())) {
        try {
            m_qualifiedTeams.remove(tmpTeam->get_teamID());
        }
        catch (const NodeNotFound& e) {}
    }
    //Remove player from overall game tree of players by score
    m_playersByScore.remove(tmpPlayer->get_goals(), tmpPlayer->get_cards(), playerId);
    //Change top scorer of all players and of team players
    m_overallTopScorer = m_playersByScore.search_and_return_max();
    tmpTeam->update_top_player();
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
    //Pointer to the team the player plays in
    std::shared_ptr<Team> tmpTeam = tmpPlayer->get_team();
    tmpTeam->remove_player_by_score(tmpPlayer->get_goals(), tmpPlayer->get_cards(), playerId);
    //Remove player from tree of all scorers
    m_playersByScore.remove(tmpPlayer->get_goals(), tmpPlayer->get_cards(), playerId);
    tmpPlayer->update_gamesPlayed(gamesPlayed);
    tmpPlayer->update_cards(cardsReceived);
    tmpPlayer->update_goals(scoredGoals);
    //Update overall game player by score tree and the overall game top scorer
    m_playersByScore.insert(tmpPlayer, tmpPlayer->get_playerId(), tmpPlayer->get_goals(), tmpPlayer->get_cards());
    m_overallTopScorer = m_playersByScore.search_and_return_max();
    //Update team - update the team players by score tree
    tmpTeam->insert_player_by_score(tmpPlayer, playerId, tmpPlayer->get_goals(), tmpPlayer->get_cards());
    //Update the teams total stats and the top scored player of the team
    tmpTeam->update_team_stats(scoredGoals, cardsReceived);
    return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
    if (teamId1 == teamId2 || teamId1 <= 0 || teamId2 <= 0) {
        return StatusType::INVALID_INPUT;
    }
    std::shared_ptr<Team> team1 = nullptr;
    std::shared_ptr<Team> team2 = nullptr;
    try {
        team1 = m_qualifiedTeams.search_and_return_data(teamId1);
        team2 = m_qualifiedTeams.search_and_return_data(teamId2);
    }
    catch (NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    this->compete(*team1, *team2);
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
    if (playerId <= 0) {
        output_t<int> outputInvalid(StatusType::INVALID_INPUT);
        return outputInvalid;
    }
    std::shared_ptr<Player> tmpPlayer;
    try {
        tmpPlayer = m_playersByID.search_and_return_data(playerId);
    }
    catch (const NodeNotFound& e) {
        output_t<int> outputFailure(StatusType::FAILURE);
        return outputFailure;
    }
    //Extract the total games played by the player - their personal played games + the total team games
    std::shared_ptr<Team> tmpTeam = tmpPlayer->get_team();
    int totalGames = tmpPlayer->get_gamesPlayed() + tmpTeam->get_games();
    output_t<int> newOutput(totalGames);
    return newOutput;
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    if (teamId <= 0) {
        output_t<int> outputInvalid(StatusType::INVALID_INPUT);
        return outputInvalid;
    }
    std::shared_ptr<Team> tmpTeam;
    try {
        tmpTeam = m_teamsByID.search_and_return_data(teamId);
    }
    catch (const NodeNotFound& e) {
        output_t<int> outputFailure(StatusType::FAILURE);
        return outputFailure;
    }
    output_t<int> newOutput(tmpTeam->get_points());
    return newOutput;
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{ //Do we need to delete team1 if team2 and team1 merge into team2????
    if (teamId1 == teamId2 || newTeamId <= 0 || teamId1 <= 0 || teamId2 <= 0) {
        return StatusType::INVALID_INPUT;
    }
    try {
        m_teamsByID.search_specific_id(newTeamId);
    }
    catch (NodeNotFound& e){
        if (newTeamId != teamId1 && newTeamId != teamId2) {
            return StatusType::FAILURE;
        }
    }
    std::shared_ptr<Team> team1;
    std::shared_ptr<Team> team2;
    try {
        team1 = m_teamsByID.search_and_return_data(teamId1);
        team2 = m_teamsByID.search_and_return_data(teamId2);
    }
    catch (NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    std::shared_ptr<Team> nTeam = std::make_shared<Team>(newTeamId, team1->get_points() + team2->get_points());
    nTeam->Team::unite_teams(team1, team2);
    nTeam->update_team_id(nTeam);
    try {
        m_qualifiedTeams.remove(teamId1);
    }
    catch (const NodeNotFound& e) {}
    try {
        m_qualifiedTeams.remove(teamId2);
    }
    catch (const NodeNotFound& e) {}
    m_teamsByID.remove(teamId1);
    m_teamsByID.remove(teamId2);
    try {
        m_teamsByID.insert(nTeam, newTeamId);
    }
    catch (const std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
    if (nTeam->is_valid()) {
        try {
            m_qualifiedTeams.insert(nTeam, newTeamId);
        }
        catch (std::bad_alloc& e) {
            return StatusType::ALLOCATION_ERROR;
        }
    }
    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
    if (teamId == 0) {
        output_t<int> outputInvalid(StatusType::INVALID_INPUT);
        return outputInvalid;
    }
    output_t<int> outputFailure(StatusType::FAILURE);
    if (teamId > 0) {
        std::shared_ptr<Team> tmpTeam;
        try {
            tmpTeam = m_teamsByID.search_and_return_data(teamId);
        }
        catch (const NodeNotFound& e) {
            return outputFailure;
        }
        if (tmpTeam->get_num_players() == 0) {
            return outputFailure;
        }
        output_t<int> newOutput(tmpTeam->get_top_scorer()->get_playerId());
        return newOutput;
    }
    if (m_totalNumPlayers == 0) {
        return outputFailure;
    }
    output_t<int> newOutput(m_overallTopScorer->get_playerId());
    return newOutput;
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
    if (teamId == 0) {
        output_t<int> outputInvalid(StatusType::INVALID_INPUT);
        return outputInvalid;
    }
    if (teamId > 0) {
        std::shared_ptr<Team> tmpTeam;
        try {
            tmpTeam = m_teamsByID.search_and_return_data(teamId);
        }
        catch (const NodeNotFound& e) {
            output_t<int> outputFailure(StatusType::FAILURE);
            return outputFailure;
        }
        output_t<int> newOutput(tmpTeam->get_num_players());
        return newOutput;
    }
    output_t<int> newOutput(m_totalNumPlayers);
    return newOutput;
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
	if (output == nullptr || teamId == 0) {
        return StatusType::INVALID_INPUT;
    }
    //If the players of a certain team is requested
    if (teamId > 0) {
        std::shared_ptr<Team> tmpTeam;
        try {
            tmpTeam = m_teamsByID.search_and_return_data(teamId);
        }
        catch (const NodeNotFound& e) {
            return StatusType::FAILURE;
        }
        if (tmpTeam-> get_num_players() == 0) {
            return StatusType::FAILURE;
        }
        //Get all the players of the specified team
        tmpTeam->get_all_team_players(output);
        return StatusType::SUCCESS;
    }
    //If the total players of the games is requested
    //If there are no players in the game, return failure
    if (m_totalNumPlayers == 0) {
        return StatusType::FAILURE;
    }
    m_playersByID.get_all_data(output);
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
    if (playerId <= 0 || teamId <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    output_t<int> outputFailure(StatusType::FAILURE);
    //If there is only one or no players in the entire game, return failure
    if (m_totalNumPlayers <= 1) {
        return outputFailure;
    }
    std::shared_ptr<Team> tmpTeam;
    try {
        tmpTeam = m_teamsByID.search_and_return_data(teamId);
    }
    catch (const NodeNotFound& e) {
        return outputFailure;
    }
    int closestPlayerId = 0;
    //Get the closest team player
    try {
        closestPlayerId = tmpTeam->get_closest_team_player(playerId);
    }
    catch (const NodeNotFound& e) {
        return outputFailure;
    }
    return output_t<int>(closestPlayerId);
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId) //check where need to send allocation error from
{
    if (maxTeamId < 0 || minTeamId < 0 || maxTeamId < minTeamId) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    //Find number of teams invovled
    int num = m_qualifiedTeams.m_node->numOfTeams(minTeamId, maxTeamId);
    //If there are no qualified teams, return failure
    if (num == 0) {
        return output_t<int>(StatusType::FAILURE);
    }
    //Create an array for the qualified teams in the range
    Team* teams = nullptr;
    try {
        teams = new Team[num*sizeof(Team)];
    }
    catch (const std::bad_alloc& e) {
        delete[] teams;
        throw e;
    }   
    //Fill in teams according to their order
    m_qualifiedTeams.m_node->addTeams(teams, minTeamId, maxTeamId);
    if (num == 1) {
        return output_t<int>(teams->get_teamID());
    }
    //In a recursive function, go over every pair in the array and send to play match.
    //Throughout the recursive function, combine teams where needed and set the open space equal to nullptr
    knockout_games(teams, num, num);
    Team* temp = teams;
    while (temp == nullptr) {
        temp = ++teams;
    }
    int winnerID = temp->get_teamID();
    delete[] teams;
	return output_t<int>(winnerID);
}


//-------------------------------------------Helper Functions----------------------------------------------

int world_cup_t::compete(Team& team1, Team& team2) {
    int point1 = team1.get_points() + team1.get_goals() - team1.get_cards();
    int point2 = team2.get_points() + team2.get_goals() - team2.get_cards();
    int winnerID = 0;
    if (point1 > point2) {
        team1.update_points(3); //---------------------------------------------can we make a define for this-------------------
        winnerID = team1.get_teamID();
    }
    else if (point1 == point2) {
        team1.update_points(1);
        team2.update_points(1);
        winnerID = 0; //a tie - b/c no team id is equal to 0
    }
    else {
        team2.update_points(3);
        winnerID = team2.get_teamID();
    }
    team1.add_game();
    team2.add_game();
    return winnerID;
}

void world_cup_t::knockout_games(Team* teams, int numTeams, const int size) {
    if (numTeams <= 1) { //stop because there's an uneven number of teams
        return; //what to return here
    }
    int currIndex1 = 0;
    Team* first = teams;
    while (first->get_teamID() < 0 && currIndex1 < size) {
        currIndex1++;
        first = teams + currIndex1;
    }
    int currIndex2 = currIndex1+1;
    Team* second = teams+currIndex2;
    while (second->get_teamID() < 0 && currIndex2 < size) {
        currIndex2++;
        second = teams + currIndex2;
    }
    if (currIndex2 >= size) { //This happens only when there is a single team left in the array, so continue the process with the other pairs
        return;
    }
    knockout_games(teams+currIndex2, numTeams-2, size-currIndex2); //might need to be currIndex-1
    int winnerID = compete(*first, *second);
    if (winnerID == first->get_teamID()) {
        first->knockout_unite(*first, *second);
        (teams+currIndex2)->knockout_setID();
        }
    else if (winnerID == second->get_teamID()) {
        first->knockout_unite(*second, *first);
        (teams+currIndex1)->knockout_setID();
    }
    else {
        if (first->get_teamID() > second->get_teamID()) {
            first->knockout_unite(*first, *second);
            (teams+currIndex2)->knockout_setID();
        }
        else {
            first->knockout_unite(*second, *first);
            (teams+currIndex1)->knockout_setID();
        }
    }
    return;
}
