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

//-------------------------------------Helper Functions for WorldCup----------------------------
StatusType Team::add_player(const std::shared_ptr<Player>& player, const int id, const int goals, const int cards, const bool goalkeeper){
    if(m_playersByID.insert(player, id)) {
        if (m_playersByScore.insert(player, player->get_goals())) {
            update_num_goals(goals);
            update_num_cards(cards);
            m_numPlayers++;
            if (goalkeeper) {
                m_numGoalkeepers++;
            }
            return StatusType::SUCCESS;
        }
    }
    return StatusType::FAILURE;
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