Here is the fully formatted **`README.md`** with GitHub-style badges and clean markdown layout, ready to drop into your project.

---

## 📘 `README.md`

````markdown
# 🔐 Crow + Firebase + JWT API (MVP)

[![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![Firebase](https://img.shields.io/badge/Firebase-Auth-yellow.svg)](https://firebase.google.com/)
[![Crow](https://img.shields.io/badge/Crow-1.0%2B-lightgrey.svg)](https://github.com/CrowCpp/Crow)
[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

Minimal secure REST API using C++ with [Crow](https://github.com/CrowCpp/Crow) and Firebase Authentication via JWT.

---

## 🧰 Tech Stack

- 🕸️ Crow – C++ microframework
- 🔐 JWT-CPP – JWT verification
- 🌐 CPR – HTTP client for REST APIs
- 🧩 nlohmann/json – JSON parsing
- 🔥 Firebase – Authentication and user management

---

## 🚀 Features

| Endpoint      | Method | Auth Required | Description                          |
|---------------|--------|----------------|--------------------------------------|
| `/signup`     | POST   | ❌ No          | Creates Firebase user (email/pass)   |
| `/login`      | POST   | ❌ No          | Verifies Firebase ID token           |
| `/protected`  | GET    | ✅ Bearer JWT  | Secured route using verified token   |

---

## ⚙️ Setup

### 📦 Prerequisites

- C++17 or newer
- [vcpkg](https://github.com/microsoft/vcpkg)
- Firebase project (with Email/Password sign-in enabled)

### 🔧 Install Dependencies

```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg install crow cpr jwt-cpp nlohmann-json
````

### 🏗️ Build the Project

```bash
git clone https://github.com/YOUR_USERNAME/crow-firebase.git
cd crow-firebase
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build .
```

---

## 🔐 Firebase Configuration

1. Visit the [Firebase Console](https://console.firebase.google.com/)
2. Enable **Email/Password** under `Authentication → Sign-in Method`
3. Go to `Project Settings → General → Web App`, and copy:

   * `apiKey`
   * `projectId`

Update `main.cpp`:

* Replace `YOUR_API_KEY` in `/signup` endpoint
* Replace `YOUR_PROJECT_ID` in JWT verification logic

---

## 🧪 Testing the API

### Run the Server

```bash
./crow_firebase_jwt_app
```

### Use the Provided Test Script

```bash
./test_app.sh <FIREBASE_ID_TOKEN>
```

### Or Use `curl`

```bash
# Create user
curl -X POST http://localhost:8080/signup \
  -H "Content-Type: application/json" \
  -d '{"email":"test@example.com","password":"mypassword"}'

# Verify token
curl -X POST http://localhost:8080/login \
  -H "Content-Type: application/json" \
  -d '{"idToken":"<FIREBASE_ID_TOKEN>"}'

# Access protected route
curl http://localhost:8080/protected \
  -H "Authorization: Bearer <FIREBASE_ID_TOKEN>"
```

---

## 🧱 Project Structure

```
crow-firebase/
├── main.cpp              # Main server source
├── CMakeLists.txt        # CMake build config
├── test_app.sh           # Bash tester script
├── firebase-client.html  # Web app to generate ID token
└── README.md             # This file
```

---

## 📌 Security Tips

* API key is safe (not secret), but protect usage from abuse.
* Always serve Crow over HTTPS in production.
* Add logging, rate-limiting, and expiry checks for robustness.

---

## ✍️ Author & License

MIT License. Built by \[Your Name].

Contributions welcome!

```

---

### ✅ What’s Next?

Would you like me to:

- Generate a `LICENSE` file?
- Push this into a live GitHub repository structure?
- Turn this into a Docker container or Heroku-compatible app?

Let me know how far you'd like to take it!
```
