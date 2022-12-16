//world_cup.cpp


#include "worldcup23a1.h"

world_cup_t::world_cup_t() :
        m_totalNumPlayers(0),
        m_totalTeams(0),
        m_overallTopScorer(nullptr),
        m_teamsByID(),
        m_qualifiedTeams(),
        m_playersByID(),
        m_playersByScore()
{}

world_cup_t::~world_cup_t() //I feel like this should be default
{
    if (m_totalNumPlayers > 0) {
        m_playersByID.erase_data(m_playersByID.m_node);
    }
    if (m_totalTeams > 0) {
        m_teamsByID.erase_data(m_teamsByID.m_node);
    }
    m_overallTopScorer = nullptr;
}


StatusType world_cup_t::add_team(int teamId, int points)
{
    if (teamId <= 0 || points < 0) {
        return StatusType::INVALID_INPUT;
    }
    Team* newTeam;
    try  {
        newTeam = new Team(teamId, points);
        m_teamsByID.insert(newTeam, teamId);
    }
    catch (const std::bad_alloc& e) {
        delete newTeam;
        return StatusType::ALLOCATION_ERROR;
    }
    catch(const InvalidID& e) {
        delete newTeam;
        return StatusType::FAILURE;
    }
    m_totalTeams++;
    return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
    if (teamId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    try {
        Team* team = m_teamsByID.search_and_return_data(teamId);
        if (team->get_num_players()) {
            return StatusType::FAILURE;
        }
        m_teamsByID.remove(teamId);

        delete team;
    }
    catch(const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    m_totalTeams--;
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
        Team* tmpTeam;
        Player* tmpPlayer;
        //Check if there already is a team with this ID.
        //If there is then continue after "catch", otherwise return failure.
        try {
            tmpTeam = m_teamsByID.search_and_return_data(teamId);
        }
        catch (const NodeNotFound& e) {
            return StatusType::FAILURE;
        }
        //Calculate the number of games the player played
        int playerGames = gamesPlayed - tmpTeam->get_games();
        try {
            //The inputs are okay - continue adding player:
            tmpPlayer = new Player(playerId, playerGames, goals, cards, goalKeeper, tmpTeam);
        }
        catch (const std::bad_alloc& e) {
            return StatusType::ALLOCATION_ERROR;
        }
        try {
            m_playersByID.insert(tmpPlayer, playerId);
            //Update top scorers
            m_playersByScore.insert(tmpPlayer, playerId, goals, cards);
            try {
                m_playersByScore.update_closest(playerId, goals, cards);
                if (tmpPlayer->get_closest_left() != nullptr) {
                    m_playersByScore.update_closest(tmpPlayer->get_closest_left()->get_playerId(), tmpPlayer->get_closest_left()->get_goals(), tmpPlayer->get_closest_left()->get_cards());
                }
                if (tmpPlayer->get_closest_right() != nullptr) {
                    m_playersByScore.update_closest(tmpPlayer->get_closest_right()->get_playerId(), tmpPlayer->get_closest_right()->get_goals(), tmpPlayer->get_closest_right()->get_cards());
                }
            }
            catch (const NodeNotFound& e) {}
            m_overallTopScorer = m_playersByScore.search_and_return_max();
            tmpTeam->add_player(tmpPlayer, playerId, goals, cards, goalKeeper);
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
                    m_qualifiedTeams.update_closest(tmpTeam->get_teamID());
                    if (tmpTeam->get_closest_left() != nullptr) {
                        m_qualifiedTeams.update_closest(tmpTeam->get_closest_left()->get_teamID());
                    }
                    if (tmpTeam->get_closest_right() != nullptr) {
                        m_qualifiedTeams.update_closest(tmpTeam->get_closest_right()->get_teamID());
                    }
                }
                catch (const std::bad_alloc& e) {
                    return StatusType::ALLOCATION_ERROR;
                }
                catch (const NodeNotFound& e) {}
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
    Player* tmpPlayer;
    try {
        tmpPlayer = m_playersByID.search_and_return_data(playerId);
    }
    catch (const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    //Remove player from team players by score tree
    Team* tmpTeam = tmpPlayer->get_team();
    Player* closestLeft = tmpPlayer->get_closest_left();
    Player* closestRight = tmpPlayer->get_closest_right();
    try {
        tmpTeam->remove_player(playerId, tmpPlayer->get_goals(), tmpPlayer->get_cards(), tmpPlayer->get_goalkeeper());
        //Remove player from tree of all players
        m_playersByID.remove(playerId);
        //Remove player from overall game tree of players by score
        m_playersByScore.remove(playerId, tmpPlayer->get_goals(), tmpPlayer->get_cards());
    }
    catch (const NodeNotFound& e) {}
    try {
        //Remove team from tree of qualified teams
        if (!(tmpTeam->is_valid())) {
            m_qualifiedTeams.remove(tmpTeam->get_teamID());
            if (tmpTeam->get_closest_left() != nullptr) {
                tmpTeam->get_closest_left()->update_closest_right(tmpTeam->get_closest_right());
            }
            if (tmpTeam->get_closest_right() != nullptr) {
                tmpTeam->get_closest_right()->update_closest_left(tmpTeam->get_closest_left());                
            }
            tmpTeam->update_closest_left(nullptr);
            tmpTeam->update_closest_right(nullptr);
        }
    }
    catch (const NodeNotFound& e) {}
    if (closestRight != nullptr) {
        closestRight->update_closest_left(closestLeft);
    }
    if (closestLeft != nullptr) {
        closestLeft->update_closest_right(closestRight);
    }
    tmpPlayer->update_closest_right(nullptr);
    tmpPlayer->update_closest_left(nullptr);
    //Change top scorer of all players and of team players
    m_overallTopScorer = m_playersByScore.search_and_return_max();
    tmpTeam->update_top_player();
    //Remove one from the counter of all players in game
    m_totalNumPlayers--;
    delete tmpPlayer;
    return StatusType::SUCCESS;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
                                            int scoredGoals, int cardsReceived)
{
    if (playerId <= 0 || gamesPlayed < 0 || scoredGoals < 0 || cardsReceived < 0) {
        return StatusType::INVALID_INPUT;
    }
    Player* tmpPlayer;
    try {
        tmpPlayer = m_playersByID.search_and_return_data(playerId);
    }
    catch (const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    //Pointer to the team the player plays in
    Team* tmpTeam = tmpPlayer->get_team();
    try {
        Player* closestLeftPrevious = tmpPlayer->get_closest_left();
        Player* closestRightPrevious = tmpPlayer->get_closest_right();
        tmpTeam->remove_player_by_score(playerId, tmpPlayer->get_goals(), tmpPlayer->get_cards());
        //Remove player from tree of all scorers
        m_playersByScore.remove(playerId, tmpPlayer->get_goals(), tmpPlayer->get_cards());
        if (closestRightPrevious != nullptr) {
            m_playersByScore.update_closest(closestRightPrevious->get_playerId(), closestRightPrevious->get_goals(), closestRightPrevious->get_cards());
        }
        if (closestLeftPrevious != nullptr) {
            m_playersByScore.update_closest(closestLeftPrevious->get_playerId(), closestLeftPrevious->get_goals(), closestLeftPrevious->get_cards());
        }
        tmpPlayer->update_closest_left(nullptr);
        tmpPlayer->update_closest_right(nullptr);
    }
    catch (const NodeNotFound& e) {}
    tmpPlayer->update_gamesPlayed(gamesPlayed);
    tmpPlayer->update_cards(cardsReceived);
    tmpPlayer->update_goals(scoredGoals);
    try {
        //Update overall game player by score tree and the overall game top scorer
        m_playersByScore.insert(tmpPlayer, tmpPlayer->get_playerId(), tmpPlayer->get_goals(), tmpPlayer->get_cards());
        //Update team - update the team players by score tree
        tmpTeam->insert_player_by_score(tmpPlayer, playerId, tmpPlayer->get_goals(), tmpPlayer->get_cards());
    }
    catch (const InvalidID& e) {}
    catch (const std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
    m_overallTopScorer = m_playersByScore.search_and_return_max();
    //Update the teams total stats and the top scored player of the team
    tmpTeam->update_team_stats(scoredGoals, cardsReceived);
    try {
        m_playersByScore.update_closest(tmpPlayer->get_playerId(), tmpPlayer->get_goals(), tmpPlayer->get_cards());
        Player* closestLeft = tmpPlayer->get_closest_left();
        Player* closestRight = tmpPlayer->get_closest_right();
        if (closestRight != nullptr) {
            m_playersByScore.update_closest(closestRight->get_playerId(), closestRight->get_goals(), closestRight->get_cards());
        }
        if (closestLeft != nullptr) {
            m_playersByScore.update_closest(closestLeft->get_playerId(), closestLeft->get_goals(), closestLeft->get_cards());
        }
    }
    catch (const NodeNotFound& e) {}
    return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
    if (teamId1 == teamId2 || teamId1 <= 0 || teamId2 <= 0) {
        return StatusType::INVALID_INPUT;
    }
    Team* team1;
    Team* team2;
    try {
        team1 = m_qualifiedTeams.search_and_return_data(teamId1);
        team2 = m_qualifiedTeams.search_and_return_data(teamId2);
    }
    catch (const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    this->compete(*team1, *team2);
    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
    if (playerId <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    Player* tmpPlayer;
    try {
        tmpPlayer = m_playersByID.search_and_return_data(playerId);
    }
    catch (const NodeNotFound& e) {
        return output_t<int>(StatusType::FAILURE);
    }
    //Extract the total games played by the player - their personal played games + the total team games
    int totalGames = tmpPlayer->get_gamesPlayed() + tmpPlayer->get_team()->get_games();
    return output_t<int>(totalGames);
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    if (teamId <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    Team* tmpTeam;
    try {
        tmpTeam = m_teamsByID.search_and_return_data(teamId);
    }
    catch (const NodeNotFound& e) {
        return output_t<int>(StatusType::FAILURE);
    }
    return output_t<int>(tmpTeam->get_points());
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{ //Do we need to delete team1 if team2 and team1 merge into team2????
    if (teamId1 == teamId2 || newTeamId <= 0 || teamId1 <= 0 || teamId2 <= 0) {
        return StatusType::INVALID_INPUT;
    }
    bool alreadyExists = true;
    try {
        m_teamsByID.search_specific_id(newTeamId);
    }
    catch (const NodeNotFound& e){
        alreadyExists = false;
    }
    if (newTeamId != teamId1 && newTeamId != teamId2 && alreadyExists) {
            return StatusType::FAILURE;
    }
    Team* team1;
    Team* team2;
    try {
        team1 = m_teamsByID.search_and_return_data(teamId1);
    
    }
    catch (const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    try {
            team2 = m_teamsByID.search_and_return_data(teamId2);
    }
    catch (const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    Team* nTeam = nullptr;
    try {
        nTeam = new Team(newTeamId, team1->get_points() + team2->get_points());
    }
    catch (const std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
    nTeam->Team::unite_teams(team1, team2);
    nTeam->update_team_id(nTeam);
    try {
        m_qualifiedTeams.remove(teamId1);
        if (team1->get_closest_left() != nullptr) {
            team1->get_closest_left()->update_closest_right(team1->get_closest_right());
        }
        if (team1->get_closest_right() != nullptr) {
            team1->get_closest_right()->update_closest_left(team1->get_closest_left());                
        }
        team1->update_closest_right(nullptr);
        team1->update_closest_left(nullptr);
    }
    catch (const NodeNotFound& e) {}
    try {
        m_qualifiedTeams.remove(teamId2);
        if (team2->get_closest_left() != nullptr) {
            team2->get_closest_left()->update_closest_right(team2->get_closest_right());
        }
        if (team2->get_closest_right() != nullptr) {
            team2->get_closest_right()->update_closest_left(team2->get_closest_left());                
        }
        team2->update_closest_right(nullptr);
        team2->update_closest_left(nullptr);
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
            m_qualifiedTeams.update_closest(nTeam->get_teamID());
            if (nTeam->get_closest_left() != nullptr) {
                m_qualifiedTeams.update_closest(nTeam->get_closest_left()->get_teamID());
            }
            if (nTeam->get_closest_right() != nullptr) {
                m_qualifiedTeams.update_closest(nTeam->get_closest_right()->get_teamID());
            }
        }
        catch (const std::bad_alloc& e) {
            return StatusType::ALLOCATION_ERROR;
        }
        catch (const NodeNotFound& e) {}
    }
    delete team1;
    delete team2;
    m_totalTeams--;
    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
    if (teamId == 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    output_t<int> outputFailure(StatusType::FAILURE);
    if (m_totalNumPlayers == 0) {
        return outputFailure;
    }
    if (teamId > 0) {
        Team* tmpTeam;
        try {
            tmpTeam = m_teamsByID.search_and_return_data(teamId);
        }
        catch (const NodeNotFound& e) {
            return outputFailure;
        }
        if (tmpTeam->get_num_players() == 0) {
            return outputFailure;
        }
        return output_t<int>(tmpTeam->get_top_scorer()->get_playerId());
    }
    return output_t<int>(m_overallTopScorer->get_playerId());
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
    if (teamId == 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    if (teamId > 0) {
        Team* tmpTeam;
        try {
            tmpTeam = m_teamsByID.search_and_return_data(teamId);
        }
        catch (const NodeNotFound& e) {
            return output_t<int>(StatusType::FAILURE);
        }
        return output_t<int>(tmpTeam->get_num_players());
    }
    return output_t<int>(m_totalNumPlayers);
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
    if (output == nullptr || teamId == 0) {
        return StatusType::INVALID_INPUT;
    }
    //If there are no players in the game, return failure
    if (m_totalNumPlayers == 0) {
        return StatusType::FAILURE;
    }
    //If the players of a certain team is requested
    if (teamId > 0) {
        Team* tmpTeam;
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
    m_playersByScore.get_all_data(output);
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
    Team* tmpTeam;
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
    if (closestPlayerId == -1) {
        return outputFailure;
    }
    return output_t<int>(closestPlayerId);
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId) //check where need to send allocation error from
{
   // m_qualifiedTeams.print_tree();
    if (maxTeamId < 0 || minTeamId < 0 || maxTeamId < minTeamId) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    if (m_qualifiedTeams.m_node->get_height() < 0) {
        return output_t<int>(StatusType::FAILURE);
    }
    //Find number of teams invovled

   // std::cout << "minTeamId is " << minTeamId << " Max is " << maxTeamId << std::endl;
    int num = m_qualifiedTeams.m_node->numOfTeams(minTeamId, maxTeamId);
   // std::cout << "Num of teams " << num << std::endl;
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
        return output_t<int>(StatusType::ALLOCATION_ERROR);
    }
    //Fill in teams according to their order
    m_qualifiedTeams.m_node->addTeams(teams, minTeamId, maxTeamId);
    for (int i = 0; i < num-1; i++) {
        (teams+i)->update_closest_right(teams+i+1);
        (teams+i+1)->update_closest_left(teams+i);
    }
    (teams+num-1)->update_closest_right(nullptr);
    teams->update_closest_left(nullptr);
    //Team* tmp = teams;
    //while (tmp != nullptr) {
        //std::cout << "Team: " << tmp->get_teamID() << std::endl;
      //  tmp = tmp->get_closest_right();
    //}
    if (num == 1) {
        int winner = teams->get_teamID();
        delete[] teams;
        return output_t<int>(winner);
    }
    //In a recursive function, go over every pair in the array and send to play match.
    //Throughout the recursive function, combine teams where needed and set the open space equal to nullptr
    Team* first = teams;
    do {
        first = knockout_games(first);
    } while (first->get_closest_right() != nullptr);
    int winnerID = first->get_teamID();
    delete[] teams;
  //  m_qualifiedTeams.print_tree();
    return output_t<int>(winnerID);
}


//-------------------------------------------Helper Functions----------------------------------------------

int world_cup_t::compete(Team& team1, Team& team2) {
    int point1 = team1.get_points() + team1.get_goals() - team1.get_cards();\
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

Team* world_cup_t::knockout_games(Team* teams) { //return first
    if (teams->get_closest_right() == nullptr) {
        return teams;
    }
    Team* second = teams->get_closest_right();
    if (second->get_closest_right() != nullptr) {
        knockout_games(second->get_closest_right());
    }
    int winnerID = compete(*teams, *second);
    if (winnerID == teams->get_teamID()) {
        teams->knockout_unite(*teams, *second);
        teams->update_closest_right(second->get_closest_right());
        if (second->get_closest_right() != nullptr) {
            second->get_closest_right()->update_closest_left(teams);
        }
    }
    else if (winnerID == second->get_teamID()) {
        teams->knockout_unite(*second, *teams);
        second->update_closest_left(teams->get_closest_left());
        if (teams->get_closest_left() != nullptr) {
            teams->get_closest_left()->update_closest_right(second);
        }
        teams = second;
    }
    else {
        if (teams->get_teamID() > second->get_teamID()) {
            teams->knockout_unite(*teams, *second);
            teams->update_closest_right(second->get_closest_right());
            if (second->get_closest_right() != nullptr) {
                second->get_closest_right()->update_closest_left(teams);
            }
        }
        else {
            teams->knockout_unite(*second, *teams);
            second->update_closest_left(teams->get_closest_left());
            if (teams->get_closest_left() != nullptr) {
                teams->get_closest_left()->update_closest_right(second);
            }
            teams = second;
        }
    }
    return teams;
}