curl -X POST http://localhost:8080/signup \
  -H "Content-Type: application/json" \
  -d '{"email":"test3@example.com","password":"secret123"}' | jq .idToken
