#include "Teams.h"

//--------------------------------Constructors, Assignment Operator and Destructor----------------------------

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
    m_topScorer = nullptr;
    return *this;
}


Team::~Team() {
    m_closestTeamRight = nullptr;
    m_closestTeamLeft = nullptr;
    m_topScorer = nullptr;
}


//-----------------------------------------------------Getters--------------------------------------------------

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


//-------------------------------------Update Stats Functions----------------------------

void Team::update_team_id(Team* team) {
    m_playersByID.m_node->inorderWalkTeamID(team);
}

bool Team::is_valid() const{
    if (m_numPlayers >= 11 && m_numGoalkeepers >= 1) {
        return true;
    }
    return false;
}

void Team::add_game() {
    m_numGames++;
}

void Team::update_points_won() {
    m_points += 3;
}

void Team::update_points_tie() {
    m_points += 1;
}

void Team::update_num_goals(const int goals) {
    m_numGoals += goals;
}

void Team::update_num_cards(const int cards) {
    m_numCards += cards;
}

void Team::update_top_player() {
    m_topScorer = m_playersByScore.search_and_return_max();
}

void Team::update_closest_right(Team* team2) {
    m_closestTeamRight = team2;
}

void Team::update_closest_left(Team* team1) {
    m_closestTeamLeft = team1;
}

//-------------------------------------Helper Functions for WorldCup----------------------------

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


void Team::remove_player_by_score(const int id, const int goals, const int cards) {
    m_playersByScore.remove(id, goals, cards);
}


void Team::insert_player_by_score(Player* player, const int id, const int goals, const int cards) {
    m_playersByScore.insert(player, id, goals, cards);
}


void Team::update_team_stats(const int goals, const int cards){
    update_num_goals(goals);
    update_num_cards(cards);
    update_top_player();
}


//--------------------------------unite_teams----------------------------

void Team::unite_teams(Team* team1, Team* team2) {
    m_numCards = team1->m_numCards + team2->m_numCards;
    m_numGames = 0;
    m_numGoalkeepers = team1->m_numGoalkeepers + team2->m_numGoalkeepers;
    m_numGoals = team1->m_numGoals + team2->m_numGoals;
    m_numPlayers = team1->m_numPlayers + team2->m_numPlayers;
    //Create an array of pointers to the players in each team
    Player** playersID1 = new Player*[team1->m_numPlayers*sizeof(Player*)];
    Player** playersID2 = new Player*[team2->m_numPlayers*sizeof(Player*)];
    Player** playersScore1 = new Player*[team1->m_numPlayers*sizeof(Player*)];
    Player** playersScore2 = new Player*[team2->m_numPlayers*sizeof(Player*)];
    //Insert the players into the array by inorder walk
    team1->m_playersByID.m_node->unite_insert(playersID1, 0);
    team2->m_playersByID.m_node->unite_insert(playersID2, 0);
    team1->m_playersByScore.m_node->unite_insert(playersScore1, 0);
    team2->m_playersByScore.m_node->unite_insert(playersScore2, 0);
    //Create an array of pointers that holds the united teams
    Player** nPlayersID = new Player*[m_numPlayers*sizeof(Player*)];
    Player** nPlayersScore = new Player*[m_numPlayers*sizeof(Player*)];
    //Merge the teams from their separate arrays into the united array
    mergeByID(nPlayersID, playersID1, playersID2, team1->m_numPlayers, team2->m_numPlayers);
    delete[] playersID1;
    delete[] playersID2;
    mergeByScore(nPlayersScore, playersScore1, playersScore2, team1->m_numPlayers, team2->m_numPlayers);
    delete[] playersScore1;
    delete[] playersScore2;
    //Create a tree from the united array
    if (m_numPlayers > 0) {
        m_playersByID.insertInorder(nPlayersID, m_numPlayers-1);
        m_playersByScore.insertInorder(nPlayersScore, m_numPlayers-1);
    }
    delete[] nPlayersID;
    delete[] nPlayersScore;
    //Update each player's num_games
    team1->m_playersByID.unite_update_games(team1->get_games());
    team2->m_playersByID.unite_update_games(team2->get_games());
    //Update the top scorer of the united team
    m_topScorer = m_playersByScore.search_and_return_max();
}


void Team::mergeByID(Player** nPlayers, Player** players1, Player** players2, const int len1, const int len2) {
    int index1 = 0, index2 = 0, indexMerged = 0;
    while(index1 < len1 && index2 < len2) {
        if(players1[index1]->get_playerId() < players2[index2]->get_playerId()) {
            nPlayers[indexMerged++] = players1[index1++];
        }
        else {
            nPlayers[indexMerged++] = players2[index2++];
        }
    }
    for(int i = index1; i < len1; i++) {
        nPlayers[indexMerged++] = players1[index1++];
    }
    for(int i = index2; i < len2; i++) {
        nPlayers[indexMerged++] = players2[index2++];
    }
}

void Team::mergeByScore(Player** nPlayers, Player** players1, Player** players2, const int len1, const int len2) {
    int index1 = 0, index2 = 0, indexMerged = 0;
    while(index1 < len1 && index2 < len2) {
        if(players1[index1]->get_goals() < players2[index2]->get_goals()) {
            nPlayers[indexMerged++] = players1[index1++];
        }
        else if (players1[index1]->get_goals() == players2[index2]->get_goals() && players1[index1]->get_cards() 
                                                                                    > players2[index2]->get_cards()) {
            nPlayers[indexMerged++] = players1[index1++];
        }
        else if (players1[index1]->get_goals() == players2[index2]->get_goals() && players1[index1]->get_cards() ==
                 players2[index2]->get_cards() && players1[index1]->get_playerId() > players2[index2]->get_playerId()) {
            nPlayers[indexMerged++] = players1[index1++];
                }
        else {
            nPlayers[indexMerged++] = players2[index2++];
        }
    }
    for(int i = index1; i < len1; i++) {
        nPlayers[indexMerged++] = players1[index1++];
    }
    for(int i = index2; i < len2; i++) {
        nPlayers[indexMerged++] = players2[index2++];
    }
}


//--------------------------------world_cup Players Functions----------------------------

void Team::get_all_team_players(int* const output) {
    m_playersByScore.get_all_data(output);
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
    if ((tmpPlayer != nullptr) && (tmpPlayer->get_closest_left() != nullptr || 
                                                                        tmpPlayer->get_closest_right() != nullptr)) {
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


//--------------------------------knockout_winner----------------------------

int Team::knockout_count(const int maxTeamID) {
    Team* current = this;
    int counter = 0;
    while (current != nullptr && current->m_id <= maxTeamID) {
        counter++;
        current = current->m_closestTeamRight;
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


void Team::knockout_unite(Team& winner, Team& loser) {
    winner.m_numCards += loser.m_numCards;
    winner.m_numGames += loser.m_numGames;
    winner.m_numGoalkeepers += loser.m_numGoalkeepers;
    winner.m_numGoals += loser.m_numGoals;
    winner.m_numPlayers += loser.m_numPlayers;
    winner.m_points += loser.m_points;
}
