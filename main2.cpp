#include "raylib.h"
#include <iostream>
#include <unordered_map>
#include <functional>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

using namespace std;
using json = nlohmann::json;


struct player {
    int velocity;
    float speed; 
    float jumpHeight;
    float height;
    float width;
};

enum GameState {
    state_loading_screen,
    state_menu,
    state_gameplay,
    state_paused, 
    state_skill_tree,
    state_setting,
    state_controls
};

enum keyaction {
    moveUp,
    moveDown, 
    moveLeft,
    moveRight,
    jump,
    attack,
    block,
    interact
};

char keycodeConvert(int k)
{
    return (char)k; 
}

bool TryRebindKey(int* targetkey)
{
    int pressedkey = GetKeyPressed();
    if(pressedkey != KEY_NULL)
    {
        *targetkey = pressedkey;
        return true; 
    }
    return false;
}
unordered_map<keyaction, int> keybinds = 
    {
        {keyaction::moveUp,      0},
        {keyaction::moveDown,    0},
        {keyaction::moveLeft,    0},
        {keyaction::moveRight,   0},
        {keyaction::jump,        0},
        {keyaction::attack,      0},
        {keyaction::block,       0},
        {keyaction::interact,    0}
    };
const char* getActionName(keyaction action)
{
    switch(action)
    {
        case keyaction::moveUp:          return "Move Up";
        case keyaction::moveDown:        return "Move Down";
        case keyaction::moveLeft:        return "Move Left";
        case keyaction::moveRight:       return "Move Right";
        case keyaction::jump:            return "Jump";
        case keyaction::interact:        return "Interact";
        case keyaction::attack:          return "Attack";
        case keyaction::block:           return "Block";
        default:                         return "Unknown";
    }
}


