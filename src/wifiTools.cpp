#include "wifiTools.h"

AsyncWebServer server(80);

int client_count = 0;
bool isWifiBlank = true;

int record_rst_time()
{
    int rst_time = 0;

    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {

        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err); 

    printf("\n");
    printf("Opening Non-Volatile Storage (NVS) handle... ");
    nvs_handle my_handle;                               
    err = nvs_open("storage", NVS_READWRITE, &my_handle); 
    if (err != ESP_OK)
    {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    else
    {
        printf("Done\n");
        // Read
        printf("Reading restart counter from NVS ... ");
        int32_t restart_counter = 0; // value will default to 0, if not set yet in NVS
        err = nvs_get_i32(my_handle, "restart_counter", &restart_counter);
        switch (err)
        {
        case ESP_OK:
            printf("Done\n");
            printf("Restart counter = %d\n", restart_counter);
            rst_time = restart_counter;
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The value is not initialized yet!\n");
            break;
        default:
            printf("Error (%s) reading!\n", esp_err_to_name(err));
        }

        // Write
        printf("Updating restart counter in NVS ... ");
        restart_counter++;
        err = nvs_set_i32(my_handle, "restart_counter", restart_counter);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

        // Commit written value.
        // After setting any values, nvs_commit() must be called to ensure changes are written
        // to flash storage. Implementations may write to storage at other times,
        // but this is not guaranteed.
        printf("Committing updates in NVS ... ");
        err = nvs_commit(my_handle);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

        // Close
        nvs_close(my_handle);
    }

    printf("\n");
    return rst_time;
}

void record_wifi(char *ssid, char *password)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    printf("\n");
    printf("Opening Non-Volatile Wifi (NVS) handle... ");
    nvs_handle my_handle;                              
    err = nvs_open("Wifi", NVS_READWRITE, &my_handle); 
    if (err != ESP_OK)
    {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    else
    {
        printf("Done\n");

        // Write
        printf("Updating ssid in NVS ... ");
        err = nvs_set_str(my_handle, "ssid", ssid);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

        printf("Updating password in NVS ... ");
        err = nvs_set_str(my_handle, "password", password);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

        // Commit written value.
        // After setting any values, nvs_commit() must be called to ensure changes are written
        // to flash storage. Implementations may write to storage at other times,
        // but this is not guaranteed.
        printf("Committing updates in NVS ... ");
        err = nvs_commit(my_handle);
        printf((err != ESP_OK) ? "Failed!\n" : "Done\n");

        // Close
        nvs_close(my_handle);
    }

    printf("\n");
}

void check_wifi(char *ssid, char *password)
{
    char saved_ssid[SSID_LENGTH];
    size_t ssid_length = SSID_LENGTH;
    char saved_password[SSID_LENGTH];
    size_t password_length = SSID_LENGTH;
    // 初始化NVS，并检查初始化情况
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {

        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    printf("\n");
    printf("Opening Non-Volatile Wifi (NVS) handle... \n");
    nvs_handle my_handle;                             
    err = nvs_open("Wifi", NVS_READWRITE, &my_handle); 
    if (err != ESP_OK)
    {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    else
    {
        printf("Done\n");

        // Read
        printf("Reading ssid and password from NVS ... \n");

        err = nvs_get_str(my_handle, "ssid", saved_ssid, &ssid_length);
        switch (err)
        {
        case ESP_OK:
            printf("Done\n");
            printf("ssid: %s\n", saved_ssid);
            printf("ssid length= %d\n", ssid_length);
            strcpy(ssid, saved_ssid);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The value is not initialized yet!\n");
            break;
        default:
            printf("Error (%s) reading!\n", esp_err_to_name(err));
        }

        err = nvs_get_str(my_handle, "password", saved_password, &password_length);
        switch (err)
        {
        case ESP_OK:
            printf("Done\n");
            printf("password: %s\n", saved_password);
            printf("password length= %d\n", password_length);
            strcpy(password, saved_password);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The value is not initialized yet!\n");
            break;
        default:
            printf("Error (%s) reading!\n", esp_err_to_name(err));
        }

        // Close
        nvs_close(my_handle);
    }

    printf("\n");
    return;
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void ap_init()
{
    initSPIFFS();
    WiFi.softAP("Handshake_Wifi");
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    server.serveStatic("/", SPIFFS, "/");

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        Serial.print("Client status: ");
        Serial.println(request->client()->connected());
        request->send(SPIFFS, "/index.html", "text/html");
        });

    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/style.css","text/css");
        });
    server.on("/set_over", HTTP_GET, [](AsyncWebServerRequest *request){
        Serial.print("URL: ");
        Serial.println(request->url());
        String ssid = request->getParam("ssid")->value();
        String pwd = request->getParam("password")->value();
        Serial.println();
        Serial.println("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
        Serial.println("Get ssid and password from url:");
        Serial.println(ssid);
        Serial.println(pwd);
        Serial.println("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");

        record_wifi((char *)ssid.c_str(), (char *)pwd.c_str());
        isWifiBlank = false;
        request->send(SPIFFS, "/index.html", "text/html");
    });
    server.onNotFound(notFound);
    server.begin();    
}

void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

int wifi_config_server()
{
    if (!isWifiBlank) {
        server.end();
    }
    return isWifiBlank;
}

int wifi_set_main()
{
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");
    char ssid[SSID_LENGTH];
    char password[SSID_LENGTH];
    pinMode(WIFI_SET_PIN, INPUT_PULLUP);
    check_wifi(ssid, password);

    Serial.println("Check WIFI_SET_PIN");
    int runtime = millis();
    int starttime = runtime;
    while ((runtime - starttime) < 3000)
    {
        if (digitalRead(WIFI_SET_PIN) == LOW)
        {

            Serial.println("Please connect \"Handshake_Wifi\".");
            Serial.println("And visit 192.168.4.1 to set WIFI.");
            ap_init();
            while (wifi_config_server());
            delay(3000);
            esp_restart();
            return 0;
        }
        Serial.print(".");
        delay(100);
        runtime = millis();
    }
    Serial.println();

    //Connect wifi
    Serial.println("Connecting WIFI");
    WiFi.begin(ssid, password);

    int connect_count = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        connect_count++;
        if (connect_count > 10)
            return 0;
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    return 1;
}

void nvs_test()
{
    char ssid[SSID_LENGTH] = "";
    char password[SSID_LENGTH] = "";
    int rst_time = 0;

    check_wifi(ssid, password);
    rst_time = record_rst_time();

    sprintf(ssid, "ssid_%d", rst_time);
    sprintf(password, "password_%d", rst_time);

    record_wifi(ssid, password);

    // Restart module
    for (int i = 10; i >= 0; i--)
    {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}