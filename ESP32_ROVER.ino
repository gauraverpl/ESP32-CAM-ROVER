#include "esp_camera.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h> 

// --- Credentials ---
const char* ssid = "*********";
const char* password = "********";

// --- Motor Pins (L298N) ---
#define IN1 12
#define IN2 13
#define IN3 14
#define IN4 15

// --- Servo Configuration ---
#define SERVO_PIN 2 
Servo tiltServo;
int currentAngle = 90; 

// --- Camera Pins (AI Thinker) ---
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

WebServer controlServer(80);
WiFiServer streamServer(81);

// ===== Motor Control Functions =====
void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

void moveForward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void moveBackward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void turnRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

// ===== Web Control UI =====
void handleRoot() {
  String html = "<html><head><meta name='viewport' content='width=device-width, initial-scale=1'><style>";
  html += "button { width: 90px; height: 60px; font-size: 18px; margin: 5px; border-radius: 10px; }";
  html += ".btns { display: flex; flex-wrap: wrap; justify-content: center; }";
  html += "img { width: 100%; max-width: 400px; border: 3px solid #333; }";
  html += "</style></head><body><h2 align='center'>PERIMETER ROVER</h2>";
  html += "<div align='center'><img src='http://" + WiFi.localIP().toString() + ":81/stream'></div>";
  html += "<div class='btns'><button onclick=\"send('F')\">FWD</button><br>";
  html += "<button onclick=\"send('L')\">LEFT</button><button onclick=\"send('S')\">STOP</button><button onclick=\"send('R')\">RIGHT</button><br>";
  html += "<button onclick=\"send('B')\">BACK</button></div>";
  html += "<hr><h3>Camera Tilt</h3><div class='btns'>";
  html += "<button onclick=\"tilt('up')\">UP</button><button onclick=\"tilt('center')\">CENTER</button><button onclick=\"tilt('down')\">DOWN</button>";
  html += "</div><script>function send(c){fetch('/control?move='+c);} function tilt(d){fetch('/servo?dir='+d);}</script></body></html>";
  controlServer.send(200, "text/html", html);
}

// ===== Handlers =====
void handleControl() {
  String move = controlServer.arg("move");
  if (move == "F") moveForward();
  else if (move == "B") moveBackward();
  else if (move == "L") turnLeft();
  else if (move == "R") turnRight();
  else stopMotors();
  controlServer.send(204, "", "");
}

void handleServo() {
  String dir = controlServer.arg("dir");
  if (dir == "up") currentAngle = min(180, currentAngle + 20);
  else if (dir == "down") currentAngle = max(0, currentAngle - 20);
  else if (dir == "center") currentAngle = 90;
  tiltServo.write(currentAngle);
  controlServer.send(204, "", "");
}

// ===== Camera & Streaming =====
void startCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM; config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM; config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM; config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM; config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href  = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_GRAYSCALE;
  config.frame_size = FRAMESIZE_CIF;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  if (esp_camera_init(&config) != ESP_OK) { Serial.println("Camera Fail"); while(1); }
}

void streamTask(void* p) {
  while (1) {
    WiFiClient client = streamServer.available();
    if (client) {
      client.println("HTTP/1.1 200 OK\r\nContent-Type: multipart/x-mixed-replace; boundary=frame\r\n");
      while (client.connected()) {
        camera_fb_t *fb = esp_camera_fb_get();
        if (!fb) continue;
        uint8_t* jb = NULL; size_t jl = 0;
        if (frame2jpg(fb, 40, &jb, &jl)) {
          client.printf("--frame\r\nContent-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n", jl);
          client.write(jb, jl); client.print("\r\n");
          free(jb);
        }
        esp_camera_fb_return(fb);
        vTaskDelay(30 / portTICK_PERIOD_MS);
      }
    }
    vTaskDelay(1);
  }
}

// ===== Mandatory Arduino Functions =====
void setup() {
  Serial.begin(115200);

  // Motors
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  stopMotors();

  // Servo (Core 3.3.7 uses specific timers)
  ESP32PWM::allocateTimer(0);
  tiltServo.setPeriodHertz(50);
  tiltServo.attach(SERVO_PIN, 500, 2400);
  tiltServo.write(currentAngle);

  // WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nIP: " + WiFi.localIP().toString());

  startCamera();
  controlServer.on("/", handleRoot);
  controlServer.on("/control", handleControl);
  controlServer.on("/servo", handleServo);
  controlServer.begin();
  streamServer.begin();

  xTaskCreatePinnedToCore(streamTask, "stream", 8192, NULL, 1, NULL, 0);
}

void loop() {
  controlServer.handleClient();
}