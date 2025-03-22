#pragma once
#include <iostream>
#include "Transform.hpp"
#include "nlohmann/json.hpp"
#include <curl/curl.h>
#include <vector>
#include <array>

namespace Chemistry {
    struct Atom {
        glm::vec4 pos;
    };

    struct Bond {
        int idA;
        int idB;
        int order;
    };

    struct Molecule {
        std::vector<Atom> atoms;
        std::vector<Bond> bonds;
        int bondsCount = 0;
    };

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
    
    constexpr float encryptColor(int red, int green, int blue) {
        return float((red << 16) | (green << 8) | blue);
    }

    static Molecule loadMolecule(std::string name) {
        Molecule result;

        std::string url = "https://pubchem.ncbi.nlm.nih.gov/rest/pug/compound/name/" + name + "/record/JSON/?record_type=3d";
        std::string data = fetchData(url);

        nlohmann::json jsonData = nlohmann::json::parse(data);
        // std::cout << jsonData << std::endl;

        if (jsonData.contains("Fault"))
            return result;

        std::vector<int> atoms = jsonData["PC_Compounds"][0]["atoms"]["element"];


        int i {0};
        for (auto _atom : atoms)
        {
            Atom atom;
            float x_coord = jsonData["PC_Compounds"][0]["coords"][0]["conformers"][0]["x"][i];
            float y_coord = jsonData["PC_Compounds"][0]["coords"][0]["conformers"][0]["y"][i];
            float z_coord = jsonData["PC_Compounds"][0]["coords"][0]["conformers"][0]["z"][i];
            atom.pos = glm::vec4(x_coord, y_coord, z_coord, _atom);
            result.atoms.push_back(atom);
            i++;
        }

        auto bonds = jsonData["PC_Compounds"][0]["bonds"];
        auto aid1 = bonds["aid1"];
        auto aid2 = bonds["aid2"];
        auto order = bonds["order"];

        for (int j {0}; j < aid1.size(); j++) {
            Bond bond;
            bond.idA = aid1[j];
            bond.idB = aid2[j];
            bond.order = order[j];
            result.bondsCount += order[j].get<int>();
            result.bonds.push_back(bond);
        }

        return result;
    }

    constexpr std::array<float, 21> colors = {
        0.0f, // Placeholder for the first entry (index 0)
        encryptColor(255, 255, 255), // Wasserstoff (H), Ordnungszahl 1 - Weiß
        encryptColor(255, 255, 255), // Helium (He), Ordnungszahl 2 - Weiß
        encryptColor(255, 0, 0),     // Lithium (Li), Ordnungszahl 3 - Rot
        encryptColor(0, 255, 0),     // Beryllium (Be), Ordnungszahl 4 - Grün
        encryptColor(0, 0, 255),     // Bor (B), Ordnungszahl 5 - Blau
        encryptColor(48, 48, 48),       // Kohlenstoff (C), Ordnungszahl 6 - Gelb
        encryptColor(0, 255, 255),   // Stickstoff (N), Ordnungszahl 7 - Cyan
        encryptColor(255, 0, 0),     // Sauerstoff (O), Ordnungszahl 8 - Rot
        encryptColor(192, 192, 192), // Fluor (F), Ordnungszahl 9 - Silber
        encryptColor(128, 128, 128), // Neon (Ne), Ordnungszahl 10 - Grau
        encryptColor(255, 165, 0),   // Natrium (Na), Ordnungszahl 11 - Orange
        encryptColor(0, 255, 0),     // Magnesium (Mg), Ordnungszahl 12 - Grün
        encryptColor(255, 0, 255),   // Aluminium (Al), Ordnungszahl 13 - Magenta
        encryptColor(255, 255, 255), // Silizium (Si), Ordnungszahl 14 - Weiß
        encryptColor(0, 0, 255),     // Phosphor (P), Ordnungszahl 15 - Blau
        encryptColor(255, 165, 0),   // Schwefel (S), Ordnungszahl 16 - Orange
        encryptColor(0, 255, 255),   // Chlor (Cl), Ordnungszahl 17 - Cyan
        encryptColor(0, 0, 255),     // Argon (Ar), Ordnungszahl 18 - Blau
        encryptColor(255, 0, 255),   // Kalium (K), Ordnungszahl 19 - Magenta
        encryptColor(255, 255, 0)    // Calcium (Ca), Ordnungszahl 20 - Gelb
    };

    static std::vector<glm::mat4> createAtomMatrices(Molecule molecule, float scalar)
    {
        std::vector<glm::mat4> instanceMatricesAtoms;
        for (int i = 0; i < molecule.atoms.size(); i++)
        {
            glm::mat4 _matrix = glm::translate(glm::scale(glm::mat4(1), glm::vec3(1.f)), glm::vec3(molecule.atoms[i].pos) * scalar);
            _matrix[3].w = colors[(int)molecule.atoms[i].pos.w];
            instanceMatricesAtoms.push_back(_matrix);
        }
        return instanceMatricesAtoms;
    }

    static std::vector<glm::mat4> createBondMatrices(Molecule molecule, float scalar) {
        std::vector<glm::mat4> instanceMatricesBonds;
        for (int i = 0; i < molecule.bonds.size(); i++)
        {
            glm::vec3 p1 = molecule.atoms[molecule.bonds[i].idA - 1].pos * scalar;
            glm::vec3 p2 = molecule.atoms[molecule.bonds[i].idB - 1].pos * scalar;
            glm::vec3 midpoint = (p1 + p2) * 0.5f;

            float length = glm::length(p2 - p1);
            glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(.1f, length, .1f));

            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
            glm::vec3 direction = glm::normalize(p2 - p1);
            glm::quat rotation = glm::rotation(up, direction);
            glm::mat4 rotMat = glm::toMat4(rotation);
            glm::mat4 model = glm::translate(glm::mat4(1.0f), midpoint) * rotMat * scale;
            model[3].w = (float)0x222222;

            if (molecule.bonds[i].order == 2)
            {
                instanceMatricesBonds.push_back(glm::translate(model, glm::vec3(2.f, 0.0f, 0.0f)));
                instanceMatricesBonds.push_back(glm::translate(model, glm::vec3(-2.f, 0.0f, 0.0f)));
            }
            else if (molecule.bonds[i].order == 3)
            {
                instanceMatricesBonds.push_back(glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f)));
                instanceMatricesBonds.push_back(glm::translate(model, glm::vec3(-2.5f, 0.0f, 0.0f)));
                instanceMatricesBonds.push_back(model); // Central bond
            }
            else if (molecule.bonds[i].order == 4)
            {
                instanceMatricesBonds.push_back(glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f)));
                instanceMatricesBonds.push_back(glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0f)));
                instanceMatricesBonds.push_back(glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f))); // Vertical offset
                instanceMatricesBonds.push_back(glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f))); // Vertical offset
            }
            else
            {
                instanceMatricesBonds.push_back(model);
            }
        }
        return instanceMatricesBonds;
    }
};