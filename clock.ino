#include "fabgl.h"
#define FABGL_FONT_INCLUDE_DEFINITION
#include "segment140.h"
#include <WiFi.h>
#include "time.h"

const char* ssid       = "BF";
const char* password   = "49724314";
const char* ntpServer = "pool.ntp.org";
fabgl::VGAController DisplayController;
fabgl::Canvas        canvas(&DisplayController);

void setTimezone(String timezone){
  Serial.printf("  Setting Timezone to %s\n",timezone.c_str());
  setenv("TZ",timezone.c_str(),1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();
}

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  
  char date_buf[80];
  strftime(date_buf,80,"          %A, %B %d %Y          ", &timeinfo);
  canvas.selectFont(&fabgl::FONT_8x8);
  canvas.drawTextFmt(50, 100, "   %s   ", date_buf);
  canvas.selectFont(&fabgl::FONT_Seven_Segment140);
  canvas.drawTextFmt(0, 120, "%02d:%02d:%02d  ", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
}

void setup()
{ 
  Serial.begin(115200);
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  } Serial.println(" CONNECTED");
  configTime(0, 0, ntpServer);
  setTimezone("EET-2EEST,M3.5.0/3,M10.5.0/4");
  delay(1000);
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  delay(1000);
  DisplayController.begin();
  DisplayController.setResolution(VGA_512x448_60Hz);
  canvas.setBrushColor(RGB888(0, 0, 0));
  canvas.setGlyphOptions(GlyphOptions().FillBackground(true));
  canvas.selectFont(&fabgl::FONT_Seven_Segment140);
  canvas.setPenColor(Color::BrightRed);
  canvas.clear();
  canvas.drawTextFmt(0, 120, "%02d:%02d:%02d", 0, 0, 0);
}

void loop()
{
  delay(1000);
  printLocalTime();
}
