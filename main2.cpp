#include "raylib.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>

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
    state_menu,
    state_gameplay,
    state_paused, 
    state_skill_tree,
    state_setting,
    state_controls
};

int main()
{
    // First parse the settings.json file
    ifstream file("config.json");
    json config = json::parse(file);

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
    InitWindow(screen_width, screen_height, "Lanterns");
    SetTargetFPS(60);

    GameState currentState = state_menu; 

    int buttonWidth = 100; 
    int buttonHeight = 50;
    int buttonX = (screen_width / 2) - (buttonWidth / 2);
    int buttonToggleX = 10.0f;
    int buttonToggleWidth = 50; 
    int SettingsbuttonWidth = 200;

    Rectangle startButton = { (float)buttonX, 250.0f, (float)buttonWidth, (float)buttonHeight };
    Rectangle socialButton = { (float)buttonX, 330.0f, (float)buttonWidth, (float)buttonHeight };
    Rectangle settingButton = { (float)buttonX, 410.0f, (float)buttonWidth, (float)buttonHeight };
    Rectangle stopButton = { (float)buttonX, 490.0f, (float)buttonWidth, (float)buttonHeight };
    
    
    //settings Buttons
    Rectangle FullscreenButton = { (float)buttonX, 250.0f, (float)SettingsbuttonWidth, (float)buttonHeight };
    Rectangle VsyncButton = { (float)buttonX, 330.0f, (float)SettingsbuttonWidth, (float)buttonHeight };
    Rectangle ControlsButton = { (float)buttonX, 410.0f, (float)SettingsbuttonWidth, (float)buttonHeight };
    Rectangle ShowfpsButton = { (float)buttonX, 490.0f, (float)SettingsbuttonWidth, (float)buttonHeight };
    Rectangle backButton = { (float)buttonX, 570.0f, (float)SettingsbuttonWidth, (float)buttonHeight };
    Rectangle ToggleModeButton = { (float)buttonToggleX, 10.0f, (float)buttonToggleWidth, (float)buttonHeight };

    Vector2 mousePoint = {0.0f, 0.0f};
    bool exitGame = false;

    int MonitorRefreshRate = GetMonitorRefreshRate(GetCurrentMonitor());

    //checking settings needed to run before opening window
    
    

    while (!WindowShouldClose() && !exitGame)
    {

        if(vsync == true) {SetConfigFlags(FLAG_VSYNC_HINT); SetTargetFPS(MonitorRefreshRate);}
        else {ClearWindowState(FLAG_VSYNC_HINT); SetTargetFPS(0);}
        if(fullscreen == true) {SetConfigFlags(FLAG_FULLSCREEN_MODE);}
        else {ClearWindowState(FLAG_FULLSCREEN_MODE);}

        

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
                    darkMode = !darkMode;
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
        if(darkMode == true) ClearBackground(BLACK);
        else ClearBackground(RAYWHITE);
        if(showfps == true) {DrawFPS(75 , 10);}
        
        

        switch (currentState)
        {
            case state_menu: 
            {
                DrawText("Lanterns", screen_width / 2 - MeasureText("Lanterns", 40) / 2, 120, 40, DARKGREEN);

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
                DrawRectangleRec(stopButton, hoverExit ? LIGHTGRAY : GRAY);
                DrawRectangleLinesEx(stopButton, 2, DARKGRAY);
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
                
            }
        } 
        if(IsWindowFullscreen()) { fullscreen = true;}
        else { fullscreen = false;}
        EndDrawing();
    }


    config["showfps"] = showfps;
    config["fullscreen"] = fullscreen;
    config["dark_mode"] = darkMode;
    config["vsync"] = vsync;

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
