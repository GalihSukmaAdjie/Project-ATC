// library yang digunakan
#include <Arduino.h>
#include <ThingerESP32.h>
#include "esp32-hal-cpu.h"
#include "DHT.h"

// inisialisasi device ke thinger.io
#define USERNAME "Galih_Sukma_Adjie"
#define DEVICE_ID "atcproject"
#define DEVICE_CREDENTIAL "selfclosingfloodbarrier"

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

// koneksi ke jaringan WIFI
#define SSID "WHOAMI"//"MyRepublic_C1533862"
#define SSID_PASSWORD "12345678"//"C1533862"

// DHT on ESP
#define DHT_PIN 13 //17
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

float suhu = 0;
float kelembaban = 0;

// Pompa pin on ESP
#define POMPA 2

// HC SR-04 on ESP
#define TRIG_PIN 23
#define ECHO_PIN 22

long duration = 0;
int distance = 0;
float trueDist = 0;
float gate = 0;

// Raindrop sensor on ESP
#define RAIN 34

int weather = 0;
String cuaca = "prediksi";

// interval pengiriman data
unsigned long currentMillis = 0;
unsigned long previousMillis = 0; 
int interval = 1000;

void setup() {
  // set kecepatan CPU
  setCpuFrequencyMhz(240);
  // kecepatan serial komunikasi
  Serial.begin(9600);
  dht.begin();
  // GPIO pin
  pinMode(DHT_PIN, INPUT);
  pinMode(POMPA, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RAIN, INPUT_PULLUP);
  // hubungkan ke WIFI
  thing.add_wifi(SSID, SSID_PASSWORD);

  // Digital pin untuk mengaktifkan relay
  thing["pompa"] << digitalPin(POMPA);

  // mengirim data data hasil olahan sensor ke Web yang dibuat
  thing["suhu"] >> outputValue(suhu);
  thing["kelembaban"] >> outputValue(kelembaban);
  thing["gate"] >> outputValue(trueDist);
  thing["cuaca"] >> outputValue(cuaca);
}

// fungsi untuk mengolah data dari DHT sensor
void dhtsend() {
  // mengambil data suhu dan kelembaban dari sensor
  suhu = dht.readTemperature();
  kelembaban = dht.readHumidity();

  if(isnan(suhu) || isnan(kelembaban))
  {
    Serial.println("Gagal membaca DHT...!!!");
    return;
  }
  Serial.print(suhu);
  Serial.println(kelembaban);
}

// fungsi untuk mengolah data dari ULTRASONIC sensor
void ultrasonic() {
  distance = 0;
  // bersihkan TRIG_PIN
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  // set TRIG_PIN HIGH selama 1 detik
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  // membaca echo pin, mengembalikan waktu tempuh gelombang suara dalam mikrodetik
  duration = pulseIn(ECHO_PIN, HIGH);
  // menghitung jarak
  distance= duration*0.034/2;

  if (distance <= 10) {
    Serial.print("Distance: ");
    Serial.println(distance);
    // trueDist = distance;
    trueDist = map(distance, 8, 3, 0, 5.5);
  }
  else {
    Serial.println("error !!!");
    return;
  }
}

// fungsi untuk mengolah data cuaca (Rain sensor)
void raindrop() {
  // membaca sinyal analog yang masuk
  weather = analogRead(RAIN);
  Serial.println(weather);
  
  // menentukan kondisi berdasarkan sinyal analog yang diterima
  if (weather >= 3000) {
    cuaca = "Terik";
  }
  else if (weather < 3000 && weather >= 1000) {
    cuaca = "Gerimis";
  }
  else if (weather < 1000) {
    cuaca = "Hujan";
  }
  else {
    Serial.println("RAIN ERROR !!!");
    return;  
  }
}

void loop() {
  thing.handle();
  // meng-eksekusi perintah sesuai interval yang ditentukan
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // menjalankan fungsi-fungsi yang telah dibuat
    dhtsend();
    ultrasonic();
    raindrop();
    previousMillis = currentMillis;
  }
}