    #include <Wire.h>
    #include "MAX30100_PulseOximeter.h"

   // #include <Adafruit_BMP085.h>
    #include <Adafruit_Sensor.h>
    #include <Adafruit_BME280.h>

    #define SEALEVELPRESSURE_HPA (1013.25)
    //#define seaLevelPressure_hPa 1013.25

    #define REPORTING_PERIOD_MS     1000

    // Create a PulseOximeter object
    PulseOximeter pox;
    MAX30100 sensor;
  
    // Adafruit_BMP085 bmp;
       Adafruit_BME280 bme;

    // Time at which the last beat occurred
    uint32_t tsLastReport = 0;

    // Callback routine is executed when a pulse is detected
    void onBeatDetected() {
       // Serial.println("Beat!");
    }

    void setup() {
        Serial.begin(115200);

        Serial.println("Initializing pulse oximeter..");

        // Initialize sensor
        if (!pox.begin()) {
            Serial.println("FAILED");
        }

            // Initialize sensor
        if (!sensor.begin()) {
            Serial.println("FAILED");
        } 
              if (!bme.begin(0x76)) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        }
      /*
      if (!bmp.begin()) {Serial.println("Could not find a valid BMP085 sensor, check wiring!");
      while (1) {}
      }
      */

      // Configure sensor to use 7.6mA for LED drive
        pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
      //pox.setIRLedCurrent(MAX30100_LED_CURR_11MA);
        // Register a callback routine
        pox.setOnBeatDetectedCallback(onBeatDetected);

        // Configure sensor
        configureMax30100();
    }

    void loop() {
        // Read from the sensor
        pox.update();

        // Grab the updated heart rate and SpO2 levels
        if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
            Serial.print("Heart rate:");
            Serial.print(pox.getHeartRate());
            Serial.print("bpm /  SpO2:");
            Serial.print(", ");
            Serial.print(pox.getSpO2());
            Serial.println("%");

      Serial.print("Temperature = ");
      Serial.print(bme.readTemperature());
      Serial.print("*C");

      Serial.print(" Pressure = ");
      Serial.print(bme.readPressure() / 100.0F);
      Serial.print("hPa");

      Serial.print(" Approx. Altitude = ");
      Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
      Serial.print("m");

      Serial.print(" Humidity = ");
      Serial.print(bme.readHumidity());
      Serial.println("%");
      Serial.println(" ");
            tsLastReport = millis();
        }
    }

    void configureMax30100() {
      sensor.setMode(MAX30100_MODE_SPO2_HR);
       sensor.setLedsCurrent(MAX30100_LED_CURR_7_6MA, MAX30100_LED_CURR_7_6MA);
     // sensor.setLedsCurrent(MAX30100_LED_CURR_50MA, MAX30100_LED_CURR_27_1MA);
      sensor.setLedsPulseWidth(MAX30100_SPC_PW_1600US_16BITS);
      sensor.setSamplingRate(MAX30100_SAMPRATE_100HZ);
      sensor.setHighresModeEnabled(true);
    }