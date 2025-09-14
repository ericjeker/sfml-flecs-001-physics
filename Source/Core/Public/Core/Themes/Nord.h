// Copyright (c) Eric Jeker. All Rights Reserved.

#pragma once



#include <SFML/Graphics/Color.hpp>

/**
 * Nord is a beautiful theme from Arctic Ice Studio: https://www.nordtheme.com/
 *
 * Note that I didn't use the same naming convention and kept the color's name instead
 * of naming each color "nord0, nord1, ... nordx".
 */
struct NordTheme
{
    // Polar Night
    static constexpr auto PolarNight1 = sf::Color(46, 52, 64);  // #2e3440
    static constexpr auto PolarNight2 = sf::Color(59, 66, 82);  // #3b4252
    static constexpr auto PolarNight3 = sf::Color(67, 76, 94);  // #434c5e
    static constexpr auto PolarNight4 = sf::Color(76, 86, 106); // #4c566a

    // Snow Storm
    static constexpr auto SnowStorm1 = sf::Color(216, 222, 233); // #d8dee9
    static constexpr auto SnowStorm2 = sf::Color(229, 233, 240); // #e5e9f0
    static constexpr auto SnowStorm3 = sf::Color(236, 239, 244); // #eceff4

    // Frost
    static constexpr auto Frost1 = sf::Color(143, 188, 187); // #8fbcbb
    static constexpr auto Frost2 = sf::Color(136, 192, 208); // #88c0d0
    static constexpr auto Frost3 = sf::Color(129, 161, 193); // #81a1c1
    static constexpr auto Frost4 = sf::Color(94, 129, 172);  // #5e81ac

    // Aurora
    static constexpr auto Aurora1 = sf::Color(191, 97, 106);  // #bf616a (Red)
    static constexpr auto Aurora2 = sf::Color(208, 135, 112); // #d08770 (Orange)
    static constexpr auto Aurora3 = sf::Color(235, 203, 139); // #ebcb8b (Yellow)
    static constexpr auto Aurora4 = sf::Color(163, 190, 140); // #a3be8c (Green)
    static constexpr auto Aurora5 = sf::Color(180, 142, 173); // #b48ead (Purple)
};


