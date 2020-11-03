#pragma once
#include "PlayerGameObject.h"
#include "Map.h"
/// <summary>
/// Funkcja odpowiedzialna za rysowanie menu
/// </summary>
int drawMenu();
/// <summary>
/// Funkcja odpowiedzialna za Wypisanie autor�w
/// </summary>
void showAuthors();
/// <summary>
/// Funkcja odpowiedzialna za Instrukcji Gry
/// </summary>
void showInstructions();
/// <summary>
/// Funkcja odpowiedzialna za statystyki gracza
/// </summary>
void drawStats(PlayerGameObject* player);

/// <summary>
/// Funkcja odpowiedzialna za rysowanie mapy
/// </summary>

void drawMap(Map* map1);
