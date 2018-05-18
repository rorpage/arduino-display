#include <Glyphduino.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

const int screenWidth = 16;
const int screenHeight = 2;

LiquidCrystal_I2C lcd(0x3F, screenWidth, screenHeight);
Glyphduino* glyph = new Glyphduino(&lcd);
SoftwareSerial BT(3, 1);

uint8_t degree[8]  = { 140, 146, 146, 140, 128, 128, 128, 128 };

String message = "Device ready";
String message_type = "GENERIC";

void setup() {
  lcd.begin(screenWidth, screenHeight);
  lcd.init();
  lcd.backlight();

  lcd.createChar(0, degree);
  glyph->registerGlyph(1, GLYPHDUINO_BELL);
  glyph->registerGlyph(2, GLYPHDUINO_MUSIC_NOTE);
  glyph->registerGlyph(3, GLYPHDUINO_CLOCK);
  glyph->registerGlyph(4, GLYPHDUINO_THERMOMETER);
  glyph->registerGlyph(5, GLYPHDUINO_TROPHY);
  glyph->registerGlyph(6, GLYPHDUINO_ARROW_N);
  glyph->registerGlyph(7, GLYPHDUINO_ARROW_S);

  BT.begin(9600);
}

void drawIcon() {
  lcd.setCursor(0, 0);

  if (message_type == "MUSIC") {
    glyph->printGlyph(GLYPHDUINO_MUSIC_NOTE);
  } else if (message_type == "CLOCK") {
    glyph->printGlyph(GLYPHDUINO_CLOCK);
  } else if (message_type == "WEATHER") {
    glyph->printGlyph(GLYPHDUINO_THERMOMETER);
  } else if (message_type == "SPORTS") {
    glyph->printGlyph(GLYPHDUINO_TROPHY);
  } else if (message_type == "STOCK") {
    lcd.print("$");
  } else {
    glyph->printGlyph(GLYPHDUINO_BELL);
  }
}

void displayMessage() {
  if (message.indexOf("\n") != -1) {
    lcd.setCursor(2, 0);
    lcd.print(message.substring(0, message.indexOf("\n")));
    lcd.setCursor(0, 1);
    lcd.print(message.substring(message.indexOf("\n") + 1, message.length()));
  } else {
    if (message.length() > 14) {
      lcd.setCursor(2, 0);
      lcd.print(message.substring(0, 13));
      lcd.setCursor(0, 1);
      lcd.print(message.substring(13, message.length()));
    } else {
      lcd.setCursor(2, 0);
      lcd.print(message);
    }
  }
}

void handleNewMessage() {
  BT.println("New message received");
  String bluetoothMessage = BT.readString();

  int separatorIndex = bluetoothMessage.indexOf("##");
  message = bluetoothMessage.substring(0, separatorIndex);
  message_type = bluetoothMessage.substring(separatorIndex + 2, bluetoothMessage.length());

  lcd.clear();
}

void loop() {
  if (BT.available()) {
    handleNewMessage();
  }

  drawIcon();
  displayMessage();
}
