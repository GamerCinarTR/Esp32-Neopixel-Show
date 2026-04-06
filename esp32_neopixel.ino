#include <Adafruit_NeoPixel.h>

#define PIN (Change this according to pin you use)
#define NUMPIXELS (Change this depending to your LED count)

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint32_t currentC1;
uint32_t currentC2;

uint32_t colorPalette[] = {
  pixels.Color(0, 0, 70),     
  pixels.Color(50, 0, 150),
  pixels.Color(130, 0, 230),  
  pixels.Color(180, 0, 120),  
  pixels.Color(255, 10, 100), 
  pixels.Color(0, 5, 100)     
};

void setup() {
  pixels.begin();
  pixels.setBrightness(120); 
  randomSeed(analogRead(0));
  pickNewColors();
}

void pickNewColors() {
  currentC1 = colorPalette[random(0, 6)];
  currentC2 = colorPalette[random(0, 6)];
  while(currentC1 == currentC2) {
    currentC2 = colorPalette[random(0, 6)];
  }
}

void fadeAll(float factor = 0.8) {
  for (int i = 0; i < NUMPIXELS; i++) {
    uint32_t col = pixels.getPixelColor(i);
    uint8_t r = (uint8_t)(col >> 16), g = (uint8_t)(col >> 8), b = (uint8_t)col;
    pixels.setPixelColor(i, pixels.Color(r * factor, g * factor, b * factor));
  }
}

uint32_t blendColors(uint32_t c1, uint32_t c2) {
  uint8_t r = qadd8((uint8_t)(c1 >> 16), (uint8_t)(c2 >> 16));
  uint8_t g = qadd8((uint8_t)(c1 >> 8), (uint8_t)(c2 >> 8));
  uint8_t b = qadd8((uint8_t)c1, (uint8_t)c2);
  return pixels.Color(r, g, b);
}

uint8_t qadd8(uint8_t a, uint8_t b) {
  int res = a + b;
  if (res > 255) return 255;
  return (uint8_t)res;
}

uint32_t lerpColor(uint32_t c1, uint32_t c2, float t) {
  if (t < 0.0) t = 0.0;
  if (t > 1.0) t = 1.0;
  uint8_t r1 = (c1 >> 16), g1 = (c1 >> 8), b1 = c1;
  uint8_t r2 = (c2 >> 16), g2 = (c2 >> 8), b2 = c2;
  return pixels.Color(r1 + (r2 - r1) * t, g1 + (g2 - g1) * t, b1 + (b2 - b1) * t);
}

void loop() {
  pickNewColors();
  int effect = random(0, 12);

  if (effect == 0) {
    dragonDuel(15000);
  } else if (effect == 1) {
    cometOverlap(12000);
  } else if (effect == 2) {
    nebulaFade(15000);
  } else if (effect == 3) {
    vortexChase(10000);
  } else if (effect == 4) {
    doublePulse(10000);
  } else if (effect == 5) {
    spiralGALAXY(15000);
  } else if (effect == 6) {
    chaosSparkle(12000);
  } else if (effect == 7) {
    breathOfVoid(10000);
  } else if (effect == 8) {
    waveScanner(10000);
  } else if (effect == 9) {
    shimmeringRain(12000);
  } else if (effect == 10) {
    colorMerge(12000);
  } else if (effect == 11) {
    plasmaDrift(15000);
  }
}

void dragonDuel(int duration) {
  unsigned long start = millis();
  float p1 = 0;
  float p2 = NUMPIXELS / 2;
  
  while (millis() - start < duration) {
    fadeAll(0.7);
    
    int pos1 = (int)p1 % NUMPIXELS;
    int pos2 = (int)p2 % NUMPIXELS;
    
    pixels.setPixelColor(pos1, blendColors(pixels.getPixelColor(pos1), currentC1));
    pixels.setPixelColor(pos2, blendColors(pixels.getPixelColor(pos2), currentC2));
    
    pixels.show();
    p1 += 0.35;
    p2 -= 0.35;
    if (p2 < 0) p2 = NUMPIXELS - 1;
    delay(40);
  }
}

void cometOverlap(int duration) {
  unsigned long start = millis();
  float pos = 0;
  while (millis() - start < duration) {
    fadeAll(0.85);
    int p1 = (int)pos % NUMPIXELS;
    int p2 = ((int)pos + NUMPIXELS/2) % NUMPIXELS;
    
    pixels.setPixelColor(p1, currentC1);
    pixels.setPixelColor(p2, currentC2);
    
    pixels.show();
    pos += 0.6;
    delay(50);
  }
}

void nebulaFade(int duration) {
  unsigned long start = millis();
  while (millis() - start < duration) {
    for (int i = 0; i < NUMPIXELS; i++) {
      float s = (sin(millis() / 1500.0 + i * 0.4) + 1.0) / 2.0;
      pixels.setPixelColor(i, lerpColor(currentC1, currentC2, s));
    }
    pixels.show();
    delay(30);
  }
}

