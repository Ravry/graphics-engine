#define GLFW_INCLUDE_NONE
#define STB_IMAGE_IMPLEMENTATION
#include "Window.hpp"
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
#include "curl/curl.h"
#include "nlohmann/json.hpp"

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
    }
    catch (std::bad_alloc& e) {
        return 0;
    }
    return newLength;
}

static std::string fetchData(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string response;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_CAINFO, "./resources/other/cacert-2025-02-25.pem");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
    }

    return response;
}

int main()
{
    std::string url = "https://pubchem.ncbi.nlm.nih.gov/rest/pug/compound/name/glucose/record/JSON/?record_type=3d";
    std::string data = fetchData(url);
    nlohmann::json jsonData = nlohmann::json::parse(data);
    std::cout << jsonData["PC_Compounds"][0]["atoms"]["element"] << std::endl;
    std::unique_ptr<Window> window = std::make_unique<Window>("", SCR_WIDTH, SCR_HEIGHT);
    window->set_icon("resources/texture/checkered.png");
    window->run();
    return 0;
}