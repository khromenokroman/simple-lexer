#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

std::set<std::string> key_words = {"show", "ip", "bgp", "summary", "neighbors", "neighbours", "advertised-routes", "received-routes"};

enum TokenType {
    KEY_WORDS,
    SKIP
};

struct Token {
    std::string value;
    TokenType type;
};

[[maybe_unused]] std::vector<std::string> split_string(std::string const &input_string) {
    std::vector<std::string> words;
    std::string word;
    for (char ch: input_string) {
        if (ch != ' ') {
            word += ch;
        } else if (!word.empty()) {
            words.emplace_back(word);
            word.clear();
        }
    }
    if (!word.empty()) {
        words.emplace_back(word);
    }
    return words;
}

std::string shift(std::vector<std::string> &src) {
    std::string current = src.front();
    src.erase(src.begin());
    return current;
}

Token token(std::string &&value, TokenType token_type) {
    return {value, token_type};
}

bool is_valid_ip(std::string ip) {
    int parts_count = 0;
    std::string part;
    ip.push_back('.');
    for (char c: ip) {
        if (c == '.') {
            if (part.empty() || stoi(part) > 255 || stoi(part) < 0) {
                return false;
            }
            parts_count++;
            part.clear();
        } else if (!isdigit(c)) {
            return false;
        } else {
            part.push_back(c);
        }
    }
    return parts_count == 4;
}

std::vector<Token> tokenize(std::string &sourceCode) {
    std::vector<Token> tokens;
    std::vector<std::string> src = split_string(sourceCode);
    fmt::print("{}\n", src);

    while (!src.empty()) {
        std::string front_token = shift(src);
        if (key_words.find(front_token) != key_words.end()) {
            tokens.emplace_back(token(std::move(front_token), TokenType::KEY_WORDS));
        } else {
            //@todo: надо бы маску проверить
            auto pos_separate = front_token.find('/');
            if (pos_separate != std::string::npos) {
                std::string ip = front_token.substr(0, pos_separate);
                fmt::print("ip: {}\n", ip);
                if (is_valid_ip(ip)) {
                    tokens.emplace_back(token(std::move(front_token), TokenType::KEY_WORDS));
                } else {
                    tokens.emplace_back(token(std::move(front_token), TokenType::SKIP));
                }
            } else {
                if (is_valid_ip(front_token)) {
                    tokens.emplace_back(token(std::move(front_token), TokenType::KEY_WORDS));
                } else {
                    tokens.emplace_back(token(std::move(front_token), TokenType::SKIP));
                }
            }
        }
    }

    return tokens;
}

int main(int argc, char *argv[]) {

    //    std::string input_sting = "show ip bgp context sample summary";
    //    std::string input_sting = "show ip bgp context sample route-table";
    //    std::string input_sting = "show ip bgp context sample route-table ip-address 192.168.100.1/24 routing";
    std::string input_sting = "show ip bgp context sample neighbours peer 192.168.100.1 received-routes";

    fmt::print("input string: {}\n", input_sting);
    std::vector<Token> tokens = tokenize(input_sting);
    std::string final_comand{};
    for (auto const &token: tokens) {
        fmt::print("{} - {}\n", token.value, static_cast<int>(token.type));
        if (static_cast<int>(token.type) == 0) {
            final_comand += token.value + " ";
        }
    }
    fmt::print("vtysh comand: {}\n", final_comand);

    std::set<std::string> k1111 = {
            "show", "ip", "bgp", "summary", "neighbors", "neighbours", "advertised-routes", "received-routes"};
    fmt::print("size: {}\n", sizeof(std::vector<std::string>));
}