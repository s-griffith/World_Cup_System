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
        m_playersByID(),
        m_playersByScore(),
        m_topScorer(nullptr),
        m_closestTeamRight(nullptr),
        m_closestTeamLeft(nullptr)
{}

Team::Team() :
        m_id(0),
        m_points(0),
        m_numPlayers(0),
        m_numGoalkeepers(0),
        m_numGoals(0),
        m_numCards(0),
        m_numGames(0),
        m_playersByID(),
        m_playersByScore(),
        m_topScorer(nullptr),
        m_closestTeamRight(nullptr),
        m_closestTeamLeft(nullptr)
{}

Team& Team::operator=(const Team& other) {
    m_id = other.m_id;
    m_numCards = other.m_numCards;
    m_numGames = other.m_numGames;
    m_numGoalkeepers = other.m_numGoalkeepers;
    m_numGoals = other.m_numGoals;
    m_numPlayers = other.m_numPlayers;
    m_points = other.m_points;
    m_closestTeamRight = nullptr;
    m_closestTeamLeft = nullptr;
    return *this;
}

Team::~Team() {
    m_closestTeamRight = nullptr;
    m_closestTeamLeft = nullptr;
    m_topScorer = nullptr;
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

const Player* Team::get_top_scorer() const {
    return m_topScorer;
}

int Team::get_teamID() const  {
    return m_id;
}

Team* Team::get_closest_left() {
    return m_closestTeamLeft;
}

Team* Team::get_closest_right() {
    return m_closestTeamRight;
}

//-------------------------------------Helper Functions for WorldCup----------------------------

void Team::unite_teams(Team* team1, Team* team2) {
    this->m_numCards = team1->m_numCards + team2->m_numCards;
    this->m_numGames = team1->m_numGames + team2->m_numGames;
    this->m_numGoalkeepers = team1->m_numGoalkeepers + team2->m_numGoalkeepers;
    this->m_numGoals = team1->m_numGoals + team2->m_numGoals;
    this->m_numPlayers = team1->m_numPlayers + team2->m_numPlayers;
    //this->m_playersByID.mergeNodesExtraPointer(team1->m_playersByID.m_node);
    this->m_playersByID.mergeNodes(team1->m_playersByID.m_node);
    this->m_playersByScore.mergeNodes(team1->m_playersByScore.m_node);
    //this->m_playersByID.mergeNodesExtraPointer(team2->m_playersByID.m_node);
    this->m_playersByID.mergeNodes(team2->m_playersByID.m_node);
    this->m_playersByScore.mergeNodes(team2->m_playersByScore.m_node);

    this->m_topScorer = m_playersByScore.search_and_return_max();
}

int Team::get_closest_team_player(const int playerId) {
    int closest_id = 0;
    Player* tmpPlayer;
    try {
        tmpPlayer = m_playersByID.search_and_return_data(playerId);
    }
    catch (const NodeNotFound& e) {
        throw e;
    }
    if ((tmpPlayer != nullptr) && (tmpPlayer->get_closest_left() != nullptr || tmpPlayer->get_closest_right() != nullptr)) {
        if (tmpPlayer->get_closest_left() == nullptr && tmpPlayer->get_closest_right() != nullptr) {
            closest_id = tmpPlayer->get_closest_right()->get_playerId();
        }
        else if (tmpPlayer->get_closest_right() == nullptr && tmpPlayer->get_closest_left() != nullptr) {
            closest_id = tmpPlayer->get_closest_left()->get_playerId();
        }
        else {
            closest_id = tmpPlayer->get_closest(tmpPlayer->get_closest_left(), tmpPlayer->get_closest_right());
        }
        return closest_id;
    }
    return -1;
}

void Team::get_all_team_players(int* const output) {
    m_playersByScore.get_all_data(output);
}

StatusType Team::add_player(Player* player, const int id, const int goals, const int cards, const bool goalkeeper){
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
    update_top_player();
    return StatusType::SUCCESS;
}

void Team::remove_player(const int playerID, const int goals, const int cards, const bool goalKeeper){
    m_playersByID.remove(playerID);
    m_playersByScore.remove(playerID, goals, cards);
    if (goalKeeper) {
        m_numGoalkeepers--;
    }
    m_numCards -= cards;
    m_numGoals -= goals;
    m_numPlayers--;
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
    winner.m_points += loser.m_points;
}


int Team::knockout_count(const int maxTeamID) {
    Team* current = this;
    int counter = 0;
    while (current != nullptr && current->m_id <= maxTeamID) {
        counter++;
        current = current->m_closestTeamRight;
        //if (current != nullptr) {
         //   std::cout << "closest right is " << current->m_id << std::endl;
       // }
    }
    return counter;
}

void Team::knockout_insert(Team* teams, const int maxID) {
    Team* current = this;
    int index = 0;
    while (current != nullptr && current->m_id <= maxID) {
        *(teams+index) = *(current);
        current = current->m_closestTeamRight;
        index++;
    }
}


void Team::remove_player_by_score(const int id, const int goals, const int cards) {
    m_playersByScore.remove(id, goals, cards);
}

void Team::insert_player_by_score(Player* player, const int id, const int goals, const int cards) {
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

void Team::update_team_id(Team* team) {
    m_playersByID.m_node->inorderWalkTeamID(team);
}

void Team::update_closest_left(Team* team1) {
    m_closestTeamLeft = team1;
}

void Team::update_closest_right(Team* team2) {
    m_closestTeamRight = team2;
}
