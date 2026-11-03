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
#include <chrono>
using namespace std; 
#pragma once 
enum MaterialType;
struct tile; 
struct enchant;
struct weapon;
struct tool;
struct armor;
class gameClock;
class world;

void clearEnchants(weapon w);
void addEnchants(weapon w);

void addEnchants(tool t);
void clearEnchants(tool t);

void addEnchants(armor a);
void clearEnchants(armor a);

void upgradeWeapon(weapon w);
void destroyWeapon(weapon w);

