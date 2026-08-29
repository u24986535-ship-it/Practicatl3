#ifndef EVENTFLOW_H
#define EVENTFLOW_H

#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <map>

// Forward declarations
class Observer;
class Subject;

// NOTICE TYPES - Constants for notification types

namespace NoticeTypes
{
    // Operational changes
    extern const std::string OPEN;
    extern const std::string CLOSE;
    extern const std::string SCHEDULE_CHANGE;
    extern const std::string STATUS_CHANGE;
    extern const std::string SCORE_CHANGE;
    extern const std::string WARMUP_START;
    extern const std::string WARMUP_END;
    extern const std::string SUBSTITUTION;

    // Capacity related changes
    extern const std::string CAPACITY_ALERT;
    extern const std::string CAPACITY_NORMAL;
    extern const std::string FULL_CAPACITY;

    // Safety-related changes
    extern const std::string WEATHER_ALERT;
    extern const std::string PAUSE;
    extern const std::string RESUME;
    extern const std::string EVACUATE;
    extern const std::string INJURY;
    extern const std::string MEDICAL_EMERGENCY;
}

// NOTIFICATION DATA - For push model

// Container for all notification information in push model
// This struct holds complete state information that is pushed to all observers.
 
struct NotificationData
{
    std::string noticeType;                           
    std::string message;                              // Message content - ADDED
    std::string timestamp;                             
    std::string source;                              
    std::string affectedEntity;                       
    std::map<std::string, std::string> additionalData; 

    // Constructor with basic notification info
    NotificationData(const std::string &type = "", const std::string &msg = "");

    // Add additional data to the notification
    void addData(const std::string &key, const std::string &value);
};

// Helper function for integer to string conversion
std::string toString(int value);

// SUBJECT ABSTRACTION
// Abstract base class for all subjects in the Observer pattern
// Maintains a list of observers and provides methods to attach, detach, and notify them.

class Subject{
private:
    std::vector<Observer *> observers; 

protected:
    void notifyObservers(const std::string &noticeType, const std::string &message);
    void notifyWithDetails(const NotificationData &data);

public:
    Subject() {}
    virtual ~Subject() {}

    virtual void attach(Observer *observer);
    virtual void detach(Observer *observer);
    size_t getObserverCount() const;
};

// OBSERVER ABSTRACTION

// Abstract base class for all observers in the Observer pattern
// Observers register with subjects and receive notifications when events occur.
// Observers are responsible for cleaning up their registrations.

class Observer{
private:
    std::vector<Subject *> attachedSubjects; 

public:
    Observer() {}
    virtual ~Observer();

    virtual void update(Subject *subject, const std::string &noticeType, const std::string &message) = 0;

    virtual void updateWithData(Subject *subject, const NotificationData &data);

    void registerSubject(Subject *subject);
    void detachFromAllSubjects();
    size_t getAttachedSubjectCount() const;
};

// CONCRETE OBSERVERS

// Observer that displays match scores and information
 
class ScoreBoard : public Observer{
private:
    std::string name; // Scoreboard identifier 

public:
    ScoreBoard(const std::string &n);
    ~ScoreBoard();

    void update(Subject *subject, const std::string &noticeType, const std::string &message);
    void updateWithData(Subject *subject, const NotificationData &data);
};

// MedicalTeam
// Observer that handles medical emergencies and injuries
class MedicalTeam : public Observer
{
private:
    std::string name; 
    bool onDuty;      

public:
    MedicalTeam(const std::string &n, bool duty = true);
    ~MedicalTeam();

    void update(Subject *subject, const std::string &noticeType,
                const std::string &message);
    void updateWithData(Subject *subject, const NotificationData &data);
};

// SpectatorGate
 
class SpectatorGate : public Observer
{
private:
    std::string name;    
    int currentCapacity; 
    int maxCapacity;    
    bool isOpen;        

public:
    SpectatorGate(const std::string &n, int max = 500);
    ~SpectatorGate();

    void update(Subject *subject, const std::string &noticeType,
                const std::string &message);
    void updateWithData(Subject *subject, const NotificationData &data);

    void open();
    void close();
    void setCapacity(int count);
};

// InformationDesk

class InformationDesk : public Observer
{
private:
    std::string name;                 
    std::vector<std::string> updates; 

public:
    InformationDesk(const std::string &n);
    ~InformationDesk();

    void update(Subject *subject, const std::string &noticeType,
                const std::string &message);
    void updateWithData(Subject *subject, const NotificationData &data);
    void displayUpdates() const;
};

