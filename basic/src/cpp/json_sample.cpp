//
// Created by binlee on 23-12-12.
//

#include "fstream"
#include "iostream"

#include "json_sample.h"
#include "nlohmann/json.hpp"

struct Results {
    std::string language{};
    float duration{};
    std::vector<std::string> data{};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Results, language, duration, data)

void jsonBasicSample() {
    // 从字符串解析
    auto j = nlohmann::json::parse(R"({"hello":"World","world":"Hello","duration": 2.79})");
    std::string h = j["hello"].get<std::string>();
    printf("%s() hello is : %s\n", __func__, h.c_str());
    std::string w = j["hello"].get<std::string>();
    printf("%s() world is : %s\n", __func__, w.c_str());
    float duration = j["duration"].get<float>();
    printf("%s() duration is : %.2f\n", __func__, duration);
    std::cout << j.dump() << std::endl;
    // 从文件解析
    std::ifstream fs("/home/binlee/code/Clearn/basic/testdata/results.json");
    j = nlohmann::json::parse(fs);
    std::cout << j.dump() << std::endl;
    fs.close();
    std::vector<float> v = {1.0f, 2.0f, 3.0f};
    v.push_back(4.0f);
    v.push_back(5.0f);
    v.push_back(6.0f);
    std::cout << nlohmann::json(v).dump() << std::endl;
    j = nlohmann::json();
    j["arr"] = {1.0f, 2.0f, 3.0f};
    j["str"] = "this is as string";
    if (j["arr"].is_array()) {
        std::cout << j["arr"].dump() << std::endl;
        auto arr = j["arr"].get<std::vector<float>>();
        for (const auto &item: arr) {
            std::cout << item << std::endl;
        }
    }
    j["obj"] = R"({"a":"a-","b":"b-"})"_json;
    j["obj_"] = {
            {"pi",    3.1415926},
            {"happy", true}
    };
    std::cout << j.dump() << std::endl;

    // 写入文件中
    std::ofstream o("/home/binlee/code/Clearn/basic/testdata/out.json");
    o << j << std::endl;
    o.close();
}

void jsonObjectSample() {
    Results r;
    r.language = "en";
    r.duration = 30.5;
    r.data = {"hello", "world"};
    auto rj = nlohmann::json(r);
    std::cout << "obj json is: " << rj.dump() << std::endl;

    Results rr;
    rj["language"] = "zh";
    rj["duration"] = 999.45;
    rj.get_to<Results>(rr);
    std::cout << "new language is: " << rr.language << std::endl;
}

void jsonTest() {
    // jsonBasicSample();
    jsonObjectSample();
}
