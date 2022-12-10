#include "Teams.h"

//-------------------------------------Constructors----------------------------
Team::Team(const int teamID, const int points) :
    m_id(teamID),
    m_points(points),
    m_numPlayers(0),
    m_numGoalkeepers(0),
    m_numGoals(0),
    m_numCards(0),
    m_numGames(0),
    m_topScorer(nullptr),
    m_playersByID(),
    m_playersByScore()
{}

Team& Team::operator=(const Team& other) {
    m_id = other.m_id;
    m_numCards = other.m_numCards;
    m_numGames = other.m_numGames;
    m_numGoalkeepers = other.m_numGoalkeepers;
    m_numGoals = other.m_numGoals;
    m_numPlayers = other.m_numPlayers;
    m_points = other.m_points;
    return *this;
}


//-------------------------------------Getters----------------------------
int Team::get_points() const {
    return m_points;
}

int Team::get_cards() const {
    return m_numCards;
}

int Team::get_goals() const {
    return m_numGoals;
}

int Team::get_num_players() const {
    return m_numPlayers;
}

int Team::get_games() const {
    return m_numGames;
}

const std::shared_ptr<Player> Team::get_top_scorer() const {
    return m_topScorer;
}

int Team::get_teamID() const  {
    return m_id;
}

//-------------------------------------Helper Functions for WorldCup----------------------------

void Team::unite_teams(std::shared_ptr<Team> team1, std::shared_ptr<Team> team2) {
    this->m_numCards = team1->m_numCards + team2->m_numCards;
    this->m_numGames = team1->m_numGames + team2->m_numGames;
    this->m_numGoalkeepers = team1->m_numGoalkeepers + team2->m_numGoalkeepers;
    this->m_numGoals = team1->m_numGoals + team2->m_numGoals;
    this->m_numPlayers = team1->m_numPlayers + team2->m_numPlayers;

    this->m_playersByID.Tree::mergeNodes(team1->m_playersByID.m_node);
    this->m_playersByScore.Tree::mergeNodes(team1->m_playersByScore.m_node);
    this->m_playersByID.Tree::mergeNodes(team2->m_playersByID.m_node);
    this->m_playersByScore.Tree::mergeNodes(team2->m_playersByScore.m_node);
    
    this->m_topScorer = m_playersByScore.search_and_return_max();
    //Qualified teams
}

int Team::get_closest_team_player(const int playerId) {
    std::shared_ptr<Player> tmpPlayer;
    try {
        tmpPlayer = m_playersByID.get_closest(playerId);
    }
    catch (const NodeNotFound& e) {
        throw e;
    }
    return tmpPlayer->get_playerId();
}

void Team::get_all_team_players(int* const output) {
    m_playersByID.get_all_data(output);
}

StatusType Team::add_player(const std::shared_ptr<Player>& player, const int id, const int goals, const int cards, const bool goalkeeper){
    try {
        m_playersByID.insert(player, id);
        m_playersByScore.insert(player, id, goals, cards);
    }
    catch (const InvalidID& e) {
        return StatusType::FAILURE;
    }
    update_num_goals(goals);
    update_num_cards(cards);
    m_numPlayers++;
    if (goalkeeper) {
         m_numGoalkeepers++;
    }
    return StatusType::SUCCESS;
}

void Team::remove_player(const int goals, const int cards, const int playerID){
    //Do the cards he had get subtracted from the team's total? Goals? etc.
    m_playersByID.remove(playerID);
    m_playersByScore.remove(goals, cards, playerID);
}

bool Team::is_valid() const{
    if (m_numPlayers >= 11 && m_numGoalkeepers >= 1) {
        return true;
    }
    return false;
}

void Team::knockout_unite(Team& winner, Team& loser) {
    winner.m_numCards += loser.m_numCards;
    winner.m_numGames += loser.m_numGames;
    winner.m_numGoalkeepers += loser.m_numGoalkeepers;
    winner.m_numGoals += loser.m_numGoals;
    winner.m_numPlayers += loser.m_numPlayers;
    if (winner.m_topScorer->get_goals() < loser.m_topScorer->get_goals()) {
        winner.m_topScorer = loser.m_topScorer;
    }
}

/*Team Team::knockout_copy() {
    Team copy;
    copy.m_id = m_id;
    copy.m_numCards = m_numCards;
    copy.m_numGames = m_numGames;
    copy.m_numGoalkeepers = m_numGoalkeepers;
    copy.m_numGoals = m_numGoals;
    copy.m_numPlayers = m_numPlayers;
    copy.m_points = m_points;
    return copy;
}
*/
void Team::knockout_setID() {
    this->m_id *= -1;
}

void Team::remove_player_by_score(const int goals, const int cards, const int id) {
    m_playersByScore.remove(goals, cards, id);
}

void Team::insert_player_by_score(const std::shared_ptr<Player>& player, const int id, const int goals, const int cards) {
    m_playersByScore.insert(player, id, goals, cards);
}

void Team::print_team() {
    m_playersByScore.print_tree();
}

//-------------------------------------Update Stats Functions----------------------------
void Team::update_team_stats(const int goals, const int cards){
    update_num_goals(goals);
    update_num_cards(cards);
    update_top_player();
}

void Team::add_game() {
    m_numGames++;
}
    
void Team::update_points(const int k) {
    m_points += k;
}
    
void Team::update_num_goals(const int goals) {
    m_numGoals += goals;
}

void Team::update_num_cards(const int cards) {
    m_numCards += cards;
}

//Private helper function
void Team::update_top_player() {
    m_topScorer = m_playersByScore.search_and_return_max();
}

void Team::update_team_id(const std::shared_ptr<Team>& team) {
    m_playersByID.m_node->inorderWalkTeamID(team);
}
