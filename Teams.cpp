#include "Teams.h"

//-------------------------------------Constructors----------------------------
Team::Team(const int teamID, const int points) : m_id(teamID), m_points(points) {}

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
    if (team1->m_topScorer->get_goals() > team2->m_topScorer->get_goals()) {
        this->m_topScorer = team1->m_topScorer;
    }
    else {
        this->m_topScorer = team2->m_topScorer;
    }
    this->m_playersByID.Tree::mergeNodes(team1->m_playersByID.m_node);
    this->m_playersByScore.Tree::mergeNodes(&(team1->m_playersByScore.m_node));
    this->m_playersByID.Tree::mergeNodes(team2->m_playersByID.m_node);
    this->m_playersByScore.Tree::mergeNodes(&(team2->m_playersByScore.m_node));
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
        m_playersByScore.insert(player, player->get_goals(), player->get_goals(), player->get_cards());
    }
    catch (InvalidID& e) {
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

std::shared_ptr<Team> Team::knockout_copy(std::shared_ptr<Team> teamCopy) { 
    teamCopy->m_id = m_id;
    teamCopy->m_numCards = m_numCards;
    teamCopy->m_numGames = m_numGames;
    teamCopy->m_numGoalkeepers = m_numGoalkeepers;
    teamCopy->m_numGoals = m_numGoals;
    teamCopy->m_numPlayers = m_numPlayers;
    teamCopy->m_points = m_points;
    return teamCopy;
}

void Team::remove_player_by_score(const int goals, const int cards, const int id) {
    m_playersByScore.remove(goals, cards, id);
}

void Team::insert_player_by_score(const std::shared_ptr<Player>& player, const int id, const int goals, const int cards) {
    m_playersByScore.insert(player, id, goals, cards);
}

//-------------------------------------Update Stats Functions----------------------------
void Team::update_team_stats(const std::shared_ptr<Player> player, const int goals, const int cards){
    add_game();
    update_num_goals(goals);
    update_num_cards(cards);
    update_top_player(player);
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
void Team::update_top_player(const std::shared_ptr<Player> player) {
    if (m_topScorer->get_goals() < player->get_goals()) {
        m_topScorer = player;
    }
}
