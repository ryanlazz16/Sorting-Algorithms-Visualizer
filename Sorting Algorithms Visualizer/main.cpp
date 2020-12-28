//
//  main.cpp
//  Sorting Algorithms Visualizer
//
//  Created by RyanLazz on 9/8/19.
//  Copyright © 2019 ryanlazzareschi. All rights reserved.
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Item.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define ITEM_WIDTH 10
#define ITEM_MAX_SIZE 1000

static const int numItems = 150;

static const float viewHeight = 1024.0f;

void resizeView(const sf::RenderWindow& window, sf::View& view) {
    float aspectRatio = float(window.getSize().x) / float(window.getSize().y);
    view.setSize(viewHeight * aspectRatio, viewHeight);
}

void display(sf::RenderWindow& window, sf::View& view, std::vector<Item> items) {
    window.clear(sf::Color(100, 100, 100));

    window.setView(view);

    for (int k = 0; k < numItems; k++) {
        items[k].SetPosition((SCREEN_WIDTH - (ITEM_WIDTH + 1) * numItems) / 2 + (ITEM_WIDTH + 1) * k, SCREEN_HEIGHT);
        items[k].Draw(window);
    }

    window.display();
}

void stallTime(sf::Clock& clock, float deltaTime, float time) {
    float totalTime = 0.0f;
    totalTime += deltaTime;
    while (totalTime < time) {
        deltaTime = clock.restart().asSeconds();
        totalTime += deltaTime;
    }
}

void stallTime(float time) {
    sf::Clock clock;
    float totalTime = 0.0f;
    float deltaTime = 0.0f;
    totalTime += deltaTime;
    while (totalTime < time) {
        deltaTime = clock.restart().asSeconds();
        totalTime += deltaTime;
    }
}

void checkIfSorted(bool sorted, bool& checkSorted, sf::Clock& clock, std::vector<Item> items, sf::RenderWindow& window, sf::View& view, float deltaTime) {
    if (sorted && !checkSorted) {
        for (int i = 0; i < items.size() - 1; i++) {
            deltaTime = clock.restart().asSeconds();

            items[i].SetColor(sf::Color::Black);

            if (items[i].GetSize() <= items[i+1].GetSize()) {
                items[i+1].SetColor(sf::Color::Green);
            } else {
                items[i+1].SetColor(sf::Color::Red);
                break;
            }

            display(window, view, items);

            items[i].SetColor(sf::Color::White);
            items[i+1].SetColor(sf::Color::White);

            stallTime(clock, deltaTime, 0.015f);
        }
        checkSorted = true;
    }
}

// selection sort finds the smallest value and places it next in the sorted list
void selectionSort(sf::RenderWindow& window, sf::View& view, std::vector<Item>& items) {
    for (int i = 0; i < items.size() - 1; i++) {
        items[i].SetColor(sf::Color::Black);

        // Find the minimum element in unsorted array
        int min_idx = i;

        for (int j = i + 1; j < items.size(); j++) {
            items[j].SetColor(sf::Color::Red);
            if (items[j].GetSize() < items[min_idx].GetSize()) {
                min_idx = j;
                items[j].SetColor(sf::Color::Green);
            }

            display(window, view, items);

            // stallTime(0.025f);

            items[j].SetColor(sf::Color::White);
        }

        items[i].SetColor(sf::Color::White);

        // Swap the found minimum element with the first element
        std::swap(items[min_idx], items[i]);
    }
}

// bubble sort swaps adjacent elements and iterates through the list n times
void bubbleSort(sf::RenderWindow& window, sf::View& view, std::vector<Item>& items)
{
    for (int i = 0; i < items.size()-1; i++) {
        // Last i elements are already in place
        for (int j = 0; j < items.size()-i-1; j++) {
            items[j].SetColor(sf::Color::Black);
            if (items[j].GetSize() > items[j+1].GetSize()) {
                items[j+1].SetColor(sf::Color::Green);
                std::swap(items[j], items[j+1]);
            } else {
                items[j+1].SetColor(sf::Color::Red);
            }

            display(window, view, items);
            items[j].SetColor(sf::Color::White);
            items[j+1].SetColor(sf::Color::White);

            // stallTime(0.01f);
        }
    }
}

