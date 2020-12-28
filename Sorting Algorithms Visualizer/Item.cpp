//
//  Item.cpp
//  Sorting Algorithms Visualizer
//
//  Created by RyanLazz on 9/9/19.
//  Copyright © 2019 ryanlazzareschi. All rights reserved.
//

#include "Item.h"

Item::Item() {
    
}

Item::Item(unsigned int maxSize, unsigned int width) {
    size = std::rand() % maxSize + 1;
    rect.setSize(sf::Vector2f(float(width), float(size)));
    rect.setOrigin(static_cast<float>(width), static_cast<float>(size));
}

Item::~Item() {
    
}

void Item::Draw(sf::RenderWindow& window) {
    window.draw(rect);
}