void vortexChase(int duration) {
  unsigned long start = millis();
  float head = 0;
  while (millis() - start < duration) {
    fadeAll(0.6);
    int p1 = (int)head % NUMPIXELS;
    int p2 = ((int)head + 2) % NUMPIXELS;
    pixels.setPixelColor(p1, currentC1);
    pixels.setPixelColor(p2, currentC2);
    pixels.show();
    head += 0.8;
    delay(40);
  }
}

void doublePulse(int duration) {
  unsigned long start = millis();
  while (millis() - start < duration) {
    for (float i = 0; i < 6.28; i += 0.08) {
      float b1 = (sin(i) + 1.0) / 2.0;
      float b2 = (cos(i) + 1.0) / 2.0;
      
      uint32_t p1 = lerpColor(0, currentC1, b1);
      uint32_t p2 = lerpColor(0, currentC2, b2);
      
      pixels.fill(blendColors(p1, p2));
      pixels.show();
      delay(25);
    }
  }
}

void spiralGALAXY(int duration) {
  unsigned long start = millis();
  float t = 0;
  while (millis() - start < duration) {
    for (int i = 0; i < NUMPIXELS; i++) {
      float angle = i * (6.28 / NUMPIXELS);
      float b = (sin(angle + t) + sin(angle * 2.0 - t * 0.5) + 2.0) / 4.0;
      pixels.setPixelColor(i, lerpColor(pixels.Color(0,0,20), blendColors(currentC1, currentC2), b));
    }
    pixels.show();
    t += 0.1;
    delay(30);
  }
}

void chaosSparkle(int duration) {
  unsigned long start = millis();
  while (millis() - start < duration) {
    fadeAll(0.5);
    if (random(10) > 6) {
      int pos = random(NUMPIXELS);
      pixels.setPixelColor(pos, (random(2) == 0) ? currentC1 : currentC2);
    }
    pixels.show();
    delay(60);
  }
}

void breathOfVoid(int duration) {
  unsigned long start = millis();
  while (millis() - start < duration) {
    for (int b = 20; b < 150; b++) {
      pixels.fill(lerpColor(currentC1, currentC2, (b-20)/130.0));
      pixels.setBrightness(b);
      pixels.show();
      delay(15);
    }
    for (int b = 150; b >= 20; b--) {
      pixels.fill(lerpColor(currentC1, currentC2, (b-20)/130.0));
      pixels.setBrightness(b);
      pixels.show();
      delay(15);
    }
    pixels.setBrightness(120);
  }
}

void waveScanner(int duration) {
  unsigned long start = millis();
  float center = 0;
  while (millis() - start < duration) {
    pixels.clear();
    for (int i = 0; i < NUMPIXELS; i++) {
      float dist = fabs(i - center);
      if (dist > NUMPIXELS/2) dist = NUMPIXELS - dist;
      float b = 1.0 - (dist / 4.0);
      if (b < 0) b = 0;
      pixels.setPixelColor(i, lerpColor(0, currentC1, b*b));
    }
    pixels.show();
    center += 0.4;
    if (center >= NUMPIXELS) center = 0;
    delay(40);
  }
}

void shimmeringRain(int duration) {
  unsigned long start = millis();
  while (millis() - start < duration) {
    fadeAll(0.8);
    int pos = random(NUMPIXELS);
    uint32_t col = blendColors(currentC1, pixels.Color(100, 100, 100));
    pixels.setPixelColor(pos, col);
    pixels.show();
    delay(80);
  }
}

void colorMerge(int duration) {
  unsigned long start = millis();
  while (millis() - start < duration) {
    for (int i = 0; i < NUMPIXELS; i++) {
      float t = (sin(millis() / 2000.0 + i * (6.28 / NUMPIXELS)) + 1.0) / 2.0;
      if (t < 0.5) {
        pixels.setPixelColor(i, lerpColor(currentC1, pixels.Color(0,0,0), t * 2.0));
      } else {
        pixels.setPixelColor(i, lerpColor(pixels.Color(0,0,0), currentC2, (t - 0.5) * 2.0));
      }
    }
    pixels.show();
    delay(30);
  }
}

void plasmaDrift(int duration) {
  unsigned long start = millis();
  while (millis() - start < duration) {
    float t = millis() / 1000.0;
    for (int i = 0; i < NUMPIXELS; i++) {
      float u = i / (float)NUMPIXELS;
      int r = 80 + 80 * sin(u * 6.28 + t);
      int g = 0; 
      int b = 120 + 120 * cos(u * 6.28 - t * 1.5);
      pixels.setPixelColor(i, pixels.Color(r, 0, b));
    }
    pixels.show();
    delay(30);
  }
}