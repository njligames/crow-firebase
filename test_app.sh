#!/bin/bash

# === CONFIGURATION ===
API_URL="http://localhost:8080"
ID_TOKEN="$1"

if [ -z "$ID_TOKEN" ]; then
  echo "Usage: $0 <FIREBASE_ID_TOKEN>"
  echo "Get this token by signing in a Firebase user on the client."
  exit 1
fi

# === Test /login ===
echo "➡️ Testing /login endpoint..."
login_response=$(curl -s -w "\nHTTP_STATUS:%{http_code}" -X POST "$API_URL/login" \
  -H "Content-Type: application/json" \
  -d "{\"idToken\": \"$ID_TOKEN\"}")

login_body=$(echo "$login_response" | sed -n '1p')
login_status=$(echo "$login_response" | sed -n '2p' | sed 's/HTTP_STATUS://')

echo "Status: $login_status"
echo "Body: $login_body"
echo

# === Test /protected ===
echo "➡️ Testing /protected endpoint..."
protected_response=$(curl -s -w "\nHTTP_STATUS:%{http_code}" "$API_URL/protected" \
  -H "Authorization: Bearer $ID_TOKEN")

protected_body=$(echo "$protected_response" | sed -n '1p')
protected_status=$(echo "$protected_response" | sed -n '2p' | sed 's/HTTP_STATUS://')

echo "Status: $protected_status"
echo "Body: $protected_body"