// insertion sort iterates through the list and inserts the current element in correct position in sorted part of list
void insertionSort(sf::RenderWindow& window, sf::View& view, std::vector<Item>& items)
{
    for (int i = 1; i < items.size(); i++)
    {
        Item key = items[i];
        int j = i - 1;

        while (j >= 0 && items[j].GetSize() > key.GetSize())
        {
            items[j + 1] = items[j];
            j--;

            items[j].SetColor(sf::Color::Red);
            display(window, view, items);
            items[j].SetColor(sf::Color::White);
            // stallTime(0.1f);
        }

        items[j].SetColor(sf::Color::Green);
        display(window, view, items);
        items[j].SetColor(sf::Color::White);

        // stallTime(0.1f);

        items[j + 1] = key;
    }
}

void merge_sort_merge(sf::RenderWindow& window, sf::View& view, std::vector<Item>& items, std::vector<Item>& aux, int lo, int mid, int hi) {
    // Save both halves into auxiliary space
    for (int k = lo; k <= hi; k++)
        aux[k] = items[k];
    // Merge the two halves: [lo,mid][mid+1,hi]
    int i = lo, j = mid + 1;
    for (int k = lo; k <= hi; k++) {
        // Right half has been entirely merged
        if (i > mid) {
            // Just take what's remaining from left half
            items[k] = aux[j++];
        }
        // Left half has been entirely merged
        else if (j > hi) {
            // Just take what's remaining from right half
            items[k] = aux[i++];
        }
        // Get smallest item from either left or right halves
        else if (aux[j].GetSize() < aux[i].GetSize()) {
            items[k] = aux[j++];
            items[k].SetColor(sf::Color::Green);
        }
        else {
            items[k] = aux[i++];
            items[k].SetColor(sf::Color::Green);
        }
        items[i].SetColor(sf::Color::Red);
        items[j].SetColor(sf::Color::Red);
        display(window, view, items);
        items[i].SetColor(sf::Color::White);
        items[j].SetColor(sf::Color::White);
        items[k].SetColor(sf::Color::White);
        stallTime(0.005f);
    }
}

void merge_sort_recur(sf::RenderWindow& window, sf::View& view, std::vector<Item>& items, std::vector<Item>& aux, int lo, int hi) {
    // Recursion stop condition: subarray is sorted if it contains 0 or 1 item
    if (hi <= lo)
        return;
    // Otherwise, further divide subarray into two halves
    int mid = lo + (hi - lo) / 2;
    // Recursively sort right half
    merge_sort_recur(window, view, items, aux, lo, mid);
    // Recursively sort left half
    merge_sort_recur(window, view, items, aux, mid + 1, hi);
    // Merge two halves
    items[mid].SetColor(sf::Color::Black);
    display(window, view, items);
    items[mid].SetColor(sf::Color::White);
    stallTime(0.005f);
    merge_sort_merge(window, view, items, aux, lo, mid, hi);
}

void mergeSort(sf::RenderWindow& window, sf::View& view, std::vector<Item>& items) {
    // Need auxiliary space of the same size
    std::vector<Item> aux(items.size());
    merge_sort_recur(window, view, items, aux, 0, items.size() - 1);
}

int quicksort_partition(sf::RenderWindow& window, sf::View& view, std::vector<Item>& items,int lo, int hi) {
    int i = lo, j = hi + 1;
    while (true) {
        // From left to right, find first item
        // larger than pivot
        while (items[++i].GetSize() < items[lo].GetSize())
            // Prevents from going out of bounds
            if (i == hi) break;
        // From right to left, find first item
        // smaller than pivot
        while (items[lo].GetSize() < items[--j].GetSize())
            // To keep code symmetry but unnecessary
            if (j == lo) break;
        // If pointers i and j have crossed, stop
        if (i >= j) break;
        // Otherwise swap items
        std::swap(items[i], items[j]);

        items[lo].SetColor(sf::Color::Red);
        items[hi].SetColor(sf::Color::Red);
        items[i].SetColor(sf::Color::Green);
        items[j].SetColor(sf::Color::Green);
        display(window, view, items);
        stallTime(0.025f);
        items[i].SetColor(sf::Color::White);
        items[j].SetColor(sf::Color::White);
        items[lo].SetColor(sf::Color::White);
        items[hi].SetColor(sf::Color::White);
    }
    // Put pivot in final position
    std::swap(items[lo], items[j]);
    // Return pivot position
    return j;
}

