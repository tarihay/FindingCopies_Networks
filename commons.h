#pragma once

#include <random>
#include <fstream>
#include <map>
#include <utility>
#include <iostream>
#include <algorithm>
#include "exceptions.h"

const std::string names[] = {"igor", "volodya", "emir", "peter", "mark"};
const int MAX_NAMES_AMOUNT = 5;

std::string generateName() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distrib(1, MAX_NAMES_AMOUNT);

    std::string name;
    unsigned long count = distrib(rng);
    for (int i = 0; i < count; ++i) {
        name = names[i];
    }

    char hexStr[10];
    snprintf(hexStr, 10, "%lX", count);
    return name + "-" + hexStr;
}

class FriendList {
private:
    std::string myName;
    const int timeoutSeconds = 30;
    struct Friend {
        time_t lastSeen{};
        std::string addr;
    };
    std::map<std::string, Friend> friends;

public:

    void setName(std::string name) {
        myName = std::move(name);
    }

    void addFriend(const std::string &addr, const std::string &name) {
        if (friends.find(name) != friends.end()) {
            friends.find(name)->second.lastSeen = time(nullptr);
        } else {
            Friend fr{
                    time(nullptr),
                    addr,
            };
            friends[name] = fr;
        }
    }

    void removeExpired() {
        for (const auto &el: friends) {
            if (time(nullptr) - el.second.lastSeen > timeoutSeconds) {
                friends.erase(el.first);
            }
        }
    }

    void showFriendList() {
        system("clear");
        printf("My name is %s\n\n", myName.c_str());
        printf("%-15s ||%-24s ||%-15s\n", "Name", "Address", "Last seen");
        printf("========================================================\n");

        for (const auto &fr: friends) {
            char buf[32];
            const auto time = localtime(&fr.second.lastSeen);
            strftime(buf, sizeof(buf), "%H:%M:%S", time);
            printf("%-15s ||%-24s ||%-15s\n", fr.first.c_str(), fr.second.addr.c_str(), buf);
        }
    }
};