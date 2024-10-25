
// g++ --std=c++11 test.cpp

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "screenmap.h"

TEST_CASE("ScreenMap basic functionality") {
    // Create a screen map for 3 LEDs
    ScreenMap map(3);
    
    // Set some x,y coordinates
    map.set(0, {1.0f, 2.0f});
    map.set(1, {3.0f, 4.0f});
    map.set(2, {5.0f, 6.0f});
    
    // Test coordinate retrieval
    CHECK(map[0].x == 1.0f);
    CHECK(map[0].y == 2.0f);
    CHECK(map[1].x == 3.0f);
    CHECK(map[1].y == 4.0f);
    CHECK(map[2].x == 5.0f);
    CHECK(map[2].y == 6.0f);
    
    // Test length
    CHECK(map.getLength() == 3);
    
    // Test diameter (default should be 1.0)
    CHECK(map.getDiameter() == 1.0f);
    
    // Test mapToIndex (should give same results as operator[])
    auto coords = map.mapToIndex(1);
    CHECK(coords.x == 3.0f);
    CHECK(coords.y == 4.0f);
}

TEST_CASE("ScreenMap JSON parsing") {
    const char* json = R"({
        "map": {
            "strip1": {
                "x": [10.5, 30.5, 50.5],
                "y": [20.5, 40.5, 60.5],
                "diameter": 2.5
            },
            "strip2": {
                "x": [15.0, 35.0],
                "y": [25.0, 45.0],
                "diameter": 1.5
            }
        }
    })";

    FixedMap<Str, ScreenMap, 16> segmentMaps;
    ScreenMap::ParseJson(json, &segmentMaps);

    ScreenMap& strip1 = segmentMaps["strip1"];
    ScreenMap& strip2 = segmentMaps["strip2"];

    // Check first strip

    CHECK(strip1.getLength() == 3);
    CHECK(strip1.getDiameter() == 2.5f);
    CHECK(strip1[0].x == 10.5f);
    CHECK(strip1[0].y == 20.5f);
    CHECK(strip1[1].x == 30.5f);
    CHECK(strip1[1].y == 40.5f);

    // Check second strip
    CHECK(strip2.getLength() == 2);
    CHECK(strip2.getDiameter() == 1.5f);
    CHECK(strip2[0].x == 15.0f);
    CHECK(strip2[0].y == 25.0f);
    CHECK(strip2[1].x == 35.0f);
    CHECK(strip2[1].y == 45.0f);
}

