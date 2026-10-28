#include "gameplay.h"

enum MaterialType {
    dirt,
    grass,
    stone,
    wood,
    mud
}
struct tile 
{
    int id; 
    MaterialType material;
    bool isSolid; 
}







//defining base functions, classes and structs for gameplay
class gameClock 
{
private:
    using Clock = chrono::steady_clock;
    using timePoint - chrono::time_point<Clock>;
    using duration = chrono::duration<float>;

    timePoint m_startTime;
    timePoint m_lastFrameTime;

    float m_totalgameplayTime;
    bool m_isPaused;
public:
    gameClock()
    : m_totalgameplayTime(0.0f), m_isPaused(true)
    {
        m_startTime = Clock::now():
        m_lastFrameTime = m_startTime; 
    }

    void resume() 
    {
        if(m_isPaused)
        {
            m_isPaused = false;
            m_lastFrametime = Clock::now();
        }
    }

    void pause() 
    {
        m_isPaused = true; 
    }

    float update() 
    {
        timePoint currentFrameTime = Clock::now();
        duration frameDuration - currentFrameTime - m_lastFrameTime; 
        m_lastFrameTime = currentFrameTime; 

        float deltaTime = frameDuration.count();

        if(!m_isPaused)
        {
            m_totalgameplayTime += deltaTime;
            return deltaTime;
        }
        return 0.0f;
    }
    float getTotatlGameplayTime() const {return m_totalgameplayTime;}
    bool isPaused() const {return m_isPaused;}
}

//basic explanation of the gameClock class
//initialize the gameClass object in the main before the loop 
//once the loop starts integrate a update function near the top of the loop 
//if the gamestate changes from gameplay --> paused then we use the 
//gameClock.pause() function to make sure the time is paused.
//we then use whenever gameState == gameplay then the clock resumes 

//Track the totalplaytime outside of both loops/if statements
//we will track the progress using gameClock.getTotalGameplayTime();
//when the game closes then we need to save the time clock to a file that is private

//-------------------------------------------------
//World Class 
Class world
{
private:
    gameClock worldClock;
    float xpRate;
    float decayRates;
    float damageMultipliers;
    bool deathPenalty;
    float droppeditemMultiplier;
    float workSpeed;
    int weightLimit;

public:
    world(float xpr, float dcr, float dmult, bool dp, float dim, float ws, int wl)
    {
        worldClock;
        xpRate = xpr;
        decayRates = dcr;
        damageMultipliers = dmult;
        deathPenalty = dp;
        droppeditemMultiplier = dim;
        workSpeed = ws;
        weightLimit = wl;
    }

    
    
}



struct player
{

}

struct EnvironmentObject
{
    Rectanlge rect; 
    MaterialType material;
};

MaterialType checkfloorMaterial(player mainp, vector<EnvironmentObject>& worldObjects)
{
    Rectangle feetSensor = {mainp.position.x , mainp.position.y + mainp.height, mainp.width, 2};

    for(const auto& obj : worldObjects)
    {
        if(CheckCollisionRecs(feetSensor, obj.rect))
        {
            return obj.material;
        }
    }
    return dirt;
}
