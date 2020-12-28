//
//  Item.h
//  Sorting Algorithms Visualizer
//
//  Created by RyanLazz on 9/9/19.
//  Copyright © 2019 ryanlazzareschi. All rights reserved.
//

#ifndef Item_h
#define Item_h

#include <stdio.h>
#include <SFML/Graphics.hpp>

class Item {
public:
    Item();
    Item(unsigned int maxSize, unsigned int width);
    ~Item();

    void Draw(sf::RenderWindow& window);

    sf::Vector2f GetPosition() {
        return rect.getPosition();
    }

    int GetSize() {
        return size;
    }

    void SetPosition(float x, float y) {
        rect.setPosition(x, y);
    }

    void SetColor(sf::Color color) {
        rect.setFillColor(color);
    }

private:
    sf::RectangleShape rect;
    unsigned int size;
};

#endif /* Item_h */