// Forward declarations for composite classes
class Court;
class Zone;

// CONCRETE SUBJECTS

// NetballMatch
// Represents an individual netball match
 
// This is a leaf in the Composite pattern and a concrete subject in the Observer pattern.
class NetballMatch : public Subject
{
private:
    std::string matchId;  
    std::string team1;    
    std::string team2;    
    int score1;           
    int score2;           
    std::string status;   
    std::string court;    
    int spectatorCount;   
    bool isWeatherDelay;  
    bool isInjuryTimeout;

public:
    NetballMatch(const std::string &id, const std::string &t1, const std::string &t2, const std::string &c = "Court 1");
    ~NetballMatch();

    // Getters
    std::string getMatchId() const { return matchId; }
    std::string getStatus() const { return status; }
    std::string getCourt() const { return court; }
    int getScore1() const { return score1; }
    int getScore2() const { return score2; }
    int getSpectatorCount() const { return spectatorCount; }

    // Operational change notifications
    void updateScore(int newScore1, int newScore2);
    void rescheduleMatch(const std::string &newCourt, const std::string &newTime);
    void startMatch();
    void completeMatch();
    void notifySubstitution(const std::string &playerIn, const std::string &playerOut);
    void notifyWarmupStart();
    void notifyWarmupEnd();

    // Capacity-related notifications
    void updateSpectatorCount(int count);
    void notifyCapacityAlert(int currentCapacity, int maxCapacity);

    // Safety-related notifications
    void issueWeatherAlert(const std::string &weatherCondition);
    void evacuateMatch();
    void pauseMatch(const std::string &reason);
    void resumeMatch();
    void notifyInjury(const std::string &playerName);
    void medicalEmergency();
};

// COMPOSITE CLASSES (Also Subjects)

// TournamentDay
// Represents a day in the tournament
// This is a composite in the Composite pattern and can act as both subject and observer.
 
class TournamentDay : public Subject
{
private:
    std::string dayName;                
    std::string schedule;                
    std::vector<NetballMatch *> matches; 
    std::vector<Court *> courts;         
    bool isActive;                       

public:
    TournamentDay(const std::string &name);
    ~TournamentDay();

    void addMatch(NetballMatch *match);
    void addCourt(Court *court);
    void changeSchedule(const std::string &newSchedule);
    void activateDay();
    void deactivateDay();

    void receiveAndCascadeNotification(const std::string &noticeType, const std::string &message);
    void receiveAndCascadeWithDetails(const NotificationData &data);

    // Getters
    std::string getDayName() const { return dayName; }
    bool isActiveDay() const { return isActive; }
    size_t getMatchCount() const { return matches.size(); }
};

// Court
// Represents a court in the tournament
// This is a composite in the Composite pattern.
 
class Court : public Subject
{
private:
    std::string courtName;              
    std::string courtType;               
    int capacity;                        
    int currentOccupancy;                
    std::vector<NetballMatch *> matches; 
    TournamentDay *parentDay;            

public:
    Court(const std::string &name, const std::string &type = "Main", int cap = 500);
    ~Court();

    void addMatch(NetballMatch *match);
    void setParentDay(TournamentDay *day);
    void updateOccupancy(int count);

    void receiveAndCascadeNotification(const std::string &noticeType, const std::string &message);
    void receiveAndCascadeWithDetails(const NotificationData &data);

    // Getters
    std::string getCourtName() const { return courtName; }
    int getCapacity() const { return capacity; }
    int getCurrentOccupancy() const { return currentOccupancy; }
    size_t getMatchCount() const { return matches.size(); }
};

// Represents a zone in the tournament
// This is the top-level composite in the Composite pattern.
 
class Zone : public Subject
{
private:
    std::string zoneName;        
    std::string division;        
    std::vector<Court *> courts; 
    TournamentDay *parentDay;    

public:
    Zone(const std::string &name, const std::string &div = "Open");
    ~Zone();

    void addCourt(Court *court);
    void setParentDay(TournamentDay *day);

    void receiveAndCascadeNotification(const std::string &noticeType, const std::string &message);
    void receiveAndCascadeWithDetails(const NotificationData &data);

    // Getters
    std::string getZoneName() const { return zoneName; }
    std::string getDivision() const { return division; }
    size_t getCourtCount() const { return courts.size(); }
};

// DEMONSTRATION FUNCTIONS

void demonstrateCascade();
void demonstratePushModel();
void runFullDemonstration();

#endif // EVENTFLOW_H