int main()
{
    // First parse the settings.json file
    ifstream file("config.json");
    json config = json::parse(file);

    //rebind keybinds from the json file parsed above.
    keybinds = {{moveUp, config["moveUp"]}, {moveDown, config["moveDown"]}, {moveLeft, config["moveLeft"]}, {moveRight, config["moveRight"]}, {jump, config["jump"]}, {attack , config["attack"]}, {block, config["block"]}
                , {interact, config["interact"]}};

    bool fullscreen = config["fullscreen"];
    bool vsync = config["vsync"];
    bool darkMode = config["dark_mode"];
    bool showfps = config["showfps"];

    string vsc = "";
    const char* vsc_c;
    string fsc = "";
    const char* fsc_c;
    string sfps = "";
    const char* sfps_c;

    float screen_height = 1080;
    float screen_width = 1920;  
    InitWindow(screen_width, screen_height, "Game");
    InitAudioDevice();
    SetTargetFPS(60);

    GameState currentState = state_menu; 
    //music variables
    SetMasterVolume(1.0f);
    Music dayMusic = LoadMusicStream("Audio/day.mp3");
    Music NightMusic = LoadMusicStream("Audio/night.mp3");
    //background variables

    
    string backgroundCloudsFilePath = "Textures/Clouds2.png";
    string backgroundFilePath = "Textures/background.png";
    Texture2D background = LoadTexture("Textures/Background.png");
    Texture2D backgroundDark = LoadTexture("Textures/BackgroundDark.png");
    Texture2D backgroundClouds = LoadTexture("Textures/Clouds2.png");   
    Texture2D backgroundCloudsDark = LoadTexture("Textures/Clouds2Dark.png");   
      
    float scrollingBack = -1000.0f;
    

    //buttons
    int buttonWidth = 400; 
    int buttonHeight = 50;
    int buttonX = (screen_width / 2) - (buttonWidth / 2);
    int buttonToggleX = 10.0f;
    int buttonToggleWidth = 50; 
    int SettingsbuttonWidth = 400;


    //keybinds  --- O(1) using a hashmap
    
    int mousesensitivityx = config["mousesensitivityx"]; int mousesensitivityy = config["mousesensitivityy"];
    


    //keybind logic variables
    bool isRebinding = false;
    keyaction actionbeingmod = keyaction::moveUp;

    
    //scroll bar variables
    int minValue = 0; 
    int maxValue = 100;
    
    //sccroll bar rectangles
    Rectangle scrollbounds = {100 , 100 , 200, 20};
    Rectangle scrollbounds2 = {100, 150, 200, 20};
    Rectangle scrollbounds3 = {100, 200, 200, 20};
    Rectangle scrollbounds4 = {100, 250, 200, 20};
    Rectangle scrollbounds5 = {100, 300, 200, 20};
    Rectangle scrollbounds6 = {100, 350, 200, 20};
    Rectangle scrollbounds7 = {100, 400, 200, 20};
    Rectangle scrollbounds8 = {100, 450, 200, 20};
    Rectangle scrollbounds9 = {100, 500, 200, 20};
    Rectangle scrollbounds10 = {100, 550, 200, 20};


    //menu button rectangles
    Rectangle startButton = { (float)buttonX, 350.0f, (float)buttonWidth, (float)buttonHeight };
    Rectangle socialButton = { (float)buttonX, startButton.y + 75.0f, (float)buttonWidth, (float)buttonHeight };
    Rectangle settingButton = { (float)buttonX, socialButton.y + 75.0f, (float)buttonWidth, (float)buttonHeight };
    Rectangle stopButton = { (float)buttonX, settingButton.y + 200.0f, (float)buttonWidth, (float)buttonHeight };
    
    
    //settings Buttons
    Rectangle FullscreenButton = { (float)buttonX, 250.0f, (float)SettingsbuttonWidth, (float)buttonHeight };
    Rectangle VsyncButton = { (float)buttonX, 330.0f, (float)SettingsbuttonWidth, (float)buttonHeight };
    Rectangle ControlsButton = { (float)buttonX, 410.0f, (float)SettingsbuttonWidth, (float)buttonHeight };
    Rectangle ShowfpsButton = { (float)buttonX, 490.0f, (float)SettingsbuttonWidth, (float)buttonHeight };
    Rectangle backButton = { (float)buttonX, 570.0f, (float)SettingsbuttonWidth, (float)buttonHeight };
    Rectangle ToggleModeButton = { (float)buttonToggleX, 10.0f, (float)buttonToggleWidth, (float)buttonHeight };
    
    
    //controls settings
    Rectangle walkupButton = { 400, 100, (float)SettingsbuttonWidth, (float)buttonHeight };
    Rectangle walkdownButton = { 400, walkupButton.y + 75, (float)SettingsbuttonWidth, (float)buttonHeight };
    Rectangle walkleftButton = { 400, walkdownButton.y + 75, (float)SettingsbuttonWidth, (float)buttonHeight };
    Rectangle walkrightButton = { 400, walkleftButton.y + 75, (float)SettingsbuttonWidth, (float)buttonHeight };
    Rectangle jumpButton = {400 , walkrightButton.y + 75, (float)SettingsbuttonWidth, (float)buttonHeight };

    Vector2 mousePoint = {0.0f, 0.0f};
    bool exitGame = false;

    int MonitorRefreshRate = GetMonitorRefreshRate(GetCurrentMonitor());

    //checking settings needed to run before opening window
    
    SetMusicVolume(dayMusic, 1.0f);
    

    while (!WindowShouldClose() && !exitGame)
    {

        if(vsync == true) {SetConfigFlags(FLAG_VSYNC_HINT); SetTargetFPS(MonitorRefreshRate);}
        else {ClearWindowState(FLAG_VSYNC_HINT); SetTargetFPS(0);}
        if(fullscreen == true) {SetConfigFlags(FLAG_FULLSCREEN_MODE);}
        else {ClearWindowState(FLAG_FULLSCREEN_MODE);}

        UpdateMusicStream(NightMusic);
        UpdateMusicStream(dayMusic);

        if(isRebinding)
        {
            int* keyptr = &keybinds[actionbeingmod];

            if(TryRebindKey(keyptr))
            {
                isRebinding = false;
            }
        }


        //background editing 

        if(GetFPS() > 240)
        {
            scrollingBack += 0.001f;
        }
        if(GetFPS() <= 240)
        {
            scrollingBack += .01;
        }
        if(GetFPS() <= 120)
        {
            scrollingBack += .5;
        }
        if(scrollingBack > -500.0f) scrollingBack = -1000; 
        

        mousePoint = GetMousePosition();

        // Game Logic Switch
        switch (currentState)
        {
            case state_menu:
            {
                if (CheckCollisionPointRec(mousePoint, startButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    currentState = state_gameplay;
                }
                if (CheckCollisionPointRec(mousePoint, stopButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    exitGame = true; 
                }
                if (CheckCollisionPointRec(mousePoint, settingButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    currentState = state_setting; // Fixed: Now actually transitions to settings instead of exiting
                }
                if (CheckCollisionPointRec(mousePoint, socialButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    exitGame = true; 
                }
                if (CheckCollisionPointRec(mousePoint, ToggleModeButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    darkMode = !darkMode;
                }
            } break; 
            
            case state_gameplay: 
            {
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    currentState = state_paused; 
                }
            } break; 
            
            case state_paused:
                if (IsKeyPressed(KEY_ESCAPE)) currentState = state_menu;
                break;
                
            case state_skill_tree:
                break;
              
            case state_setting: 
            {
                // Toggle mode can still be used here because the button draw is rendered in both screens
                if (CheckCollisionPointRec(mousePoint, FullscreenButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    if(IsWindowFullscreen()) { ToggleFullscreen();SetWindowSize(1280, 720);}
                    else
                    {
                        int currentMonitor = GetCurrentMonitor();
                        SetWindowSize(GetMonitorWidth(currentMonitor), GetMonitorHeight(currentMonitor));
                        ToggleFullscreen();
                    }
                }
                // Back to Main Menu functionality
                if (CheckCollisionPointRec(mousePoint, VsyncButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    vsync = !vsync;
                    
                }
                if (CheckCollisionPointRec(mousePoint, ControlsButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    currentState = state_controls;
                }
                // Back to Main Menu functionality
                if (CheckCollisionPointRec(mousePoint, ShowfpsButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    showfps = !showfps;
                }
                if (CheckCollisionPointRec(mousePoint, ToggleModeButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    darkMode = !darkMode;
                }
                // Back to Main Menu functionality
                if (CheckCollisionPointRec(mousePoint, backButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    currentState = state_menu;
                }
            } break;

            case state_controls:
            {
                if(CheckCollisionPointRec(mousePoint, walkdownButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    isRebinding = true; 
                    actionbeingmod = keyaction::moveDown;
                }
                if(CheckCollisionPointRec(mousePoint, walkupButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    isRebinding = true; 
                    actionbeingmod = keyaction::moveUp;
                }
                if(CheckCollisionPointRec(mousePoint, walkleftButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    isRebinding = true; 
                    actionbeingmod = keyaction::moveLeft;
                }
                if(CheckCollisionPointRec(mousePoint, walkrightButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    isRebinding = true; 
                    actionbeingmod = keyaction::moveRight;
                }
                if(CheckCollisionPointRec(mousePoint, jumpButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    isRebinding = true; 
                    actionbeingmod = keyaction::jump;
                }

            } break; 
        }

        vsc = vsync ? "-- True" : "-- False";
        vsc_c = vsc.c_str();
        fsc = fullscreen ? "-- True" : "-- False";
        fsc_c = fsc.c_str();
        sfps = showfps ? "-- True" : "-- False";
        sfps_c = sfps.c_str();



        // Drawing Switch
        BeginDrawing();
        if(darkMode == true) 
        {
            ClearBackground(BLACK);
            
        }
        else 
        {
            ClearBackground(RAYWHITE);
        }
        
        Vector2 sec = { backgroundClouds.width*2 + scrollingBack, 20 };
        Vector2 fir = { scrollingBack, 20 };
        if(darkMode == false)
        {   
            DrawTexture(background, 0.0f, 0.0f, WHITE);
            DrawTextureEx(backgroundClouds, fir , 0.0f, 0.75f, WHITE);
            DrawTextureEx(backgroundClouds, sec , 0.0f, 0.75f, WHITE);
        }
        else
        {
            DrawTexture(backgroundDark, 0.0f, 0.0f, WHITE);
            DrawTextureEx(backgroundCloudsDark, fir , 0.0f, 0.75f, WHITE);
            DrawTextureEx(backgroundCloudsDark, sec , 0.0f, 0.75f, WHITE);
        }

        if(isRebinding)
        {
            DrawText(TextFormat("Press any key to rebind: %s", getActionName(actionbeingmod)), 200, 200, 20, RED );
        }
        



        if(showfps == true) {DrawFPS(75 , 10);}
        
        

        switch (currentState)
        {
            case state_menu: 
            {
                DrawText("Game", screen_width / 2 - MeasureText("Game", 40) / 2, 50, 40, WHITE);

                bool hoverstart = CheckCollisionPointRec(mousePoint, startButton);
                DrawRectangleRec(startButton, hoverstart ? LIGHTGRAY : GRAY);
                DrawRectangleLinesEx(startButton, 2, DARKGRAY);
                DrawText("Start", (int)(startButton.x + (buttonWidth / 2) - MeasureText("Start", 20) / 2), (int)(startButton.y + 15), 20, hoverstart ? BLACK : WHITE );

                bool hoverSocial = CheckCollisionPointRec(mousePoint, socialButton);
                DrawRectangleRec(socialButton, hoverSocial ? LIGHTGRAY : GRAY);
                DrawRectangleLinesEx(socialButton, 2, DARKGRAY);
                DrawText("Social", (int)(socialButton.x + (buttonWidth / 2) - MeasureText("Social", 20) / 2), (int)(socialButton.y + 15), 20, hoverSocial ? BLACK : WHITE);

                bool hoverSettings = CheckCollisionPointRec(mousePoint, settingButton);
                DrawRectangleRec(settingButton, hoverSettings ? LIGHTGRAY : GRAY);
                DrawRectangleLinesEx(settingButton, 2, DARKGRAY);
                DrawText("Settings", (int)(settingButton.x + (buttonWidth / 2) - MeasureText("Settings", 20) / 2), (int)(settingButton.y + 15), 20, hoverSettings ? BLACK : WHITE);

                bool hoverExit = CheckCollisionPointRec(mousePoint, stopButton);
                DrawRectangleRec(stopButton, hoverExit ? PINK : DARKPURPLE);
                DrawRectangleLinesEx(stopButton, 2, PURPLE);
                DrawText("EXIT", (int)(stopButton.x + (buttonWidth / 2) - MeasureText("EXIT", 20) / 2), (int)(stopButton.y + 15), 20, hoverExit ? BLACK : WHITE);

                // Keep Toggle Mode button visible on menu
                bool hoverToggleMode = CheckCollisionPointRec(mousePoint, ToggleModeButton);
                DrawRectangleRec(ToggleModeButton, hoverToggleMode ? LIGHTGRAY : GRAY);
                DrawRectangleLinesEx(ToggleModeButton, 2, DARKGRAY);
                DrawText("[TM]", (int)(ToggleModeButton.x + (buttonToggleWidth / 2) - MeasureText("[TM]", 20) / 2), (int)(ToggleModeButton.y + 15), 20, hoverToggleMode ? BLACK : WHITE);
                    
                DrawText("Use mouse to navigate", 10, screen_height - 30, 20, GRAY);
            } break;

            case state_setting: 
            {
                DrawText("Settings Menu", screen_width / 2 - MeasureText("Settings Menu", 40) / 2, 120, 40, DARKGREEN);

                bool hoverToggleMode = CheckCollisionPointRec(mousePoint, ToggleModeButton);
                DrawRectangleRec(ToggleModeButton, hoverToggleMode ? LIGHTGRAY : GRAY);
                DrawRectangleLinesEx(ToggleModeButton, 2, DARKGRAY);
                DrawText("[TM]", (int)(ToggleModeButton.x + (buttonToggleWidth / 2) - MeasureText("[TM]", 20) / 2), (int)(ToggleModeButton.y + 15), 20, hoverToggleMode ? BLACK : WHITE);
                
                bool hoverFullscreen = CheckCollisionPointRec(mousePoint, FullscreenButton);
                DrawRectangleRec(FullscreenButton, hoverFullscreen ? LIGHTGRAY : GRAY);
                DrawRectangleLinesEx(FullscreenButton, 2, DARKGRAY);
                DrawText("Fullscreen", (int)(FullscreenButton.x + (SettingsbuttonWidth / 2) - MeasureText("Fullscreen", 20) / 2), (int)(FullscreenButton.y + 15), 20, hoverFullscreen ? BLACK : WHITE );
                DrawText((fsc_c), (int)(FullscreenButton.x + (SettingsbuttonWidth) + 10), (int)(FullscreenButton.y + 15), 20, GREEN);

                bool hoverVsync = CheckCollisionPointRec(mousePoint, VsyncButton);
                DrawRectangleRec(VsyncButton, hoverVsync ? LIGHTGRAY : GRAY);
                DrawRectangleLinesEx(VsyncButton, 2, DARKGRAY);
                DrawText("Vsync", (int)(VsyncButton.x + (SettingsbuttonWidth / 2) - MeasureText("Vsync", 20) / 2), (int)(VsyncButton.y + 15), 20, hoverVsync ? BLACK : WHITE);
                DrawText((vsc_c), (int)(VsyncButton.x + (SettingsbuttonWidth) + 10), (int)(VsyncButton.y + 15), 20, GREEN);

                bool hoverControls = CheckCollisionPointRec(mousePoint, ControlsButton);
                DrawRectangleRec(ControlsButton, hoverControls ? LIGHTGRAY : GRAY);
                DrawRectangleLinesEx(ControlsButton, 2, DARKGRAY);
                DrawText("Controls", (int)(ControlsButton.x + (SettingsbuttonWidth / 2) - MeasureText("Controls", 20) / 2), (int)(ControlsButton.y + 15), 20, hoverControls ? BLACK : WHITE);

                bool hoverShowfps = CheckCollisionPointRec(mousePoint, ShowfpsButton);
                DrawRectangleRec(ShowfpsButton, hoverShowfps ? LIGHTGRAY : GRAY);
                DrawRectangleLinesEx(ShowfpsButton, 2, DARKGRAY);
                DrawText("ShowFPS?", (int)(ShowfpsButton.x + (SettingsbuttonWidth / 2) - MeasureText("ShowFPS?", 20) / 2), (int)(ShowfpsButton.y + 15), 20, hoverShowfps ? BLACK : WHITE);
                DrawText((sfps_c), (int)(ShowfpsButton.x + (SettingsbuttonWidth) + 10), (int)(ShowfpsButton.y + 15), 20, GREEN);
                // Back Button
                bool hoverBack = CheckCollisionPointRec(mousePoint, backButton);
                DrawRectangleRec(backButton, hoverBack ? LIGHTGRAY : GRAY);
                DrawRectangleLinesEx(backButton, 2, DARKGRAY);
                DrawText("Back", (int)(backButton.x + (SettingsbuttonWidth / 2) - MeasureText("Back", 20) / 2), (int)(backButton.y + 15), 20, hoverBack ? BLACK : WHITE);
            } break;

            case state_gameplay: 
            {
                DrawRectangle(0, 0, screen_width, screen_height, DARKBLUE);
                DrawText("GAMEPLAY ACTIVE", screen_width / 2 - MeasureText("GAMEPLAY ACTIVE", 30) / 2, screen_height / 2 - 15, 30, RAYWHITE);
                DrawText("Press ESC to return to Main Menu", screen_width / 2 - MeasureText("Press ESC to return to Main Menu", 20) / 2, screen_height / 2 + 40, 20, LIGHTGRAY);
                
            } break; 

            case state_paused:
            {
                DrawRectangle(0, 0, screen_width, screen_height, RED);
                DrawText("PAUSED", screen_width / 2 - MeasureText("PAUSED", 30) / 2, screen_height / 2, 30, WHITE);
            } break;
            
            case state_skill_tree:
                break;

            case state_controls:
            {
                
                mousesensitivityx = GuiScrollBar(scrollbounds, mousesensitivityx, minValue, maxValue);
                DrawText(TextFormat("Mouse Sensitivity X: %i", mousesensitivityx), scrollbounds.x, scrollbounds.y + 25, 20, WHITE);
                mousesensitivityy = GuiScrollBar(scrollbounds2, mousesensitivityy, minValue, maxValue);
                DrawText(TextFormat("Mouse Sensitivity Y: %i", mousesensitivityy), scrollbounds2.x, scrollbounds2.y + 25, 20, WHITE);

                bool hoverWalkdown = CheckCollisionPointRec(mousePoint, walkdownButton);
                DrawRectangleRec(walkdownButton, hoverWalkdown ? LIGHTGRAY : GRAY);
                DrawRectangleLinesEx(walkdownButton, 2, DARKGRAY);
                DrawText(TextFormat("Walk Down [KEY]:  %c", keycodeConvert(keybinds.at(moveDown))), (int)(walkdownButton.x + 10), (int)(walkdownButton.y + 15), 20, hoverWalkdown ? BLACK : WHITE);

                bool hoverWalkup = CheckCollisionPointRec(mousePoint, walkupButton);
                DrawRectangleRec(walkupButton, hoverWalkup ? LIGHTGRAY : GRAY);
                DrawRectangleLinesEx(walkupButton, 2, DARKGRAY);
                DrawText(TextFormat("Walk Down [KEY]:  %c", keycodeConvert(keybinds.at(moveUp))), (int)(walkupButton.x + 10), (int)(walkupButton.y + 15), 20, hoverWalkup ? BLACK : WHITE);

                bool hoverWalkleft = CheckCollisionPointRec(mousePoint, walkleftButton);
                DrawRectangleRec(walkleftButton, hoverWalkleft ? LIGHTGRAY : GRAY);
                DrawRectangleLinesEx(walkleftButton, 2, DARKGRAY);
                DrawText(TextFormat("Walk Down [KEY]:  %c", keycodeConvert(keybinds.at(moveLeft))), (int)(walkleftButton.x + 10), (int)(walkleftButton.y + 15), 20, hoverWalkleft ? BLACK : WHITE);

                bool hoverWalkright = CheckCollisionPointRec(mousePoint, walkrightButton);
                DrawRectangleRec(walkrightButton, hoverWalkright ? LIGHTGRAY : GRAY);
                DrawRectangleLinesEx(walkrightButton, 2, DARKGRAY);
                DrawText(TextFormat("Walk Down [KEY]:  %c", keycodeConvert(keybinds.at(moveRight))), (int)(walkrightButton.x + 10), (int)(walkrightButton.y + 15), 20, hoverWalkright ? BLACK : WHITE);

                bool hoverjump = CheckCollisionPointRec(mousePoint, jumpButton);
                DrawRectangleRec(jumpButton, hoverjump ? LIGHTGRAY : GRAY);
                DrawRectangleLinesEx(jumpButton, 2, DARKGRAY);
                DrawText(TextFormat("Jump [KEY]:  %c", keycodeConvert(keybinds.at(jump))), (int)(jumpButton.x + 10), (int)(jumpButton.y + 15), 20, hoverjump ? BLACK : WHITE);

            
                

            }
            break;
        } 
        if(IsWindowFullscreen()) { fullscreen = true;}
        else { fullscreen = false;}
        EndDrawing();
    }

    config["mousesensitivityx"] = mousesensitivityx;
    config["mousesensitivityy"] = mousesensitivityy;
    config["showfps"] = showfps;
    config["fullscreen"] = fullscreen;
    config["dark_mode"] = darkMode;
    config["vsync"] = vsync;
    config["moveDown"] = (int)keybinds.at(moveDown);
    config["moveUp"] = (int)keybinds.at(moveUp);
    config["moveLeft"] = (int)keybinds.at(moveLeft);
    config["moveRight"] = (int)keybinds.at(moveRight);
    config["jump"] = (int)keybinds.at(jump);
    config["interact"] = (int)keybinds.at(interact);
    config["attack"] = (int)keybinds.at(attack);
    config["block"] = (int)keybinds.at(block);

    ofstream outFile("config.json");
    if(!outFile.is_open())
    {
        cerr << "Could not open file config.json\n";
        return 1;
    }
    outFile << std::setw(4) << config << "\n";
    outFile.close();

    CloseWindow();
    
    return 0;
}
