#include "raylib.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include "C:\msys64\ucrt64\include\nlohmann\json.hpp"
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
    state_setting
};

int main()
{
    //first parse the settings.json file
    ifstream file("config.json");
    json config = json::parse(file);

    bool darkMode = config["dark_mode"];





















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

    Rectangle startButton = { (float)buttonX, 250.0f, (float)buttonWidth, (float)buttonHeight };
    Rectangle socialButton = { (float)buttonX, 330.0f, (float)buttonWidth, (float)buttonHeight };
    Rectangle settingButton = { (float)buttonX, 410.0f, (float)buttonWidth, (float)buttonHeight };
    Rectangle stopButton = { (float)buttonX, 490.0f, (float)buttonWidth, (float)buttonHeight };
    Rectangle ToggleModeButton = { (float)buttonToggleX, 10.0f, (float)buttonToggleWidth, (float)buttonHeight };

    Vector2 mousePoint = {0.0f, 0.0f};
    bool exitGame = false;

    while (!WindowShouldClose() && !exitGame)
    {
        mousePoint = GetMousePosition();

        // Game Logic Switch
        switch (currentState)
        {
            case state_menu:
            {
                if (CheckCollisionPointRec(mousePoint, startButton))
                {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        currentState = state_gameplay;
                    }
                }
                if (CheckCollisionPointRec(mousePoint, stopButton))
                {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        exitGame = true; // Uncommented to make exit button work
                    }
                }
                if (CheckCollisionPointRec(mousePoint, settingButton))
                {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        exitGame = true; // Uncommented to make exit button work
                    }
                }
                if (CheckCollisionPointRec(mousePoint, socialButton))
                {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        exitGame = true; // Uncommented to make exit button work
                    }
                }
                if (CheckCollisionPointRec(mousePoint, ToggleModeButton))
                {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        darkMode = !darkMode;
                         // Uncommented to make exit button work
                    }
                }
            } break; 
            case state_gameplay: 
            {
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    currentState = state_paused; // Or state_menu depending on preference
                }
            } break; 
            case state_paused: // Added empty case to prevent compiler warnings
                break;
            case state_skill_tree:
                break;
            case state_setting: 
            {
                
            }
        }

        // Drawing Switch
        BeginDrawing();
        if(darkMode == true)
        {
            ClearBackground(BLACK);
        }
        if(darkMode == false)
        {
            ClearBackground(RAYWHITE);
        }

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

                bool hoverToggleMode = CheckCollisionPointRec(mousePoint, ToggleModeButton);
                DrawRectangleRec(ToggleModeButton, hoverToggleMode ? LIGHTGRAY : GRAY);
                DrawRectangleLinesEx(ToggleModeButton, 2, DARKGRAY);
                DrawText("[TM]", (int)(ToggleModeButton.x + (buttonToggleWidth / 2) - MeasureText("[TM]", 20) / 2), (int)(ToggleModeButton.y + 15), 20, hoverToggleMode ? BLACK : WHITE);


                    
                DrawText("Use mouse to navigate", 10, screen_height - 30, 20, GRAY);
            } break;

            case state_gameplay: 
            {
                DrawRectangle(0, 0, screen_width, screen_height, DARKBLUE);
                // Fixed: changed screen_width/2 to screen_height/2 for the Y positions
                DrawText("GAMEPLAY ACTIVE", screen_width / 2 - MeasureText("GAMEPLAY ACTIVE", 30) / 2, screen_height / 2 - 15, 30, RAYWHITE);
                DrawText("Press ESC to return to Main Menu", screen_width / 2 - MeasureText("Press ESC to return to Main Menu", 20) / 2, screen_height / 2 + 40, 20, LIGHTGRAY);
            } break; // Moved inside the closing bracket

            case state_paused:
            {
                DrawRectangle(0, 0, screen_width, screen_height, RED);
                DrawText("PAUSED", screen_width / 2 - MeasureText("PAUSED", 30) / 2, screen_height / 2, 30, WHITE);
            } break;
            
            case state_skill_tree:
                break;
        } // The closing brace MUST be here before EndDrawing

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
