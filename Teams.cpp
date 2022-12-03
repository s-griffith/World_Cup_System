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

const std::shared_ptr<Player> Team::get_top_scorer() const {
    return m_topScorer;
}

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
    this->m_playersByScore.Tree::mergeNodes(team1->m_playersByScore.m_node);
    this->m_playersByID.Tree::mergeNodes(team2->m_playersByID.m_node);
    this->m_playersByScore.Tree::mergeNodes(team2->m_playersByScore.m_node);
}

//-------------------------------------Helper Functions for WorldCup----------------------------
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

void Team::remove_player(int playerID){
    //Do the cards he had get subtracted from the team's total? Goals? etc.
    m_playersByID.remove(playerID);
    m_playersByScore.remove(playerID);
}

bool Team::is_valid() const{
    if (m_numPlayers >= 11 && m_numGoalkeepers >= 1) {
        return true;
    }
    return false;
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