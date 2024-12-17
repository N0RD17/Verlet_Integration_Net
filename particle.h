#pragma once

#include <SFML/Graphics.hpp>

class Particle {
public:
    /** STATIC PARTICLE VALUS **/
    sf::Vector2f position;
    sf::Vector2f previousPosition;
    sf::Vector2f acceleration;
    bool is_pinned;

    Particle(float x, float y, bool pinned = false) : position(x, y), previousPosition(x, y),
                                                      acceleration(0.f, 0.f) { is_pinned = pinned; }

    /** Add gravity to partcile by implementing Vertel Integration **/
    void apply_force(const sf::Vector2f& force)
    {
        if(!is_pinned) {
            acceleration += force;
        }
    }
    void update(float time_step)
    {
        // verlet integration
        if(!is_pinned) {
            sf::Vector2f velocity { position - previousPosition };
            previousPosition = position;
            position += velocity + acceleration * time_step * time_step;
            acceleration = sf::Vector2f(0.f, 0.f); // reset after update
        }
    }

    // Prevent particle from falling at bottomof screen
    // void constrain_to_bounds(float width, float height, float radius)
    // {
    //     if(position.x < radius) {
    //         position.x = radius;
    //     }

    //     if(position.x > width - radius) {
    //         position.x = width - radius;
    //     }

    //     if(position.y < radius) {
    //         position.y = radius;
    //     }

    //     if(position.y > height - radius) {
    //         position.y = height - radius;
    //     }
    // }

    // Prevents particle from falling at bottom of screen
    void constrain_to_bounds(float width, float height)
    {
        if(position.x < 0) position.x = 0;
        if(position.x > width) position.x = width;
        
        if(position.y < 0) position.y = 0;
        if(position.y > height) position.y = height;
    }
};
