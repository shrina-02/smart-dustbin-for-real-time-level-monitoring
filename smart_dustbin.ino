#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ESP_Mail_Client.h>

// Wi-Fi credentials
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// Email credentials
#define SENDER_EMAIL "YOUR_SENDER_EMAIL"
#define SENDER_PASSWORD "YOUR_GMAIL_APP_PASSWORD"
#define RECIPIENT_EMAIL "YOUR_RECIPIENT_EMAIL"

// SMTP settings
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 587

// Ultrasonic sensor pins
#define TRIG_PIN D5
#define ECHO_PIN D6

LiquidCrystal_I2C lcd(0x27, 16, 2);
SMTPSession smtp;

float duration_us, distance_cm;
const float DISTANCE_THRESHOLD = 10.0; // Change as needed

void setup() {
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  lcd.begin();
  lcd.backlight();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration_us = pulseIn(ECHO_PIN, HIGH);
  distance_cm = 0.017 * duration_us;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distance: ");
  lcd.print(distance_cm);
  lcd.print(" cm");

  if (distance_cm < DISTANCE_THRESHOLD) {
    sendEmail(distance_cm);
  }

  delay(500);
}

void sendEmail(float distance) {
  MailClient.networkReconnect(true);

  smtp.debug(1);
  smtp.callback(smtpCallback);

  Session_Config config;

  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;

  config.login.email = SENDER_EMAIL;
  config.login.password = SENDER_PASSWORD;
  config.login.user_domain = "127.0.0.1";

  SMTP_Message message;

  message.sender.name = "ESP8266";
  message.sender.email = SENDER_EMAIL;
  message.subject = "Distance Alert from ESP8266";

  message.addRecipient("User", RECIPIENT_EMAIL);

  String textMsg = "Alert! Distance detected: ";
  textMsg += String(distance);
  textMsg += " cm";

  message.text.content = textMsg;
  message.text.charSet = "utf-8";

  message.priority =
    esp_mail_smtp_priority::esp_mail_smtp_priority_low;

  if (!smtp.connect(&config) ||
      !MailClient.sendMail(&smtp, &message)) {

    Serial.printf(
      "Email sending failed: %d, %s\n",
      smtp.errorCode(),
      smtp.errorReason().c_str()
    );
  }
}

void smtpCallback(SMTP_Status status) {
  Serial.println(status.info());

  if (status.success()) {
    Serial.println("Email sent successfully!");
  }
}
