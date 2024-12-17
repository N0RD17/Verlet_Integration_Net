#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>

#include <vector>

#include "input_handler.h"
#include "particle.h"
#include "constraint.h"

inline constexpr int WIDTH  { 1080 };
inline constexpr int HEIGHT { 640  };
inline constexpr float PARTICLE_RADIUS { 10.0f };
inline constexpr float GRAVITY { 10.0f };
inline constexpr float TIME_STEP { 0.1f  };
inline constexpr int ROW { 30 };
inline constexpr int COL { 30 };
inline constexpr float REST_DISTANCE { 10.0f };

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Cloth Simulation", sf::Style::Default);

    std::vector<Particle> particles{};
    std::vector<Constraint> constraints{};

    for(int row { 0 }; row < ROW; row++)
    {
        for(int col { 0 }; col < COL; col++)
        {
            float x { col * REST_DISTANCE + (float)WIDTH/3};
            float y { row * REST_DISTANCE + (float)HEIGHT/3};
            bool pinned = (row == 0); // Only pin when row is 0
            particles.emplace_back(x, y, pinned);
        }
    }

    // Initialize constraints
    for(int row { 0 }; row < ROW; row++)
    {
        for(int col { 0 }; col < COL; col++)
        {
            if(col < COL - 1) {
                // Horizontal constraint
                constraints.emplace_back(&particles[row * COL + col],
                                         &particles[row * COL + col + 1]);
            }
            if(row < ROW - 1) {
                // Vertical constraint
                constraints.emplace_back(&particles[row * COL + col],
                                         &particles[(row + 1) * COL + col]);
            }
        }
    }
    

    // std::vector<Particle> particles;
    // particles.emplace_back(WIDTH/2 - 50, HEIGHT/2 - 50);
    // particles.emplace_back(WIDTH/2 + 50, HEIGHT/2 + 50);
    // particles.emplace_back(WIDTH/2 + 50, HEIGHT/2 - 50);
    // particles.emplace_back(WIDTH/2 - 50, HEIGHT/2 + 50);

    // std::vector<Constraint> constraints;
    // constraints.emplace_back(&particles[0], &particles[1]);
    // constraints.emplace_back(&particles[0], &particles[2]);
    // constraints.emplace_back(&particles[0], &particles[3]);
    // constraints.emplace_back(&particles[3], &particles[1]);
    // constraints.emplace_back(&particles[1], &particles[2]);
    // constraints.emplace_back(&particles[2], &particles[3]);

    window.setFramerateLimit(60);
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed ||
               sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }

            // Handle mouse clicks
            InputHandler::handleMouseClick(event, constraints);
        }

        // Apply gravity and update particles
        for(auto& particle: particles)
        {
            particle.apply_force(sf::Vector2f(0, GRAVITY));
            particle.update(TIME_STEP);
            // particle.constrain_to_bounds(WIDTH, HEIGHT, PARTICLE_RADIUS);
            particle.constrain_to_bounds(WIDTH, HEIGHT);
        }

        // for(size_t i { 0 }; i < 5; i++)
        // {

        // }

        for (auto& constraint: constraints)
        {
            constraint.satisfy();
        }

        window.clear(sf::Color::Black);

        // Draw Particles as balls
        // for(const auto&  particle: particles)
        // {
        //     sf::CircleShape circle(PARTICLE_RADIUS);
        //     circle.setFillColor(sf::Color::White);
        //     circle.setPosition(particle.position.x - PARTICLE_RADIUS,
        //         particle.position.y - PARTICLE_RADIUS);

        //     window.draw(circle);
        // }

        // Draw particles as points
        for(const auto& particle: particles)
        {
            sf::Vertex point(particle.position, sf::Color::White);
            window.draw(&point, 1, sf::Points);
        }

        // Draw constraints as lines
        for(const auto& constraint: constraints)
        {
            if(!constraint.active) {
                continue;
            }
            sf::Vertex line[] = {
                sf::Vertex(constraint.p1->position, sf::Color::White),
                sf::Vertex(constraint.p2->position, sf::Color::White),
            };

            window.draw(line, 2, sf::Lines);
        }

        window.display();
    }
    
    return 0;
}
