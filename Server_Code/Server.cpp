#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <ctime>
#include <fstream>
#include <thread>
#include <mutex>
#include <string>
#include <cstring>
using namespace std;

ofstream myfile;
std::string latest_data;
std::mutex mtx;

void data_receiver(int clientSocket) {
    while (true) {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytes > 0) {
            std::lock_guard<std::mutex> lock(mtx);
            latest_data = std::string(buffer);
            time_t curr_time = time(0);
            tm* tm_local = localtime(&curr_time);
            myfile.seekp(0, ios::beg);
            myfile << tm_local->tm_hour << ":" << tm_local->tm_min << " Message from client: " << buffer << endl;
        }
    }
}

void handle_request(int client) 
{
    char request[2048];
    memset(request, 0, sizeof(request));
    recv(client, request, sizeof(request) - 1, 0);
    std::string req_str(request);

    size_t pos = req_str.find(" ");
    if (pos != std::string::npos) {
        size_t pos2 = req_str.find(" ", pos + 1);
        if (pos2 != std::string::npos) {
            std::string path = req_str.substr(pos + 1, pos2 - pos - 1);

            if (path == "/") {
                // Your new HTML design
                std::string html = R"(
<!DOCTYPE html>
<html>
<head>
  <title>ADC Monitor</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px;
           background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
           min-height: 100vh; }
    .container { background: white; padding: 40px; border-radius: 10px;
                 box-shadow: 0 4px 6px rgba(0,0,0,0.1); display: inline-block; }
    h1 { color: #333; margin-bottom: 30px; }
    .value-display { font-size: 48px; font-weight: bold; color: #667eea; margin: 20px 0; }
    .status { font-size: 18px; margin-top: 20px; }
    .connected { color: green; }
    .disconnected { color: red; }
  </style>
</head>
<body>
  <div class='container'>
    <h1>ADC Value Monitor</h1>
    <div class='value-display' id='adc-value'>--</div>
    <div class='status'>
      Status: <span id='status' class='connected'>Connected</span>
    </div>
  </div>
  <script>
    const adcValue = document.getElementById('adc-value');
    const status = document.getElementById('status');
    setInterval(() => {
      fetch('/api/adc')
        .then(response => response.json())
        .then(data => {
          adcValue.textContent = data.value;
          status.className = 'connected';
        })
        .catch(err => {
          status.className = 'disconnected';
        });
    }, 500);
  </script>
</body>
</html>
)";
                std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: "
                    + std::to_string(html.size()) + "\r\n\r\n" + html;
                send(client, response.c_str(), response.size(), 0);

            } else if (path == "/api/adc") {
                // Return latest data as JSON
                std::lock_guard<std::mutex> lock(mtx);
                std::string data = latest_data.empty() ? "0" : latest_data;
                std::string json = "{\"value\":\"" + data + "\"}";
                std::string response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: "
                    + std::to_string(json.size()) + "\r\n\r\n" + json;
                send(client, response.c_str(), response.size(), 0);

            } else {
                std::string response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
                send(client, response.c_str(), response.size(), 0);
            }
        }
    }
    close(client);
}

int main() {
    std::cout << "Starting ADC Monitor Server..." << std::endl;
    myfile.open("Gas_Reading.txt", ios::trunc);

    // --- Data server on port 8080 ---
    int dataSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (dataSocket < 0) { perror("Data socket failed"); exit(1); }

    int opt = 1;
    setsockopt(dataSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in dataAddress;
    memset(&dataAddress, 0, sizeof(dataAddress));
    dataAddress.sin_family = AF_INET;
    dataAddress.sin_port = htons(8080);
    dataAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(dataSocket, (struct sockaddr*)&dataAddress, sizeof(dataAddress)) < 0) {
        perror("Data bind failed");
        exit(1);
    }
    if (listen(dataSocket, 5) < 0) {
        perror("Data listen failed");
        exit(1);
    }
    std::cout << "Data server listening on port 8080" << std::endl;

    std::thread data_thread([&]() {
        while (true) {
            int dataClient = accept(dataSocket, nullptr, nullptr);
            if (dataClient >= 0) {
                std::thread(data_receiver, dataClient).detach();
            }
        }
    });
    data_thread.detach();

    // --- HTTP server on port 8081 ---
    int httpSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (httpSocket < 0) { perror("HTTP socket failed"); exit(1); }

    setsockopt(httpSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in httpAddress;
    memset(&httpAddress, 0, sizeof(httpAddress));
    httpAddress.sin_family = AF_INET;
    httpAddress.sin_port = htons(255);
    httpAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(httpSocket, (struct sockaddr*)&httpAddress, sizeof(httpAddress)) < 0) {
        perror("HTTP bind failed");
        exit(1);
    }
    if (listen(httpSocket, 5) < 0) {
        perror("HTTP listen failed");
        exit(1);
    }
    std::cout << "HTTP server listening on port 255" << std::endl;

    // --- Accept HTTP clients ---
    while (true) {
        int httpClient = accept(httpSocket, nullptr, nullptr);
        if (httpClient >= 0) {
            std::thread http_thread(handle_request, httpClient);
            http_thread.detach();
        }
    }

    close(dataSocket);
    close(httpSocket);
    myfile.close();
    return 0;
}

