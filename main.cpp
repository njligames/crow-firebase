#include "crow.h"
#include <jwt-cpp/jwt.h>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

// Replace with your Firebase project ID
const std::string FIREBASE_PROJECT_ID = "mosaify-b923c";

// Firebase public keys endpoint
const std::string GOOGLE_CERTS_URL = "https://www.googleapis.com/robot/v1/metadata/x509/securetoken@system.gserviceaccount.com";

// Cached keys
json firebase_keys;

// Utility to load Firebase public keys
void load_firebase_keys() {
    auto r = cpr::Get(cpr::Url{GOOGLE_CERTS_URL});
    if (r.status_code == 200) {
        firebase_keys = json::parse(r.text);
    } else {
        throw std::runtime_error("Failed to fetch Firebase keys.");
    }
}

// Verify Firebase JWT
bool verify_token(const std::string& token, std::string& user_id) {
    try {
        auto decoded = jwt::decode(token);

        // Firebase JWTs have "kid" header
        auto kid = decoded.get_header_claim("kid").as_string();

        if (!firebase_keys.contains(kid)) return false;

        auto pub_key_pem = firebase_keys[kid].get<std::string>();

        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::rs256(pub_key_pem, "", "", ""))
            .with_issuer("https://securetoken.google.com/" + FIREBASE_PROJECT_ID)
            .with_audience(FIREBASE_PROJECT_ID);

        verifier.verify(decoded);
        user_id = decoded.get_payload_claim("user_id").as_string();

        return true;
    } catch (...) {
        return false;
    }
}

// Middleware to protect routes
struct AuthMiddleware {
    struct context {
        std::string user_id = "";
    };

    void before_handle(crow::request& req, crow::response& res, context& ctx) {
        // Allow unauthenticated access to certain endpoints
        if (req.url == "/login" || req.url == "/signup") {
            return;
        }

        auto auth_header = req.get_header_value("Authorization");

        if (auth_header.substr(0, 7) != "Bearer ") {
            res.code = 401;
            res.end("Unauthorized: No Bearer token");
            return;
        }

        std::string token = auth_header.substr(7);
        std::string user_id;

        if (!verify_token(token, user_id)) {
            res.code = 401;
            res.end("Unauthorized: Invalid token");
            return;
        }

        ctx.user_id = user_id;
    }


    void after_handle(crow::request& req, crow::response& res, context& ctx) {}
};

int main() {
    crow::App<AuthMiddleware> app;

    load_firebase_keys();

    CROW_ROUTE(app, "/signup").methods("POST"_method)([](const crow::request& req, crow::response& res) {
        auto body = json::parse(req.body);
        std::string email = body["email"];
        std::string password = body["password"];

        std::string API_KEY = getenv("FIREBASE_API_KEY");

        auto response = cpr::Post(
                cpr::Url{"https://identitytoolkit.googleapis.com/v1/accounts:signUp?key=" + API_KEY},
                cpr::Header{{"Content-Type", "application/json"}},
                cpr::Body{json{
                        {"email", email},
                        {"password", password},
                        {"returnSecureToken", true}
                }.dump()}
        );

        if (response.status_code == 200) {
            auto res_json = json::parse(response.text);
//            return crow::response(200, "User created. ID Token:\n" + res_json["idToken"].get<std::string>());
            nlohmann::json response_json;
            response_json["message"] = "user created successfully";
            response_json["idToken"] = res_json["idToken"].get<std::string>();
            res.code = 200;
            res.write(response_json.dump());
            res.end();
        } else {
            nlohmann::json response_json;
//            return crow::response(400, "Failed to create user: " + response.text);
            response_json["error"] = "Failed to create user: " + response.text;
            res.code = 400;
            res.write(response_json.dump());
            res.end();
        }
    });


    CROW_ROUTE(app, "/login").methods("POST"_method)([](const crow::request& req) {
        auto body = json::parse(req.body);
        std::string id_token = body["idToken"];

        std::string user_id;
        if (!verify_token(id_token, user_id)) {
            return crow::response(401, "Invalid ID token");
        }

        return crow::response(200, "Welcome, user " + user_id);
    });

    // CROW_ROUTE(app, "/protected").methods("GET"_method)([&](const crow::request& req, crow::response& res, const AuthMiddleware::context& ctx) {
    //     res.write("Hello, authenticated user: " + ctx.user_id);
    //     res.end();
    // });

    CROW_ROUTE(app, "/protected").methods("GET"_method)
        ([&](const crow::request& req, crow::response& res) {
            auto& ctx = app.get_context<AuthMiddleware>(req);
            res.write("Hello, authenticated user: " + ctx.user_id);
            res.end();
        });


    app.port(8080).multithreaded().run();
}
