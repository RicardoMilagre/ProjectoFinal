#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

const int num_samples = 1000;
const float walking_freq_low = 0.5;
const float walking_freq_high = 3;
const float running_freq_low = 3;
const float running_freq_high = 10;
const float walking_power_threshold = 10000;
const float running_power_threshold = 50000;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();
  mpu.setFullScaleAccelRange(2); // set accelerometer range to +/-2g
}

void loop() {
  float x[num_samples], y[num_samples];
  for (int i = 0; i < num_samples; i++) {
    mpu.getAcceleration(&x[i], &y[i], NULL);
    delay(2);
  }
  float fft_x[num_samples], fft_y[num_samples];
  FFT(fft_x, x, num_samples);
  FFT(fft_y, y, num_samples);
  float walking_power = 0, running_power = 0;
  int walking_freq_low_index = int(walking_freq_low * num_samples / 1000);
  int walking_freq_high_index = int(walking_freq_high * num_samples / 1000);
  int running_freq_low_index = int(running_freq_low * num_samples / 1000);
  int running_freq_high_index = int(running_freq_high * num_samples / 1000);
  for (int i = walking_freq_low_index; i < walking_freq_high_index; i++) {
    walking_power += fft_x[i] * fft_x[i] + fft_y[i] * fft_y[i];
  }
  for (int i = running_freq_low_index; i < running_freq_high_index; i++) {
    running_power += fft_x[i] * fft_x[i] + fft_y[i] * fft_y[i];
  }
  if (walking_power > walking_power_threshold) {
    Serial.println("Walking detected.");
  }
  else if (running_power > running_power_threshold) {
    Serial.println("Running detected.");
  }
  else {
    Serial.println("No movement detected.");
  }
}

void FFT(float *fft, float *signal, int n) {
  for (int k = 0; k < n; k++) {
    fft[k] = 0;
    for (int t = 0; t < n; t++) {
      fft[k] += signal[t] * cos(2 * PI * k * t / n);
    }
  }
}
