/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
#include "sdkconfig.h"
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>            // struct addrinfo
#include <arpa/inet.h>
#include "esp_netif.h"
#include "esp_log.h"
#if defined(CONFIG_EXAMPLE_SOCKET_IP_INPUT_STDIN)
#include "addr_from_stdin.h"
#endif

#if defined(CONFIG_EXAMPLE_IPV4)
#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV4_ADDR
#elif defined(CONFIG_EXAMPLE_SOCKET_IP_INPUT_STDIN)
#define HOST_IP_ADDR "192.168.1.5"
#endif

#define PORT CONFIG_EXAMPLE_PORT

static const char *TAG = "example";
static const char *payload = "Message from ESP32 ";

extern void example_connect(void);
#define EXAMPLE_READ_LEN                    256
extern uint32_t data;
void tcp_client(void)
{
   // char rx_buffer[128];
    char host_ip[] = HOST_IP_ADDR;
    int addr_family = 0;
    int ip_protocol = 0;
    char ADC_READING[4];
    while (1) 
    {
        #if defined(CONFIG_EXAMPLE_IPV4)
            struct sockaddr_in dest_addr;
            inet_pton(AF_INET, host_ip, &dest_addr.sin_addr);
            dest_addr.sin_family = AF_INET;
            dest_addr.sin_port = htons(PORT);
            addr_family = AF_INET;
            ip_protocol = IPPROTO_IP;
        #elif defined(CONFIG_EXAMPLE_SOCKET_IP_INPUT_STDIN)
            struct sockaddr_storage dest_addr = { 0 };
            ESP_ERROR_CHECK(get_addr_from_stdin(PORT, SOCK_STREAM, &ip_protocol, &addr_family, &dest_addr));
        #endif

        int sock =  socket(addr_family, SOCK_STREAM, ip_protocol);
        if (sock < 0) 
        {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        }
        ESP_LOGI(TAG, "Socket created, connecting to %s:%d", host_ip, PORT);

        int err = connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (err != 0) {
            ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);
        }
        ESP_LOGI(TAG, "about to send");

        while (1) 
        {
            itoa(data,ADC_READING,10);
            int err = send(sock, ADC_READING, strlen(ADC_READING), 0);
            ESP_LOGI(TAG, "ADC_Reading:%s\ndata:%"PRIu32"",ADC_READING,data);
            ADC_READING[0]=0;
            ADC_READING[1]=0;
            ADC_READING[2]=0;
            ADC_READING[3]=0;
            
            if (err < 0) 
            {

                ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);

                example_connect();
                
                int sock =  socket(addr_family, SOCK_STREAM, ip_protocol);
                if (sock < 0) 
                {
                    ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
                    break;
                }
                ESP_LOGI(TAG, "Socket created, connecting to %s:%d", host_ip, PORT);

                int err = connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
                if (err != 0) {
                    ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);
                    break;
                }
                ESP_LOGI(TAG, "about to send");
            }
            vTaskDelay(pdMS_TO_TICKS(3500));

        //     int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
        //     // Error occurred during receiving
        //     if (len < 0) 
        //     {
        //         ESP_LOGE(TAG, "recv failed: errno %d", errno);
        //         break;
        //     }
        //     // Data received
        //     else 
        //     {
        //         rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
        //         ESP_LOGI(TAG, "Received %d bytes from %s:", len, host_ip);
        //         ESP_LOGI(TAG, "%s", rx_buffer);
        //     }
        // }

        // if (sock != -1) 
        // {
        //     ESP_LOGE(TAG, "Shutting down socket and restarting...");
        //     shutdown(sock, 0);
        //     close(sock);
        }
    }
}