void quicksort_recur(sf::RenderWindow& window, sf::View& view, std::vector<Item>& items,int lo, int hi) {
    // Recursion stop condition:
    // Sub-array is sorted if it contains 0 or 1 item
    if (hi <= lo)
        return;
    // Partition array and get resulting pivot pos
    int p = quicksort_partition(window, view, items, lo, hi);
    items[p].SetColor(sf::Color::Black);
    display(window, view, items);
    stallTime(0.05f);
    items[p].SetColor(sf::Color::White);
    // Recursively sort left sub-partition
    quicksort_recur(window, view, items, lo, p - 1);
    // Recursively sort right sub-partition
    quicksort_recur(window, view, items, p + 1, hi);
}

// Generic quicksort functiontemplate<typename K>
void quicksort(sf::RenderWindow& window, sf::View& view, std::vector<Item>& items) {
    quicksort_recur(window, view, items, 0, items.size() - 1);
}

void randomize(std::vector<Item>& items) {
    items.clear();
    for (int i = 0; i < numItems; i++) {
        items.push_back(Item(ITEM_MAX_SIZE, ITEM_WIDTH));
        items[i].SetColor(sf::Color::White);
    }
}

int main(int argc, const char * argv[]) {
    // creates a window thats closable and resizable, but with no title bar (use bitwise or operator)
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Sorting Algorithms Visualizer", sf::Style::Close | sf::Style::Resize);

    sf::View view(sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));

    std::vector<Item> items;
    randomize(items);
//    for (int i = 0; i < numItems; i++) {
//        items.push_back(Item(ITEM_MAX_SIZE, ITEM_WIDTH));
//        items[i].SetColor(sf::Color::White);
//    }

    float deltaTime = 0.0f;
//    float totalTime = 0.0f;
    sf::Clock clock;

    bool doSelectionSort = false;
    bool doBubbleSort = false;
    bool doInsertionSort = false;
    bool doMergeSort = false;
    bool doQuicksort = false;
    bool sorted = false;
    bool checkSorted = false;

    // Game Loop
    while (window.isOpen()) {
        //deltaTime = clock.restart().asSeconds();

        // create event since pollEvent needs to take an event
        sf::Event evnt;
        while (window.pollEvent(evnt)) {
            switch (evnt.type) {
                    // if the event type is closing the window, then close the window
                case sf::Event::Closed:
                    window.close();
                    break;
                    // print new dimensions when user resizes the window
                case sf::Event::Resized:
                    resizeView(window, view);
                    std::cout << "New Window Dimensions: " << evnt.size.width << " x " << evnt.size.height << std::endl;
                    // printf("New window width: %i New window height: %i\n", evnt.size.width, evnt.size.height);
                    break;
                    // print entered text
                case sf::Event::TextEntered:
                    if (evnt.text.unicode < 128)
                        printf("%c", evnt.text.unicode);
            }
        }

        // check if mouse is pressed
//        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
//            // get current position of the mouse relative to the window
//            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
//            // set the position of the player to the mouse position
//            rect.setPosition((float) mousePos.x, static_cast<float>(mousePos.y));
//            std::cout << "new mouse pos: " << mousePos.x << " " << mousePos.y << std::endl;
//        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            doSelectionSort = true;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B)) {
            doBubbleSort = true;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I)) {
            doInsertionSort = true;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M)) {
            doMergeSort = true;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
            doQuicksort = true;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
            sorted = false;
            checkSorted = false;
            doSelectionSort = false;
            doBubbleSort = false;
            doInsertionSort = false;
            doMergeSort = false;
            doQuicksort = false;
            randomize(items);
        }

        display(window, view, items);

        if (!sorted && doSelectionSort) {
            selectionSort(window, view, items);

            sorted = true;
            doSelectionSort = false;
        } else if (!sorted && doBubbleSort) {
            bubbleSort(window, view, items);

            sorted = true;
            doBubbleSort = false;
        } else if (!sorted && doInsertionSort) {
            insertionSort(window, view, items);

            sorted = true;
            doInsertionSort = false;
        } else if (!sorted && doMergeSort) {
            mergeSort(window, view, items);

            sorted = true;
            doMergeSort = false;
        } else if (!sorted && doQuicksort) {
            quicksort(window, view, items);

            sorted = true;
            doQuicksort = false;
        }

        checkIfSorted(sorted, checkSorted, clock, items, window, view, deltaTime);
    }

    return 0;
}
