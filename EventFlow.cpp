#include "EventFlow.h"
#include <sstream>

// HELPER FUNCTIONS

std::string toString(int value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

// NOTICE TYPES - Implementation

namespace NoticeTypes
{
    // Operational changes
    const std::string OPEN = "OPEN";
    const std::string CLOSE = "CLOSE";
    const std::string SCHEDULE_CHANGE = "SCHEDULE_CHANGE";
    const std::string STATUS_CHANGE = "STATUS_CHANGE";
    const std::string SCORE_CHANGE = "SCORE_CHANGE";
    const std::string WARMUP_START = "WARMUP_START";
    const std::string WARMUP_END = "WARMUP_END";
    const std::string SUBSTITUTION = "SUBSTITUTION";

    // Capacity related changes
    const std::string CAPACITY_ALERT = "CAPACITY_ALERT";
    const std::string CAPACITY_NORMAL = "CAPACITY_NORMAL";
    const std::string FULL_CAPACITY = "FULL_CAPACITY";

    // Safety-related changes
    const std::string WEATHER_ALERT = "WEATHER_ALERT";
    const std::string PAUSE = "PAUSE";
    const std::string RESUME = "RESUME";
    const std::string EVACUATE = "EVACUATE";
    const std::string INJURY = "INJURY";
    const std::string MEDICAL_EMERGENCY = "MEDICAL_EMERGENCY";
}

// NOTIFICATION DATA - Implementation

NotificationData::NotificationData(const std::string &type, const std::string &msg)
    : noticeType(type), message(msg), timestamp(""), source(""), affectedEntity("")
{
    // Simple timestamp using string stream (no ctime)
    std::stringstream ss;
    ss << "2026-08-29 10:00:00"; // Static timestamp for C++11 compatibility
    timestamp = ss.str();
}

void NotificationData::addData(const std::string &key, const std::string &value)
{
    additionalData[key] = value;
}

// SUBJECT - Implementation

void Subject::attach(Observer *observer)
{
    if (observer == 0)
    {
        std::cout << "[Subject] WARNING: Attempted to attach null observer. Ignored." << std::endl;
        return;
    }

    // Check if observer already registered
    std::vector<Observer *>::iterator it = std::find(observers.begin(), observers.end(), observer);
    if (it == observers.end())
    {
        observers.push_back(observer);
        observer->registerSubject(this);
        std::cout << "[Subject] Observer attached successfully. Total: " << observers.size() << std::endl;
    }
    else
    {
        std::cout << "[Subject] Observer already registered. Duplicate ignored." << std::endl;
    }
}

void Subject::detach(Observer *observer)
{
    if (observer == 0)
    {
        std::cout << "[Subject] WARNING: Attempted to detach null observer. Ignored." << std::endl;
        return;
    }

    std::vector<Observer *>::iterator it = std::find(observers.begin(), observers.end(), observer);
    if (it != observers.end())
    {
        observers.erase(it);
        std::cout << "[Subject] Observer detached successfully. Remaining: " << observers.size() << std::endl;
    }
    else
    {
        std::cout << "[Subject] Observer not found. Detach attempt ignored." << std::endl;
    }
}

void Subject::notifyObservers(const std::string &noticeType, const std::string &message)
{
    if (observers.empty())
    {
        std::cout << "[Subject] No observers to notify." << std::endl;
        return;
    }

    std::cout << "\n[Subject] Broadcasting " << noticeType << ": " << message << std::endl;
    std::cout << "[Subject] Notifying " << observers.size() << " observer(s)..." << std::endl;

    // Create a copy of the observers list to handle detachment during notification
    std::vector<Observer *> observersCopy = observers;
    for (std::vector<Observer *>::size_type i = 0; i < observersCopy.size(); ++i)
    {
        observersCopy[i]->update(this, noticeType, message);
    }
}

void Subject::notifyWithDetails(const NotificationData &data)
{
    if (observers.empty())
    {
        std::cout << "[Subject] No observers to notify." << std::endl;
        return;
    }

    std::cout << "\n[Subject] Pushing detailed notification: " << data.noticeType << std::endl;
    std::cout << "[Subject] Message: " << data.message << std::endl;
    std::cout << "[Subject] Notifying " << observers.size() << " observer(s)..." << std::endl;

    std::vector<Observer *> observersCopy = observers;
    for (std::vector<Observer *>::size_type i = 0; i < observersCopy.size(); ++i)
    {
        observersCopy[i]->updateWithData(this, data);
    }
}

size_t Subject::getObserverCount() const
{
    return observers.size();
}

// OBSERVER - Implementation

Observer::~Observer()
{
    detachFromAllSubjects();
}

void Observer::updateWithData(Subject *subject, const NotificationData &data)
{
    // Default implementation just calls the simpler update method
    update(subject, data.noticeType, data.message);
}

void Observer::registerSubject(Subject *subject)
{
    if (subject == 0)
        return;

    // Check if already registered
    std::vector<Subject *>::iterator it = std::find(attachedSubjects.begin(), attachedSubjects.end(), subject);
    if (it == attachedSubjects.end())
    {
        attachedSubjects.push_back(subject);
    }
}

void Observer::detachFromAllSubjects()
{
    for (std::vector<Subject *>::size_type i = 0; i < attachedSubjects.size(); ++i)
    {
        attachedSubjects[i]->detach(this);
    }
    attachedSubjects.clear();
}

size_t Observer::getAttachedSubjectCount() const
{
    return attachedSubjects.size();
}

// SCOREBOARD - Implementation

ScoreBoard::ScoreBoard(const std::string &n) : name(n)
{
    std::cout << "[ScoreBoard] " << name << " initialized." << std::endl;
}

ScoreBoard::~ScoreBoard()
{
    std::cout << "[ScoreBoard] " << name << " destroyed. Detaching from all subjects..." << std::endl;
    detachFromAllSubjects();
}

void ScoreBoard::update(Subject *subject, const std::string &noticeType,
                        const std::string &message)
{
    std::cout << "  [ScoreBoard " << name << "] Received: " << noticeType << " - " << message << std::endl;

    if (noticeType == NoticeTypes::SCORE_CHANGE)
    {
        std::cout << "  [ScoreBoard " << name << "] UPDATING SCORE DISPLAY!" << std::endl;
    }
    else if (noticeType == NoticeTypes::SCHEDULE_CHANGE)
    {
        std::cout << "  [ScoreBoard " << name << "] UPDATING SCHEDULE DISPLAY!" << std::endl;
    }
}

void ScoreBoard::updateWithData(Subject *subject, const NotificationData &data)
{
    std::cout << "  [ScoreBoard " << name << "] Detailed update received:" << std::endl;
    std::cout << "    Type: " << data.noticeType << std::endl;
    std::cout << "    Message: " << data.message << std::endl;
    std::cout << "    Timestamp: " << data.timestamp << std::endl;

    // Display additional data
    if (!data.additionalData.empty())
    {
        std::cout << "    Additional data:" << std::endl;
        std::map<std::string, std::string>::const_iterator it;
        for (it = data.additionalData.begin(); it != data.additionalData.end(); ++it)
        {
            std::cout << "      " << it->first << ": " << it->second << std::endl;
        }
    }
}

// MEDICALTEAM - Implementation

MedicalTeam::MedicalTeam(const std::string &n, bool duty) : name(n), onDuty(duty)
{
    std::cout << "[MedicalTeam] " << name << " initialized. On duty: " << (onDuty ? "Yes" : "No") << std::endl;
}

MedicalTeam::~MedicalTeam()
{
    std::cout << "[MedicalTeam] " << name << " destroyed. Detaching from all subjects..." << std::endl;
    detachFromAllSubjects();
}

void MedicalTeam::update(Subject *subject, const std::string &noticeType,
                         const std::string &message)
{
    std::cout << "  [MedicalTeam " << name << "] Received: " << noticeType << " - " << message << std::endl;

    if (noticeType == NoticeTypes::INJURY)
    {
        std::cout << "  [MedicalTeam " << name << "] INJURY ALERT! Dispatching team!" << std::endl;
    }
    else if (noticeType == NoticeTypes::MEDICAL_EMERGENCY)
    {
        std::cout << "  [MedicalTeam " << name << "] MEDICAL EMERGENCY! Full response!" << std::endl;
    }
    else if (noticeType == NoticeTypes::EVACUATE)
    {
        std::cout << "  [MedicalTeam " << name << "] EVACUATION! Assisting!" << std::endl;
    }
}

void MedicalTeam::updateWithData(Subject *subject, const NotificationData &data)
{
    std::cout << "  [MedicalTeam " << name << "] Detailed medical update:" << std::endl;
    std::cout << "    Type: " << data.noticeType << std::endl;
    std::cout << "    Message: " << data.message << std::endl;

    // Check for specific medical data
    std::map<std::string, std::string>::const_iterator it = data.additionalData.find("severity");
    if (it != data.additionalData.end())
    {
        std::cout << "    Severity: " << it->second << std::endl;
        if (it->second == "CRITICAL")
        {
            std::cout << "  [MedicalTeam " << name << "] CRITICAL CASE!" << std::endl;
        }
    }
}

// SPECTATORGATE - Implementation

SpectatorGate::SpectatorGate(const std::string &n, int max)
    : name(n), currentCapacity(0), maxCapacity(max), isOpen(false)
{
    std::cout << "[SpectatorGate] " << name << " initialized. Capacity: " << max << std::endl;
}

SpectatorGate::~SpectatorGate()
{
    std::cout << "[SpectatorGate] " << name << " destroyed. Detaching from all subjects..." << std::endl;
    detachFromAllSubjects();
}

void SpectatorGate::update(Subject *subject, const std::string &noticeType,
                           const std::string &message)
{
    std::cout << "  [SpectatorGate " << name << "] Received: " << noticeType << " - " << message << std::endl;

    if (noticeType == NoticeTypes::CAPACITY_ALERT)
    {
        std::cout << "  [SpectatorGate " << name << "] CAPACITY ALERT! Adjusting entry!" << std::endl;
        close();
    }
    else if (noticeType == NoticeTypes::CAPACITY_NORMAL)
    {
        std::cout << "  [SpectatorGate " << name << "] Capacity normal. Resuming entry." << std::endl;
        open();
    }
    else if (noticeType == NoticeTypes::OPEN)
    {
        open();
    }
    else if (noticeType == NoticeTypes::CLOSE)
    {
        close();
    }
    else if (noticeType == NoticeTypes::EVACUATE)
    {
        std::cout << "  [SpectatorGate " << name << "] EVACUATION! Opening all exits!" << std::endl;
        open();
    }
}

void SpectatorGate::updateWithData(Subject *subject, const NotificationData &data)
{
    std::cout << "  [SpectatorGate " << name << "] Detailed gate update:" << std::endl;
    std::cout << "    Type: " << data.noticeType << std::endl;
    std::cout << "    Message: " << data.message << std::endl;
    std::cout << "    Current occupancy: " << currentCapacity << "/" << maxCapacity << std::endl;
}

void SpectatorGate::open()
{
    isOpen = true;
    std::cout << "  [SpectatorGate " << name << "] Gate OPEN" << std::endl;
}

void SpectatorGate::close()
{
    isOpen = false;
    std::cout << "  [SpectatorGate " << name << "] Gate CLOSED" << std::endl;
}

void SpectatorGate::setCapacity(int count)
{
    currentCapacity = count;
    std::cout << "  [SpectatorGate " << name << "] Current capacity: " << currentCapacity << "/" << maxCapacity << std::endl;
}

// INFORMATIONDESK - Implementation

InformationDesk::InformationDesk(const std::string &n) : name(n)
{
    std::cout << "[InformationDesk] " << name << " initialized." << std::endl;
}

InformationDesk::~InformationDesk()
{
    std::cout << "[InformationDesk] " << name << " destroyed. Detaching from all subjects..." << std::endl;
    detachFromAllSubjects();
}

void InformationDesk::update(Subject *subject, const std::string &noticeType,
                             const std::string &message)
{
    std::cout << "  [InfoDesk " << name << "] Received: " << noticeType << " - " << message << std::endl;
    updates.push_back(noticeType + ": " + message);

    if (noticeType == NoticeTypes::SCHEDULE_CHANGE)
    {
        std::cout << "  [InfoDesk " << name << "] UPDATING SCHEDULE BOARDS!" << std::endl;
    }
    else if (noticeType == NoticeTypes::WEATHER_ALERT)
    {
        std::cout << "  [InfoDesk " << name << "] WEATHER ALERT! Posting advisory!" << std::endl;
    }
}

void InformationDesk::updateWithData(Subject *subject, const NotificationData &data)
{
    std::cout << "  [InfoDesk " << name << "] Detailed info update:" << std::endl;
    std::cout << "    Type: " << data.noticeType << std::endl;
    std::cout << "    Message: " << data.message << std::endl;
    std::cout << "    Source: " << data.source << std::endl;

    updates.push_back(data.noticeType + ": " + data.message + " (Source: " + data.source + ")");
}

void InformationDesk::displayUpdates() const
{
    std::cout << "\n  [InfoDesk " << name << "] All updates received:" << std::endl;
    for (std::vector<std::string>::size_type i = 0; i < updates.size(); ++i)
    {
        std::cout << "    - " << updates[i] << std::endl;
    }
}

// NETBALLMATCH - Implementation

NetballMatch::NetballMatch(const std::string &id, const std::string &t1, const std::string &t2,
                           const std::string &c)
    : matchId(id), team1(t1), team2(t2), score1(0), score2(0),
      status("SCHEDULED"), court(c), spectatorCount(0),
      isWeatherDelay(false), isInjuryTimeout(false)
{
    std::cout << "[NetballMatch] Created: " << id << " - " << t1 << " vs " << t2 << std::endl;
}

NetballMatch::~NetballMatch()
{
    std::cout << "[NetballMatch] Destroyed: " << matchId << std::endl;
}

// Operational change notifications
void NetballMatch::updateScore(int newScore1, int newScore2)
{
    score1 = newScore1;
    score2 = newScore2;
    std::string message = team1 + " " + toString(score1) + " - " +
                          toString(score2) + " " + team2;
    notifyObservers(NoticeTypes::SCORE_CHANGE, message);
}

void NetballMatch::rescheduleMatch(const std::string &newCourt, const std::string &newTime)
{
    court = newCourt;
    std::string message = "Match " + matchId + " moved to " + newCourt + " at " + newTime;
    notifyObservers(NoticeTypes::SCHEDULE_CHANGE, message);
}

void NetballMatch::startMatch()
{
    status = "IN_PROGRESS";
    std::string message = "Match " + matchId + " has started";
    notifyObservers(NoticeTypes::STATUS_CHANGE, message);
}

void NetballMatch::completeMatch()
{
    status = "COMPLETED";
    std::string message = "Match " + matchId + " completed. Final: " +
                          toString(score1) + "-" + toString(score2);
    notifyObservers(NoticeTypes::STATUS_CHANGE, message);
}

void NetballMatch::notifySubstitution(const std::string &playerIn, const std::string &playerOut)
{
    std::string message = playerIn + " replaces " + playerOut + " in Match " + matchId;
    notifyObservers(NoticeTypes::SUBSTITUTION, message);
}

void NetballMatch::notifyWarmupStart()
{
    std::string message = "Warmup started for Match " + matchId;
    notifyObservers(NoticeTypes::WARMUP_START, message);
}

void NetballMatch::notifyWarmupEnd()
{
    std::string message = "Warmup ended for Match " + matchId;
    notifyObservers(NoticeTypes::WARMUP_END, message);
}

// Capacity-related notifications
void NetballMatch::updateSpectatorCount(int count)
{
    spectatorCount = count;
    std::string message = "Spectator count for " + matchId + ": " + toString(count);
    notifyObservers(NoticeTypes::STATUS_CHANGE, message);

    if (count > 400)
    {
        notifyCapacityAlert(count, 500);
    }
}

void NetballMatch::notifyCapacityAlert(int currentCapacity, int maxCapacity)
{
    std::string message = "Match " + matchId + " capacity: " +
                          toString(currentCapacity) + "/" + toString(maxCapacity);
    notifyObservers(NoticeTypes::CAPACITY_ALERT, message);
}

// Safety-related notifications
void NetballMatch::issueWeatherAlert(const std::string &weatherCondition)
{
    isWeatherDelay = true;
    std::string message = "Match " + matchId + " weather alert: " + weatherCondition;
    notifyObservers(NoticeTypes::WEATHER_ALERT, message);

    if (weatherCondition == "Storm" || weatherCondition == "Tornado")
    {
        evacuateMatch();
    }
    else if (weatherCondition == "Heavy Rain")
    {
        pauseMatch("Weather: " + weatherCondition);
    }
}

void NetballMatch::evacuateMatch()
{
    status = "PAUSED";
    std::string message = "Match " + matchId + " EVACUATED - All personnel to safety";
    notifyObservers(NoticeTypes::EVACUATE, message);
}

void NetballMatch::pauseMatch(const std::string &reason)
{
    status = "PAUSED";
    std::string message = "Match " + matchId + " paused: " + reason;
    notifyObservers(NoticeTypes::PAUSE, message);
}

void NetballMatch::resumeMatch()
{
    status = "IN_PROGRESS";
    std::string message = "Match " + matchId + " resumed";
    notifyObservers(NoticeTypes::RESUME, message);
}

void NetballMatch::notifyInjury(const std::string &playerName)
{
    isInjuryTimeout = true;
    std::string message = "Injury: " + playerName + " in Match " + matchId;
    notifyObservers(NoticeTypes::INJURY, message);
    notifyObservers(NoticeTypes::PAUSE, "Injury timeout");
}

void NetballMatch::medicalEmergency()
{
    std::string message = "MEDICAL EMERGENCY in Match " + matchId;
    notifyObservers(NoticeTypes::MEDICAL_EMERGENCY, message);
    evacuateMatch();
}

// TOURNAMENTDAY - Implementation

TournamentDay::TournamentDay(const std::string &name)
    : dayName(name), schedule("Regular schedule"), isActive(false)
{
    std::cout << "[TournamentDay] Created: " << name << std::endl;
}

TournamentDay::~TournamentDay()
{
    std::cout << "[TournamentDay] Destroyed: " << dayName << std::endl;
}

void TournamentDay::addMatch(NetballMatch *match)
{
    matches.push_back(match);
    std::cout << "[TournamentDay] Added match " << match->getMatchId() << " to " << dayName << std::endl;
}

void TournamentDay::addCourt(Court *court)
{
    courts.push_back(court);
    court->setParentDay(this);
    std::cout << "[TournamentDay] Added court " << court->getCourtName() << " to " << dayName << std::endl;
}

void TournamentDay::changeSchedule(const std::string &newSchedule)
{
    schedule = newSchedule;
    std::string message = "Day " + dayName + " schedule changed: " + newSchedule;
    notifyObservers(NoticeTypes::SCHEDULE_CHANGE, message);
}

void TournamentDay::activateDay()
{
    isActive = true;
    std::string message = "Day " + dayName + " activated";
    notifyObservers(NoticeTypes::STATUS_CHANGE, message);
}

void TournamentDay::deactivateDay()
{
    isActive = false;
    std::string message = "Day " + dayName + " deactivated";
    notifyObservers(NoticeTypes::STATUS_CHANGE, message);
}

void TournamentDay::receiveAndCascadeNotification(const std::string &noticeType, const std::string &message)
{
    std::cout << "\n[TournamentDay " << dayName << "] Received notification from above:" << std::endl;
    std::cout << "  Type: " << noticeType << std::endl;
    std::cout << "  Message: " << message << std::endl;

    notifyObservers(noticeType, "CASCADED FROM DAY: " + message);

    std::cout << "[TournamentDay " << dayName << "] Cascading to " << courts.size() << " court(s)..." << std::endl;
    for (std::vector<Court *>::size_type i = 0; i < courts.size(); ++i)
    {
        courts[i]->receiveAndCascadeNotification(noticeType, message);
    }
}

void TournamentDay::receiveAndCascadeWithDetails(const NotificationData &data)
{
    std::cout << "\n[TournamentDay " << dayName << "] Received detailed notification from above:" << std::endl;
    std::cout << "  Type: " << data.noticeType << std::endl;
    std::cout << "  Message: " << data.message << std::endl;

    notifyWithDetails(data);

    std::cout << "[TournamentDay " << dayName << "] Cascading detailed notification to "
              << courts.size() << " court(s)..." << std::endl;
    for (std::vector<Court *>::size_type i = 0; i < courts.size(); ++i)
    {
        courts[i]->receiveAndCascadeWithDetails(data);
    }
}

// COURT - Implementation

Court::Court(const std::string &name, const std::string &type, int cap)
    : courtName(name), courtType(type), capacity(cap), currentOccupancy(0), parentDay(0)
{
    std::cout << "[Court] Created: " << name << " (Type: " << type << ", Capacity: " << cap << ")" << std::endl;
}

Court::~Court()
{
    std::cout << "[Court] Destroyed: " << courtName << std::endl;
}

void Court::addMatch(NetballMatch *match)
{
    matches.push_back(match);
    std::cout << "[Court " << courtName << "] Added match " << match->getMatchId() << std::endl;
}

void Court::setParentDay(TournamentDay *day)
{
    parentDay = day;
}

void Court::updateOccupancy(int count)
{
    currentOccupancy = count;
    std::string message = "Court " + courtName + " occupancy: " + toString(count) + "/" + toString(capacity);
    notifyObservers(NoticeTypes::STATUS_CHANGE, message);

    if (count > capacity * 0.9)
    {
        notifyObservers(NoticeTypes::CAPACITY_ALERT, "Court " + courtName + " near capacity");
    }
}

void Court::receiveAndCascadeNotification(const std::string &noticeType, const std::string &message)
{
    std::cout << "\n[Court " << courtName << "] Received notification from above:" << std::endl;
    std::cout << "  Type: " << noticeType << std::endl;
    std::cout << "  Message: " << message << std::endl;

    notifyObservers(noticeType, "CASCADED FROM COURT: " + message);

    std::cout << "[Court " << courtName << "] Cascading to " << matches.size() << " match(es)..." << std::endl;
    for (std::vector<NetballMatch *>::size_type i = 0; i < matches.size(); ++i)
    {
        std::cout << "  [Court " << courtName << "] Match " << matches[i]->getMatchId() << " notified." << std::endl;
    }
}

void Court::receiveAndCascadeWithDetails(const NotificationData &data)
{
    std::cout << "\n[Court " << courtName << "] Received detailed notification from above:" << std::endl;
    std::cout << "  Type: " << data.noticeType << std::endl;
    std::cout << "  Message: " << data.message << std::endl;

    notifyWithDetails(data);

    std::cout << "[Court " << courtName << "] Cascading detailed notification to "
              << matches.size() << " match(es)..." << std::endl;
    for (std::vector<NetballMatch *>::size_type i = 0; i < matches.size(); ++i)
    {
        std::cout << "  [Court " << courtName << "] Match " << matches[i]->getMatchId() << " receives detailed notification." << std::endl;
    }
}

// ZONE - Implementation

Zone::Zone(const std::string &name, const std::string &div)
    : zoneName(name), division(div), parentDay(0)
{
    std::cout << "[Zone] Created: " << name << " (Division: " << div << ")" << std::endl;
}

Zone::~Zone()
{
    std::cout << "[Zone] Destroyed: " << zoneName << std::endl;
}

void Zone::addCourt(Court *court)
{
    courts.push_back(court);
    std::cout << "[Zone " << zoneName << "] Added court " << court->getCourtName() << std::endl;
}

void Zone::setParentDay(TournamentDay *day)
{
    parentDay = day;
}

void Zone::receiveAndCascadeNotification(const std::string &noticeType, const std::string &message)
{
    std::cout << "\n[Zone " << zoneName << "] TOP LEVEL: Received notification:" << std::endl;
    std::cout << "  Type: " << noticeType << std::endl;
    std::cout << "  Message: " << message << std::endl;

    notifyObservers(noticeType, "CASCADED FROM ZONE: " + message);

    std::cout << "[Zone " << zoneName << "] Cascading to " << courts.size() << " court(s)..." << std::endl;
    for (std::vector<Court *>::size_type i = 0; i < courts.size(); ++i)
    {
        courts[i]->receiveAndCascadeNotification(noticeType, message);
    }
}

void Zone::receiveAndCascadeWithDetails(const NotificationData &data)
{
    std::cout << "\n[Zone " << zoneName << "] TOP LEVEL: Received detailed notification:" << std::endl;
    std::cout << "  Type: " << data.noticeType << std::endl;
    std::cout << "  Message: " << data.message << std::endl;
    std::cout << "  Timestamp: " << data.timestamp << std::endl;

    notifyWithDetails(data);

    std::cout << "[Zone " << zoneName << "] Cascading detailed notification to "
              << courts.size() << " court(s)..." << std::endl;
    for (std::vector<Court *>::size_type i = 0; i < courts.size(); ++i)
    {
        courts[i]->receiveAndCascadeWithDetails(data);
    }
}

// DEMONSTRATION FUNCTIONS

void demonstrateCascade()
{
    std::cout << "\n"
              << std::string(70, '=') << std::endl;
    std::cout << "DEMONSTRATION: 3-LEVEL NOTIFICATION CASCADE" << std::endl;
    std::cout << "Zone -> Court -> Match" << std::endl;
    std::cout << std::string(70, '=') << std::endl;

    // Create hierarchy: Zone -> Court -> Match
    Zone zoneA("Zone A", "U16");
    Court court1("Court 1", "Main", 500);
    Court court2("Court 2", "Practice", 300);

    NetballMatch match1("M001", "Team Alpha", "Team Beta", "Court 1");
    NetballMatch match2("M002", "Team Gamma", "Team Delta", "Court 1");
    NetballMatch match3("M003", "Team Epsilon", "Team Zeta", "Court 2");

    // Build hierarchy
    court1.addMatch(&match1);
    court1.addMatch(&match2);
    court2.addMatch(&match3);
    zoneA.addCourt(&court1);
    zoneA.addCourt(&court2);

    // Create observers
    InformationDesk infoDesk("Main Information Desk");
    ScoreBoard scoreBoard("Main Scoreboard");
    MedicalTeam medicalTeam("Emergency Medical Team");
    SpectatorGate gate("Gate 1", 500);

    // Attach observers to various levels
    std::cout << "\n--- Attaching Observers ---" << std::endl;
    zoneA.attach(&infoDesk);
    zoneA.attach(&scoreBoard);
    court1.attach(&medicalTeam);
    court2.attach(&gate);

    // Scenario 1: Weather Alert cascading from Zone level
    std::cout << "\n--- SCENARIO 1: Weather Alert from Zone Level ---" << std::endl;
    zoneA.receiveAndCascadeNotification(NoticeTypes::WEATHER_ALERT,
                                        "Severe thunderstorm approaching");

    // Scenario 2: Schedule Change from Zone level
    std::cout << "\n--- SCENARIO 2: Schedule Change from Zone Level ---" << std::endl;
    zoneA.receiveAndCascadeNotification(NoticeTypes::SCHEDULE_CHANGE,
                                        "All matches delayed by 1 hour");

    // Scenario 3: Capacity Alert from Court level
    std::cout << "\n--- SCENARIO 3: Capacity Alert from Court Level ---" << std::endl;
    court1.receiveAndCascadeNotification(NoticeTypes::CAPACITY_ALERT,
                                         "Court 1 at 95% capacity");

    // Scenario 4: Evacuation from Zone level
    std::cout << "\n--- SCENARIO 4: Evacuation from Zone Level ---" << std::endl;
    zoneA.receiveAndCascadeNotification(NoticeTypes::EVACUATE,
                                        "Zone A - Immediate evacuation required");
}

void demonstratePushModel()
{
    std::cout << "\n"
              << std::string(70, '=') << std::endl;
    std::cout << "DEMONSTRATION: PUSH MODEL WITH COMPLETE STATE" << std::endl;
    std::cout << std::string(70, '=') << std::endl;

    // Create a match
    NetballMatch match("M004", "Team Eagles", "Team Hawks", "Court 3");

    // Create observers
    ScoreBoard scoreBoard("Digital Scoreboard");
    InformationDesk infoDesk("Main Info Desk");
    MedicalTeam medicalTeam("Medical Response Team");
    SpectatorGate gate("Gate 3", 500);

    // Attach observers
    std::cout << "\n--- Attaching Observers to Match ---" << std::endl;
    match.attach(&scoreBoard);
    match.attach(&infoDesk);
    match.attach(&medicalTeam);
    match.attach(&gate);

    // Scenario 1: Operational Change - Score Change
    std::cout << "\n--- SCENARIO 1: Operational Change (Score Update) ---" << std::endl;
    match.updateScore(15, 12);

    // Scenario 2: Operational Change - Start Match
    std::cout << "\n--- SCENARIO 2: Operational Change (Match Start) ---" << std::endl;
    match.startMatch();

    // Scenario 3: Capacity-Related Change
    std::cout << "\n--- SCENARIO 3: Capacity-Related Change ---" << std::endl;
    match.updateSpectatorCount(450);

    // Scenario 4: Operational Change - Substitution
    std::cout << "\n--- SCENARIO 4: Operational Change (Substitution) ---" << std::endl;
    match.notifySubstitution("Jane Doe", "Mary Smith");

    // Scenario 5: Safety-Related Change - Weather Alert
    std::cout << "\n--- SCENARIO 5: Safety-Related Change (Weather) ---" << std::endl;
    match.issueWeatherAlert("Heavy Rain");

    // Scenario 6: Safety-Related Change - Injury
    std::cout << "\n--- SCENARIO 6: Safety-Related Change (Injury) ---" << std::endl;
    match.notifyInjury("Sarah Williams");

    // Scenario 7: Safety-Related Change - Medical Emergency
    std::cout << "\n--- SCENARIO 7: Safety-Related Change (Medical Emergency) ---" << std::endl;
    match.medicalEmergency();

    // Display info desk updates
    std::cout << "\n--- Information Desk Update Summary ---" << std::endl;
    infoDesk.displayUpdates();
}

void runFullDemonstration()
{
    std::cout << "\n"
              << std::string(70, '=') << std::endl;
    std::cout << "NETBALL TOURNAMENT OBSERVER SYSTEM" << std::endl;
    std::cout << "Composite + Observer Pattern Implementation" << std::endl;
    std::cout << std::string(70, '=') << std::endl;

    demonstrateCascade();
    demonstratePushModel();

    std::cout << "\n"
              << std::string(70, '=') << std::endl;
    std::cout << "DEMONSTRATION COMPLETE" << std::endl;
    std::cout << std::string(70, '=') << std::endl;
}