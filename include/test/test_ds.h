#pragma once

#include <iostream>
#include <string>

#include "util/bit_array.h"
#include "util/ring_queue.h"

void test_ringQueue() {
    std::cout << "------- RingQueue Test -------";
    endl(std::cout);

    struct Info {
        int         id;
        std::string name;
    };

    RingQueue<Info> ring_queue(10);

    Info info{1, "1"};
    ring_queue.Push(info);
    ring_queue.EmplacePush<Info>({2, "2"});

    Info result;
    ring_queue.TryGetLast(&result);
    std::cout << result.id << " " << result.name;
    endl(std::cout);

    // FIFO
    ring_queue.TryPop();

    ring_queue.TryGetLast(&result);
    std::cout << result.id << " " << result.name;
    endl(std::cout);

    std::cout << "------- RingQueue Test -------";
    endl(std::cout);
    endl(std::cout);
}

void test_bitArray() {
    std::cout << "------- BitArray Test -------";
    endl(std::cout);

    BitArray bit_array(8); // 8 bit

    for (auto it = bit_array.begin(); it != bit_array.end(); ++it) {
        std::cout << bit_array[it];
    }
    endl(std::cout);

    // if `0` is 0
    if (bit_array[0])
        std::cout << "0" << std::endl;

    bit_array[1] = 1; // set `1` to 1
    bit_array[2] = 1; // set `2` to 1

    for (auto it = bit_array.begin(); it != bit_array.end(); ++it) {
        std::cout << bit_array[it];
    }
    endl(std::cout);

    std::cout << "------- BitArray Test -------";
    endl(std::cout);
}